#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <memory>
#include <sstream>

#include <QString>
#include <QtDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QStringList>
#include <QProgressDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QDir>
#include <math.h>
#include <QtAV_Global.h>

#include <util.h>
#include <videoexportprocessor.h>

using namespace std;

// todo: move somewhere else
const QString APP_NAME = "VideoCrop";
const qint64 SEEK_JUMP = 2000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    exportProcessor(this)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowTitle(APP_NAME);
    this->updateControls(false);

    // default icon states
    ui->togglePlayButton->setIcon(QIcon(":/images/play.png"));
    ui->speedDecreaseButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->speedIncreaseButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->skipBackwardsButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->skipForwardsButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    videoOutput = make_shared<QtAV::VideoOutput>(this);
    if (!videoOutput->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }

    // Initialize Video player widget
    videoPlayer = make_shared<QtAV::AVPlayer>();
    videoPlayer->setRenderer(videoOutput.get());
    videoPlayer->setMediaEndAction(QtAV::MediaEndActionFlag::MediaEndAction_Pause);

    // set video widget
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(videoOutput->widget());
    ui->videoWidget->setLayout(layout);

    // bind ui elements
    ui->seekbar->setRangeContainer(ranges);
    ui->seekbar->bindTo(videoPlayer.get());
    ui->positionLabel->bindTo(videoPlayer.get());

    // wire up additional events
    connect(videoPlayer.get(), SIGNAL(loaded()), SLOT(on_playerLoaded()));
    connect(videoPlayer.get(), SIGNAL(internalAudioTracksChanged(QVariantList)), SLOT(on_playerAudioTracksLoaded(QVariantList)));
    connect(videoPlayer.get(), SIGNAL(stateChanged(QtAV::AVPlayer::State)), SLOT(on_playerStateChanged(QtAV::AVPlayer::State)));
    connect(videoPlayer.get(), SIGNAL(seekFinished(qint64)), SLOT(on_seeked()));
    connect(&exportProcessor, SIGNAL(progress(int)), SLOT(on_exportProgress(int)));
    connect(&exportProcessor, SIGNAL(finishedItem(int)), SLOT(on_exportedItem(int)));
    connect(&exportProcessor, SIGNAL(finishedAll()), SLOT(on_exportFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isLoaded()
{
    return videoPlayer->isLoaded();
}

bool MainWindow::fileExists()
{
    QFileInfo fileToExport(this->filename);
    return fileToExport.exists() || fileToExport.isFile();
}

QString MainWindow::getFileDialogLocation()
{
    if (!fileExists()) return "";

    QFileInfo fileToExport(this->filename);
    return fileToExport.dir().absolutePath();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *evt)
{
    const QMimeData* mimeData = evt->mimeData();
    if (mimeData->hasUrls())
    {
        QUrl firstUrl = mimeData->urls().first();
        this->openFile(firstUrl.toLocalFile());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // Clicking anywhere else should remove focus from the rangeInput.
    // If we're clicking on rangeInput, we'll just reacquire focus and nothing happens.
    ui->rangeInput->clearFocus();
    QMainWindow::mousePressEvent(event);
}

void MainWindow::syncRangesToText()
{
    ui->rangeInput->property("plainText").toString();
    QString newText = "";
    for (pair<qint64, qint64> range : ranges)
    {
        QString first = millisecondsToTimestamp(range.first, false);
        QString second = millisecondsToTimestamp(range.second, false);
        newText += first + "," + second + "\n";
    }
    ui->rangeInput->setPlainText(newText);
}

void MainWindow::openFile(const QString& filename)
{
    qDebug() << "opening " << filename;
    this->filename = filename;

    if (videoPlayer->isPlaying())
        videoPlayer->stop();

    videoPlayer->setFile(filename);
    videoPlayer->load();

    // once the video is loaded, an event is fired and loading continues
}

void MainWindow::closeVideo()
{
    this->filename = QString();
    videoPlayer->stop();

    ui->rangeInput->setPlainText("");

    ui->menuAudioTracks->clear();
    menuAudioTracksGroup.reset();

    updateControls(false);

    this->setWindowTitle(APP_NAME);
}

void MainWindow::updateControls(bool isLoaded)
{
    ui->rangeInput->setEnabled(isLoaded);
    ui->exportButton->setEnabled(isLoaded);
}

void MainWindow::skipAmount(qint64 skipAmount)
{
    qint64 position = (seekPosition > -1) ? seekPosition: videoPlayer->position();
    seekPosition = position + skipAmount;
    videoPlayer->seek(seekPosition);
}

void MainWindow::on_playerLoaded()
{
    qDebug() << "video loaded";
    this->setWindowTitle(this->filename);

    updateControls(true);

    ranges.clear();
    ranges.setVideoLength(videoPlayer->duration());
    ranges.add(0, videoPlayer->duration());
    syncRangesToText();

    videoPlayer->setSeekType(QtAV::SeekType::KeyFrameSeek);
    videoPlayer->play(); // temp
}

void MainWindow::on_playerAudioTracksLoaded(QVariantList tracks)
{
    menuAudioTracksGroup = shared_ptr<QActionGroup>(new QActionGroup(this));
    connect(menuAudioTracksGroup.get(), SIGNAL(triggered(QAction*)), SLOT(on_changeAudioTrackTriggered(QAction*)));

    ui->menuAudioTracks->clear();
    for (int i = 0; i < tracks.size(); i++)
    {
        QString trackName = tracks[i].toMap()["title"].toString();

        QAction* trackAction = ui->menuAudioTracks->addAction(trackName);
        trackAction->setCheckable(true);
        trackAction->setProperty("trackIdx", i);
        menuAudioTracksGroup->addAction(trackAction);

        if (i == 0) trackAction->setChecked(true);

        qDebug() << "Added audio track " << trackName;
    }
}

void MainWindow::on_playerStateChanged(QtAV::AVPlayer::State state)
{
    QIcon playIcon;

    if (state == QtAV::AVPlayer::State::PlayingState)
        playIcon = QIcon(":/images/pause.png");
    else
        playIcon = QIcon(":/images/play.png");
    ui->togglePlayButton->setIcon(playIcon);
}

void MainWindow::on_seeked()
{
    seekPosition = -1;
}

void MainWindow::on_togglePlayButton_clicked()
{
    qDebug() << "clicked play button; was playing" << videoPlayer->isPlaying();
    videoPlayer->togglePause();
}

void MainWindow::on_rangeInput_textChanged()
{
    ranges.clear();

    QString text = ui->rangeInput->property("plainText").toString();
    QStringList lines = text.split('\n');
    for (QString line : lines)
    {
        QStringList sections = line.split(',');
        if (sections.count() != 2)
            continue;

        qint64 first = timestampToMilliseconds(sections[0].trimmed());
        qint64 second = timestampToMilliseconds(sections[1].trimmed());

        if (first < 0 || second < 0)
            continue;

        ranges.add(first, second);
    }

    // todo: move, perhaps do this when there is an event on ranges
    ui->seekbar->repaint();
}

void MainWindow::on_speedDecreaseButton_clicked()
{
    if (!isLoaded()) return;

    qreal newSpeed = videoPlayer->speed() - 0.5;
    newSpeed = max(newSpeed, 1.0);
    videoPlayer->setSpeed(newSpeed);
}

void MainWindow::on_speedIncreaseButton_clicked()
{
    if (!isLoaded()) return;

    qreal newSpeed = videoPlayer->speed() + 0.5;
    newSpeed = min(newSpeed, 6.0);
    videoPlayer->setSpeed(newSpeed);
}

void MainWindow::on_trimLeftButton_clicked()
{
    if (!isLoaded()) return;

    ranges.trimLeftAt(videoPlayer->position());
    syncRangesToText();
}

void MainWindow::on_splitMiddleButton_clicked()
{
    if (!isLoaded()) return;

    ranges.splitAt(videoPlayer->position());
    syncRangesToText();
}

void MainWindow::on_trimRightButton_clicked()
{
    if (!isLoaded()) return;

    ranges.trimRightAt(videoPlayer->position());
    syncRangesToText();
}

void MainWindow::on_exportButton_clicked()
{
    // todo: show what files conflict, allow overwriting
    if (!this->fileExists())
    {
        QMessageBox message;
        message.setText("Source file does not exist");
        message.exec();
        return;
    }

    videoPlayer->pause();

    QString outputFilename = QFileDialog::getSaveFileName(
                this, "Save File", getFileDialogLocation(),
                "Video Files (*.mp4)");

    if (outputFilename == "")
    {
        qDebug() << "cancelled export";
        return;
    }

    if (!outputFilename.toLower().endsWith(".mp4"))
        outputFilename += ".mp4";

    qDebug() << "output filename " << outputFilename;

    exportProcessor.setInputFilename(this->filename);
    exportProcessor.clearRanges();
    for (pair<qint64, qint64> range : ranges)
    {
        exportProcessor.addRange(range.first, range.second);
    }

    vector<QString> generatedFiles = exportProcessor.getFilenames(outputFilename);
    vector<QString> existingFiles = checkExistingFiles(generatedFiles);

    if (existingFiles.size() > 0)
    {
        QMessageBox message;
        message.setText("This will override an existing file");
        message.exec();
        return;
    }

    // open export process dialog
    exportDialog = make_shared<ExportDialog>(this, ranges.size());
    connect(exportDialog.get(), SIGNAL(canceled()), SLOT(on_exportCancelled()));
    exportDialog->open();

    exportProcessor.process(outputFilename);
}

void MainWindow::on_exportCancelled()
{
    qDebug() << "attempted cancel";
    // todo: implement
}

void MainWindow::on_exportedItem(int itemIdx)
{
    qDebug() << "Exported file number " << itemIdx;
    exportDialog->incrementItem();
}

void MainWindow::on_exportFinished()
{
    QMessageBox msgBox;
    msgBox.setText("Export complete");

    qDebug() << "Export complete" << endl;
    exportDialog->close();
    msgBox.exec();
}

void MainWindow::on_exportProgress(int progress)
{
    exportDialog->setValue(progress);
}

void MainWindow::on_snapshotButton_clicked()
{
    if (!this->fileExists())
    {
        QMessageBox message;
        message.setText("Source file does not exist");
        message.exec();
        return;
    }

    videoPlayer->pause();

    QString outputFilename = QFileDialog::getSaveFileName(
                this, "Save File", getFileDialogLocation(),
                "PNG (*.png);;JPEG (*.jpeg)");

    if (outputFilename == "") return;

    qint64 position = videoPlayer->position();

    if (exportVideoFrame(this->filename, outputFilename, position))
    {
        QMessageBox message;
        message.setText("Finished");
        message.exec();
    }
    else
    {
        QMessageBox message;
        message.setText("Write failed");
        message.exec();
    }
}

void MainWindow::on_skipBackwardsButton_clicked()
{
    skipAmount(-SEEK_JUMP);
}

void MainWindow::on_skipForwardsButton_clicked()
{
    skipAmount(SEEK_JUMP);
}

void MainWindow::on_menuOpen_triggered()
{
    videoPlayer->pause();
    QString filename = QFileDialog::getOpenFileName(
                this, "Open Video", getFileDialogLocation(),
                "Video Files (*.mp4 *.mkv *.avi);;All files (*)");
    if (filename == "") return;

    this->openFile(filename);
}

void MainWindow::on_menuUnload_triggered()
{
    this->closeVideo();
}

void MainWindow::on_menuExit_triggered()
{
    this->closeVideo();
    this->close();
}

void MainWindow::on_changeAudioTrackTriggered(QAction *source)
{
    int trackIdx = source->property("trackIdx").toInt();
    qDebug() << "Changing to track " << trackIdx;
    this->videoPlayer->setAudioStream(trackIdx);

    // workaround for a video freeze bug
    this->videoPlayer->seek(this->videoPlayer->position());
}

void MainWindow::on_menuAbout_triggered()
{
    QMessageBox message(this);
    message.setWindowTitle("About " + APP_NAME);
    message.setTextFormat(Qt::RichText);
    message.setText("An open source cropping application application written by Carlos Fernandez. "
                    "Uses <a href=\"http://www.qtav.org/\">QtAV</a> and <a href=\"https://ffmpeg.org/\">FFmpeg</a>. "
                    "Main site available <a href=\"https://carlosfdez.github.io/videocrop/\">here</a>.");
    message.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDropEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QActionGroup>
#include <memory>
#include <QtAV>

#include <exportdialog.h>
#include <videoseekbar.h>
#include <videoexportprocessor.h>
#include <rangecontainer.h>

using namespace std;

namespace Ui {
class MainWindow;
}

/**
 * @brief The main window for the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// Returns true if the video is loaded
    bool isLoaded();

    /// Returns a path to the location for any open/save dialogs
    QString getFileDialogLocation();

    /// Programmatically opens a video file
    void openFile(const QString& filename);

    /// Programmatically closes any open videos
    void closeVideo();

protected:
    /// Used to enable/disable controls for loaded/unloaded
    /// (todo: may replace with state machine?)
    void updateControls(bool isLoaded);

    /// Returns true if a file is open and it exists
    bool fileExists();

    /// Skips a certain amount of video. Can be positive or negative
    void skipAmount(qint64 skipAmount);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *evt);
    virtual void mousePressEvent(QMouseEvent *event);

    /// Internal function to update the textfield based on the range collection
    virtual void syncRangesToText();

private slots:
    void on_playerLoaded();
    void on_playerAudioTracksLoaded(QVariantList tracks);
    void on_playerStateChanged(QtAV::AVPlayer::State state);
    void on_seeked();

    void on_togglePlayButton_clicked();

    void on_rangeInput_textChanged();

    void on_speedDecreaseButton_clicked();
    void on_speedIncreaseButton_clicked();
    void on_trimLeftButton_clicked();
    void on_splitMiddleButton_clicked();
    void on_trimRightButton_clicked();

    void on_exportButton_clicked();

    void on_exportCancelled();
    void on_exportedItem(int item);
    void on_exportFinished();
    void on_exportProgress(int progress);

    void on_snapshotButton_clicked();

    void on_skipBackwardsButton_clicked();

    void on_skipForwardsButton_clicked();

    /* file menu */
    void on_menuOpen_triggered();
    void on_menuUnload_triggered();
    void on_menuExit_triggered();

    /* player menu (note: some are binded in the ui's signal/slots mechanism) */
    void on_changeAudioTrackTriggered(QAction* source);

    void on_menuAbout_triggered();

private:
    Ui::MainWindow *ui;

    // widgets (memory managed by QT)
    VideoSeekBar *seekbar;

    shared_ptr<QtAV::VideoOutput> videoOutput;
    shared_ptr<QtAV::AVPlayer> videoPlayer;
    qint64 seekPosition = -1;

    // information about the loaded video
    QString filename;
    RangeContainer ranges;

    // replaced on every reload
    shared_ptr<QActionGroup> menuAudioTracksGroup;

    shared_ptr<ExportDialog> exportDialog;
    VideoExportProcessor exportProcessor;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDropEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QString>
#include <memory>
#include <QtAV>
#include <QProgressDialog>

#include <videoseekbar.h>
#include <videoexportprocessor.h>

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

    /// Programmatically opens a video file
    void openFile(const QString& filename);

    /// Programmatically closes any open videos
    void close();

protected:
    /// Skips a certain amount of video. Can be positive or negative
    void skipAmount(qint64 skipAmount);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *evt);
    virtual void keyPressEvent(QKeyEvent *);

    /// Internal function to update the textfield based on the range collection
    virtual void syncRangesToText();

    /// Gets the idx of the region containing position, or the one before
    int getRegionAtOrBeforeIdx(qint64 position);

private slots:
    void on_playerLoaded();
    void on_playerPositionChanged(qint64 position);
    void on_playerStateChanged(QtAV::AVPlayer::State state);
    void on_seeked();

    void on_togglePlayButton_clicked(bool);
    void on_seekbar_seek(qint64 position);

    void on_rangeInput_textChanged();

    void on_speedDecreaseButton_clicked();

    void on_speedIncreaseButton_clicked();

    void on_trimLeftButton_clicked();

    void on_splitMiddleButton_clicked();

    void on_trimRightButton_clicked();

    void on_unloadButton_clicked();
    void on_exportButton_clicked();

    void on_exportCancelled();
    void on_exportedItem(int item);
    void on_exportFinished();
    void on_exportProgress(int progress);

private:
    Ui::MainWindow *ui;

    // widgets (memory managed by QT)
    VideoSeekBar *seekbar;

    shared_ptr<QtAV::VideoOutput> videoOutput;
    shared_ptr<QtAV::AVPlayer> videoPlayer;
    qint64 seekPosition = -1;

    QString filename;
    vector<pair<qint64, qint64>> ranges;

    shared_ptr<QProgressDialog> exportDialog;
    VideoExportProcessor exportProcessor;
};

#endif // MAINWINDOW_H

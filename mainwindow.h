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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openFile(const QString& filename);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *evt);
    virtual void keyPressEvent(QKeyEvent *);

private slots:
    void on_playerLoaded();
    void on_playerPositionChanged(qint64 position);
    void on_playerStateChanged(QtAV::AVPlayer::State state);
    void on_togglePlayButton_clicked(bool);
    void on_seek(qint64 position);

    void on_rangeInput_textChanged();

    void on_exportButton_clicked();

    void on_exportCancelled();
    void on_exportedItem(int item);
    void on_exportFinished();
    void on_exportProgress(int progress);

    void on_speedDecreaseButton_clicked();

    void on_speedIncreaseButton_clicked();

private:
    Ui::MainWindow *ui;

    // widgets (memory managed by QT)
    VideoSeekBar *seekbar;

    shared_ptr<QtAV::VideoOutput> videoOutput;
    shared_ptr<QtAV::AVPlayer> videoPlayer;

    QString filename;
    vector<pair<qint64, qint64>> ranges;

    shared_ptr<QProgressDialog> exportDialog;
    VideoExportProcessor exportProcessor;
};

#endif // MAINWINDOW_H

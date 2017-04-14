#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QWidget>
#include <QtAV>
#include <QVariantList>
#include <memory>

using namespace std;

/**
 * @brief A wrapper over the QtAV video player and widget.
 * This class exists both to add additional behavior and bug fixes
 * and allow a way to change the implementation if we want to in the future.
 */
class VideoPlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlayerWidget(QWidget *parent = 0);

    void load(QString video);
    void unload();

    void play();
    void pause();
    void togglePause();
    void seek(qint64 position);
    void skipAmount(qint64 skipAmount);

    bool isLoaded();
    bool isPlaying();
    bool isPaused();

    qint64 position();
    qint64 duration();

    qreal speed();
    void setSpeed(qreal speed);

    void setAudioStream(int idx);

    void setScrubbing(bool scrubbing);

protected:
    /// Used to test if the state needs to be changed, and if it does,
    /// perform the change and emit the event
    void testStateChanged();

signals:
    void loaded();
    void unloaded();
    void positionChanged(qint64);
    void stateChanged(QtAV::AVPlayer::State);
    void seekFinished();

    // todo: combine with loaded
    void audioTracksLoaded(QVariantList);

public slots:


private slots:
    void inner_loaded();
    void inner_stateChanged(QtAV::AVPlayer::State);
    void inner_seekFinished();
    void inner_positionChanged(qint64);

private:
    shared_ptr<QtAV::AVPlayer> videoPlayer;
    shared_ptr<QtAV::VideoOutput> videoOutput;

    QtAV::AVPlayer::State state;

    qint64 seekPosition = -1;
};

#endif // VIDEOPLAYERWIDGET_H

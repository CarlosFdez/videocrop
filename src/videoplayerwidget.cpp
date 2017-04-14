#include "videoplayerwidget.h"

#include <QLayout>
#include <QMessageBox>

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent) : QWidget(parent)
{
    videoOutput = make_shared<QtAV::VideoOutput>(this);
    if (!videoOutput->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }

    videoPlayer = make_shared<QtAV::AVPlayer>();
    videoPlayer->setRenderer(videoOutput.get());
    videoPlayer->setMediaEndAction(QtAV::MediaEndActionFlag::MediaEndAction_Pause);
    videoPlayer->setSeekType(QtAV::SeekType::AccurateSeek);

    // set video widget
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(videoOutput->widget());
    this->setLayout(layout);

    connect(videoPlayer.get(), SIGNAL(stopped()), this, SIGNAL(unloaded()));
    connect(videoPlayer.get(), SIGNAL(internalAudioTracksChanged(QVariantList)), this, SIGNAL(audioTracksLoaded(QVariantList)));

    connect(videoPlayer.get(), SIGNAL(loaded()), SLOT(inner_loaded()));
    connect(videoPlayer.get(), SIGNAL(positionChanged(qint64)), SLOT(inner_positionChanged(qint64)));
    connect(videoPlayer.get(), SIGNAL(stateChanged(QtAV::AVPlayer::State)), SLOT(inner_stateChanged(QtAV::AVPlayer::State)));
    connect(videoPlayer.get(), SIGNAL(seekFinished(qint64)), SLOT(inner_seekFinished()));
}

void VideoPlayerWidget::load(QString video)
{
    if (videoPlayer->isPlaying())
        videoPlayer->stop();

    videoPlayer->setFile(video);
    videoPlayer->load();
}

void VideoPlayerWidget::unload()
{
    videoPlayer->stop();
}

void VideoPlayerWidget::play()
{
    videoPlayer->pause(false);
}

void VideoPlayerWidget::pause()
{
    videoPlayer->pause(true);
}

void VideoPlayerWidget::togglePause()
{
    (this->isPaused()) ? this->play() : this->pause();
}

void VideoPlayerWidget::seek(qint64 position)
{
    videoPlayer->seek(position);
}

void VideoPlayerWidget::skipAmount(qint64 skipAmount)
{
    qint64 position = (seekPosition > -1) ? seekPosition: videoPlayer->position();
    seekPosition = position + skipAmount;
    videoPlayer->seek(seekPosition);
}

bool VideoPlayerWidget::isLoaded()
{
    return videoPlayer->isLoaded();
}

bool VideoPlayerWidget::isPlaying()
{
    // There's a bug in QtAV where it is possible to be paused and playing.
    // If paused is ever true, we want to declare false
    if (isPaused())
        return false;
    return videoPlayer->isPlaying();
}

bool VideoPlayerWidget::isPaused()
{
    return videoPlayer->isPaused();
}

qint64 VideoPlayerWidget::position()
{
    return videoPlayer->position();
}

qint64 VideoPlayerWidget::duration()
{
    return videoPlayer->duration();
}

qreal VideoPlayerWidget::speed()
{
    return videoPlayer->speed();
}

void VideoPlayerWidget::setSpeed(qreal speed)
{
    videoPlayer->setSpeed(speed);
}

void VideoPlayerWidget::setAudioStream(int idx)
{
    videoPlayer->setAudioStream(idx);

    // workaround for a video freeze bug
    videoPlayer->seek(videoPlayer->position());
}

void VideoPlayerWidget::setScrubbing(bool scrubbing)
{
    if (scrubbing)
    {
        videoPlayer->pause();
    }

    videoPlayer->setSeekType((scrubbing)
                             ? QtAV::SeekType::KeyFrameSeek
                             : QtAV::SeekType::AccurateSeek);
}

void VideoPlayerWidget::testStateChanged()
{
    QtAV::AVPlayer::State newState = (isPaused())
            ? QtAV::AVPlayer::State::PausedState
            : QtAV::AVPlayer::State::PlayingState;

    if (this->state != newState)
    {
        this->state = newState;
        emit stateChanged(this->state);
    }
}

void VideoPlayerWidget::inner_loaded()
{
    setScrubbing(false);
    emit loaded();

    // player has a different definition in QtAV: the stream starts on play.
    // QtAV calls resuming pause(false) instead of play()
    // we start consuming the stream after its loaded
    videoPlayer->play();
}

void VideoPlayerWidget::inner_stateChanged(QtAV::AVPlayer::State)
{
    testStateChanged();
}

void VideoPlayerWidget::inner_seekFinished()
{
    this->seekPosition = -1;
    emit seekFinished();
}

void VideoPlayerWidget::inner_positionChanged(qint64 position)
{
    // there's a bug in QtAV stable where reaching the end of the video
    // in end action pause does not trigger a state change, so we need
    // to expose the state change ourselves if we have to.
    testStateChanged();
    emit positionChanged(position);
}

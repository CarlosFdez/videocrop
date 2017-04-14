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

    // set video widget
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(videoOutput->widget());
    this->setLayout(layout);

    connect(videoPlayer.get(), SIGNAL(loaded()), this, SIGNAL(loaded()));
    connect(videoPlayer.get(), SIGNAL(stopped()), this, SIGNAL(unloaded()));
    connect(videoPlayer.get(), SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
    connect(videoPlayer.get(), SIGNAL(internalAudioTracksChanged(QVariantList)), this, SIGNAL(audioTracksLoaded(QVariantList)));

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
    videoPlayer->play();
}

void VideoPlayerWidget::pause()
{
    videoPlayer->pause();
}

void VideoPlayerWidget::togglePause()
{
    videoPlayer->togglePause();
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
    return !videoPlayer->isPaused() && videoPlayer->isPlaying();
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

void VideoPlayerWidget::setSeekType(QtAV::SeekType seekType)
{
    videoPlayer->setSeekType(seekType);
}

void VideoPlayerWidget::inner_stateChanged(QtAV::AVPlayer::State state)
{
    emit stateChanged(state);
}

void VideoPlayerWidget::inner_seeked()
{
    this->seekPosition = -1;
    emit seekFinished();
}

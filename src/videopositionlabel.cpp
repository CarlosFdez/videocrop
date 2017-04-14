#include "videopositionlabel.h"

#include <QVBoxLayout>
#include "videoplayerwidget.h"
#include "util.h"

VideoPositionLabel::VideoPositionLabel(QWidget *parent) : QLabel(parent)
{
}

void VideoPositionLabel::bindPlayer(VideoPlayerWidget *player)
{
    if (videoPlayer != nullptr)
    {
        qWarning() << "Unbinding currently doesn't exist";
    }

    videoPlayer = player;
    connect(videoPlayer, SIGNAL(unloaded()), SLOT(on_playerUnloaded()));
    connect(videoPlayer, SIGNAL(positionChanged(qint64)), SLOT(on_playerPositionChanged(qint64)));
}

void VideoPositionLabel::on_playerUnloaded()
{
    this->setText("");
}

void VideoPositionLabel::on_playerPositionChanged(qint64 position)
{
    bool hasHours = testHasHours(videoPlayer->duration());
    QString positionTime = millisecondsToTimestamp(position, hasHours);
    QString totalTime = millisecondsToTimestamp(videoPlayer->duration(), hasHours);
    qreal speed = videoPlayer->speed();

    QString label = QString("(x%1) %2 / %3").arg(speed).arg(positionTime, totalTime);
    this->setText(label);
}

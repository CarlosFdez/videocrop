#include "videoseekbar.h"

#include <QRect>
#include <QPainter>
#include <QtDebug>

#include <algorithm>

// todo: define some other way (stylesheet?)
const int RANGE_BORDER_WIDTH = 3;
const QColor BASE_COLOR(130, 130, 130);
const QColor RANGE_COLOR(60, 60, 255);
const QColor RANGE_BORDER_COLOR(50, 50, 232);
const QColor SEEK_LINE_COLOR(255, 0, 0);

VideoSeekBar::VideoSeekBar(QWidget *parent) : QWidget(parent)
{
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
}

void VideoSeekBar::setPosition(qint64 milliseconds)
{
    this->position = milliseconds;
    this->repaint();
    emit positionChanged(milliseconds);
}

void VideoSeekBar::setVideoLength(qint64 milliseconds)
{
    this->videoLength = milliseconds;
    this->repaint();
}

void VideoSeekBar::bindRangeContainer(RangeContainer *ranges)
{
    if (this->ranges != nullptr)
    {
        qWarning() << "Unbinding currently doesn't exist";
    }

    this->ranges = ranges;
    connect(ranges, SIGNAL(changed()), SLOT(on_rangeChanged()));
}

void VideoSeekBar::bindPlayer(VideoPlayerWidget *player)
{
    if (videoPlayer != nullptr)
    {
        qWarning() << "Unbinding currently doesn't exist";
    }

    this->videoPlayer = player;
    connect(videoPlayer, SIGNAL(loaded()), SLOT(on_playerLoaded()));
    connect(videoPlayer, SIGNAL(unloaded()), SLOT(on_playerUnloaded()));
    connect(videoPlayer, SIGNAL(positionChanged(qint64)), SLOT(on_playerPositionChanged(qint64)));
}

void VideoSeekBar::mousePressEvent(QMouseEvent *evt)
{
    if (this->videoLength > 0
            && evt->button() == Qt::MouseButton::LeftButton)
    {
        qint64 position = (evt->x() * 1.0 / this->width()) * videoLength;
        this->performSeek(position);
    }
    QWidget::mousePressEvent(evt);
}

void VideoSeekBar::mouseMoveEvent(QMouseEvent *evt)
{
    if (this->videoLength > 0
            && evt->buttons() & Qt::MouseButton::LeftButton)
    {
        qint64 position = (evt->x() * 1.0 / this->width()) * videoLength;
        this->performSeek(position);

        if (videoPlayer && !scrubbing)
        {
            scrubbing = true;

            // pause video, and attempt to improve its scrub performance
            videoPlayer->setScrubbing(true);

            emit startScrubbing();
        }
    }

    QWidget::mouseMoveEvent(evt);
}

void VideoSeekBar::mouseReleaseEvent(QMouseEvent *evt)
{
    if (this->videoLength > 0
            && evt->button() == Qt::MouseButton::LeftButton)
    {
        // empty in case we want to add anything else

        if (videoPlayer && scrubbing)
        {
            scrubbing = false;

            // restore video seek mechanism
            videoPlayer->setScrubbing(false);

            emit stopScrubbing();
        }
    }

    QWidget::mouseReleaseEvent(evt);
}

void VideoSeekBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int width = this->width();
    int height = this->height();

    int lineThickness = 2;
    int gutterSize = 10;

    // draw background color
    QRect rectangle(0, gutterSize, this->width(), this->height() - gutterSize);
    painter.fillRect(rectangle, BASE_COLOR);

    QPen rangeBorderPen(RANGE_BORDER_COLOR, RANGE_BORDER_WIDTH,
                        Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);

    for (Range p : *this->ranges)
    {
        int start = (p.start * 1.0 / videoLength) * width;
        int finish = (p.end * 1.0 / videoLength) * width;

        QRectF rangeRect(start, gutterSize,
                         finish - start, this->height() - gutterSize);

        // draw filled range
        painter.fillRect(rangeRect, RANGE_COLOR);

        // draw border of range
        painter.setPen(rangeBorderPen);
        rangeRect -= (QMarginsF() + RANGE_BORDER_WIDTH/2.0);
        painter.drawRect(rangeRect);
    }

    // draw seek line
    if (this->videoLength > 0)
    {
        int lineDrawPosition = (this->position * 1.0 / this->videoLength) * width;
        QRect line(lineDrawPosition - lineThickness / 2, 0, lineThickness, height);
        painter.fillRect(line, SEEK_LINE_COLOR);
    }
}

void VideoSeekBar::performSeek(qint64 position)
{
    // clamp to min/max
    position = max((qint64)0, position);
    position = min(this->videoLength, position);

    this->setPosition(position);
    if (videoPlayer)
    {
        videoPlayer->seek(position);
    }
    emit positionSeeked(position);
}

void VideoSeekBar::on_playerLoaded()
{
    this->setVideoLength(videoPlayer->duration());
}

void VideoSeekBar::on_playerUnloaded()
{
    this->setVideoLength(0);
}

void VideoSeekBar::on_playerPositionChanged(qint64 position)
{
    this->setPosition(position);
}

void VideoSeekBar::on_rangeChanged()
{
    this->repaint();
}

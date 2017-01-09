#include "videoseekbar.h"

#include <QRect>
#include <QPainter>
#include <QtDebug>

// todo: define some other way (stylesheet?)
const int RANGE_BORDER_WIDTH = 3;
const QColor BASE_COLOR(130, 130, 130);
const QColor RANGE_COLOR(60, 60, 255);
const QColor RANGE_BORDER_COLOR(50, 50, 232);
const QColor SEEK_LINE_COLOR(255, 0, 0);

VideoSeekBar::VideoSeekBar()
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

void VideoSeekBar::setRangeContainer(RangeContainer& ranges)
{
    this->ranges = &ranges;
}

void VideoSeekBar::mousePressEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::MouseButton::LeftButton)
    {
        qint64 position = (evt->x() * 1.0 / this->width()) * videoLength;
        setPosition(position);
        emit positionSeeked(position);
    }
}

void VideoSeekBar::mouseMoveEvent(QMouseEvent *evt)
{
    if (evt->buttons() && Qt::MouseButton::LeftButton)
    {
        qint64 position = (evt->x() * 1.0 / this->width()) * videoLength;
        setPosition(position);
        emit positionSeeked(position);
    }
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

    for (pair<qint64, qint64> p : *this->ranges)
    {
        int start = (p.first * 1.0 / videoLength) * width;
        int finish = (p.second * 1.0 / videoLength) * width;

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

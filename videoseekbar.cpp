#include "videoseekbar.h"

#include <QRect>
#include <QPainter>
#include <QtDebug>

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
}

void VideoSeekBar::setRanges(vector<pair<qint64, qint64> > ranges)
{
    this->ranges = ranges;
    this->repaint();
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

    // dark backdrop
    QRect rectangle(0, gutterSize, this->width(), this->height() - gutterSize);
    painter.fillRect(rectangle, QColor(130, 130, 130));

    for (pair<qint64, qint64> p : this->ranges)
    {
        int start = (p.first * 1.0 / videoLength) * width;
        int finish = (p.second * 1.0 / videoLength) * width;

        QRect rectangle(start, gutterSize, finish - start, this->height() - gutterSize);
        painter.fillRect(rectangle, QColor(60, 60, 255));
    }

    // draw seek line
    if (this->videoLength > 0)
    {
        int lineDrawPosition = (this->position * 1.0 / this->videoLength) * width;
        QRect line(lineDrawPosition - lineThickness / 2, 0, lineThickness, height);
        painter.fillRect(line, QColor(255, 0, 0));
    }
}

#ifndef VIDEOSEEKBAR_H
#define VIDEOSEEKBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <vector>
#include <rangecontainer.h>

using namespace std;

class VideoSeekBar : public QWidget
{
    Q_OBJECT

public:
    VideoSeekBar();
    void setPosition(qint64 milliseconds);
    void setVideoLength(qint64 milliseconds);
    void setRangeContainer(RangeContainer& ranges);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    /// fired when the postion changes for any reason.
    /// This may happen on seek attempt or if setPosition is called.
    void positionChanged(qint64 milliseconds);

    /// fired when the position changes due to an attempt at seeking
    void positionSeeked(qint64 milliseconds);

    /// emitted when scrubbing starts
    void startScrubbing();

    /// emitted when scrubbing stops
    void stopScrubbing();

private:
    bool scrubbing = false;
    RangeContainer *ranges;
    qint64 position;
    qint64 videoLength;
};

#endif // VIDEOSEEKBAR_H

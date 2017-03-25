#ifndef VIDEOSEEKBAR_H
#define VIDEOSEEKBAR_H

#include <QtAV>
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
    explicit VideoSeekBar(QWidget *parent = 0);
    void setPosition(qint64 milliseconds);
    void setVideoLength(qint64 milliseconds);
    void setRangeContainer(RangeContainer& ranges);

    void bindTo(QtAV::AVPlayer *videoPlayer);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void mouseReleaseEvent(QMouseEvent *evt);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void performSeek(qint64 position);

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

protected slots:
    void on_playerLoaded();
    void on_playerUnloaded();
    void on_playerPositionChanged(qint64 position);

private:
    QtAV::AVPlayer *videoPlayer = nullptr;

    bool scrubbing = false;
    RangeContainer *ranges;
    qint64 position = 0;
    qint64 videoLength = 0;
};

#endif // VIDEOSEEKBAR_H

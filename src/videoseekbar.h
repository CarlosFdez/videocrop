#ifndef VIDEOSEEKBAR_H
#define VIDEOSEEKBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <rangecontainer.h>
#include <videoplayerwidget.h>

/// Defines a  user element that manages the video progress bar
/// and allows the user to seek a position or scrub the video.
class VideoSeekBar : public QWidget
{
    Q_OBJECT

public:
    explicit VideoSeekBar(QWidget *parent = 0);
    void setPosition(qint64 milliseconds);
    void setVideoLength(qint64 milliseconds);

    void bindRangeContainer(RangeContainer *ranges);
    void bindPlayer(VideoPlayerWidget *videoPlayer);

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
    void on_rangeChanged();

private:
    VideoPlayerWidget *videoPlayer = nullptr;
    RangeContainer *ranges = nullptr;

    bool scrubbing = false;
    qint64 position = 0;
    qint64 videoLength = 0;
};

#endif // VIDEOSEEKBAR_H

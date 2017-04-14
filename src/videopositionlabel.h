#ifndef VIDEOPOSITIONLABEL_H
#define VIDEOPOSITIONLABEL_H

#include <QWidget>
#include <QLabel>
#include <videoplayerwidget.h>

class VideoPositionLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VideoPositionLabel(QWidget *parent = 0);

    void bindPlayer(VideoPlayerWidget *player);

protected slots:
    void on_playerUnloaded();
    void on_playerPositionChanged(qint64 position);

private:
    VideoPlayerWidget *videoPlayer = nullptr;
};

#endif // VIDEOPOSITIONLABEL_H

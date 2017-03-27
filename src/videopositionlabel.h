#ifndef VIDEOPOSITIONLABEL_H
#define VIDEOPOSITIONLABEL_H

#include <QWidget>
#include <QLabel>
#include <QtAV>

class VideoPositionLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VideoPositionLabel(QWidget *parent = 0);

    void bindPlayer(QtAV::AVPlayer *player);

protected slots:
    void on_playerLoaded();
    void on_playerUnloaded();
    void on_playerPositionChanged(qint64 position);

private:
    QtAV::AVPlayer *videoPlayer = nullptr;
};

#endif // VIDEOPOSITIONLABEL_H

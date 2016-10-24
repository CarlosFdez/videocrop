#ifndef VIDEOSEEKBAR_H
#define VIDEOSEEKBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <vector>

using namespace std;

class VideoSeekBar : public QWidget
{
    Q_OBJECT

public:
    VideoSeekBar();
    void setPosition(qint64 milliseconds);
    void setVideoLength(qint64 milliseconds);
    void setRanges(vector<pair<qint64, qint64>> ranges);

signals:
    // fired when the postion changes for any reason
    void positionChanged(qint64 milliseconds);

    // fired when the position changes due to an attempt at seeking
    void positionSeeked(qint64 milliseconds);

protected:
    void mousePressEvent(QMouseEvent *evt);
    void mouseMoveEvent(QMouseEvent *evt);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    vector<pair<qint64, qint64>> ranges;
    qint64 position;
    qint64 videoLength;
};

#endif // VIDEOSEEKBAR_H

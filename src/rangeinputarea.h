#ifndef RANGEINPUTAREA_H
#define RANGEINPUTAREA_H

#include <QObject>
#include <QPlainTextEdit>
#include "rangecontainer.h"

class RangeInputArea : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit RangeInputArea(QWidget *parent = 0);

    void bindRangeContainer(RangeContainer *ranges);

protected slots:
    void on_textChanged();
    void on_rangeChanged();

private:
    RangeContainer *ranges = nullptr;

    bool suppressUpdate = false;
};

#endif // RANGEINPUTAREA_H

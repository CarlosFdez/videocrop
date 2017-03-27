#include "rangeinputarea.h"

#include <QDebug>
#include <QString>
#include "util.h"

RangeInputArea::RangeInputArea(QWidget *parent) : QPlainTextEdit(parent)
{
    connect(this, SIGNAL(textChanged()), SLOT(on_textChanged()));
}

void RangeInputArea::bindRangeContainer(RangeContainer *ranges)
{
    if (this->ranges != nullptr)
    {
        qWarning() << "Unbinding currently doesn't exist";
    }

    this->ranges = ranges;
    connect(ranges, SIGNAL(changed()), SLOT(on_rangeChanged()));
}

void RangeInputArea::on_textChanged()
{
    // todo: toggle using RAII instead in case of error or something
    suppressUpdate = true;

    ranges->clear();

    QString text = this->property("plainText").toString();
    QStringList lines = text.split('\n');
    for (QString line : lines)
    {
        QStringList sections = line.split(',');
        if (sections.count() != 2)
            continue;

        qint64 first = timestampToMilliseconds(sections[0].trimmed());
        qint64 second = timestampToMilliseconds(sections[1].trimmed());

        if (first < 0 || second < 0)
            continue;

        ranges->add(first, second);
    }

    suppressUpdate = false;
}

void RangeInputArea::on_rangeChanged()
{
    // skip if we're suppressing. This happens if we're the ones changing it...
    if (suppressUpdate) return;

    this->property("plainText").toString();
    QString newText = "";
    for (Range range : *ranges)
    {
        QString first = millisecondsToTimestamp(range.start, false);
        QString second = millisecondsToTimestamp(range.end, false);
        newText += first + "," + second + "\n";
    }

    this->setPlainText(newText);
}

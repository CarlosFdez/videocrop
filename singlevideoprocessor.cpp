#include "singlevideoprocessor.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QRegularExpressionMatch>

#include <util.h>

SingleVideoProcessor::SingleVideoProcessor(QObject *parent) : QObject(parent)
{
    regex.setPattern("time=([0-9:.]+)");

    connect(&backgroundProcess, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(on_finished(int, QProcess::ExitStatus)));
    connect(&backgroundProcess, SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(&backgroundProcess, SIGNAL(readyReadStandardError()),this,SLOT(readyReadStandardError()));
}


void SingleVideoProcessor::process(
        const QString &inputFile, const QString &outputFile, qint64 start, qint64 end)
{
    this->startPosition = start;
    this->duration = end - start;

    // starts before the point, and duration makes it finish after the span
    QString startStr = millisecondsToTimestamp(start, false);
    QString durationStr = millisecondsToTimestamp(end - start, false);

    qDebug() << "range " << startStr << " duration " << durationStr;

    QStringList arguments;
    arguments << "-ss" << startStr;
    arguments << "-i" << inputFile;
    arguments << "-t" << durationStr;
    arguments << "-map" << "0";
    arguments << "-codec" << "copy";
    arguments << outputFile;

    backgroundProcess.start("ffmpeg", arguments);
}

void SingleVideoProcessor::on_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    emit finished();
}

void SingleVideoProcessor::readyReadStandardOutput()
{
    qDebug() << backgroundProcess.readAllStandardOutput();
}

void SingleVideoProcessor::readyReadStandardError()
{
    // progress information is in the standard error
    QString line = backgroundProcess.readAllStandardError();
    qDebug() << line;

    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch())
    {
        QString timestamp = match.captured(1);
        qint64 position = timestampToMilliseconds(timestamp);

        int progressValue = (position * 1.0 / duration) * 100;
        qDebug() << "position " << position << "start " << startPosition
                 << "duration " << duration << "progress" << progressValue;
        emit progress(progressValue);
    }
}

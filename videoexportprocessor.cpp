#include "videoexportprocessor.h"

#include <QDebug>
#include <QString>
#include <QProcess>
#include <vector>
#include <math.h>

#include "util.h"

VideoExportProcessor::VideoExportProcessor(QObject *parent) :
     QObject(parent),
     processor(this)
{
    connect(&processor, SIGNAL(finished()), SLOT(on_finishedItem()));
    connect(&processor, SIGNAL(progress(int)), SLOT(on_itemProgress(int)));
}

void VideoExportProcessor::setInputFilename(const QString &filename)
{
    this->filename = filename;
}

void VideoExportProcessor::clearRanges()
{
    this->ranges.clear();
}

void VideoExportProcessor::addRange(qint64 start, qint64 end)
{
    this->ranges.push_back(pair<qint64, qint64>(start, end));
}

vector<QString> VideoExportProcessor::getFilenames(const QString &exportPath)
{
    vector<QString> results;
    if (ranges.size() == 1)
        results.push_back(exportPath);
    else
    {
        int extPosition = exportPath.lastIndexOf('.');
        QString basePath = exportPath.left(extPosition);
        QString extension = exportPath.right(exportPath.size() - extPosition);

        qDebug() << "Export Base Path " << basePath;
        qDebug() << "Export Extension " << extension;

        for (int i = 0; i < ranges.size(); i++)
        {
            auto path = QString("%1-%2%3").arg(basePath).arg(i).arg(extension);
            results.push_back(path);
        }
    }
    return results;
}

void VideoExportProcessor::process(const QString& exportPath)
{
    if (ranges.size() == 0)
    {
        emit finishedAll();
        return;
    }

    processedDuration = 0;
    totalDuration = 0;
    processQueue = make_shared<queue<int>>();
    this->filesToProcess = getFilenames(exportPath);
    for (int i = 0; i < ranges.size(); i++)
    {
        processQueue->push(i);
        totalDuration += ranges[i].second - ranges[i].first;
    }

    processNext();
}

void VideoExportProcessor::processNext()
{
    this->currentItem = processQueue->front();
    processQueue->pop();

    int start = ranges[currentItem].first;
    int end = ranges[currentItem].second;
    processor.process(this->filename, this->filesToProcess[currentItem], start, end);
}

void VideoExportProcessor::on_finishedItem()
{
    processedDuration += ranges[currentItem].second - ranges[currentItem].first;

    emit finishedItem(currentItem);
    if (processQueue->empty())
    {
        processQueue = nullptr;
        emit finishedAll();
    }
    else
    {
        processNext();
    }
}

void VideoExportProcessor::on_itemProgress(int itemProgress)
{
    // handles rounding
    if (processedDuration == totalDuration)
    {
        emit progress(100);
        return;
    }

    double completedProgress = (processedDuration * 1.0 / totalDuration) * 100;
    qint64 currentDuration = ranges[currentItem].second - ranges[currentItem].first;
    double currentWeight = currentDuration * 1.0 / totalDuration;

    int totalProgress = floor(completedProgress + (itemProgress * currentWeight));

    emit progress(totalProgress);
}

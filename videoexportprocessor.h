#ifndef VIDEOCUTTER_H
#define VIDEOCUTTER_H

#include <QString>
#include <QProcess>
#include <vector>
#include <queue>
#include <memory>

#include <singlevideoprocessor.h>

using namespace std;

class VideoExportProcessor : public QObject
{
    Q_OBJECT

public:
    explicit VideoExportProcessor(QObject *parent);

    void setFilename(const QString& filename);

    // adds a range in milliseconds
    void addRange(qint64 start, qint64 end);

    /// Sets the ranges that will be processed, each one a [start, end] range in milliseconds
    void setRanges(const vector<pair<qint64, qint64>>& ranges);

    // Calculates export filepaths using the export path.
    // If there is only one item, its unchanged.
    // If there is more than one, its base-{idx}.{extension}
    // Assumes that the exportPath ends with an extension
    vector<QString> getFilenames(const QString& exportPath);

    void process(const QString& exportPath);

signals:
    void progress(int progress);
    void finishedItem(int idx);
    void finishedAll();

protected:
    void processNext();

protected slots:
    void on_finishedItem();
    void on_itemProgress(int progress);

private:
    QString filename;
    vector<pair<qint64, qint64>> ranges;

    SingleVideoProcessor processor;

    int currentItem;
    vector<QString> filesToProcess;
    shared_ptr<queue<int>> processQueue;
    qint64 processedDuration;
    qint64 totalDuration;
};

#endif // VIDEOCUTTER_H

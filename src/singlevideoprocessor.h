#ifndef SINGLEVIDEOPROCESSOR_H
#define SINGLEVIDEOPROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QRegularExpression>

/**
 * @brief Used to perform an export for a single range of a file.
 */
class SingleVideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit SingleVideoProcessor(QObject *parent = 0);

    /// Begins extracting of the input file to the output file along the range in a background thread.
    void process(const QString& inputFile, const QString& outputFile, qint64 start, qint64 end);

signals:
    /// Event fired when the export is complete
    void finished();

    /// Event fired when there is progress (ratio is a number between 0 and 100)
    void progress(int ratio);

protected slots:
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    bool finished_flag;
    qint64 startPosition;
    qint64 duration;

    QRegularExpression regex;
    QProcess backgroundProcess;
};

#endif // SINGLEVIDEOPROCESSOR_H

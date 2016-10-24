#ifndef SINGLEVIDEOPROCESSOR_H
#define SINGLEVIDEOPROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QRegularExpression>

class SingleVideoProcessor : public QObject
{
    Q_OBJECT
public:
    explicit SingleVideoProcessor(QObject *parent = 0);
    void process(const QString& inputFile, const QString& outputFile, qint64 start, qint64 end);

signals:
    void finished();

    // triggered when there is progress (ratio is a number between 0 and 100)
    void progress(int ratio);

protected slots:
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    qint64 startPosition;
    qint64 duration;

    QRegularExpression regex;
    QProcess backgroundProcess;
};

#endif // SINGLEVIDEOPROCESSOR_H

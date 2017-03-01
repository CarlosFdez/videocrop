#include "util.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QProcess>

using namespace std;

// returns whether the number of total seconds surpasses an hour
bool testHasHours(qint64 milliseconds)
{
    return milliseconds >= (60 * 60 * 1000);
}

QString millisecondsToTimestamp(qint64 milliseconds, bool forceHours)
{
    double totalSeconds = milliseconds / 1000.0;
    double seconds = fmod(totalSeconds, 60);
    int minutes = fmod(totalSeconds / 60, 60);
    int hours = (totalSeconds / 60 / 60);

    // note: seconds format specifier is because 4 precision and 7 characters (xx.xxxx)

    if (hours > 0 || forceHours)
        return QString().sprintf("%d:%02d:%07.4f", hours, minutes, seconds);
    else
        return QString().sprintf("%02d:%07.4f", minutes, seconds);
}

qint64 timestampToMilliseconds(const QString time)
{
    QStringList sections = time.split(':');
    bool ok = true;
    bool allOk = true;
    int count = sections.count();

    int hours = 0;
    int minutes = 0;
    double seconds = 0;

    // validation check
    if (count > 3)
        return -1;

    seconds = sections[count - 1].toDouble(&ok);
    if (!ok) allOk = false;

    if (count > 1)
    {
        minutes = sections[count - 2].toInt(&ok);
        if (!ok) allOk = false;
    }

    if (count > 2)
    {
        hours = sections[count - 3].toInt(&ok);
        if (!ok) allOk = false;
    }

    if (!allOk)
    {
        return -1;
    }

    return (hours * 60 * 60 * 1000)
            + (minutes * 60 * 1000)
            + (seconds * 1000);
}

vector<QString> checkExistingFiles(vector<QString> filesToTest)
{
    vector<QString> results;
    for (QString filename : filesToTest)
    {
        QFileInfo file(filename);
        if (file.exists())
            results.push_back(filename);
    }
    return results;
}

bool exportVideoFrame(QString inputFilename, QString outputFilename, qint64 position)
{
    QProcess backgroundProcess;
    QStringList arguments;
    arguments << "-ss" << millisecondsToTimestamp(position, false);
    arguments << "-y"; // overwrite
    arguments << "-i" << inputFilename;
    arguments << "-vframes" << "1";
    arguments << "-f" << "image2";
    arguments << outputFilename;

    backgroundProcess.start("ffmpeg", arguments);
    backgroundProcess.waitForFinished();

    QFileInfo exported(outputFilename);
    return exported.exists() && exported.isFile();
}

#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <vector>

using namespace std;

bool testHasHours(qint64 milliseconds);
QString millisecondsToTimestamp(qint64 milliseconds, bool forceHours);
qint64 timestampToMilliseconds(const QString time);

vector<QString> checkExistingFiles(vector<QString> filesToTest);

#endif // UTIL_H

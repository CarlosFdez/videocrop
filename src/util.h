#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <vector>

using namespace std;

/// Returns true if the number of milliseconds is at least an hour of time.
bool testHasHours(qint64 milliseconds);

QString millisecondsToTimestamp(qint64 milliseconds, bool forceHours);
qint64 timestampToMilliseconds(const QString time);

/// Exports a frame to an image file. Returns true if succeeded.
/// This is a blocking operation, but with a trivial delay.
bool exportVideoFrame(QString inputFile, QString outputFile, qint64 position);

/**
 * Checks if any of the test files exist in the filesystem,
 * and returns the ones that do.
 */
vector<QString> checkExistingFiles(vector<QString> filesToTest);

#endif // UTIL_H

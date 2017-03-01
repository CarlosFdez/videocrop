#ifndef RANGECONTAINER_H
#define RANGECONTAINER_H

#include <vector>
#include <Qt>

using namespace std;

/// Defines a class that contains export regions with operations that manipulate them.
class RangeContainer
{
public:
    RangeContainer();

    /// Sets the duration of the video
    void setVideoLength(qint64 milliseconds);

    /// Clears all ranges in the collection
    void clear();

    /// Adds a new range to the collection
    void add(qint64 start, qint64 end);

    /// Performs a split on any range at the given position, splitting one range into two.
    /// If there is nothing at the position, create a new range that fills the gap.
    void splitAt(qint64 position);

    /// Performs a left trim.
    /// If position intersects with a range, the starting point is moved to position
    /// If this is after the last range, creates a new range that starts at position.
    /// If this is not on a range but before one, extends that range to match position.
    void trimLeftAt(qint64 position);

    /// Performs a right trim.
    /// If position intersects with a range, the ending point is moved to position.
    /// If this is before the first range, creates a new range that starts at 0 and ends at position.
    /// If this is not on a range but after one, extends that range to position.
    void trimRightAt(qint64 position);

    /// Returns the number of ranges in the collection
    int size();

    int max_size();

    vector<pair<qint64, qint64>>::iterator begin();
    vector<pair<qint64, qint64>>::iterator end();

protected:
    /// Gets the idx of the region containing position, or the one before
    /// The region before is determined by the one with the largest end position
    /// which has a start position before position
    int getRegionAtOrBeforeIdx(qint64 position);

    /// Gets the idx of the region containing position, or the one after.
    /// The region after is determined by the one with lowest start position
    /// with an end after the position
    int getRegionAtOrAfterIdx(qint64 position);

private:
    qint64 videoLength;
    vector<pair<qint64, qint64>> ranges;
};

#endif // RANGECONTAINER_H

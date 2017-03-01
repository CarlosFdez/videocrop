#include "rangecontainer.h"

#include <QDebug>

RangeContainer::RangeContainer()
{

}

void RangeContainer::setVideoLength(qint64 milliseconds)
{
    this->videoLength = milliseconds;
}

void RangeContainer::clear()
{
    ranges.clear();
}

void RangeContainer::add(qint64 start, qint64 end)
{
    ranges.push_back(pair<qint64, qint64>(start, end));
}

int RangeContainer::getRegionAtOrBeforeIdx(qint64 position)
{
    // return the region with the highest .second
    // that has a .first at or below position
    int currentResultIdx = -1;
    for (int i = 0; i < ranges.size(); i++)
    {
        if (ranges[i].first > position)
            continue;
        else if (currentResultIdx == -1)
            currentResultIdx = i;
        else if (ranges[i].second > ranges[currentResultIdx].second)
            currentResultIdx = i;
    }

    return currentResultIdx;
}

int RangeContainer::getRegionAtOrAfterIdx(qint64 position)
{
    int currentResultIdx = -1;
    for (int i = 0; i < ranges.size(); i++)
    {
        if (ranges[i].second <= position)
            continue;
        else if (currentResultIdx == -1)
            currentResultIdx = i;
        else if (ranges[i].first < ranges[currentResultIdx].first)
            currentResultIdx = i;
    }
    return currentResultIdx;
}

void RangeContainer::splitAt(qint64 position)
{
    // if empty, fill everything
    if (ranges.empty())
    {
        ranges.push_back(pair<qint64, qint64>(0, videoLength));
        return;
    }

    int beforeIdx = getRegionAtOrBeforeIdx(position);
    int afterIdx = getRegionAtOrAfterIdx(position);

    if (beforeIdx < 0)
    {
        qDebug() << "adding range to fill starting gap";

        // we are on an empty space before any ranges, fill in the start
        // since it is not empty, we are guaranteed an after
        int firstRangeStart = ranges[afterIdx].first;
        ranges.insert(ranges.begin(), pair<qint64, qint64>(0, firstRangeStart));
    }
    else if (afterIdx < 0)
    {
        qDebug() << "adding range to fill final gap";

        // we are on an empty space after all ranges, fill in the end
        int lastRangeEnd = ranges[beforeIdx].second;
        ranges.insert(ranges.begin(), pair<qint64, qint64>(lastRangeEnd, videoLength));
    }
    else if (ranges[beforeIdx].first == position || ranges[beforeIdx].second == position)
    {
        qDebug() << "skipping due to being on a border";

        // if on an edge of a range, do nothing
        return;
    }
    else if (position > ranges[beforeIdx].first && position < ranges[beforeIdx].second)
    {
        qDebug() << "splitting an existing range";

        // if inside a range, split down the middle.
        pair<qint64, qint64> newRange(position, ranges[beforeIdx].second);
        ranges[beforeIdx].second = position;
        ranges.insert(ranges.begin() + beforeIdx + 1, newRange);
    }
    else
    {
        qDebug() << "Adding range to fill gap between ranges";

        // inside a gap between ranges
        qint64 startPosition = ranges[beforeIdx].second;
        qint64 endPosition = ranges[afterIdx].first;

        pair<qint64, qint64> newRange(startPosition, endPosition);
        ranges.insert(ranges.begin() + beforeIdx + 1, newRange);
    }
}

void RangeContainer::trimLeftAt(qint64 position)
{
    if (ranges.empty())
    {
        ranges.push_back(pair<qint64, qint64>(position, videoLength));
        return;
    }

    int idx = getRegionAtOrAfterIdx(position);

    if (idx < 0)
    {
        // no region here or after = extend to end
        ranges.push_back(pair<qint64, qint64>(position, videoLength));
    }
    else
    {
        // pull range here or after to here
        ranges[idx].first = position;
    }
}

void RangeContainer::trimRightAt(qint64 position)
{
    // if we're before all ranges, then create a new one at the start
    if (ranges.empty())
    {
        ranges.insert(ranges.begin(), pair<qint64, qint64>(0, position));
        return;
    }


    int idx = getRegionAtOrBeforeIdx(position);

    if (idx < 0)
    {
        // no region here or before = extend to end
        ranges.push_back(pair<qint64, qint64>(0, position));
    }
    else
    {
        // pull end to here
        ranges[idx].second = position;
    }
}

int RangeContainer::size()
{
    return ranges.size();
}

int RangeContainer::max_size()
{
    return ranges.max_size();
}

vector<pair<qint64, qint64>>::iterator RangeContainer::begin()
{
    return ranges.begin();
}

vector<pair<qint64, qint64>>::iterator RangeContainer::end()
{
    return ranges.end();
}

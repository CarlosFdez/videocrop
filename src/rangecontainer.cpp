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
    emit changed();
}

void RangeContainer::add(qint64 start, qint64 end)
{
    ranges.push_back(Range(start, end));
    emit changed();
}

int RangeContainer::getRegionAtOrBeforeIdx(qint64 position)
{
    // return the region with the highest .end
    // that has a .start at or below position
    int currentResultIdx = -1;
    for (int i = 0; i < ranges.size(); i++)
    {
        if (ranges[i].start > position)
            continue;
        else if (currentResultIdx == -1)
            currentResultIdx = i;
        else if (ranges[i].end > ranges[currentResultIdx].end)
            currentResultIdx = i;
    }

    return currentResultIdx;
}

int RangeContainer::getRegionAtOrAfterIdx(qint64 position)
{
    int currentResultIdx = -1;
    for (int i = 0; i < ranges.size(); i++)
    {
        if (ranges[i].end <= position)
            continue;
        else if (currentResultIdx == -1)
            currentResultIdx = i;
        else if (ranges[i].start < ranges[currentResultIdx].start)
            currentResultIdx = i;
    }
    return currentResultIdx;
}

void RangeContainer::splitAt(qint64 position)
{
    // if empty, fill everything
    if (ranges.empty())
    {
        ranges.push_back(Range(0, videoLength));
        emit changed();
        return;
    }

    int beforeIdx = getRegionAtOrBeforeIdx(position);
    int afterIdx = getRegionAtOrAfterIdx(position);

    if (beforeIdx < 0)
    {
        qDebug() << "split: adding range to fill starting gap";

        // we are on an empty space before any ranges, fill in the start
        // since it is not empty, we are guaranteed an after
        int firstRangeStart = ranges[afterIdx].start;
        ranges.insert(ranges.begin(), Range(0, firstRangeStart));
        emit changed();
    }
    else if (afterIdx < 0)
    {
        qDebug() << "split: adding range to fill final gap";

        // we are on an empty space after all ranges, fill in the end
        int lastRangeEnd = ranges[beforeIdx].end;
        ranges.insert(ranges.begin(), Range(lastRangeEnd, videoLength));
        emit changed();
    }
    else if (ranges[beforeIdx].start == position || ranges[beforeIdx].end == position)
    {
        qDebug() << "split: skipping due to being on a border";

        // if on an edge of a range, do nothing
        return;
    }
    else if (position > ranges[beforeIdx].start && position < ranges[beforeIdx].end)
    {
        qDebug() << "split: splitting an existing range";

        // if inside a range, split down the middle.
        Range newRange(position, ranges[beforeIdx].end);
        ranges[beforeIdx].end = position;
        ranges.insert(ranges.begin() + beforeIdx + 1, newRange);
        emit changed();
    }
    else
    {
        qDebug() << "split: adding range to fill gap between ranges";

        // inside a gap between ranges
        qint64 startPosition = ranges[beforeIdx].end;
        qint64 endPosition = ranges[afterIdx].start;

        Range newRange(startPosition, endPosition);
        ranges.insert(ranges.begin() + beforeIdx + 1, newRange);
        emit changed();
    }
}

void RangeContainer::trimLeftAt(qint64 position)
{
    if (ranges.empty())
    {
        ranges.push_back(Range(position, videoLength));
        emit changed();
        return;
    }

    int idx = getRegionAtOrAfterIdx(position);

    if (idx < 0)
    {
        // no region here or after = extend to end
        ranges.push_back(Range(position, videoLength));
    }
    else
    {
        // pull range here or after to here
        ranges[idx].start = position;
    }

    emit changed();
}

void RangeContainer::trimRightAt(qint64 position)
{
    // if we're before all ranges, then create a new one at the start
    if (ranges.empty())
    {
        ranges.insert(ranges.begin(), Range(0, position));
        emit changed();
        return;
    }


    int idx = getRegionAtOrBeforeIdx(position);

    if (idx < 0)
    {
        // no region here or before = extend to end
        ranges.push_back(Range(0, position));
    }
    else
    {
        // pull end to here
        ranges[idx].end = position;
    }

    emit changed();
}

size_t RangeContainer::size()
{
    return ranges.size();
}

size_t RangeContainer::max_size()
{
    return ranges.max_size();
}

vector<Range>::iterator RangeContainer::begin()
{
    return ranges.begin();
}

vector<Range>::iterator RangeContainer::end()
{
    return ranges.end();
}

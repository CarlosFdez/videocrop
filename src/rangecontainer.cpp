#include "rangecontainer.h"

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
    for (int i = 0; i < ranges.size(); i++)
    {
        pair<qint64, qint64> range = ranges[i];
        if (range.first <= position && range.second > position)
            return i;
        if (range.first > position)
            return i-1;
    }
    return ranges.size() - 1;
}

void RangeContainer::splitAt(qint64 position)
{
    vector<pair<qint64, qint64>> oldRanges = ranges;
    ranges.clear();

    for (pair<qint64, qint64> &range : oldRanges)
    {
        if (range.first < position && range.second > position)
        {
            ranges.push_back(pair<qint64, qint64>(range.first, position));
            ranges.push_back(pair<qint64, qint64>(position, range.second));
        }
        else
        {
            ranges.push_back(range);
        }
    }
}

void RangeContainer::trimLeftAt(qint64 position)
{
    // if we're after all ranges, then create a new one at the end
    if (ranges.empty() || ranges.back().second < position)
    {
        ranges.push_back(pair<qint64, qint64>(position, videoLength));
    }
    else
    {
        int idx = getRegionAtOrBeforeIdx(position);
        if (ranges[idx].second <= position)
            idx++;
        ranges[idx].first = position;
    }
}

void RangeContainer::trimRightAt(qint64 position)
{
    // if we're before all ranges, then create a new one at the start
    if (ranges.empty() || ranges.front().first > position)
    {
        ranges.insert(ranges.begin(), pair<qint64, qint64>(0, position));
    }
    else
    {
        // find the one at or before and set it
        // This won't return -1 as the check above handles it
        int idx = getRegionAtOrBeforeIdx(position);
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

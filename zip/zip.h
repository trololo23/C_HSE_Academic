#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class ZippedIterator {
public:
    ZippedIterator(Iterator a_it, Iterator b_it);

    ZippedIterator& operator++();

    bool operator!=(const ZippedIterator& other) const;

    ZippedPair operator*() const;

private:
    std::pair<Iterator, Iterator> cur_;
};

class Zipped {
public:
    Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

    ZippedIterator begin() const;

    ZippedIterator end() const;

private:
    ZippedIterator begin_;
    ZippedIterator end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

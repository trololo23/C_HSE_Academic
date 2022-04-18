#include "zip.h"

ZippedIterator::ZippedIterator(Iterator a_it, Iterator b_it) : cur_({a_it, b_it}) {
}

ZippedIterator &ZippedIterator::operator++() {
    ++cur_.first;
    ++cur_.second;
    return *this;
}

bool ZippedIterator::operator!=(const ZippedIterator &other) const {
    return (cur_.first != other.cur_.first) && (cur_.second != other.cur_.second);
}

ZippedPair ZippedIterator::operator*() const {
    return {*cur_.first, *cur_.second};
}

Zipped::Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end)
    : begin_(a_begin, b_begin), end_(a_end, b_end) {
}

ZippedIterator Zipped::begin() const {
    return begin_;
}

ZippedIterator Zipped::end() const {
    return end_;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
};

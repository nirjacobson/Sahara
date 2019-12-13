#include "source.h"

Sahara::Source::Source(const QList<float> data, const int stride)
    : _data(data)
    , _stride(stride)
{

}

int Sahara::Source::count() const
{
    return _data.size() / _stride;
}

int Sahara::Source::stride() const
{
    return _stride;
}

QList<float> Sahara::Source::at(const int i) const
{
    return _data.mid(i * _stride, _stride);
}

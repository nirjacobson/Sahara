#include "asset.h"

Sahara::Asset::Asset(const QString& id)
    : _id(id)
{

}

Sahara::Asset::~Asset()
{

}

const QString&Sahara::Asset::id() const
{
    return _id;
}

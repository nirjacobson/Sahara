#include "volume.h"

Sahara::Volume::Volume()
{

}

Sahara::Volume::Volume(const QVector3D& lowerVertex, const QVector3D& upperVertex)
    : _lowerVertex(lowerVertex)
    , _upperVertex(upperVertex)
{

}

const QVector3D& Sahara::Volume::lowerVertex() const
{
    return _lowerVertex;
}

const QVector3D& Sahara::Volume::upperVertex() const
{
    return _upperVertex;
}

float Sahara::Volume::width()
{
    return _upperVertex.x() - _lowerVertex.x();
}

float Sahara::Volume::height()
{
    return _upperVertex.y() - _lowerVertex.y();
}

float Sahara::Volume::length()
{
    return _upperVertex.z() - _lowerVertex.z();
}

bool Sahara::Volume::intersects(const QVector3D& point) const {
    return (
        point.x() >= _lowerVertex.x() && point.x() < _upperVertex.x() &&
        point.y() >= _lowerVertex.y() && point.y() < _upperVertex.y() &&
        point.z() >= _lowerVertex.z() && point.z() < _upperVertex.z()
    );
}

#include "controller.h"



Sahara::Controller::Controller(Sahara::Mesh* const mesh, const QMatrix4x4& bindShapeMatrix, const QStringList& bones, const QList<QMatrix4x4>& inverseBindMatrices)
    : _mesh(mesh)
    , _bindShapeMatrix(bindShapeMatrix)
    , _bones(bones)
    , _inverseBindMatrices(inverseBindMatrices)
{

}

const QMatrix4x4& Sahara::Controller::bindShapeMatrix() const
{
    return _bindShapeMatrix;
}

const QStringList& Sahara::Controller::bones() const
{
    return _bones;
}

const QList<QMatrix4x4>& Sahara::Controller::inverseBindMatrices() const
{
    return _inverseBindMatrices;
}

Sahara::Mesh& Sahara::Controller::mesh()
{
    return *_mesh;
}

const Sahara::Mesh& Sahara::Controller::mesh() const
{
    return *_mesh;
}

#include "controller.h"

Sahara::Controller::Controller(const QString& id,
                               Sahara::Mesh* const mesh,
                               const QMatrix4x4& bindShapeMatrix,
                               const QStringList& joints,
                               const QList<QMatrix4x4>& inverseBindMatrices,
                               const QList<float>& weights,
                               const QList<int>& jointCounts,
                               const QList<int>& jointMappings)
    : Asset(id)
    , _mesh(mesh)
    , _bindShapeMatrix(bindShapeMatrix)
    , _joints(joints)
    , _inverseBindMatrices(inverseBindMatrices)
    , _weights(weights)
    , _jointCounts(jointCounts)
    , _jointMappings(jointMappings)
{

}

const QMatrix4x4& Sahara::Controller::bindShapeMatrix() const
{
    return _bindShapeMatrix;
}

const QStringList& Sahara::Controller::joints() const
{
    return _joints;
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

void Sahara::Controller::generateVertexBuffers()
{
    QList<QList<QPair<int, float>>> verticesJointsAndWeights;
    int jointMappingsIndex = 0;
    for (int i = 0; i < _jointCounts.size(); i++) {
      int numJoints = _jointCounts[i];

      QList<QPair<int, float>> vertexJointsAndWeights;
      for (int j = 0; j < numJoints; j++) {
        vertexJointsAndWeights.append( QPair<int, float>(
          _jointMappings[jointMappingsIndex],
          _weights[ _jointMappings[jointMappingsIndex + 1] ]
        ) );
        jointMappingsIndex += 2;
      }

      verticesJointsAndWeights.append( vertexJointsAndWeights );
    }

    QList<float> jointsSourceData;
    QList<float> weightsSourceData;

    for (int i = 0; i < _jointCounts.size(); i++) {
        const QList<QPair<int, float>> vertexJointsAndWeights = reduceJoints(verticesJointsAndWeights.at(i), 4);

        for (int k = 0; k < vertexJointsAndWeights.size(); k++) {
            jointsSourceData.append(vertexJointsAndWeights.at(k).first);
            weightsSourceData.append(vertexJointsAndWeights.at(k).second);
        }

        for (int k = 0; k < qMax(0, 4 - vertexJointsAndWeights.size()); k++) {
            jointsSourceData.append(-1);
            weightsSourceData.append(-1);
        }
    }

    Sahara::Source* jointsSource = new Sahara::Source(jointsSourceData, 4);
    Sahara::Source* weightsSource = new Sahara::Source(weightsSourceData, 4);

    _mesh->add("joints", jointsSource);
    _mesh->add("weights", weightsSource);

    for (int i = 0; i < _mesh->count(); i++) {
        Sahara::Surface& surface = _mesh->surface(i);

        if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::JOINTS) ||
            !surface.inputs().contains(Sahara::Surface::Input::Semantic::WEIGHTS)) {
            QList<QList<int>> parts;
            int maxOffset = 0;
            for (int j = 0; j < surface.inputs().size(); j++) {
                if (surface.offset(surface.inputs()[j]) > maxOffset) {
                    maxOffset = surface.offset(surface.inputs()[j]);
                }
            }
            int partSize = maxOffset + 1;
            for (int j = 0; j < surface.elements().size() / partSize; j++) {
                parts.append(surface.elements().mid(j * partSize, partSize));
            }

            int vertexOffset = surface.offset(Sahara::Surface::Input::Semantic::POSITION);

            if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::JOINTS)) {
                surface.setInput(Sahara::Surface::Input::Semantic::JOINTS, "joints", maxOffset + 1);

                for (int j = 0; j < parts.size(); j++) {
                    parts[j].append(parts[j].at(vertexOffset));
                }
            }

            if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::WEIGHTS)) {
                surface.setInput(Sahara::Surface::Input::Semantic::WEIGHTS, "weights", maxOffset + 2);

                for (int j = 0; j < parts.size(); j++) {
                    parts[j].append(parts[j].at(vertexOffset));
                }
            }

            QList<int> elements;
            for (int j = 0; j < parts.size(); j++) {
                elements.append(parts.at(j));
            }

            surface.setElements(elements);
        }

        surface.generateVertexBuffer(Sahara::Surface::Input::Semantic::JOINTS);
        surface.generateVertexBuffer(Sahara::Surface::Input::Semantic::WEIGHTS);
    }
}

QList<QPair<int, float> > Sahara::Controller::reduceJoints(const QList<QPair<int, float> >& joints, const int max)
{
    auto sortJoints =  [](const QPair<int, float>& j1, const QPair<int, float>& j2) {
        return j1.second > j2.second;
    };

    QList<QPair<int, float>> sortedJoints = joints;
    std::sort(sortedJoints.begin(), sortedJoints.end(), sortJoints);

    QList<QPair<int, float>> reducedJoints = sortedJoints.mid(0, max);

    float sum = 0.0f;
    for (int i = 0; i < reducedJoints.size(); i++) {
        sum += reducedJoints[i].second;
    }
    for (int i = 0; i < reducedJoints.size(); i++) {
        reducedJoints[i].second /= sum;
    }

    return reducedJoints;
}

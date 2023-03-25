#include "controller.h"

Sahara::Controller::Controller(const QString& id,
                               Sahara::Mesh* const mesh,
                               const QMatrix4x4& bindShapeMatrix,
                               const QStringList& bones,
                               const QList<QMatrix4x4>& inverseBindMatrices,
                               const QList<float>& weights,
                               const QList<int>& boneCounts,
                               const QList<int>& boneMappings)
    : Asset(id)
    , _mesh(mesh)
    , _bindShapeMatrix(bindShapeMatrix)
    , _bones(bones)
    , _inverseBindMatrices(inverseBindMatrices)
    , _weights(weights)
    , _boneCounts(boneCounts)
    , _boneMappings(boneMappings)
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

void Sahara::Controller::generateVertexBuffers()
{
    QList<QList<QPair<int, float>>> verticesBonesAndWeights;
    int boneMappingsIndex = 0;
    for (int i = 0; i < _boneCounts.size(); i++) {
      int numBones = _boneCounts[i];

      QList<QPair<int, float>> vertexBonesAndWeights;
      for (int j = 0; j < numBones; j++) {
        vertexBonesAndWeights.append( QPair<int, float>(
          _boneMappings[boneMappingsIndex],
          _weights[ _boneMappings[boneMappingsIndex + 1] ]
        ) );
        boneMappingsIndex += 2;
      }

      verticesBonesAndWeights.append( vertexBonesAndWeights );
    }

    QList<float> bonesSourceData;
    QList<float> weightsSourceData;

    for (int i = 0; i < _boneCounts.size(); i++) {
        const QList<QPair<int, float>> vertexBonesAndWeights = reduceBones(verticesBonesAndWeights.at(i), 4);

        for (int k = 0; k < vertexBonesAndWeights.size(); k++) {
            bonesSourceData.append(vertexBonesAndWeights.at(k).first);
            weightsSourceData.append(vertexBonesAndWeights.at(k).second);
        }

        for (int k = 0; k < qMax(0, 4 - vertexBonesAndWeights.size()); k++) {
            bonesSourceData.append(-1);
            weightsSourceData.append(-1);
        }
    }

    Sahara::Source* bonesSource = new Sahara::Source(bonesSourceData, 4);
    Sahara::Source* weightsSource = new Sahara::Source(weightsSourceData, 4);

    _mesh->add("bones", bonesSource);
    _mesh->add("weights", weightsSource);

    for (int i = 0; i < _mesh->count(); i++) {
        Sahara::Surface& surface = _mesh->surface(i);

        if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::BONES) ||
            !surface.inputs().contains(Sahara::Surface::Input::Semantic::WEIGHTS)) {
            QList<QList<int>> parts;
            int partSize = surface.inputs().size();
            for (int j = 0; j < surface.elements().size() / partSize; j++) {
                parts.append(surface.elements().mid(j * partSize, partSize));
            }

            int vertexOffset = surface.offset(Sahara::Surface::Input::Semantic::POSITION);

            if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::BONES)) {
                surface.setInput(Sahara::Surface::Input::Semantic::BONES, "bones", surface.inputs().size());

                for (int j = 0; j < parts.size(); j++) {
                    parts[j].append(parts[j].at(vertexOffset));
                }
            }

            if (!surface.inputs().contains(Sahara::Surface::Input::Semantic::WEIGHTS)) {
                surface.setInput(Sahara::Surface::Input::Semantic::WEIGHTS, "weights", surface.inputs().size());

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

        surface.generateVertexBuffer(Sahara::Surface::Input::Semantic::BONES);
        surface.generateVertexBuffer(Sahara::Surface::Input::Semantic::WEIGHTS);
    }
}

QList<QPair<int, float> > Sahara::Controller::reduceBones(const QList<QPair<int, float> >& bones, const int max)
{
    auto sortBones =  [](const QPair<int, float>& b1, const QPair<int, float>& b2) {
        return b1.second > b2.second;
    };

    QList<QPair<int, float>> sortedBones = bones;
    std::sort(sortedBones.begin(), sortedBones.end(), sortBones);

    QList<QPair<int, float>> reducedBones = sortedBones.mid(0, max);

    float sum = 0.0f;
    for (int i = 0; i < reducedBones.size(); i++) {
        sum += reducedBones[i].second;
    }
    for (int i = 0; i < reducedBones.size(); i++) {
        reducedBones[i].second /= sum;
    }

    return reducedBones;
}

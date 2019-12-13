#ifndef SAHARA_CONTROLLER_H
#define SAHARA_CONTROLLER_H

#include <QMatrix4x4>

#include "asset.h"
#include "mesh/mesh.h"

namespace Sahara
{

    class Controller : public Asset
    {
        public:
            Controller(Mesh* const mesh, const QMatrix4x4& bindShapeMatrix, const QStringList& bones, const QList<QMatrix4x4>& inverseBindMatrices, const QList<float>& weights, const QList<int>& boneCounts, const QList<int>& boneMappings);

            const QMatrix4x4& bindShapeMatrix() const;
            const QStringList& bones() const;
            const QList<QMatrix4x4>& inverseBindMatrices() const;

            Mesh& mesh();
            const Mesh& mesh() const;

            void generateVertexBuffers();

        private:
            Mesh* _mesh;
            QMatrix4x4 _bindShapeMatrix;
            QStringList _bones;
            QList<QMatrix4x4> _inverseBindMatrices;
            QList<float> _weights;
            QList<int> _boneCounts;
            QList<int> _boneMappings;

            static QList<QPair<int, float>> reduceBones(const QList<QPair<int, float> >& bones, const int max);
    };

}

#endif // SAHARA_CONTROLLER_H

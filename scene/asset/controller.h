#ifndef SAHARA_CONTROLLER_H
#define SAHARA_CONTROLLER_H

#include <QMatrix4x4>

#include "asset.h"
#include "mesh/mesh.h"

namespace Sahara
{

    class Controller : public Asset
    {
        friend class JSON;

        public:
            Controller(const QString& id,
                    Mesh* const mesh,
                    const QMatrix4x4& bindShapeMatrix,
                    const QStringList& joints,
                    const QList<QMatrix4x4>& inverseBindMatrices,
                    const QList<float>& weights,
                    const QList<int>& jointCounts,
                    const QList<int>& jointMappings);

            const QMatrix4x4& bindShapeMatrix() const;
            const QStringList& joints() const;
            const QList<QMatrix4x4>& inverseBindMatrices() const;

            Mesh& mesh();
            const Mesh& mesh() const;

            void generateVertexBuffers();

        private:
            Mesh* _mesh;
            QMatrix4x4 _bindShapeMatrix;
            QStringList _joints;
            QList<QMatrix4x4> _inverseBindMatrices;
            QList<float> _weights;
            QList<int> _jointCounts;
            QList<int> _jointMappings;

            static QList<QPair<int, float>> reduceJoints(const QList<QPair<int, float> >& joints, const int max);
    };

}

#endif // SAHARA_CONTROLLER_H

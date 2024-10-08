#ifndef SAHARA_INSTANCEMESH_H
#define SAHARA_INSTANCEMESH_H

#include "instance.h"
#include "../asset/mesh/openglmesh.h"

namespace Sahara
{

    class InstanceMesh : public Instance
    {
        friend class JSON;

        public:
            InstanceMesh(const MaterialDict& materials, const QMatrix4x4& transform, Mesh* mesh);

            const Mesh& mesh() const;
            Mesh& mesh();

        private:
            Mesh* _mesh;
    };

}

#endif // SAHARA_INSTANCEMESH_H

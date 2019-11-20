#ifndef SAHARA_MESHINSTANCE_H
#define SAHARA_MESHINSTANCE_H

#include "instance.h"
#include "../asset/mesh.h"

namespace Sahara
{

    class MeshInstance : public Instance
    {
        public:
            MeshInstance(const MaterialDict& materials, const QMatrix4x4& transform, Mesh* mesh);

            const Mesh& mesh() const;
            Mesh& mesh();

        private:
            Mesh* _mesh;
    };

}

#endif // SAHARA_MESHINSTANCE_H

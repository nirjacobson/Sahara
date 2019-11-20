#ifndef SAHARA_MESH_H
#define SAHARA_MESH_H

#include <QList>

#include "surface.h"
#include "asset.h"

namespace Sahara
{
    class Mesh : public Asset
    {
        public:
            Mesh(const QList<Surface>& surfaces);

            const QList<Surface>& surfaces() const;
            QList<Surface>& surfaces();

        private:
            QList<Surface> _surfaces;
    };
}

#endif // SAHARA_MESH_H

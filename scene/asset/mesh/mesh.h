#ifndef SAHARA_MESH_H
#define SAHARA_MESH_H

#include <QList>

#include "../asset.h"
#include "source.h"
#include "surface.h"

namespace Sahara
{
    class Mesh : public Asset
    {

        public:
            Mesh();
            ~Mesh();

            QStringList sources() const;
            void add(const QString& name, Source* source);

            int count() const;
            Surface& surface(const int i);
            Surface& add(const QString& material);

        private:
            SourceDict _sources;
            QList<Surface*> _surfaces;
    };
}

#endif // SAHARA_MESH_H

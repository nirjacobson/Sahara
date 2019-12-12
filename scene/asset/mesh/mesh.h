#ifndef SAHARA_MESH_H
#define SAHARA_MESH_H

#include <QList>

#include "../asset.h"
#include "source.h"

namespace Sahara
{
    class Surface;

    class Mesh : public Asset
    {

        public:
            Mesh();
            ~Mesh();

            QStringList sources() const;
            const Source& source(const QString& name) const;
            void addSource(const QString& name, Source* source);

            int surfaces() const;
            Surface& surface(const int i);
            void addSurface(Surface* surface);

        private:
            QMap<QString, Source*> _sources;
            QList<Surface*> _surfaces;
    };
}

#endif // SAHARA_MESH_H

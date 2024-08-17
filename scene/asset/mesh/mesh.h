#ifndef MESH_H
#define MESH_H

#include "../asset.h"
#include "surface.h"

namespace Sahara {
    class Mesh : public Asset
    {
        friend class JSON;

    public:
        Mesh(const QString& id);
        virtual ~Mesh() { }

        QStringList sources() const;
        void add(const QString& name, Source* source);

        int count() const;
        virtual Surface& surface(const int i) = 0;
        virtual Surface& add(const QString& material) = 0;

        int triangles() const;

    protected:
        SourceDict _sources;
        QList<Surface*> _surfaces;
    };
}

#endif // MESH_H

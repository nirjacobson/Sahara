#ifndef SAHARA_OPENGL_MESH_H
#define SAHARA_OPENGL_MESH_H

#include <QList>

#include "mesh.h"
#include "source.h"
#include "openglsurface.h"

namespace Sahara
{
    class OpenGLMesh : public Mesh
    {
        friend class JSON;

        public:
            OpenGLMesh(const QString& id);
            ~OpenGLMesh();

            Surface& surface(const int i) override;
            Surface& add(const QString& material) override;
    };
}

#endif // SAHARA_OPENGL_MESH_H

#ifndef SAHARA_SURFACE_H
#define SAHARA_SURFACE_H

#include <QMap>

#include "../../gl/vertexbuffer.h"

namespace Sahara
{
    typedef QMap<QString, VertexBuffer> VertexBufferDict;

    class Surface
    {
        public:
            Surface(const QString& material);

            void addVertexBuffer(const QString& name, const GLenum type, const char* const data, const int size, const int stride);

            const VertexBufferDict& vertexBuffers();
            QString material() const;

        private:
            VertexBufferDict _vertexBuffers;
            QString _material;
    };

}

#endif // SAHARA_SURFACE_H

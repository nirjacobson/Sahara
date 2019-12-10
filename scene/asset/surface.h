#ifndef SAHARA_SURFACE_H
#define SAHARA_SURFACE_H

#include <QMap>

#include "../../render/withvertexbuffers.h"

namespace Sahara
{

    class Surface : public WithVertexBuffers
    {
        public:
            Surface(const QString& material);

            void addVertexBuffer(const QString& name, const GLfloat* const data, const int size, const int stride);

            QString material() const;

        private:
            QString _material;
    };

}

#endif // SAHARA_SURFACE_H

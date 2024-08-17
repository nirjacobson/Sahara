#ifndef SAHARA_OPENGL_SURFACE_H
#define SAHARA_OPENGL_SURFACE_H

#include <QMap>

#include "../../../render/openglwithvertexbuffers.h"
#include "surface.h"
#include "source.h"

namespace Sahara {

    class OpenGLSurface : public Surface, public OpenGLWithVertexBuffers
    {
        friend class JSON;

        public:
            OpenGLSurface(const SourceDict& sourceDict, const QString& material);

            void generateVertexBuffer(const Input::Semantic input) override;
    };

}

#endif // SAHARA_OPENGL_SURFACE_H

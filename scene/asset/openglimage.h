#ifndef SAHARA_OPENGL_IMAGE_H
#define SAHARA_OPENGL_IMAGE_H

#include "Sahara_global.h"

#include <QImage>
#include <QOpenGLTexture>

#include "image.h"

namespace Sahara
{

    class OpenGLRenderer;

    class SAHARA_EXPORT OpenGLImage : public Image
    {
        friend class JSON;

        public:
            OpenGLImage(const QString& id, const QString& uri);
            ~OpenGLImage();

            void bind();
            void release();

        private:
            QOpenGLTexture _texture;
    };

}

#endif // SAHARA_OPENGL_IMAGE_H

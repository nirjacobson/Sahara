#ifndef SAHARA_OPENGL_IMAGE_H
#define SAHARA_OPENGL_IMAGE_H

#include <QImage>
#include <QOpenGLTexture>

#include "image.h"

namespace Sahara
{

    class OpenGLRenderer;

    class OpenGLImage : public Image
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

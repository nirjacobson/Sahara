#ifndef SAHARA_IMAGE_H
#define SAHARA_IMAGE_H

#include <QImage>
#include <QOpenGLTexture>

#include "asset.h"

namespace Sahara
{

    class Image : public Asset
    {
        public:
            Image(const QString& uri);
            ~Image();

            void bind();
            void release();

        private:
            QOpenGLTexture _texture;
    };

}

#endif // SAHARA_IMAGE_H

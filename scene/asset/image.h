#ifndef SAHARA_IMAGE_H
#define SAHARA_IMAGE_H

#include <QImage>
#include <QOpenGLTexture>

#include "asset.h"

namespace Sahara
{

    class Image : public Asset
    {
        friend class JSON;

        public:
            Image(const QString& id, const QString& uri);
            ~Image();

            void bind();
            void release();

            const QString& uri() const;

        private:
            QString _uri;
            QOpenGLTexture _texture;
    };

}

#endif // SAHARA_IMAGE_H

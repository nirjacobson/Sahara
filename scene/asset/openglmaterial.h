#ifndef SAHARA_OPENGL_MATERIAL_H
#define SAHARA_OPENGL_MATERIAL_H

#include "../../Sahara_global.h"

#include <QColor>
#include <optional>

#include "material.h"
#include "openglimage.h"

namespace Sahara
{

    class SAHARA_EXPORT OpenGLMaterial : public Material
    {
        friend class JSON;

        public:
            OpenGLMaterial(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess);
            OpenGLMaterial(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, OpenGLImage* const image, const QColor& specular, const float shininess);


            const std::optional<OpenGLImage*>& image() const;
            std::optional<OpenGLImage*>& image();

        private:
            std::optional<OpenGLImage*> _image;
    };

}

#endif // SAHARA_OPENGL_MATERIAL_H

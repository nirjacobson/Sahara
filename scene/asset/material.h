#ifndef SAHARA_MATERIAL_H
#define SAHARA_MATERIAL_H

#include <QColor>
#include <optional>

#include "asset.h"
#include "image.h"

namespace Sahara
{

    class Material : public Asset
    {
        friend class JSON;

        public:
            Material(const QString& id, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess);
            Material(const QString& id, const QColor& emission, const QColor& ambient, Image* const image, const QColor& specular, const float shininess);

            const QColor& emission() const;
            const QColor& ambient() const;
            const QColor& diffuse() const;
            const QColor& specular() const;
            float shininess() const;

            const std::optional<Image*>& image() const;
            std::optional<Image*>& image();

        private:
            QColor _emission;
            QColor _ambient;
            QColor _diffuse;
            std::optional<Image*> _image;
            QColor _specular;
            float _shininess;
    };

}

#endif // SAHARA_MATERIAL_H

#ifndef SAHARA_MATERIAL_H
#define SAHARA_MATERIAL_H

#include <QVector4D>
#include <optional>

#include "asset.h"
#include "image.h"

namespace Sahara
{

    class Material : public Asset
    {
        public:
            Material(const QVector4D& emission, const QVector4D& ambient, const QVector4D& diffuse, const QVector4D& specular, const float shininess);
            Material(const QVector4D& emission, const QVector4D& ambient, Image* const image, const QVector4D& specular, const float shininess);

            const QVector4D& emission() const;
            const QVector4D& ambient() const;
            const QVector4D& diffuse() const;
            const QVector4D& specular() const;
            float shininess() const;

            const std::optional<Image*>& image() const;
            std::optional<Image*>& image();

        private:
            QVector4D _emission;
            QVector4D _ambient;
            QVector4D _diffuse;
            std::optional<Image*> _image;
            QVector4D _specular;
            float _shininess;
    };

}

#endif // SAHARA_MATERIAL_H

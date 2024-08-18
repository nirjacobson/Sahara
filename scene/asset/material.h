#ifndef MATERIAL_H
#define MATERIAL_H

#include "Sahara_global.h"

#include <QColor>

#include "asset.h"

namespace Sahara {
    class SAHARA_EXPORT Material : public Asset
    {
        friend class JSON;

    public:
        Material(const QString& id, const QString& name, const QColor& emission, const QColor& ambient, const QColor& diffuse, const QColor& specular, const float shininess);
        virtual ~Material() { };

        const QColor& emission() const;
        void setEmission(const QColor& color);
        const QColor& ambient() const;
        void setAmbient(const QColor& color);
        const QColor& diffuse() const;
        void setDiffuse(const QColor& color);
        const QColor& specular() const;
        void setSpecular(const QColor& color);
        float shininess() const;
        void setShininess(const float shininess);

        const QString& name() const;

    protected:
        QString _name;
        QColor _emission;
        QColor _ambient;
        QColor _diffuse;
        QColor _specular;
        float _shininess;
    };
}

#endif // MATERIAL_H

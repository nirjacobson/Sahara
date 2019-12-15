#ifndef SAHARA_POINTLIGHT_H
#define SAHARA_POINTLIGHT_H

#include <QColor>

#include "light.h"

namespace Sahara
{

    class PointLight : public Light
    {
        friend class JSON;

        public:
            PointLight(const QString& id, const QColor& color, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation);

            const QColor& color() const;
            float constantAttenuation() const;
            float linearAttenuation() const;
            float quadraticAttenuation() const;

            void setColor(const QColor& color);
            void setConstantAttenuation(const float constantAttenuation);
            void setLinearAttenuation(const float linearAttenuation);
            void setQuadraticAttenuation(const float quadraticAttenuation);

        private:
            QColor _color;
            float _constantAttenuation;
            float _linearAttenuation;
            float _quadraticAttenuation;
    };

}

#endif // SAHARA_POINTLIGHT_H

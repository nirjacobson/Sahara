#ifndef SAHARA_POINTLIGHT_H
#define SAHARA_POINTLIGHT_H

#include "Sahara_global.h"

#include <QColor>

#include "../asset.h"
#include "../../node/nodeitem.h"
#include "light.h"

namespace Sahara
{

    class SAHARA_EXPORT PointLight : public Asset, public NodeItem,  public Light
    {
        friend class JSON;

        public:
            PointLight(const QString& id, const QColor& color, const float constantAttenuation, const float linearAttenuation, const float quadraticAttenuation);

            float constantAttenuation() const;
            float linearAttenuation() const;
            float quadraticAttenuation() const;

            void setConstantAttenuation(const float constantAttenuation);
            void setLinearAttenuation(const float linearAttenuation);
            void setQuadraticAttenuation(const float quadraticAttenuation);

            Sahara::Volume volume() const override;
        private:
            float _constantAttenuation;
            float _linearAttenuation;
            float _quadraticAttenuation;
    };

}

#endif // SAHARA_POINTLIGHT_H

#ifndef SAHARA_AMBIENTLIGHT_H
#define SAHARA_AMBIENTLIGHT_H

#include "Sahara_global.h"

#include "light.h"

namespace Sahara
{
    class SAHARA_EXPORT AmbientLight : public Light
    {
        public:
            AmbientLight();

            float strength() const;
            void setStrength(const float strength);

        private:
            float _strength;
    };
}

#endif // SAHARA_AMBIENTLIGHT_H

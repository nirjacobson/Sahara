#ifndef SAHARALIGHT_H
#define SAHARALIGHT_H

#include "../node/nodeitem.h"
#include "asset.h"

namespace Sahara
{

    class Light : public Asset, public NodeItem
    {
        public:
            Light(const QString& id);

            Volume volume() const override;
    };

}

#endif // SAHARALIGHT_H

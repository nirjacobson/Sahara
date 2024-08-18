#ifndef SAHARA_NODEITEM_H
#define SAHARA_NODEITEM_H

#include "Sahara_global.h"

#include "common/volume.h"

namespace Sahara
{

    class SAHARA_EXPORT NodeItem
    {
        public:
            NodeItem();
            virtual ~NodeItem();

            virtual Volume volume() const = 0;
    };

}

#endif // SAHARA_NODEITEM_H

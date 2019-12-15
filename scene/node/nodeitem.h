#ifndef SAHARA_NODEITEM_H
#define SAHARA_NODEITEM_H

#include "common/volume.h"

namespace Sahara
{

    class NodeItem
    {
        public:
            NodeItem();
            virtual ~NodeItem();

            virtual Volume volume() const = 0;
    };

}

#endif // SAHARA_NODEITEM_H

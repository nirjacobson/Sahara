#ifndef SAHARA_INSTANCECONTROLLER_H
#define SAHARA_INSTANCECONTROLLER_H

#include "instance.h"
#include "../asset/controller.h"
#include "../asset/armature.h"

namespace Sahara
{
    class InstanceController : public Instance
    {
        public:
            InstanceController(Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller);

            Controller& controller();
            const Controller& controller() const;

            const Armature& armature() const;

        private:
            Armature* _armature;
            Controller* _controller;
    };

}

#endif // SAHARA_INSTANCECONTROLLER_H

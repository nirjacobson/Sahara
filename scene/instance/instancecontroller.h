#ifndef INSTANCECONTROLLER_H
#define INSTANCECONTROLLER_H

#include "instance.h"
#include "../asset/controller.h"
#include "../asset/armature.h"

namespace Sahara {
    class InstanceController : public Instance
    {
        friend class JSON;

    public:
        InstanceController(const Armature* armature, const MaterialDict& materials, const QMatrix4x4& transform, Controller* controller);
        virtual ~InstanceController() { }

        Controller& controller();
        const Controller& controller() const;

        const Armature& armature() const;

    protected:
        const Armature* _armature;
        Controller* _controller;
    };
}

#endif // INSTANCECONTROLLER_H

#ifndef SAHARA_ARMATURE_H
#define SAHARA_ARMATURE_H

#include "asset.h"
#include "joint.h"
#include "vulkanutil.h"

namespace Sahara
{
    class Renderer;

    class Armature : public Asset
    {
        friend class JSON;

        public:
            Armature(Renderer* renderer, const QString& id, Joint* const root);
            ~Armature();

            const Joint& root() const;

            Joint* getJointById(const QString& id);
            const Joint* getJointByName(const QString& name) const;

        private:
            Joint* _root;
    };

}

#endif // SAHARA_ARMATURE_H

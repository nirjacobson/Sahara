#ifndef SAHARA_ARMATURE_H
#define SAHARA_ARMATURE_H

#include "asset.h"
#include "joint.h"

namespace Sahara
{
    class Armature : public Asset
    {
        friend class JSON;

        public:
            Armature(const QString& id, Joint* const root);
            virtual ~Armature();

            const Joint& root() const;

            Joint* getJointById(const QString& id);
            const Joint* getJointByName(const QString& name) const;

        protected:
            Joint* _root;
    };

}

#endif // SAHARA_ARMATURE_H

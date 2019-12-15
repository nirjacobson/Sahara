#ifndef SAHARA_ARMATURE_H
#define SAHARA_ARMATURE_H

#include "asset.h"
#include "bone.h"

namespace Sahara
{

    class Armature : public Asset
    {
        friend class JSON;

        public:
            Armature(const QString& id, Bone* const root);
            ~Armature();

            const Bone& root() const;

            Bone* getBoneById(const QString& id);
            const Bone* getBoneByName(const QString& name) const;

        private:
            Bone* _root;
    };

}

#endif // SAHARA_ARMATURE_H

#ifndef SAHARA_BONE_H
#define SAHARA_BONE_H

#include <QList>

#include "../../common/transform.h"

namespace Sahara {

    class Bone
    {
        friend class JSON;

        private:
            typedef std::function<bool(Bone&)> BoneVisitor;
            typedef std::function<bool(const Bone&)> BoneVisitorConst;

        public:
            Bone(Bone* const parent, const QString& id, const QString& name, const Transform& transform);
            ~Bone();

            const Bone& parent() const;
            Bone& parent();
            QString id() const;
            QString name() const;
            void addChild(Bone* const child);
            const Transform& transform() const;
            void setTransform(const Transform& transform);

            bool depthFirst(const BoneVisitor& visitor);
            bool depthFirst(const BoneVisitorConst& visitor) const;

        private:
            Bone* _parent;
            QString _id;
            QString _name;
            Transform _transform;
            QList<Bone*> _children;


    };

}

#endif // SAHARA_BONE_H

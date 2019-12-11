#ifndef SAHARA_BONE_H
#define SAHARA_BONE_H

#include <QList>

#include "../../common/transform.h"

namespace Sahara {

    class Bone
    {
        friend class JSON;

        private:
            typedef std::function<void(void)> BoneVisitorStopFn;
            typedef std::function<void(Bone&, const BoneVisitorStopFn&)> BoneVisitor;
            typedef std::function<void(const Bone&, const BoneVisitorStopFn&)> BoneVisitorConst;

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

            void depthFirst(const BoneVisitor& visitor);
            void depthFirst(const BoneVisitorConst& visitor) const;

        private:
            Bone* _parent;
            QString _id;
            QString _name;
            Transform _transform;
            QList<Bone*> _children;

            void depthFirstOnBone(const BoneVisitor& visitor, Bone& bone, const BoneVisitorStopFn& stopFn, const bool& stop);
            void depthFirstOnBone(const BoneVisitorConst& visitor, const Bone& bone, const BoneVisitorStopFn& stopFn, const bool& stop) const;


    };

}

#endif // SAHARA_BONE_H

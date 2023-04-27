#ifndef SAHARA_JOINT_H
#define SAHARA_JOINT_H

#include <QList>

#include "../../common/transform.h"

namespace Sahara {

    class Joint
    {
        friend class JSON;

        private:
            typedef std::function<bool(Joint&)> JointVisitor;
            typedef std::function<bool(const Joint&)> JointVisitorConst;

        public:
            Joint(Joint* const parent, const QString& id, const QString& name, const Transform& transform);
            ~Joint();

            const Joint& parent() const;
            Joint& parent();
            QString id() const;
            QString name() const;
            void addChild(Joint* const child);
            const Transform& transform() const;
            void setTransform(const Transform& transform);

            bool depthFirst(const JointVisitor& visitor);
            bool depthFirst(const JointVisitorConst& visitor) const;

        private:
            Joint* _parent;
            QString _id;
            QString _name;
            Transform _transform;
            QList<Joint*> _children;


    };

}

#endif // SAHARA_JOINT_H

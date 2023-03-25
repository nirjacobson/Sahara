#ifndef SAHARA_NODE_H
#define SAHARA_NODE_H

#include <QList>
#include <QMatrix4x4>

#include "nodeitem.h"

namespace Sahara
{

    class Node
    {
        friend class JSON;

        private:
            typedef std::function<bool(Node&)> NodeVisitor;
            typedef std::function<bool(const Node&)> NodeVisitorConst;

        public:
            Node(const QString& name, NodeItem* const nodeItem, const QMatrix4x4& transform);
            ~Node();

            bool isRoot() const;
            Node& parent();

            int index() const;

            const QString& name() const;
            void setName(const QString& name);

            const NodeItem& item() const;
            NodeItem& item();

            int children() const;
            const Node& child(const int index) const;
            Node& child(const int index);
            void addChild(Node* node);

            QMatrix4x4 transform() const;
            QMatrix4x4 globalTransform() const;
            QVector3D globalPosition() const;
            void setTransform(const QMatrix4x4& transform);

            bool hasFocus() const;
            void setFocus(const bool focus);

            void remove();

            bool intersects(const QVector3D& point) const;

            bool depthFirst(const NodeVisitor& visitor);
            bool depthFirst(const NodeVisitorConst& visitor) const;

            bool depthFirst(const NodeVisitor& visitorBefore, const NodeVisitor& visitorAfter);
            bool depthFirst(const NodeVisitorConst& visitorBefore, const NodeVisitorConst& visitorAfter) const;

        private:
            Node* _parent;
            int _index;
            QString _name;
            NodeItem* _item;
            QList<Node*> _children;
            QMatrix4x4 _transform;
            bool _hasFocus;

            void trySetName(Node* root, const QString name, const int suffix);
    };

}

#endif // SAHARA_NODE_H

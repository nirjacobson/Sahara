#ifndef SAHARA_NODE_H
#define SAHARA_NODE_H

#include <QList>
#include <QMatrix4x4>

#include "nodeitem.h"

namespace Sahara
{

    class Node
    {
        private:
            typedef std::function<void(void)> NodeVisitorStopFn;
            typedef std::function<void(Node&, const NodeVisitorStopFn&)> NodeVisitor;
            typedef std::function<void(const Node&, const NodeVisitorStopFn)> NodeVisitorConst;

        public:
            Node(const QString& name, NodeItem* nodeItem, const QMatrix4x4& transform);
            ~Node();

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

            void depthFirst(const NodeVisitor& visitor);
            void depthFirst(const NodeVisitorConst& visitor) const;

            void depthFirst(const NodeVisitor& visitorBefore, const NodeVisitor& visitorAfter);
            void depthFirst(const NodeVisitorConst& visitorBefore, const NodeVisitorConst& visitorAfter) const;

        private:
            Node* _parent;
            int _index;
            QString _name;
            NodeItem* _item;
            QList<Node*> _children;
            QMatrix4x4 _transform;
            bool _hasFocus;

            void trySetName(Node* root, const QString name, const int suffix);

            void depthFirstOnNode(const NodeVisitor& visitor, Node& node, const NodeVisitorStopFn& stopFn, const bool& stop);
            void depthFirstOnNode(const NodeVisitorConst& visitor, const Node& node, const NodeVisitorStopFn& stopFn, const bool& stop) const;

            void depthFirstOnNode(const NodeVisitor& visitorBefore, const NodeVisitor& visitorAfter, Node& node, const NodeVisitorStopFn& stopFn, const bool& stop);
            void depthFirstOnNode(const NodeVisitorConst& visitorBefore, const NodeVisitorConst& visitorAfter, const Node& node, const NodeVisitorStopFn& stopFn, const bool& stop) const;
    };

}

#endif // SAHARA_NODE_H

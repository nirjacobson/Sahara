#include "node.h"


Sahara::Node::Node(const QString& name, NodeItem* const nodeItem, const QMatrix4x4& transform)
    : _parent(nullptr)
    , _name(name)
    , _item(nodeItem)
    , _transform(transform)
    , _hasFocus(false)
{

}

Sahara::Node::~Node()
{
    delete _item;

    for (const Node* node : _children) {
        delete node;
    }
}

Sahara::Node& Sahara::Node::parent()
{
    return *_parent;
}

int Sahara::Node::index() const
{
    return _index;
}

const QString& Sahara::Node::name() const
{
    return _name;
}

void Sahara::Node::setName(const QString& name)
{
    _name = name;

    if (_parent || !_children.isEmpty()) {
        Node* root = this;
        for (; root->_parent; root = root->_parent) ;

        root->depthFirst([&](Node& node, auto& stop) {
           if (&node == this)
               return;

           if (node.name() == name) {
               node.trySetName(root, name, 1);
               stop();
           }
        });
    }
}

const Sahara::NodeItem& Sahara::Node::item() const
{
    return *_item;
}

Sahara::NodeItem& Sahara::Node::item()
{
    return *_item;
}

int Sahara::Node::children() const
{
    return _children.size();
}

const Sahara::Node& Sahara::Node::child(const int index) const
{
    return *_children.at(index);
}

Sahara::Node& Sahara::Node::child(const int index)
{
    return *_children.at(index);
}

void Sahara::Node::addChild(Sahara::Node* node)
{
    node->_index = _children.size();
    node->_parent = this;

    _children.append(node);

    node->setName(node->name());
}

QMatrix4x4 Sahara::Node::transform() const
{
    return _transform;
}

QMatrix4x4 Sahara::Node::globalTransform() const
{
    const Node* currentNode = this;
    QMatrix4x4 globalTransform;
    while (currentNode->_parent != nullptr) {
        globalTransform = currentNode->transform() * globalTransform;
        currentNode = currentNode->_parent;
    }

    return globalTransform;
}

QVector3D Sahara::Node::globalPosition() const
{
    return QVector3D(globalTransform().column(3));
}

void Sahara::Node::setTransform(const QMatrix4x4& transform)
{
    _transform = transform;
}

bool Sahara::Node::hasFocus() const
{
    return _hasFocus;
}

void Sahara::Node::setFocus(const bool focus)
{
    _hasFocus = focus;

    if ((_parent || !_children.isEmpty()) && focus) {
        Node* root = this;
        for (; root->_parent; root = root->_parent) ;

        root->depthFirst([&](Node& node, auto& stop) {
            if (&node == this)
                return;

            if (node._hasFocus) {
                node._hasFocus = false;
                stop();
            }
        });
    }
}

void Sahara::Node::remove()
{
    if (_parent) {
        _parent->_children.removeOne(this);

        for (int i = 0; i < _parent->children(); i++) {
            _parent->child(i)._index = i;
        }
    }
}

bool Sahara::Node::intersects(const QVector3D& point) const
{
    if (!_item)
        return false;

    QMatrix4x4 transform = globalTransform();
    QVector3D globalVolumeLowerVertex = transform.map(_item->volume().lowerVertex());
    QVector3D globalVolumeUpperVertex = transform.map(_item->volume().upperVertex());

    Volume globalVolume(globalVolumeLowerVertex, globalVolumeUpperVertex);

    return globalVolume.intersects(point);
}

void Sahara::Node::depthFirst(const Sahara::Node::NodeVisitor& visitor)
{
    bool stop = false;
    NodeVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnNode(visitor, *this, doStop, stop);
}

void Sahara::Node::depthFirst(const Sahara::Node::NodeVisitorConst& visitor) const
{
    bool stop = false;
    NodeVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnNode(visitor, *this, doStop, stop);
}

void Sahara::Node::depthFirst(const Sahara::Node::NodeVisitor& visitorBefore, const Sahara::Node::NodeVisitor& visitorAfter)
{
    bool stop = false;
    NodeVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnNode(visitorBefore, visitorAfter, *this, doStop, stop);
}

void Sahara::Node::depthFirst(const Sahara::Node::NodeVisitorConst& visitorBefore, const Sahara::Node::NodeVisitorConst& visitorAfter) const
{
    bool stop = false;
    NodeVisitorStopFn doStop = [&]() {
        stop = true;
    };
    depthFirstOnNode(visitorBefore, visitorAfter, *this, doStop, stop);
}

void Sahara::Node::trySetName(Sahara::Node* root, const QString name, const int suffix)
{
    QString newName = name+"."+QString("%1").arg(suffix, 3, 10, QChar('0'));

    bool found = false;
    root->depthFirst([&](const Node& node, auto& stop) {
        if (node.name() == newName) {
            found = true;
            trySetName(root, name, suffix+1);
            stop();
        }
    });

    if (!found) {
        _name = newName;
    }
}

void Sahara::Node::depthFirstOnNode(const Sahara::Node::NodeVisitor& visitor, Sahara::Node& node, const Sahara::Node::NodeVisitorStopFn& stopFn, const bool& stop)
{
    visitor(node, stopFn);

    for (Node* childNode : node._children) {
        if (stop) return;
        depthFirstOnNode(visitor, *childNode, stopFn, stop);
    }
}

void Sahara::Node::depthFirstOnNode(const Sahara::Node::NodeVisitorConst& visitor, const Sahara::Node& node, const Sahara::Node::NodeVisitorStopFn& stopFn, const bool& stop) const
{
    visitor(node, stopFn);

    for (Node* childNode : node._children) {
        if (stop) return;
        depthFirstOnNode(visitor, *childNode, stopFn, stop);
    }
}

void Sahara::Node::depthFirstOnNode(const Sahara::Node::NodeVisitor& visitorBefore, const Sahara::Node::NodeVisitor& visitorAfter, Sahara::Node& node, const Sahara::Node::NodeVisitorStopFn& stopFn, const bool& stop)
{
    visitorBefore(node, stopFn);

    if (stop) return;

    for (Node* childNode : node._children) {
        depthFirstOnNode(visitorBefore, visitorAfter, *childNode, stopFn, stop);
        if (stop) return;
    }

    visitorAfter(node, stopFn);
}

void Sahara::Node::depthFirstOnNode(const Sahara::Node::NodeVisitorConst& visitorBefore, const Sahara::Node::NodeVisitorConst& visitorAfter, const Sahara::Node& node, const Sahara::Node::NodeVisitorStopFn& stopFn, const bool& stop) const
{
    visitorBefore(node, stopFn);

    if (stop) return;

    for (Node* childNode : node._children) {
        depthFirstOnNode(visitorBefore, visitorAfter, *childNode, stopFn, stop);
        if (stop) return;
    }

    visitorAfter(node, stopFn);
}

#include "node.h"


Sahara::Node::Node(const QString& name, NodeItem* const nodeItem, const QMatrix4x4& transform)
    : _parent(nullptr)
    , _name(name)
    , _item(nodeItem)
    , _transform(transform)
    , _hasFocus(false)
    , _scale({1, 1, 1})
    , _index(0)
{

}

Sahara::Node::~Node()
{
    delete _item;

    for (const Node* node : _children) {
        delete node;
    }
}

bool Sahara::Node::isRoot() const
{
    return _parent == nullptr;
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

        root->depthFirst([&](Node& node) {
           if (&node == this)
               return false;

           if (node.name() == name) {
               node.trySetName(root, name, 1);
               return true;
           }

           return false;
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
    QMatrix4x4 globalTransform = transform();
    while (currentNode->_parent != nullptr) {
        globalTransform = currentNode->_parent->transform() * globalTransform;
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

    float m20 = _transform.row(2)[0];
    float m10 = _transform.row(1)[0];
    float m00 = _transform.row(0)[0];
    float m21 = _transform.row(2)[1];
    float m22 = _transform.row(2)[2];
    float m12 = _transform.row(1)[2];
    float m11 = _transform.row(1)[1];

    float lx = globalPosition().x();
    float ly = globalPosition().y();
    float lz = globalPosition().z();

    float rx, ry, rz;
    if (m20 < 1) {
        if (m20 > -1) {
            rx = qAtan2(m21, m22);
            ry = qAsin(-m20);
            rz = qAtan2(m10, m00);
        } else {
            rx = 0;
            ry = M_PI / 2;
            rz = -qAtan2(-m12, m11);
        }
    } else {
        rx = 0;
        ry = -M_PI / 2;
        rz = qAtan2(-m12, m11);
    }

    rx = qRadiansToDegrees(rx);
    ry = qRadiansToDegrees(ry);
    rz = qRadiansToDegrees(rz);

    float sx = _transform.column(0).length();
    float sy = _transform.column(1).length();
    float sz = _transform.column(2).length();

    _location = { lx, ly, lz };
    _rotation = { rx, ry, rz };
    _scale = { sx, sy, sz };
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

        root->depthFirst([&](Node& node) {
            if (&node == this)
                return false;

            if (node._hasFocus) {
                node._hasFocus = false;
                return true;
            }

            return false;
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

    if (globalVolumeLowerVertex.x() > globalVolumeUpperVertex.x()) {
        float x = globalVolumeLowerVertex.x();
        globalVolumeLowerVertex.setX(globalVolumeUpperVertex.x());
        globalVolumeUpperVertex.setX(x);
    }

    if (globalVolumeLowerVertex.y() > globalVolumeUpperVertex.y()) {
        float y = globalVolumeLowerVertex.y();
        globalVolumeLowerVertex.setY(globalVolumeUpperVertex.y());
        globalVolumeUpperVertex.setY(y);
    }

    if (globalVolumeLowerVertex.z() > globalVolumeUpperVertex.z()) {
        float z = globalVolumeLowerVertex.z();
        globalVolumeLowerVertex.setZ(globalVolumeUpperVertex.z());
        globalVolumeUpperVertex.setZ(z);
    }

    Volume globalVolume(globalVolumeLowerVertex, globalVolumeUpperVertex);

    return globalVolume.intersects(point);
}

bool Sahara::Node::depthFirst(const Sahara::Node::NodeVisitor& visitor)
{
    if (visitor(*this)) {
        return true;
    }

    for (Node* childNode : _children) {
        if (childNode->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}

bool Sahara::Node::depthFirst(const Sahara::Node::NodeVisitorConst& visitor) const
{
    if (visitor(*this)) {
        return true;
    }

    for (const Node* childNode : _children) {
        if (childNode->depthFirst(visitor)) {
            return true;
        }
    }

    return false;
}

bool Sahara::Node::depthFirst(const Sahara::Node::NodeVisitor& visitorBefore, const Sahara::Node::NodeVisitor& visitorAfter)
{
    if (visitorBefore(*this)) {
        return true;
    }

    for (Node* childNode : _children) {
        if (childNode->depthFirst(visitorBefore, visitorAfter)) {
            return true;
        }
    }

    if (visitorAfter(*this)) {
        return true;
    }

    return false;
}

bool Sahara::Node::depthFirst(const Sahara::Node::NodeVisitorConst& visitorBefore, const Sahara::Node::NodeVisitorConst& visitorAfter) const
{
    if (visitorBefore(*this)) {
        return true;
    }

    for (const Node* childNode : _children) {
        if (childNode->depthFirst(visitorBefore, visitorAfter)) {
            return true;
        }
    }

    if (visitorAfter(*this)) {
        return true;
    }

    return false;
}

const QVector3D Sahara::Node::location() const
{
    return _location;
}

const QVector3D Sahara::Node::rotation() const
{
    return _rotation;
}

const QVector3D Sahara::Node::scale() const
{
    return _scale;
}

void Sahara::Node::setLocation(const QVector3D& vec)
{
    _location = vec;
    recomputeTransform();
}

void Sahara::Node::setRotation(const QVector3D& vec)
{
    _rotation = vec;
    recomputeTransform();
}

void Sahara::Node::setScale(const QVector3D& vec)
{
    _scale = vec;
    recomputeTransform();
}

void Sahara::Node::trySetName(Sahara::Node* root, const QString name, const int suffix)
{
    QString newName = name+"."+QString("%1").arg(suffix, 3, 10, QChar('0'));

    bool found = false;
    root->depthFirst([&](const Node& node) {
        if (node.name() == newName) {
            found = true;
            trySetName(root, name, suffix+1);
            return true;
        }
        return false;
    });

    if (!found) {
        _name = newName;
    }
}

void Sahara::Node::recomputeTransform()
{
    QMatrix4x4 transform;
    transform.rotate(_rotation.z(), {0, 0, 1});
    transform.rotate(_rotation.y(), {0, 1, 0});
    transform.rotate(_rotation.x(), {1, 0, 0});

    if (!isRoot()) {
        QVector3D localPosition = parent().globalTransform().inverted().map(_location);
        transform.setColumn(3, {localPosition, 1});
    } else {
        transform.setColumn(3, {_location, 1});
    }

    float sx = _scale.x();
    float sy = _scale.y();
    float sz = _scale.z();

    QMatrix4x4 scale;
    scale.setRow(0, {sx, 0, 0, 0});
    scale.setRow(1, {0, sy, 0, 0});
    scale.setRow(2, {0, 0, sz, 0});

    _transform = transform * scale;
}

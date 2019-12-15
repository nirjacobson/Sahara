#include "json.h"

Sahara::JSON::JSON()
{

}

QJsonArray Sahara::JSON::fromVector3D(const QVector3D& vector)
{
    QJsonArray array;

    array.append(static_cast<double>(vector.x()));
    array.append(static_cast<double>(vector.y()));
    array.append(static_cast<double>(vector.z()));

    return array;
}

QVector3D Sahara::JSON::toVector3D(const QJsonArray& array)
{
    QVector3D vector;

    vector.setX(static_cast<float>(array.at(0).toDouble()));
    vector.setY(static_cast<float>(array.at(1).toDouble()));
    vector.setZ(static_cast<float>(array.at(2).toDouble()));

    return vector;
}

QJsonArray Sahara::JSON::fromVector4D(const QVector4D& vector)
{
    QJsonArray array;

    array.append(static_cast<double>(vector.x()));
    array.append(static_cast<double>(vector.y()));
    array.append(static_cast<double>(vector.z()));
    array.append(static_cast<double>(vector.w()));

    return array;
}

QVector4D Sahara::JSON::toVector4D(const QJsonArray& array)
{
    QVector4D vector;

    vector.setX(static_cast<float>(array.at(0).toDouble()));
    vector.setY(static_cast<float>(array.at(1).toDouble()));
    vector.setZ(static_cast<float>(array.at(2).toDouble()));
    vector.setW(static_cast<float>(array.at(3).toDouble()));

    return vector;
}

QJsonArray Sahara::JSON::fromQuaternion(const QQuaternion& quaternion)
{
    return fromVector4D(quaternion.toVector4D());
}

QQuaternion Sahara::JSON::toQuaternion(const QJsonArray& array)
{
    return QQuaternion(toVector4D(array));
}

QJsonArray Sahara::JSON::fromMatrix4x4(const QMatrix4x4& matrix)
{
    QJsonArray array;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            array.append(static_cast<double>(matrix.column(i)[j]));
        }
    }

    return array;
}

QMatrix4x4 Sahara::JSON::toMatrix4x4(const QJsonArray& array)
{
    QMatrix4x4 matrix;

    QVariantList variantList = array.toVariantList();

    for (int i = 0; i < 4; i++) {
        QVariantList column = variantList.mid(i * 4, 4);
        QVector4D columnVector = {
            static_cast<float>(column[0].toDouble()),
            static_cast<float>(column[1].toDouble()),
            static_cast<float>(column[2].toDouble()),
            static_cast<float>(column[3].toDouble()),
        };

        matrix.setColumn(i, columnVector);
    }

    return matrix;
}

QJsonArray Sahara::JSON::fromColor(const QColor& color)
{
    QJsonArray array;

    array.append(color.red());
    array.append(color.green());
    array.append(color.blue());
    array.append(color.alpha());

    return array;
}

QColor Sahara::JSON::toColor(const QJsonArray& array)
{
    QColor color;

    color.setRed(array.at(0).toInt());
    color.setGreen(array.at(1).toInt());
    color.setBlue(array.at(2).toInt());
    color.setAlpha(array.at(3).toInt());

    return color;
}

QJsonObject Sahara::JSON::fromTransform(const Sahara::Transform& transform)
{
    QJsonObject object;

    object["_type"] = "Transform";

    object["rotation"] = JSON::fromQuaternion(transform._rotation);
    object["translation"] = JSON::fromVector3D(transform._translation);

    return object;
}

Sahara::Transform Sahara::JSON::toTransform(const QJsonObject& object)
{
    assert(object["_type"] == "Transform");

    QQuaternion rotation = JSON::toQuaternion(object["rotation"].toArray());
    QVector3D translation = JSON::toVector3D(object["translation"].toArray());

    return Transform(rotation, translation);
}

QJsonObject Sahara::JSON::fromBone(const Sahara::Bone* bone)
{
    QJsonObject object;

    object["_type"] = "Bone";

    object["id"] = bone->_id;
    object["name"] = bone->_name;
    object["transform"] = fromTransform(bone->_transform);

    QJsonArray children;
    for (int i = 0; i < bone->_children.size(); i++) {
        children.append( fromBone(bone->_children.at(i)) );
    }

    return object;
}

Sahara::Bone*Sahara::JSON::toBone(const QJsonObject& object)
{
    assert(object["_type"] == "Bone");

    QString id = object["id"].toString();
    QString name = object["name"].toString();
    Transform transform = toTransform(object["transform"].toObject());

    Bone* bone = new Bone(nullptr, id, name, transform);

    QJsonArray children = object["children"].toArray();
    for (int i = 0; i < children.size(); i++) {
        bone->addChild( toBone(children.at(i).toObject()) );
    }

    return bone;
}

QJsonObject Sahara::JSON::fromAnimationKeyframe(const Sahara::Animation::Keyframe& keyframe)
{
    QJsonObject object;

    object["_type"] = "AnimationKeyframe";

    object["time"] = static_cast<double>(keyframe.time);
    object["transform"] = fromTransform(keyframe.transform);

    return object;
}

Sahara::Animation::Keyframe Sahara::JSON::toAnimationKeyframe(const QJsonObject& object)
{
    assert(object["_type"] == "AnimationKeyframe");

    Animation::Keyframe keyframe;
    keyframe.time = static_cast<float>(object["time"].toDouble());
    keyframe.transform = toTransform(object["transform"].toObject());

    return keyframe;
}

QJsonObject Sahara::JSON::fromAnimation(const Sahara::Animation* animation)
{
    QJsonObject object;

    object["_type"] = "Animation";
    object["id"] = animation->id();

    object["bone"] = animation->_bone->id();

    QJsonArray keyframes;
    for (int i = 0; i < animation->_keyframes.size(); i++) {
        keyframes.append( fromAnimationKeyframe(animation->_keyframes.at(i)) );
    }
    object["keyframes"] = keyframes;

    return object;
}

Sahara::Animation* Sahara::JSON::toAnimation(const QJsonObject& object, Armature& armature)
{
    assert(object["_type"] == "Animation");
    QString id = object["id"].toString();

    QString boneId = object["bone"].toString();
    Bone* bone = armature.getBoneById(boneId);

    QJsonArray keyframes = object["keyframes"].toArray();
    QList<Animation::Keyframe> animationKeyframes;
    for (int i = 0; i < keyframes.size(); i++) {
        animationKeyframes.append( toAnimationKeyframe(keyframes.at(i).toObject()) );
    }

    return new Animation(id, bone, animationKeyframes);
}

QJsonObject Sahara::JSON::fromAnimationClip(const Sahara::AnimationClip* animationClip)
{
    QJsonObject object;

    object["_type"] = "AnimationClip";
    object["id"] = animationClip->id();

    object["name"] = animationClip->_name;

    QJsonArray animations;
    for (int i = 0; i < animationClip->_animations.size(); i++) {
        animations.append( fromAnimation(animationClip->_animations.at(i)) );
    }

    object["animations"] = animations;

    return object;
}

Sahara::AnimationClip* Sahara::JSON::toAnimationClip(const QJsonObject& object, Armature& armature)
{
    assert(object["_type"] == "AnimationClip");
    QString id = object["id"].toString();

    QString name = object["name"].toString();

    QJsonArray animations = object["animations"].toArray();
    QList<Animation*> clipAnimations;
    for (int i = 0; i < animations.size(); i++) {
        clipAnimations.append( toAnimation(animations.at(i).toObject(), armature) );
    }

    return new AnimationClip(id, name, clipAnimations);
}

QJsonObject Sahara::JSON::fromArmature(const Sahara::Armature* armature)
{
    QJsonObject object;

    object["_type"] = "Armature";
    object["id"] = armature->id();

    object["root"] = fromBone(armature->_root);

    return object;
}

Sahara::Armature* Sahara::JSON::toArmature(const QJsonObject& object)
{
    assert(object["_type"] == "Armature");
    QString id = object["id"].toString();

    Bone* root = toBone(object["root"].toObject());

    return new Armature(id, root);
}

QJsonObject Sahara::JSON::fromCamera(const Sahara::Camera* camera)
{
    QJsonObject object;

    object["_type"] = "Camera";
    object["id"] = camera->id();

    object["mode"] = camera->_mode;
    object["xfov"] = static_cast<double>(camera->_xfov);
    object["xmag"] = static_cast<double>(camera->_xmag);
    object["aspect"] = static_cast<double>(camera->_aspect);
    object["znear"] = static_cast<double>(camera->_znear);
    object["zfar"] = static_cast<double>(camera->_zfar);

    return object;
}

Sahara::Camera* Sahara::JSON::toCamera(const QJsonObject& object)
{
    assert(object["_type"] == "Camera");
    QString id = object["id"].toString();

    Camera::Mode mode = static_cast<Camera::Mode>(object["mode"].toInt());
    float xfov = static_cast<float>(object["xfov"].toDouble());
    float xmag = static_cast<float>(object["xmag"].toDouble());
    float aspect = static_cast<float>(object["aspect"].toDouble());
    float znear = static_cast<float>(object["znear"].toDouble());
    float zfar = static_cast<float>(object["zfar"].toDouble());

    return new Camera(id, mode, xfov, xmag, aspect, znear, zfar);
}

QJsonObject Sahara::JSON::fromController(const Sahara::Controller* controller)
{
    QJsonObject object;

    object["_type"] = "Controller";
    object["id"] = controller->id();

    object["mesh"] = controller->_mesh->id();
    object["bindShapeMatrix"] = fromMatrix4x4(controller->_bindShapeMatrix);

    QJsonArray bonesArray;
    for (int i = 0; i < controller->_bones.size(); i++) {
        bonesArray.append(controller->_bones.at(i));
    }
    object["bones"] = bonesArray;

    QJsonArray inverseBindMatricesArray;
    for (int i = 0; i < controller->_inverseBindMatrices.size(); i++) {
        inverseBindMatricesArray.append(fromMatrix4x4(controller->_inverseBindMatrices.at(i)));
    }
    object["inverseBindMatrices"] = inverseBindMatricesArray;

    QJsonArray weightsArray;
    for (int i = 0; i < controller->_weights.size(); i++) {
        weightsArray.append(static_cast<double>(controller->_weights.at(i)));
    }
    object["weights"] = weightsArray;

    QJsonArray boneCountsArray;
    for (int i = 0; i < controller->_boneCounts.size(); i++) {
        boneCountsArray.append(controller->_boneCounts.at(i));
    }
    object["boneCounts"] = boneCountsArray;

    QJsonArray boneMappingsArray;
    for (int i = 0; i < controller->_boneMappings.size(); i++) {
        boneMappingsArray.append(controller->_boneMappings.at(i));
    }
    object["boneMappings"] = boneMappingsArray;

    return object;
}

Sahara::Controller* Sahara::JSON::toController(const QJsonObject& object, const Model& model)
{
    assert(object["_type"] == "Controller");
    QString id = object["id"].toString();

    Mesh* mesh = model.meshes()[object["mesh"].toString()];

    QMatrix4x4 bindShapeMatrix = toMatrix4x4(object["bindShapeMatrix"].toArray());

    QJsonArray bonesArray = object["bones"].toArray();
    QStringList bones;
    for (int i = 0; i < bonesArray.size(); i++) {
        bones.append(bonesArray.at(i).toString());
    }

    QJsonArray inverseBindMatricesArray = object["inverseBindMatrices"].toArray();
    QList<QMatrix4x4> inverseBindMatrices;
    for (int i = 0; i < inverseBindMatricesArray.size(); i++) {
        inverseBindMatrices.append( toMatrix4x4(inverseBindMatricesArray.at(i).toArray()) );
    }

    QJsonArray weightsArray = object["weights"].toArray();
    QList<float> weights;
    for (int i = 0; i < weightsArray.size(); i++) {
        weights.append(static_cast<float>(weightsArray.at(i).toDouble()));
    }

    QJsonArray boneCountsArray = object["boneCounts"].toArray();
    QList<int> boneCounts;
    for (int i = 0; i < boneCountsArray.size(); i++) {
        boneCounts.append(boneCountsArray.at(i).toInt());
    }

    QJsonArray boneMappingsArray = object["boneMappings"].toArray();
    QList<int> boneMappings;
    for (int i = 0; i < boneMappingsArray.size(); i++) {
        boneMappings.append(boneMappingsArray.at(i).toInt());
    }

    return new Controller(
        id,
        mesh,
        bindShapeMatrix,
        bones,
        inverseBindMatrices,
        weights,
        boneCounts,
                boneMappings);
}

QJsonObject Sahara::JSON::fromImage(const Sahara::Image* image)
{
    QJsonObject object;

    object["_type"] = "Image";
    object["id"] = image->id();

    object["uri"] = image->_uri;

    return object;
}

Sahara::Image*Sahara::JSON::toImage(const QJsonObject& object)
{
    assert(object["_type"] == "Image");
    QString id = object["id"].toString();

    QString uri = object["uri"].toString();

    return new Image(id, uri);
}

QJsonObject Sahara::JSON::fromMaterial(const Sahara::Material* material)
{
    QJsonObject object;

    object["_type"] = "Material";
    object["id"] = material->id();

    QJsonArray emissionArray = fromColor(material->_emission);
    QJsonArray ambientArray = fromColor(material->_ambient);
    QJsonArray diffuseArray = fromColor(material->_diffuse);
    QJsonArray specularArray = fromColor(material->_specular);

    object["emission"] = emissionArray;
    object["ambient"] = ambientArray;
    object["diffuse"] = diffuseArray;
    object["specular"] = specularArray;
    object["shininess"] = static_cast<double>(material->_shininess);

    if (material->image().has_value()) {
        object["image"] = (*material->image())->id();
    }

    return object;
}

Sahara::Material*Sahara::JSON::toMaterial(const QJsonObject& object, const Sahara::Model& model)
{
    assert(object["_type"] == "Material");
    QString id = object["id"].toString();

    QColor emission = toColor(object["emission"].toArray());
    QColor ambient = toColor(object["ambient"].toArray());
    QColor diffuse = toColor(object["diffuse"].toArray());
    QColor specular = toColor(object["specular"].toArray());
    float shininess = static_cast<float>(object["shininess"].toDouble());

    if (object.contains("image")) {
        Image* image = model.images()[object["image"].toString()];

        return new Material(id, emission, ambient, image, specular, shininess);
    } else {
        return new Material(id, emission, ambient, diffuse, specular, shininess);
    }
}

QJsonObject Sahara::JSON::fromPointLight(const Sahara::PointLight* pointLight)
{
    QJsonObject object;

    object["_type"] = "PointLight";
    object["id"] = pointLight->id();

    QJsonArray colorArray = fromColor(pointLight->_color);

    object["color"] = colorArray;
    object["constantAttenuation"] = static_cast<double>(pointLight->_constantAttenuation);
    object["linearAttenuation"] = static_cast<double>(pointLight->_linearAttenuation);
    object["quadraticAttenuation"] = static_cast<double>(pointLight->_quadraticAttenuation);

    return object;
}

Sahara::PointLight*Sahara::JSON::toPointLight(const QJsonObject& object)
{
    assert(object["_type"] == "PointLight");
    QString id = object["id"].toString();

    QColor color = toColor(object["color"].toArray());
    float constantAttenuation = static_cast<float>(object["constantAttenuation"].toDouble());
    float linearAttenuation = static_cast<float>(object["linearAttenuation"].toDouble());
    float quadraticAttenuation = static_cast<float>(object["quadraticAttenuation"].toDouble());

    return new PointLight(id, color, constantAttenuation, linearAttenuation, quadraticAttenuation);
}

QJsonObject Sahara::JSON::fromSource(const Sahara::Source* source)
{
    QJsonObject object;

    object["_type"] = "Source";

    QJsonArray dataArray;
    for (int i = 0; i < source->_data.size(); i++) {
        dataArray.append(static_cast<double>(source->_data.at(i)));
    }

    object["data"] = dataArray;
    object["stride"] = source->_stride;

    return object;
}

Sahara::Source*Sahara::JSON::toSource(const QJsonObject& object)
{
    assert(object["_type"] == "Source");

    QJsonArray dataArray = object["data"].toArray();
    QList<float> data;
    for (int i = 0; i < dataArray.size(); i++) {
        data.append(static_cast<float>(dataArray.at(i).toDouble()));
    }

    int stride = object["stride"].toInt();

    return new Source(data, stride);
}

QJsonObject Sahara::JSON::fromSurfaceInput(const Sahara::Surface::Input& input)
{
    QJsonObject object;

    object["_type"] = "Surface::Input";

    object["source"] = input.source();
    object["offset"] = input.offset();

    return object;
}

Sahara::Surface::Input Sahara::JSON::toSurfaceInput(const QJsonObject& object)
{
    assert(object["_type"] == "Surface::Input");

    QString source = object["source"].toString();
    int offset = object["offset"].toInt();

    return Surface::Input(source, offset);
}

QJsonObject Sahara::JSON::fromSurface(const Sahara::Surface* surface)
{
    QJsonObject object;

    object["_type"] = "Surface";

    object["material"] = surface->_material;

    QJsonObject inputsObject;
    for (QMap<Surface::Input::Semantic, Surface::Input>::const_iterator i = surface->_inputs.begin(); i != surface->_inputs.end(); i++) {
        object[Surface::Input::semanticToString(i.key())] = fromSurfaceInput(i.value());
    }

    QJsonArray elementsArray;
    for (int i = 0; i < surface->_elements.size(); i++) {
        elementsArray.append(surface->_elements.at(i));
    }

    object["inputs"] = inputsObject;
    object["elements"] = elementsArray;

    return object;
}

Sahara::Surface*Sahara::JSON::toSurface(const QJsonObject& object, const Sahara::Mesh& mesh)
{
    assert(object["_type"] == "Surface");

    QString material = object["material"].toString();

    QMap<Surface::Input::Semantic, Surface::Input> inputs;
    for (QJsonObject::const_iterator i = object.begin(); i != object.end(); i++) {
        Surface::Input::Semantic semantic = Surface::Input::semanticFromString(i.key());
        Surface::Input input = toSurfaceInput(i.value().toObject());

        inputs[semantic] = input;
    }

    QJsonArray elementsArray = object["elements"].toArray();
    QList<int> elements;
    for (int i = 0; i < elementsArray.size(); i++) {
        elements.append(elementsArray.at(i).toInt());
    }

    Surface* surface = new Surface(mesh._sources, material);
    surface->_inputs = inputs;
    surface->_elements = elements;

    return surface;
}

QJsonObject Sahara::JSON::fromMesh(const Sahara::Mesh* mesh)
{
    QJsonObject object;

    object["_type"] = "Mesh";
    object["id"] = mesh->id();

    QJsonObject sourcesObject;
    for (SourceDict::const_iterator i = mesh->_sources.begin(); i != mesh->_sources.end(); i++) {
        sourcesObject[i.key()] = fromSource(i.value());
    }

    QJsonArray surfacesArray;
    for (int i = 0; i < mesh->_surfaces.size(); i++) {
        surfacesArray.append(fromSurface(mesh->_surfaces.at(i)));
    }

    object["sources"] = sourcesObject;
    object["surfaces"] = surfacesArray;

    return object;
}

Sahara::Mesh*Sahara::JSON::toMesh(const QJsonObject& object)
{
    assert(object["_type"] == "Mesh");
    QString id = object["id"].toString();

    Mesh* mesh = new Mesh(id);

    QJsonObject sourcesObject = object["sources"].toObject();
    SourceDict sources;
    for (QJsonObject::iterator i = sourcesObject.begin(); i != sourcesObject.end(); i++) {
        sources[i.key()] = toSource(i.value().toObject());
    }

    QJsonArray surfacesArray = object["surfaces"].toArray();
    QList<Surface*> surfaces;
    for (int i = 0; i < surfacesArray.size(); i++) {
        surfaces.append(toSurface(surfacesArray.at(i).toObject(), *mesh));
    }

    mesh->_sources = sources;
    mesh->_surfaces = surfaces;

    return mesh;
}

QJsonObject Sahara::JSON::fromInstance(const Sahara::Instance* instance)
{
    QJsonObject object;

    QJsonObject materialsObject;
    for (MaterialDict::const_iterator i = instance->_materials.begin(); i != instance->_materials.end(); i++) {
        materialsObject[i.key()] = i.value()->id();
    }

    QJsonArray transformArray = fromMatrix4x4(instance->_transform);

    object["materials"] = materialsObject;
    object["transform"] = transformArray;

    return object;
}

void Sahara::JSON::toInstance(const QJsonObject& object, Sahara::Instance* instance, const Model& model)
{
    QJsonObject materialsObject = object["materials"].toObject();
    MaterialDict materials;
    for (QJsonObject::iterator i = materialsObject.begin(); i != materialsObject.end(); i++) {
        materials[i.key()] = model.materials()[i.value().toString()];
    }

    QMatrix4x4 transform = toMatrix4x4(object["transform"].toArray());

    instance->_materials = materials;
    instance->_transform = transform;
}

QJsonObject Sahara::JSON::fromInstanceMesh(const Sahara::InstanceMesh* instanceMesh)
{
    QJsonObject object = fromInstance(instanceMesh);

    object["_type"] = "InstanceMesh";

    object["mesh"] = instanceMesh->mesh().id();

    return object;
}

Sahara::InstanceMesh* Sahara::JSON::toInstanceMesh(const QJsonObject& object, const Sahara::Model& model)
{
    assert(object["_type"] == "InstanceMesh");

    Mesh* mesh = model.meshes()[object["mesh"].toString()];

    InstanceMesh* instanceMesh = new InstanceMesh({}, {}, mesh);

    toInstance(object, instanceMesh, model);

    return instanceMesh;
}

QJsonObject Sahara::JSON::fromInstanceController(const Sahara::InstanceController* instanceController)
{
    QJsonObject object = fromInstance(instanceController);

    object["_type"] = "InstanceController";

    object["controller"] = instanceController->_controller->id();

    return object;
}

Sahara::InstanceController*Sahara::JSON::toInstanceController(const QJsonObject& object, const Sahara::Model& model)
{
    assert(object["_type"] == "InstanceController");

    Controller* controller = model.controllers()[object["controller"].toString()];

    InstanceController* instanceController = new InstanceController(&model.armature(), {}, {}, controller);

    toInstance(object, instanceController, model);

    return instanceController;
}


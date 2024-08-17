#include "json.h"
#include "render/renderer.h"

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

QJsonObject Sahara::JSON::fromJoint(const Sahara::Joint* joint)
{
    QJsonObject object;

    object["_type"] = "Joint";

    object["id"] = joint->_id;
    object["name"] = joint->_name;
    object["transform"] = fromTransform(joint->_transform);

    QJsonArray children;
    for (int i = 0; i < joint->_children.size(); i++) {
        children.append( fromJoint(joint->_children.at(i)) );
    }
    object["children"] = children;

    return object;
}

Sahara::Joint*Sahara::JSON::toJoint(const QJsonObject& object)
{
    assert(object["_type"] == "Joint");

    QString id = object["id"].toString();
    QString name = object["name"].toString();
    Transform transform = toTransform(object["transform"].toObject());

    Joint* joint = new Joint(nullptr, id, name, transform);

    QJsonArray children = object["children"].toArray();
    for (int i = 0; i < children.size(); i++) {
        joint->addChild( toJoint(children.at(i).toObject()) );
    }

    return joint;
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

    object["joint"] = animation->_joint->id();

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

    QString jointId = object["joint"].toString();
    Joint* joint = armature.getJointById(jointId);

    QJsonArray keyframes = object["keyframes"].toArray();
    QList<Animation::Keyframe> animationKeyframes;
    for (int i = 0; i < keyframes.size(); i++) {
        animationKeyframes.append( toAnimationKeyframe(keyframes.at(i).toObject()) );
    }

    return new Animation(id, joint, animationKeyframes);
}

QJsonObject Sahara::JSON::fromAnimationClip(const Sahara::AnimationClip* animationClip)
{
    QJsonObject object;

    object["_type"] = "AnimationClip";
    object["id"] = animationClip->id();

    object["name"] = animationClip->_name;

    QJsonArray animations;
    for (int i = 0; i < animationClip->_animations.size(); i++) {
        animations.append( animationClip->_animations.at(i)->id() );
    }

    object["animations"] = animations;

    return object;
}

Sahara::AnimationClip* Sahara::JSON::toAnimationClip(const QJsonObject& object, Model& model)
{
    assert(object["_type"] == "AnimationClip");
    QString id = object["id"].toString();

    QString name = object["name"].toString();

    QJsonArray animations = object["animations"].toArray();
    QList<Animation*> clipAnimations;
    for (int i = 0; i < animations.size(); i++) {
        clipAnimations.append( model.animations()[animations.at(i).toString()] );
    }

    return new AnimationClip(id, name, clipAnimations);
}

QJsonObject Sahara::JSON::fromArmature(const Sahara::Armature* armature)
{
    QJsonObject object;

    object["_type"] = "Armature";
    object["id"] = armature->id();

    object["root"] = fromJoint(armature->_root);

    return object;
}

Sahara::Armature* Sahara::JSON::toArmature(const QJsonObject& object)
{
    assert(object["_type"] == "Armature");
    QString id = object["id"].toString();

    Joint* root = toJoint(object["root"].toObject());

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

    QJsonArray jointsArray;
    for (int i = 0; i < controller->_joints.size(); i++) {
        jointsArray.append(controller->_joints.at(i));
    }
    object["joints"] = jointsArray;

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

    QJsonArray jointCountsArray;
    for (int i = 0; i < controller->_jointCounts.size(); i++) {
        jointCountsArray.append(controller->_jointCounts.at(i));
    }
    object["jointCounts"] = jointCountsArray;

    QJsonArray jointMappingsArray;
    for (int i = 0; i < controller->_jointMappings.size(); i++) {
        jointMappingsArray.append(controller->_jointMappings.at(i));
    }
    object["jointMappings"] = jointMappingsArray;

    return object;
}

Sahara::Controller* Sahara::JSON::toController(const QJsonObject& object, const Model& model)
{
    assert(object["_type"] == "Controller");
    QString id = object["id"].toString();

    Mesh* mesh = model.meshes()[object["mesh"].toString()];

    QMatrix4x4 bindShapeMatrix = toMatrix4x4(object["bindShapeMatrix"].toArray());

    QJsonArray jointsArray = object["joints"].toArray();
    QStringList joints;
    for (int i = 0; i < jointsArray.size(); i++) {
        joints.append(jointsArray.at(i).toString());
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

    QJsonArray jointCountsArray = object["jointCounts"].toArray();
    QList<int> jointCounts;
    for (int i = 0; i < jointCountsArray.size(); i++) {
        jointCounts.append(jointCountsArray.at(i).toInt());
    }

    QJsonArray jointMappingsArray = object["jointMappings"].toArray();
    QList<int> jointMappings;
    for (int i = 0; i < jointMappingsArray.size(); i++) {
        jointMappings.append(jointMappingsArray.at(i).toInt());
    }

    Controller* controller = new Controller(
        id,
        mesh,
        bindShapeMatrix,
        joints,
        inverseBindMatrices,
        weights,
        jointCounts,
        jointMappings);

    controller->generateVertexBuffers();

    return controller;
}

QJsonObject Sahara::JSON::fromImage(const Sahara::Image* image)
{
    QJsonObject object;

    object["_type"] = "Image";
    object["id"] = image->id();

    object["uri"] = image->_uri;

    return object;
}

Sahara::Image* Sahara::JSON::toImage(Renderer* renderer, const QJsonObject& object)
{
    assert(object["_type"] == "Image");
    QString id = object["id"].toString();

    QString uri = object["uri"].toString();

    OpenGLRenderer* oglRenderer;
    VulkanRenderer* vlkRenderer;
    if ((oglRenderer = dynamic_cast<OpenGLRenderer*>(renderer))) {
        return new OpenGLImage(id, uri);
    }

    vlkRenderer = dynamic_cast<VulkanRenderer*>(renderer);
    return new VulkanImage(vlkRenderer, id, uri);
}

QJsonObject Sahara::JSON::fromMaterial(const Sahara::Material* material)
{
    QJsonObject object;

    object["_type"] = "Material";
    object["id"] = material->id();
    object["name"] = material->name();

    QJsonArray emissionArray = fromColor(material->_emission);
    QJsonArray ambientArray = fromColor(material->_ambient);
    QJsonArray diffuseArray = fromColor(material->_diffuse);
    QJsonArray specularArray = fromColor(material->_specular);

    object["emission"] = emissionArray;
    object["ambient"] = ambientArray;
    object["diffuse"] = diffuseArray;
    object["specular"] = specularArray;
    object["shininess"] = static_cast<double>(material->_shininess);

    const Sahara::OpenGLMaterial* oglMaterial;
    const Sahara::VulkanMaterial* vlkMaterial;
    if ((oglMaterial = dynamic_cast<const Sahara::OpenGLMaterial*>(material))) {
        if (oglMaterial->image().has_value()) {
            object["image"] = (*oglMaterial->image())->id();
        }
    } else {
        vlkMaterial = dynamic_cast<const Sahara::VulkanMaterial*>(material);
        if (vlkMaterial->image().has_value()) {
            object["image"] = (*vlkMaterial->image())->id();
        }
    }

    return object;
}

Sahara::Material* Sahara::JSON::toMaterial(Renderer* renderer, const QJsonObject& object, const Sahara::Model& model)
{
    assert(object["_type"] == "Material");
    QString id = object["id"].toString();
    QString name = object["name"].toString();

    QColor emission = toColor(object["emission"].toArray());
    QColor ambient = toColor(object["ambient"].toArray());
    QColor diffuse = toColor(object["diffuse"].toArray());
    QColor specular = toColor(object["specular"].toArray());
    float shininess = static_cast<float>(object["shininess"].toDouble());

    OpenGLRenderer* oglRenderer;
    VulkanRenderer* vlkRenderer;
    if ((oglRenderer = dynamic_cast<OpenGLRenderer*>(renderer))) {
        if (object.contains("image")) {
            OpenGLImage* image = dynamic_cast<OpenGLImage*>(model.images()[object["image"].toString()]);

            return new OpenGLMaterial(id, name, emission, ambient, image, specular, shininess);
        } else {
            return new OpenGLMaterial(id, name, emission, ambient, diffuse, specular, shininess);
        }
    }

    vlkRenderer = dynamic_cast<VulkanRenderer*>(renderer);
    if (object.contains("image")) {
        VulkanImage* image = dynamic_cast<VulkanImage*>(model.images()[object["image"].toString()]);

        return new VulkanMaterial(vlkRenderer, id, name, emission, ambient, image, specular, shininess);
    } else {
        return new VulkanMaterial(vlkRenderer, id, name, emission, ambient, diffuse, specular, shininess);
    }

}

QJsonObject Sahara::JSON::fromPointLight(const Sahara::PointLight* pointLight)
{
    QJsonObject object;

    object["_type"] = "PointLight";
    object["id"] = pointLight->id();

    QJsonArray colorArray = fromColor(pointLight->color());

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
        inputsObject[Surface::Input::semanticToString(i.key())] = fromSurfaceInput(i.value());
    }

    QJsonArray elementsArray;
    for (int i = 0; i < surface->_elements.size(); i++) {
        elementsArray.append(surface->_elements.at(i));
    }

    object["inputs"] = inputsObject;
    object["elements"] = elementsArray;

    return object;
}

Sahara::Surface*Sahara::JSON::toSurface(QVulkanWindow* window, const QJsonObject& object, const Sahara::Mesh& mesh)
{
    assert(object["_type"] == "Surface");

    QString material = object["material"].toString();

    QJsonObject inputsObject = object["inputs"].toObject();
    QMap<Surface::Input::Semantic, Surface::Input> inputs;
    for (QJsonObject::const_iterator i = inputsObject.begin(); i != inputsObject.end(); i++) {
        Surface::Input::Semantic semantic = Surface::Input::semanticFromString(i.key());
        Surface::Input input = toSurfaceInput(i.value().toObject());

        inputs[semantic] = input;
    }

    QJsonArray elementsArray = object["elements"].toArray();
    QList<int> elements;
    for (int i = 0; i < elementsArray.size(); i++) {
        elements.append(elementsArray.at(i).toInt());
    }

    Surface* surface;

    if (window) {
        surface = new VulkanSurface(window, mesh._sources, material);
    } else {
        surface = new OpenGLSurface(mesh._sources, material);
    }

    surface->_inputs = inputs;
    surface->_elements = elements;

    for (const auto semantic : inputs.keys()) {
        surface->generateVertexBuffer(semantic);
    }

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

Sahara::Mesh*Sahara::JSON::toMesh(QVulkanWindow* window, const QJsonObject& object)
{
    assert(object["_type"] == "Mesh");
    QString id = object["id"].toString();

    Mesh* mesh;

    if (window) {
        mesh = new VulkanMesh(window, id);
    } else {
        mesh = new OpenGLMesh(id);
    }

    QJsonObject sourcesObject = object["sources"].toObject();
    SourceDict sources;
    for (QJsonObject::iterator i = sourcesObject.begin(); i != sourcesObject.end(); i++) {
        sources[i.key()] = toSource(i.value().toObject());
    }
    mesh->_sources = sources;

    QJsonArray surfacesArray = object["surfaces"].toArray();
    QList<Surface*> surfaces;
    for (int i = 0; i < surfacesArray.size(); i++) {
        surfaces.append(toSurface(window, surfacesArray.at(i).toObject(), *mesh));
    }
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

QJsonObject Sahara::JSON::fromInstanceController(const InstanceController* instanceController)
{
    QJsonObject object = fromInstance(instanceController);

    object["_type"] = "InstanceController";

    object["controller"] = instanceController->_controller->id();

    return object;
}

Sahara::InstanceController* Sahara::JSON::toInstanceController(Renderer* renderer, const QJsonObject& object, const Sahara::Model& model)
{
    assert(object["_type"] == "InstanceController");

    Controller* controller = model.controllers()[object["controller"].toString()];

    InstanceController* instanceController;

    OpenGLRenderer* oglRenderer;
    VulkanRenderer* vlkRenderer;
    if ((oglRenderer = dynamic_cast<OpenGLRenderer*>(renderer))) {
        instanceController = new OpenGLInstanceController(&model.armature(), {}, {}, controller);
    } else {
        vlkRenderer = dynamic_cast<VulkanRenderer*>(renderer);
        instanceController = new VulkanInstanceController(vlkRenderer, &model.armature(), {}, {}, controller);
    }

    instanceController->_armature = model._armature;

    toInstance(object, instanceController, model);

    return instanceController;
}

QJsonObject Sahara::JSON::fromVolume(const Sahara::Volume& volume)
{
    QJsonObject object;

    object["_type"] = "Volume";

    object["lowerVertex"] = fromVector3D(volume._lowerVertex);
    object["upperVertex"] = fromVector3D(volume._upperVertex);

    return object;
}

Sahara::Volume Sahara::JSON::toVolume(const QJsonObject& object)
{
    assert(object["_type"] == "Volume");

    QVector3D lowerVertex = toVector3D(object["lowerVertex"].toArray());
    QVector3D upperVertex = toVector3D(object["upperVertex"].toArray());

    return Volume(lowerVertex, upperVertex);
}

QJsonObject Sahara::JSON::fromModel(const Sahara::Model* model)
{
    QJsonObject object;

    object["_type"] = "Model";

    QJsonObject volumeObject = fromVolume(model->_volume);
    object["volume"] = volumeObject;

    QJsonObject imagesObject;
    for (ImageDict::const_iterator i = model->_images.begin(); i != model->_images.end(); i++) {
        imagesObject[i.key()] = fromImage(i.value());
    }
    object["images"] = imagesObject;

    QJsonObject materialsObject;
    for (MaterialDict::const_iterator i = model->_materials.begin(); i != model->_materials.end(); i++) {
        materialsObject[i.key()] = fromMaterial(i.value());
    }
    object["materials"] = materialsObject;

    QJsonObject meshesObject;
    for (MeshDict::const_iterator i = model->_meshes.begin(); i != model->_meshes.end(); i++) {
        meshesObject[i.key()] = fromMesh(i.value());
    }
    object["meshes"] = meshesObject;

    QJsonObject controllersObject;
    for (ControllerDict::const_iterator i = model->_controllers.begin(); i != model->_controllers.end(); i++) {
        controllersObject[i.key()] = fromController(i.value());
    }
    object["controllers"] = controllersObject;

    QJsonArray instancesArray;
    for (int i = 0; i < model->_instances.size(); i++) {
        Instance* instance = model->_instances.at(i);
        InstanceMesh* instanceMesh;
        InstanceController* instanceController;
        if ((instanceMesh = dynamic_cast<InstanceMesh*>(instance))) {
            instancesArray.append( fromInstanceMesh(instanceMesh) );
        } else if ((instanceController = dynamic_cast<InstanceController*>(instance))) {
            instancesArray.append( fromInstanceController(instanceController) );
        }
    }
    object["instances"] = instancesArray;

    if (model->_armature) {
        QJsonObject armatureObject = fromArmature(model->_armature);
        object["armature"] = armatureObject;

        QJsonObject animationsObject;
        for (AnimationDict::const_iterator i = model->_animations.begin(); i != model->_animations.end(); i++) {
            animationsObject[i.key()] = fromAnimation(i.value());
        }
        object["animations"] = animationsObject;

        QJsonObject animationClipsObject;
        for (AnimationClipDict::const_iterator i = model->_animationClips.begin(); i != model->_animationClips.end(); i++) {
            animationClipsObject[i.key()] = fromAnimationClip(i.value());
        }
        object["animationClips"] = animationClipsObject;

        QString animationClip = model->_animationClip->id();
        object["animationClip"] = animationClip;
    }

    return object;
}

Sahara::Model* Sahara::JSON::toModel(Renderer* renderer, const QJsonObject& object)
{
    assert(object["_type"] == "Model");

    Model* model;

    OpenGLRenderer* oglRenderer = nullptr;
    VulkanRenderer* vlkRenderer = nullptr;
    if ((oglRenderer = dynamic_cast<OpenGLRenderer*>(renderer))) {
        model = new OpenGLModel;
    } else {
        model = new VulkanModel;
    }

    Volume volume = toVolume(object["volume"].toObject());
    model->_volume = volume;

    QJsonObject imagesObject = object["images"].toObject();
    ImageDict images;
    for (QJsonObject::iterator i = imagesObject.begin(); i != imagesObject.end(); i++) {
        images[i.key()] = toImage(renderer, i.value().toObject());
    }
    model->_images = images;

    QJsonObject materialsObject = object["materials"].toObject();
    MaterialDict materials;
    for (QJsonObject::iterator i = materialsObject.begin(); i != materialsObject.end(); i++) {
        materials[i.key()] = toMaterial(renderer, i.value().toObject(), *model);
    }
    model->_materials = materials;

    QJsonObject meshesObject = object["meshes"].toObject();
    MeshDict meshes;
    for (QJsonObject::iterator i = meshesObject.begin(); i != meshesObject.end(); i++) {
        QVulkanWindow* window = vlkRenderer ? vlkRenderer->window() : nullptr;
        meshes[i.key()] = toMesh(window, i.value().toObject());
    }
    model->_meshes = meshes;

    QJsonObject controllersObject = object["controllers"].toObject();
    ControllerDict controllers;
    for (QJsonObject::iterator i = controllersObject.begin(); i != controllersObject.end(); i++) {
        controllers[i.key()] = toController(i.value().toObject(), *model);
    }
    model->_controllers = controllers;

    if (object.contains("armature")) {
        QJsonObject armatureObject = object["armature"].toObject();
        Armature* armature = toArmature(armatureObject);
        model->_armature = armature;

        QJsonObject animationsObject = object["animations"].toObject();
        AnimationDict animations;
        for (QJsonObject::iterator i = animationsObject.begin(); i != animationsObject.end(); i++) {
            animations[i.key()] = toAnimation(i.value().toObject(), *model->_armature);
        }
        model->_animations = animations;

        QJsonObject animationClipsObject = object["animationClips"].toObject();
        AnimationClipDict animationClips;
        for (QJsonObject::iterator i = animationClipsObject.begin(); i != animationClipsObject.end(); i++) {
            animationClips[i.key()] = toAnimationClip(i.value().toObject(), *model);
        }
        model->_animationClips = animationClips;

        QString animationClip = object["animationClip"].toString();
        model->_animationClip = model->_animationClips[animationClip];
    } else {
        model->_armature = nullptr;
        model->_animationClip = nullptr;
    }

    QJsonArray instancesArray = object["instances"].toArray();
    QList<Instance*> instances;
    for (int i = 0; i < instancesArray.size(); i++) {
        QJsonObject instanceObject = instancesArray.at(i).toObject();

        if (instanceObject["_type"] == "InstanceMesh") {
            instances.append( toInstanceMesh(instanceObject, *model) );
        } else if (instanceObject["_type"] == "InstanceController") {
            instances.append( toInstanceController(renderer, instanceObject, *model) );
        }
    }
    model->_instances = instances;

    return model;
}

QJsonObject Sahara::JSON::fromNode(const Sahara::Node* node)
{
    QJsonObject object;

    object["_type"] = "Node";

    object["index"] = node->_index;
    object["name"] = node->_name;
    object["item"] = QString::number(reinterpret_cast<long>(&node->item()));
    object["transform"] = fromMatrix4x4(node->_transform);
    object["hasFocus"] = node->_hasFocus;

    QJsonArray childrenArray;
    for (int i = 0; i < node->_children.size(); i++) {
        childrenArray.append(fromNode(node->_children.at(i)));
    }
    object["children"] = childrenArray;

    return object;
}

Sahara::Node* Sahara::JSON::toNode(const QJsonObject& object, const QMap<QString, NodeItem*>& items)
{
    assert(object["_type"] == "Node");

    QString name = object["name"].toString();
    NodeItem* item = items[object["item"].toString()];
    QMatrix4x4 transform = toMatrix4x4(object["transform"].toArray());

    Node* node = new Node(name, item, transform);

    node->_index = object["index"].toInt();
    node->_hasFocus = object["hasFocus"].toBool();

    QJsonArray childrenArray = object["children"].toArray();
    QList<Node*> children;
    for (int i = 0; i < childrenArray.size(); i++) {
        QJsonObject nodeObject = childrenArray.at(i).toObject();
        Node* childNode = toNode(nodeObject, items);
        childNode->_parent = node;
        children.append(childNode);
    }
    node->_children = children;

    return node;
}

QJsonObject Sahara::JSON::fromScene(const Sahara::Scene* scene)
{
    QJsonObject object;

    object["_type"] = "Scene";

    QJsonObject itemsObject;
    scene->_root->depthFirst([&](const Node& node) {
        if (&node == scene->_root)
            return false;

        QString id = QString::number(reinterpret_cast<long>(&node.item()));
        if (itemsObject.contains(id))
            return false;

        const NodeItem& nodeItem = node.item();
        QJsonObject itemObject;
        const Camera* camera;
        const PointLight* pointLight;
        const Model* model;
        if ((camera = dynamic_cast<const Camera*>(&nodeItem))) {
            itemObject = fromCamera(camera);
        } else if ((pointLight = dynamic_cast<const PointLight*>(&nodeItem))) {
            itemObject = fromPointLight(pointLight);
        } else if ((model = dynamic_cast<const Model*>(&nodeItem))) {
            itemObject = fromModel(model);
        }

        itemsObject[id] = itemObject;

        return false;
    });
    object["items"] = itemsObject;

    object["root"] = fromNode(&scene->root());

    object["camera"] = static_cast<const Camera&>(scene->_cameraNode->item()).id();

    return object;
}

Sahara::Scene* Sahara::JSON::toScene(Renderer* renderer, const QJsonObject& object)
{
    assert(object["_type"] == "Scene");

    QJsonObject itemsObject = object["items"].toObject();
    QMap<QString, NodeItem*> items;
    for (QJsonObject::iterator i = itemsObject.begin(); i != itemsObject.end(); i++) {
        QString id = i.key();
        QJsonObject itemObject = i.value().toObject();
        if (itemObject["_type"] == "PointLight") {
            items.insert(id, toPointLight(itemObject));
        } else if (itemObject["_type"] == "Camera") {
            items.insert(id, toCamera(itemObject));
        } else if (itemObject["_type"] == "Model") {
            items.insert(id, toModel(renderer, itemObject));
        }
    }

    Scene* scene;

    OpenGLRenderer* oglRenderer;
    VulkanRenderer* vlkRenderer;

    if ((oglRenderer = dynamic_cast<OpenGLRenderer*>(renderer))) {
        scene = new OpenGLScene;
    } else {
        vlkRenderer = dynamic_cast<VulkanRenderer*>(renderer);
        scene = new VulkanScene(vlkRenderer);
    }

    scene->_root = toNode(object["root"].toObject(), items);

    scene->_root->depthFirst([&](Node& node) {
        const Camera* camera;
        if ((camera = dynamic_cast<const Camera*>(&node.item()))) {
            if (camera->id() == object["camera"].toString()) {
                scene->_cameraNode = &node;
                return true;
            }
        }
        return false;
    });

    return scene;
}


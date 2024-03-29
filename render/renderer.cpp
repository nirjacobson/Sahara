#include "renderer.h"

Sahara::Renderer::Renderer()
    : _grid(32)
    , _showGrid(false)
    , _showAxes(true)
    , _showLights(true)
    , _showCameras(true)
{
    glClearColor(0.5f, 0.75f, 0.86f, 1.0f);
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glClearDepthf(1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    assert(glGetError() == GL_NO_ERROR);
}

Sahara::Renderer::~Renderer()
{

}

void Sahara::Renderer::render(Sahara::Scene& scene, const float time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene(scene, time);
}

bool Sahara::Renderer::showGrid() const
{
    return _showGrid;
}

void Sahara::Renderer::showGrid(const bool visible)
{
    _showGrid = visible;
}

bool Sahara::Renderer::showAxes() const
{
    return _showAxes;
}

void Sahara::Renderer::showAxes(const bool visible)
{
    _showAxes = visible;
}

bool Sahara::Renderer::showLights() const
{
    return _showLights;
}

void Sahara::Renderer::showLights(const bool visible)
{
    _showLights = visible;
}

bool Sahara::Renderer::showCameras() const
{
    return _showCameras;
}

void Sahara::Renderer::showCameras(const bool visible)
{
    _showCameras = visible;
}

void Sahara::Renderer::renderScene(Scene& scene, const float time)
{
    if (_showGrid)
        renderGrid(scene);

    _sceneProgram.bind();

    _sceneProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _sceneProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _sceneProgram.setProjection(scene.camera().projection());

    processSceneLighting(scene);

    QStack<QMatrix4x4> transforms;
    transforms.push(QMatrix4x4());
    scene.root().depthFirst([&](Node& node) {
       transforms.push(transforms.top() * node.transform());
       Model* model;
       PointLight* pointLight;
       Camera* camera;
       if ((model = dynamic_cast<Model*>(&node.item()))) {
           _sceneProgram.setFocus(false);
           renderModel(*model, transforms, false, time);
           if (node.hasFocus()) {
               _sceneProgram.setFocus(true);
               renderModel(*model, transforms, true, time);
           }
       } else {
           _sceneProgram.release();

           if ((pointLight = dynamic_cast<PointLight*>(&node.item()))) {
               if (_showLights) {
                   renderPointLight(scene, transforms.top(), node.hasFocus());
               }
           } else if ((camera = dynamic_cast<Camera*>(&node.item()))) {
               if (_showCameras && &node != &scene.cameraNode()) {
                   renderCamera(scene, transforms.top(), node.hasFocus());
               }
           }

           _sceneProgram.bind();
       }

       return false;
    }, [&](Node&) {
        transforms.pop();
        return false;
     });

    _sceneProgram.release();
}

void Sahara::Renderer::renderGrid(Scene& scene)
{
    _gridProgram.bind();

    _gridProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _gridProgram.setProjection(scene.camera().projection());

    _gridProgram.setGrid(_grid);
    for (int i = 0; i < 2 * _grid.length(); i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    _gridProgram.clearGrid(_grid);

    if (_showAxes) {
        _gridProgram.setAxis(_grid.xAxis());
        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
        }
        _gridProgram.clearAxis(_grid.xAxis());

        _gridProgram.setAxis(_grid.yAxis());
        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
        }
        _gridProgram.clearAxis(_grid.yAxis());

        _gridProgram.setAxis(_grid.zAxis());
        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
        }
        _gridProgram.clearAxis(_grid.zAxis());
    }

    _gridProgram.release();
}

void Sahara::Renderer::renderPointLight(Scene& scene, const QMatrix4x4& modelView, const bool focus)
{
    _displayProgram.bind();

    _displayProgram.setModelView(modelView);
    _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _displayProgram.setProjection(scene.camera().projection());
    _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _displayProgram.setFocus(focus);

    _displayProgram.setDisplay(_pointLightDisplay);

    VertexBuffer buffer = _pointLightDisplay.vertexBuffers().first();
    int vertices = buffer.count();

    for (int i = 0; i < vertices / 3; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 3, 3);
    }

    _displayProgram.clearDisplay(_pointLightDisplay);

    _displayProgram.release();
}

void Sahara::Renderer::renderCamera(Sahara::Scene& scene, const QMatrix4x4& modelView, const bool focus)
{
    _displayProgram.bind();

    _displayProgram.setModelView(modelView);
    _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _displayProgram.setProjection(scene.camera().projection());
    _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _displayProgram.setFocus(focus);

    _displayProgram.setDisplay(_cameraDisplay);

    VertexBuffer buffer = _cameraDisplay.vertexBuffers().first();
    int vertices = buffer.count();

    for (int i = 0; i < vertices / 3; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 3, 3);
    }

    _displayProgram.clearDisplay(_cameraDisplay);

    _displayProgram.release();
}

void Sahara::Renderer::renderModel(Sahara::Model& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time)
{
    model.animate(time);

    for (Instance* instance : model.instances()) {
        transformStack.push(transformStack.top() * instance->transform());
        _sceneProgram.setModelView(transformStack.top());

        InstanceMesh* meshInstance;
        InstanceController* controllerInstance;
        if ((meshInstance = dynamic_cast<InstanceMesh*>(instance))) {
            _sceneProgram.setArticulated(false);

            for (int i = 0; i < meshInstance->mesh().count(); i++) {
                renderSurface(meshInstance->mesh().surface(i), *meshInstance, focus);
            }
        } else if ((controllerInstance = dynamic_cast<InstanceController*>(instance))) {
            _sceneProgram.setArticulated(true);

            processControllerInstanceArmature(*controllerInstance);

            for (int i = 0; i < controllerInstance->controller().mesh().count(); i++) {
                renderSurface(controllerInstance->controller().mesh().surface(i), *controllerInstance, focus);
            }
        }

        transformStack.pop();
    }
}

void Sahara::Renderer::renderSurface(Sahara::Surface& surface, Instance& instance, const bool focus)
{
    const Material& material = instance.getMaterial(surface.material());
    _sceneProgram.setMaterial(material);
    if (material.image().has_value()) {
        (*material.image())->bind();
    }

    _sceneProgram.setSurface(surface);
    if (focus) {
        VertexBuffer buffer = surface.vertexBuffers().first();
        int vertices = buffer.count();

        for (int i = 0; i < vertices / 3; i++) {
            glDrawArrays(GL_LINE_LOOP, i * 3, 3);
        }
    } else {
        glDrawArrays(GL_TRIANGLES, 0, surface.vertexBuffers().first().count());
    }
   _sceneProgram.clearSurface(surface);

   assert(glGetError() == GL_NO_ERROR);
}

void Sahara::Renderer::processSceneLighting(Sahara::Scene& scene)
{
    _sceneProgram.setAmbientLight(scene.ambientLight());
    _sceneProgram.clearPointLights();
    scene.root().depthFirst([&](const Node& node) {
        const PointLight* pointLight;
        if ((pointLight = dynamic_cast<const PointLight*>(&node.item()))) {
            _sceneProgram.addPointLight(*pointLight, node.globalPosition());
        }
        return false;
    });
}

void Sahara::Renderer::processControllerInstanceArmature(Sahara::InstanceController& controllerInstance)
{
    const Controller& controller = controllerInstance.controller();

    QList<Transform> jointTransforms;
    for (int i = 0; i < controller.joints().size(); i++) {
        QString jointName = controller.joints().at(i);
        const Joint* joint = controllerInstance.armature().getJointByName(jointName);
        QMatrix4x4 inverseBindMatrix = controller.inverseBindMatrices().at(i);
        QMatrix4x4 matrix = inverseBindMatrix * controller.bindShapeMatrix();

        Transform transform = Transform(matrix);

        while (joint) {
            transform = joint->transform() * transform;
            joint = &joint->parent();
        }

        jointTransforms.push_back(transform);
    }

    _sceneProgram.setJointTransforms(jointTransforms);
}

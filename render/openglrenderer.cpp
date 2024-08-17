#include "openglrenderer.h"

Sahara::OpenGLRenderer::OpenGLRenderer()
    : _grid(32)
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

Sahara::OpenGLRenderer::~OpenGLRenderer()
{

}

void Sahara::OpenGLRenderer::render(Sahara::OpenGLScene& scene, const float time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene(scene, time);
}



void Sahara::OpenGLRenderer::renderScene(OpenGLScene& scene, const float time)
{
    if (showGrid())
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
       OpenGLModel* model;
       PointLight* pointLight;
       Camera* camera;
       if ((model = dynamic_cast<OpenGLModel*>(&node.item()))) {
           _sceneProgram.setFocus(false);
           renderModel(*model, transforms, false, time);
           if (node.hasFocus()) {
               _sceneProgram.setFocus(true);
               renderModel(*model, transforms, true, time);
           }
       } else {
           _sceneProgram.release();

           if ((pointLight = dynamic_cast<PointLight*>(&node.item()))) {
               if (showLights()) {
                   renderPointLight(scene, transforms.top(), node.hasFocus());
               }
           } else if ((camera = dynamic_cast<Camera*>(&node.item()))) {
               if (showCameras() && &node != &scene.cameraNode()) {
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

void Sahara::OpenGLRenderer::renderGrid(OpenGLScene& scene)
{
    _gridProgram.bind();

    _gridProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _gridProgram.setProjection(scene.camera().projection());

    _gridProgram.setGrid(_grid);
    for (int i = 0; i < 2 * _grid.length(); i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    _gridProgram.clearGrid(_grid);

    if (showAxes()) {
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

void Sahara::OpenGLRenderer::renderPointLight(OpenGLScene& scene, const QMatrix4x4& modelView, const bool focus)
{
    _displayProgram.bind();

    _displayProgram.setModelView(modelView);
    _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _displayProgram.setProjection(scene.camera().projection());
    _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _displayProgram.setFocus(focus);

    _displayProgram.setDisplay(_pointLightDisplay);

    OpenGLVertexBuffer buffer = _pointLightDisplay.vertexBuffers().first();
    int vertices = buffer.count();

    for (int i = 0; i < vertices / 3; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 3, 3);
    }

    _displayProgram.clearDisplay(_pointLightDisplay);

    _displayProgram.release();
}

void Sahara::OpenGLRenderer::renderCamera(Sahara::OpenGLScene& scene, const QMatrix4x4& modelView, const bool focus)
{
    _displayProgram.bind();

    _displayProgram.setModelView(modelView);
    _displayProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _displayProgram.setProjection(scene.camera().projection());
    _displayProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _displayProgram.setFocus(focus);

    _displayProgram.setDisplay(_cameraDisplay);

    OpenGLVertexBuffer buffer = _cameraDisplay.vertexBuffers().first();
    int vertices = buffer.count();

    for (int i = 0; i < vertices / 3; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 3, 3);
    }

    _displayProgram.clearDisplay(_cameraDisplay);

    _displayProgram.release();
}

void Sahara::OpenGLRenderer::renderModel(Sahara::OpenGLModel& model, QStack<QMatrix4x4>& transformStack, const bool focus, const float time)
{
    model.animate(time);

    for (Instance* instance : model.instances()) {
        transformStack.push(transformStack.top() * instance->transform());
        _sceneProgram.setModelView(transformStack.top());

        InstanceMesh* meshInstance;
        OpenGLInstanceController* controllerInstance;
        if ((meshInstance = dynamic_cast<InstanceMesh*>(instance))) {
            _sceneProgram.setArticulated(false);

            for (int i = 0; i < meshInstance->mesh().count(); i++) {
                renderSurface(dynamic_cast<OpenGLSurface&>(meshInstance->mesh().surface(i)), *meshInstance, focus);
            }
        } else if ((controllerInstance = dynamic_cast<OpenGLInstanceController*>(instance))) {
            _sceneProgram.setArticulated(true);

            processControllerInstanceArmature(*controllerInstance);

            for (int i = 0; i < controllerInstance->controller().mesh().count(); i++) {
                renderSurface(dynamic_cast<OpenGLSurface&>(controllerInstance->controller().mesh().surface(i)), *controllerInstance, focus);
            }
        }

        transformStack.pop();
    }
}

void Sahara::OpenGLRenderer::renderSurface(Sahara::OpenGLSurface& surface, Instance& instance, const bool focus)
{
    const OpenGLMaterial& material = dynamic_cast<const OpenGLMaterial&>(instance.getMaterial(surface.material()));
    _sceneProgram.setMaterial(material);
    if (material.image().has_value()) {
        (*material.image())->bind();
    }

    _sceneProgram.setSurface(surface);
    if (focus) {
        OpenGLVertexBuffer buffer = surface.vertexBuffers().first();
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

void Sahara::OpenGLRenderer::processSceneLighting(Sahara::OpenGLScene& scene)
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

void Sahara::OpenGLRenderer::processControllerInstanceArmature(Sahara::OpenGLInstanceController& controllerInstance)
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

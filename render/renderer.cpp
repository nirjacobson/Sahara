#include "renderer.h"

Sahara::Renderer::Renderer()
    : _grid(32)
    , _renderGrid(false)
{
    glClearColor(0.5f, 0.75f, 0.86f, 1.0f);
    glClearDepthf(1.0f);
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

void Sahara::Renderer::renderGrid(const bool visible)
{
    _renderGrid = visible;
}

void Sahara::Renderer::renderScene(Scene& scene, const float time)
{
    if (_renderGrid)
        renderGrid(scene);

    _sceneProgram.bind();

    _sceneProgram.setInverseCamera(scene.cameraNode().globalTransform().inverted());
    _sceneProgram.setCameraPosition(scene.cameraNode().globalPosition());
    _sceneProgram.setProjection(scene.camera().projection());

    processSceneLighting(scene);

    QStack<QMatrix4x4> transforms;
    transforms.push(QMatrix4x4());
    scene.root().depthFirst([&](Node& node, auto&) {
       transforms.push(transforms.top() * node.transform());
       Model* model;
       if ((model = dynamic_cast<Model*>(&node.item()))) {
           _sceneProgram.setModelView(transforms.top());
           _sceneProgram.setFocus(false);
           renderModel(*model, time);
           if (node.hasFocus()) {
               _sceneProgram.setFocus(true);
               renderModel(*model, time);
           }
       }
    }, [&](Node&, auto&) {
        transforms.pop();
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

    _gridProgram.release();
}

void Sahara::Renderer::renderModel(Sahara::Model& model, const float time)
{
    model.animate(time);

    for (Instance* instance : model.instances()) {
        MeshInstance* meshInstance;
        ControllerInstance* controllerInstance;
        if ((meshInstance = dynamic_cast<MeshInstance*>(instance))) {
            _sceneProgram.setBoned(false);

            for (Surface& surface : meshInstance->mesh().surfaces()) {
                renderSurface(surface, *meshInstance);
            }
        } else if ((controllerInstance = dynamic_cast<ControllerInstance*>(instance))) {
            _sceneProgram.setBoned(true);

            processControllerInstanceArmature(*controllerInstance);

            for (Surface& surface : controllerInstance->controller().mesh().surfaces()) {
                renderSurface(surface, *controllerInstance);
            }
        }
    }
}

void Sahara::Renderer::renderSurface(Sahara::Surface& surface, Instance& instance)
{
    const Material& material = instance.getMaterial(surface.material());
    _sceneProgram.setMaterial(material);
    if (material.image().has_value()) {
        (*material.image())->bind();
    }

   _sceneProgram.setSurface(surface);
   glDrawArrays(GL_TRIANGLES, 0, surface.vertexBuffers().first().count());
   _sceneProgram.clearSurface(surface);

   assert(glGetError() == GL_NO_ERROR);
}

void Sahara::Renderer::processSceneLighting(Sahara::Scene& scene)
{
    _sceneProgram.clearPointLights();
    scene.root().depthFirst([&](const Node& node, auto&) {
        const PointLight* pointLight;
        if ((pointLight = dynamic_cast<const PointLight*>(&node.item()))) {
            _sceneProgram.addPointLight(*pointLight, node.globalPosition());
        }
    });
}

void Sahara::Renderer::processControllerInstanceArmature(Sahara::ControllerInstance& controllerInstance)
{
    const Controller& controller = controllerInstance.controller();

    QList<Transform> boneTransforms;
    for (int i = 0; i < controller.bones().size(); i++) {
        QString boneName = controller.bones().at(i);
        const Bone* bone = controllerInstance.armature().getBoneByName(boneName);
        QMatrix4x4 inverseBindMatrix = controller.inverseBindMatrices().at(i);
        QMatrix4x4 matrix = inverseBindMatrix * controller.bindShapeMatrix();

        Transform transform = Transform(matrix);

        while (bone) {
            transform = bone->transform() * transform;
            bone = &bone->parent();
        }

        boneTransforms.push_back(transform);
    }

    _sceneProgram.setBoneTransforms(boneTransforms);
}

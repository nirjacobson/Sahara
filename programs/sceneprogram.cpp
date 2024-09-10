#include "sceneprogram.h"

Sahara::SceneProgram::SceneProgram()
    : _pointLights(0)
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!_vertexShader->compileSourceFile(":/programs/glsl/300/scene.vert.glsl")) {
        _vertexShader->compileSourceFile(":/programs/glsl/120/scene.vert.glsl");
    }

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!_fragmentShader->compileSourceFile(":/programs/glsl/300/scene.frag.glsl")) {
        _fragmentShader->compileSourceFile(":/programs/glsl/120/scene.frag.glsl");
    }

    program().addShader(_vertexShader);
    program().addShader(_fragmentShader);
    program().link();

    _render.modelView = program().uniformLocation("uRender.modelView");
    _render.inverseCamera = program().uniformLocation("uRender.inverseCamera");
    _render.projection = program().uniformLocation("uRender.projection");

    for (int i = 0; i < MAX_JOINTS; i++) {
        _armature.joints[i].rotation = program().uniformLocation("uArmature.joints["+QString::number(i)+"].rotation");
        _armature.joints[i].translation = program().uniformLocation("uArmature.joints["+QString::number(i)+"].translation");
    }
    _armature.present = program().uniformLocation("uArmature.present");

    _lighting.ambientLight.color = program().uniformLocation("uLighting.ambientLight.color");
    _lighting.ambientLight.strength = program().uniformLocation("uLighting.ambientLight.strength");
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        _lighting.pointLights[i].position = program().uniformLocation("uLighting.pointLights["+QString::number(i)+"].position");
        _lighting.pointLights[i].color = program().uniformLocation("uLighting.pointLights["+QString::number(i)+"].color");
        _lighting.pointLights[i].constantAttenuation = program().uniformLocation("uLighting.pointLights["+QString::number(i)+"].constantAttenuation");
        _lighting.pointLights[i].linearAttenuation = program().uniformLocation("uLighting.pointLights["+QString::number(i)+"].linearAttenuation");
        _lighting.pointLights[i].quadraticAttenuation = program().uniformLocation("uLighting.pointLights["+QString::number(i)+"].quadraticAttenuation");
    }
    _lighting.pointLightCount = program().uniformLocation("uLighting.pointLightCount");

    _material.emission = program().uniformLocation("uMaterial.emission");
    _material.ambient = program().uniformLocation("uMaterial.ambient");
    _material.diffuse = program().uniformLocation("uMaterial.diffuse");
    _material.specular = program().uniformLocation("uMaterial.specular");
    _material.shininess = program().uniformLocation("uMaterial.shininess");
    _material.textured = program().uniformLocation("uMaterial.textured");

    _focus = program().uniformLocation("uFocus");
    _cameraPosition = program().uniformLocation("uCameraPosition");

    _sampler = program().uniformLocation("uSampler");

    QOpenGLFunctions(QOpenGLContext::currentContext()).glActiveTexture(GL_TEXTURE0);
    program().bind();
    program().setUniformValue(_sampler, static_cast<GLint>(0));
    program().setUniformValue(_lighting.pointLightCount, static_cast<GLint>(0));
    program().release();
}

void Sahara::SceneProgram::setModelView(const QMatrix4x4& modelView)
{
    program().setUniformValue(_render.modelView, modelView);

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    program().setUniformValue(_render.inverseCamera, inverseCamera);

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setProjection(const QMatrix4x4& projection)
{
    program().setUniformValue(_render.projection, projection);

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setArticulated(const bool articulated)
{
    program().setUniformValue(_armature.present, static_cast<GLint>(articulated));

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setFocus(const bool focus)
{
    program().setUniformValue(_focus, static_cast<GLint>(focus));

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setJointTransforms(const QList<Sahara::Transform>& transforms)
{
    for (int i = 0; i < transforms.size(); i++) {
        program().setUniformValue(_armature.joints[i].rotation, transforms[i].rotation().toVector4D());
        program().setUniformValue(_armature.joints[i].translation, transforms[i].translation());
    }

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setAmbientLight(const AmbientLight& ambientLight)
{
    QVector3D color(ambientLight.color().red(), ambientLight.color().green(), ambientLight.color().blue());
    color /= 255.0f;
    program().setUniformValue(_lighting.ambientLight.color, color);
    program().setUniformValue(_lighting.ambientLight.strength, ambientLight.strength());

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::addPointLight(const Sahara::PointLight& pointLight, const QVector3D& position)
{
    program().setUniformValue(_lighting.pointLights[_pointLights].position, position);

    QVector3D color(pointLight.color().red(), pointLight.color().green(), pointLight.color().blue());
    color /= 255.0f;
    program().setUniformValue(_lighting.pointLights[_pointLights].color, color);

    program().setUniformValue(_lighting.pointLights[_pointLights].constantAttenuation, pointLight.constantAttenuation());
    program().setUniformValue(_lighting.pointLights[_pointLights].linearAttenuation, pointLight.linearAttenuation());
    program().setUniformValue(_lighting.pointLights[_pointLights].quadraticAttenuation, pointLight.quadraticAttenuation());
    program().setUniformValue(_lighting.pointLightCount, ++_pointLights);

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);

}

void Sahara::SceneProgram::clearPointLights()
{
    _pointLights = 0;
    program().setUniformValue(_lighting.pointLightCount, _pointLights);
}

void Sahara::SceneProgram::setMaterial(const Sahara::OpenGLMaterial& material)
{
    program().setUniformValue(_material.emission, material.emission());
    program().setUniformValue(_material.ambient, material.ambient());
    program().setUniformValue(_material.diffuse, material.diffuse());
    program().setUniformValue(_material.specular, material.specular());
    program().setUniformValue(_material.shininess, material.shininess());
    program().setUniformValue(_material.textured, static_cast<GLint>(material.image().has_value()));

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setCameraPosition(const QVector3D& position)
{
    program().setUniformValue(_cameraPosition, position);

    assert(QOpenGLFunctions(QOpenGLContext::currentContext()).glGetError() == GL_NO_ERROR);
}

Sahara::SceneProgram::~SceneProgram()
{
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::SceneProgram::bind()
{
    program().bind();
}

void Sahara::SceneProgram::release()
{
    program().release();
}

void Sahara::SceneProgram::setSurface(Sahara::OpenGLSurface& surface)
{
    layout(surface);
}

void Sahara::SceneProgram::clearSurface(Sahara::OpenGLSurface& surface)
{
    unlayout(surface);
}

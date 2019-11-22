#include "sceneprogram.h"

Sahara::SceneProgram::SceneProgram()
    : _pointLights(0)
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    _vertexShader->compileSourceFile(":/programs/glsl/scene.vsh");

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    _fragmentShader->compileSourceFile(":/programs/glsl/scene.fsh");

    _program = new QOpenGLShaderProgram;
    _program->addShader(_vertexShader);
    _program->addShader(_fragmentShader);
    _program->link();

    _position = _program->attributeLocation("position");
    _normal = _program->attributeLocation("normal");
    _texcoord = _program->attributeLocation("texcoord");
    _bones1 = _program->attributeLocation("bones1");
    _bones2 = _program->attributeLocation("bones2");
    _weights1 = _program->attributeLocation("weights1");
    _weights2 = _program->attributeLocation("weights2");

    _render.modelView = _program->uniformLocation("uRender.modelView");
    _render.inverseCamera = _program->uniformLocation("uRender.inverseCamera");
    _render.projection = _program->uniformLocation("uRender.projection");
    _render.boned = _program->uniformLocation("uRender.boned");
    _render.focus = _program->uniformLocation("uRender.focus");

    for (int i = 0; i < MAX_BONES; i++) {
        _armature.bones[i].rotation = _program->uniformLocation("uArmature.bones["+QString::number(i)+"].rotation");
        _armature.bones[i].translation = _program->uniformLocation("uArmature.bones["+QString::number(i)+"].translation");
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        _lighting.pointLights[i].position = _program->uniformLocation("uLighting.pointLights["+QString::number(i)+"].position");
        _lighting.pointLights[i].color = _program->uniformLocation("uLighting.pointLights["+QString::number(i)+"].color");
        _lighting.pointLights[i].constantAttenuation = _program->uniformLocation("uLighting.pointLights["+QString::number(i)+"].constantAttenuation");
        _lighting.pointLights[i].linearAttenuation = _program->uniformLocation("uLighting.pointLights["+QString::number(i)+"].linearAttenuation");
        _lighting.pointLights[i].quadraticAttenuation = _program->uniformLocation("uLighting.pointLights["+QString::number(i)+"].quadraticAttenuation");
    }
    _lighting.pointLightCount = _program->uniformLocation("uLighting.pointLightCount");

    _material.emission = _program->uniformLocation("uMaterial.emission");
    _material.ambient = _program->uniformLocation("uMaterial.ambient");
    _material.diffuse = _program->uniformLocation("uMaterial.diffuse");
    _material.specular = _program->uniformLocation("uMaterial.specular");
    _material.shininess = _program->uniformLocation("uMaterial.shininess");
    _material.textured = _program->uniformLocation("uMaterial.textured");

    _sampler = _program->uniformLocation("uSampler");

    glActiveTexture(GL_TEXTURE0);
    _program->setUniformValue(_sampler, static_cast<GLint>(0));
}

void Sahara::SceneProgram::setPosition(const int stride, const void* offset)
{
    glVertexAttribPointer(_position, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setNormal(const int stride, const void* offset)
{
    glVertexAttribPointer(_normal, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setTexcoord(const int stride, const void* offset)
{
    glVertexAttribPointer(_texcoord, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setBones1(const int stride, const void* offset)
{
    glVertexAttribPointer(_bones1, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setBones2(const int stride, const void* offset)
{
    glVertexAttribPointer(_bones2, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setWeights1(const int stride, const void* offset)
{
    glVertexAttribPointer(_weights1, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setWeights2(const int stride, const void* offset)
{
    glVertexAttribPointer(_weights2, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::SceneProgram::setModelView(const QMatrix4x4& modelView)
{
    _program->setUniformValue(_render.modelView, modelView);
}

void Sahara::SceneProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    _program->setUniformValue(_render.inverseCamera, inverseCamera);
}

void Sahara::SceneProgram::setProjection(const QMatrix4x4& projection)
{
    _program->setUniformValue(_render.inverseCamera, projection);
}

void Sahara::SceneProgram::setBoned(const bool boned)
{
    _program->setUniformValue(_render.boned, static_cast<GLint>(boned));
}

void Sahara::SceneProgram::setFocus(const bool focus)
{
    _program->setUniformValue(_render.focus, static_cast<GLint>(focus));
}

void Sahara::SceneProgram::setBone(const int index, const Bone& bone)
{

}

void Sahara::SceneProgram::addPointLight(const Sahara::PointLight& pointLight, const QVector3D& position)
{
    _program->setUniformValue(_lighting.pointLights[_pointLights].position, position);
    _program->setUniformValue(_lighting.pointLights[_pointLights].color, pointLight.color());
    _program->setUniformValue(_lighting.pointLights[_pointLights].constantAttenuation, pointLight.constantAttenuation());
    _program->setUniformValue(_lighting.pointLights[_pointLights].linearAttenuation, pointLight.linearAttenuation());
    _program->setUniformValue(_lighting.pointLights[_pointLights].quadraticAttenuation, pointLight.quadraticAttenuation());
    _program->setUniformValue(_lighting.pointLightCount, ++_pointLights);

}

void Sahara::SceneProgram::clearPointLights()
{
    _pointLights = 0;
}

void Sahara::SceneProgram::setMaterial(const Sahara::Material& material)
{
    _program->setUniformValue(_material.emission, material.emission());
    _program->setUniformValue(_material.ambient, material.ambient());
    _program->setUniformValue(_material.diffuse, material.diffuse());
    _program->setUniformValue(_material.specular, material.specular());
    _program->setUniformValue(_material.shininess, material.shininess());
    _program->setUniformValue(_material.textured, static_cast<GLint>(material.image().has_value()));
}

Sahara::SceneProgram::~SceneProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::SceneProgram::bind()
{
    _program->bind();
}

void Sahara::SceneProgram::release()
{
    _program->release();
}

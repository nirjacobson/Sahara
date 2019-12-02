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

    _render.modelView = _program->uniformLocation("uRender.modelView");
    _render.inverseCamera = _program->uniformLocation("uRender.inverseCamera");
    _render.projection = _program->uniformLocation("uRender.projection");
    _render.boned = _program->uniformLocation("uRender.boned");

    _boneRotations = _program->uniformLocation("uBoneRotations");
    _boneTranslations = _program->uniformLocation("uBoneTranslations");

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

    _focus = _program->uniformLocation("uFocus");
    _cameraPosition = _program->uniformLocation("uCameraPosition");

    _sampler = _program->uniformLocation("uSampler");

    glActiveTexture(GL_TEXTURE0);
    _program->bind();
    _program->setUniformValue(_sampler, static_cast<GLint>(0));
    _program->setUniformValue(_lighting.pointLightCount, static_cast<GLint>(0));
    _program->release();
}

void Sahara::SceneProgram::setModelView(const QMatrix4x4& modelView)
{
    _program->setUniformValue(_render.modelView, modelView);

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    _program->setUniformValue(_render.inverseCamera, inverseCamera);

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setProjection(const QMatrix4x4& projection)
{
    _program->setUniformValue(_render.projection, projection);

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setBoned(const bool boned)
{
    _program->setUniformValue(_render.boned, static_cast<GLint>(boned));

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setFocus(const bool focus)
{
    _program->setUniformValue(_focus, static_cast<GLint>(focus));

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setBoneTransforms(const QList<Sahara::Transform>& transforms)
{
    QVector<QVector4D> rotations;
    QVector<QVector3D> translations;
    std::for_each(transforms.begin(), transforms.end(), [&](const Transform& transform) {
       rotations.append(transform.rotation().toVector4D());
       translations.append(transform.translation());
    });

    _program->setUniformValueArray(_boneRotations, rotations.data(), rotations.size());
    _program->setUniformValueArray(_boneTranslations, translations.data(), translations.size());

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::addPointLight(const Sahara::PointLight& pointLight, const QVector3D& position)
{
    _program->setUniformValue(_lighting.pointLights[_pointLights].position, position);

    QVector3D color(pointLight.color().red(), pointLight.color().green(), pointLight.color().blue());
    color /= 255.0f;
    _program->setUniformValue(_lighting.pointLights[_pointLights].color, color);

    _program->setUniformValue(_lighting.pointLights[_pointLights].constantAttenuation, pointLight.constantAttenuation());
    _program->setUniformValue(_lighting.pointLights[_pointLights].linearAttenuation, pointLight.linearAttenuation());
    _program->setUniformValue(_lighting.pointLights[_pointLights].quadraticAttenuation, pointLight.quadraticAttenuation());
    _program->setUniformValue(_lighting.pointLightCount, ++_pointLights);

    assert(glGetError() == GL_NO_ERROR);

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

    assert(glGetError() == GL_NO_ERROR);
}

void Sahara::SceneProgram::setCameraPosition(const QVector3D& position)
{
    _program->setUniformValue(_cameraPosition, position);

    assert(glGetError() == GL_NO_ERROR);
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

void Sahara::SceneProgram::setSurface(Sahara::Surface& surface)
{
    for (VertexBufferDict::iterator i = surface.vertexBuffers().begin(); i != surface.vertexBuffers().end(); i++) {
        GLint location = _program->attributeLocation(i.key());
        if (location >= 0) {
            i.value().bind();
            _program->enableAttributeArray(location);
            glVertexAttribPointer(static_cast<GLuint>(location), i.value().stride(), i.value().type(), GL_FALSE, 0, reinterpret_cast<void*>(0));
            i.value().release();
        }

        assert(glGetError() == GL_NO_ERROR);
    }
}

void Sahara::SceneProgram::clearSurface(Sahara::Surface& surface)
{
    for (VertexBufferDict::iterator i = surface.vertexBuffers().begin(); i != surface.vertexBuffers().end(); i++) {
        GLint location = _program->attributeLocation(i.key());
        _program->disableAttributeArray(location);
    }
}

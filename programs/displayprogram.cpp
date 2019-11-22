#include "displayprogram.h"

Sahara::DisplayProgram::DisplayProgram()
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    _vertexShader->compileSourceFile(":/programs/glsl/display.vsh");

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    _fragmentShader->compileSourceFile(":/programs/glsl/display.fsh");

    _program = new QOpenGLShaderProgram;
    _program->addShader(_vertexShader);
    _program->addShader(_fragmentShader);
    _program->link();

    _position = _program->attributeLocation("position");
    _normal = _program->attributeLocation("normal");
    _vertexRender.modelView = _program->uniformLocation("uVertexRender.modelView");
    _vertexRender.inverseCamera = _program->uniformLocation("uVertexRender.inverseCamera");
    _vertexRender.projection = _program->uniformLocation("uVertexRender.projection");
    _fragmentRender.cameraPosition = _program->uniformLocation("uFragmentRender.cameraPosition");
    _fragmentRender.focus = _program->uniformLocation("uFragmentRender.focus");
}

Sahara::DisplayProgram::~DisplayProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::DisplayProgram::bind()
{
    _program->bind();
}

void Sahara::DisplayProgram::release()
{
    _program->release();
}

void Sahara::DisplayProgram::setPosition(const int stride, const void* offset)
{
    glVertexAttribPointer(_position, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::DisplayProgram::setNormal(const int stride, const void* offset)
{
    glVertexAttribPointer(_normal, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Sahara::DisplayProgram::setModelView(const QMatrix4x4& modelView)
{
    _program->setUniformValue(_vertexRender.modelView, modelView);
}

void Sahara::DisplayProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    _program->setUniformValue(_vertexRender.inverseCamera, inverseCamera);
}

void Sahara::DisplayProgram::setProjection(const QMatrix4x4& projection)
{
    _program->setUniformValue(_vertexRender.projection, projection);
}

void Sahara::DisplayProgram::setCameraPosition(const QVector3D& cameraPosition)
{
    _program->setUniformValue(_fragmentRender.cameraPosition, cameraPosition);
}

void Sahara::DisplayProgram::setFocus(const bool focus)
{
    _program->setUniformValue(_fragmentRender.focus, static_cast<GLint>(focus));
}


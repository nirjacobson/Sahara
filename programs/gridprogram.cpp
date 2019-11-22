#include "gridprogram.h"

Sahara::GridProgram::GridProgram()
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    _vertexShader->compileSourceFile(":/programs/glsl/grid.vsh");

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    _fragmentShader->compileSourceFile(":/programs/glsl/grid.fsh");

    _program = new QOpenGLShaderProgram;
    _program->addShader(_vertexShader);
    _program->addShader(_fragmentShader);
    _program->link();

    _position = _program->attributeLocation("position");
    _render.inverseCamera = _program->uniformLocation("uRender.inverseCamera");
    _render.projection = _program->uniformLocation("uRender.projection");
}

Sahara::GridProgram::~GridProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::GridProgram::bind()
{
    _program->bind();
}

void Sahara::GridProgram::release()
{
    _program->release();
}

void Sahara::GridProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    _program->setUniformValue(_render.inverseCamera, inverseCamera);
}

void Sahara::GridProgram::setProjection(const QMatrix4x4& projection)
{
    _program->setUniformValue(_render.projection, projection);
}

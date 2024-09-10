#include "gridprogram.h"

Sahara::GridProgram::GridProgram()
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!_vertexShader->compileSourceFile(":/programs/glsl/300/grid.vert.glsl")) {
        _vertexShader->compileSourceFile(":/programs/glsl/120/grid.vert.glsl");
    }

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!_fragmentShader->compileSourceFile(":/programs/glsl/300/grid.frag.glsl")) {
        _fragmentShader->compileSourceFile(":/programs/glsl/120/grid.frag.glsl");
    }

    program().addShader(_vertexShader);
    program().addShader(_fragmentShader);
    program().link();

    _position = program().attributeLocation("position");
    _render.inverseCamera = program().uniformLocation("uRender.inverseCamera");
    _render.projection = program().uniformLocation("uRender.projection");
}

Sahara::GridProgram::~GridProgram()
{
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::GridProgram::bind()
{
    program().bind();
}

void Sahara::GridProgram::release()
{
    program().release();
}

void Sahara::GridProgram::setGrid(Sahara::OpenGLGrid &grid)
{
    layout(grid);
}

void Sahara::GridProgram::setAxis(Sahara::OpenGLAxis& axis)
{
    layout(axis);
}

void Sahara::GridProgram::clearAxis(Sahara::OpenGLAxis& axis)
{
    unlayout(axis);
}

void Sahara::GridProgram::clearGrid(Sahara::OpenGLGrid& grid)
{
    unlayout(grid);
}

void Sahara::GridProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    program().setUniformValue(_render.inverseCamera, inverseCamera);
}

void Sahara::GridProgram::setProjection(const QMatrix4x4& projection)
{
    program().setUniformValue(_render.projection, projection);
}

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
}

Sahara::GridProgram::~GridProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

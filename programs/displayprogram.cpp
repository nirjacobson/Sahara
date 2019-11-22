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
}

Sahara::DisplayProgram::~DisplayProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

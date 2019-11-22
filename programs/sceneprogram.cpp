#include "sceneprogram.h"

Sahara::SceneProgram::SceneProgram()
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    _vertexShader->compileSourceFile(":/programs/glsl/scene.vsh");

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    _fragmentShader->compileSourceFile(":/programs/glsl/scene.fsh");

    _program = new QOpenGLShaderProgram;
    _program->addShader(_vertexShader);
    _program->addShader(_fragmentShader);
    _program->link();
}

Sahara::SceneProgram::~SceneProgram()
{
    delete _program;
    delete _fragmentShader;
    delete _vertexShader;
}

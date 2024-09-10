#include "displayprogram.h"

Sahara::DisplayProgram::DisplayProgram()
{
    _vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!_vertexShader->compileSourceFile(":/programs/glsl/300/display.vert.glsl")) {
        _vertexShader->compileSourceFile(":/programs/glsl/120/display.vert.glsl");
    }

    _fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!_fragmentShader->compileSourceFile(":/programs/glsl/300/display.frag.glsl")) {
        _fragmentShader->compileSourceFile(":/programs/glsl/120/display.frag.glsl");
    }

    program().addShader(_vertexShader);
    program().addShader(_fragmentShader);
    program().link();

    _position = program().attributeLocation("position");
    _normal = program().attributeLocation("normal");
    _vertexRender.modelView = program().uniformLocation("uVertexRender.modelView");
    _vertexRender.inverseCamera = program().uniformLocation("uVertexRender.inverseCamera");
    _vertexRender.projection = program().uniformLocation("uVertexRender.projection");
    _fragmentRender.cameraPosition = program().uniformLocation("uFragmentRender.cameraPosition");
    _fragmentRender.focus = program().uniformLocation("uFragmentRender.focus");
}

Sahara::DisplayProgram::~DisplayProgram()
{
    delete _fragmentShader;
    delete _vertexShader;
}

void Sahara::DisplayProgram::bind()
{
    program().bind();
}

void Sahara::DisplayProgram::release()
{
    program().release();
}

void Sahara::DisplayProgram::setDisplay(Sahara::OpenGLDisplay& display)
{
    layout(display);
}

void Sahara::DisplayProgram::clearDisplay(Sahara::OpenGLDisplay& display)
{
    unlayout(display);
}

void Sahara::DisplayProgram::setModelView(const QMatrix4x4& modelView)
{
    program().setUniformValue(_vertexRender.modelView, modelView);
}

void Sahara::DisplayProgram::setInverseCamera(const QMatrix4x4& inverseCamera)
{
    program().setUniformValue(_vertexRender.inverseCamera, inverseCamera);
}

void Sahara::DisplayProgram::setProjection(const QMatrix4x4& projection)
{
    program().setUniformValue(_vertexRender.projection, projection);
}

void Sahara::DisplayProgram::setCameraPosition(const QVector3D& cameraPosition)
{
    program().setUniformValue(_fragmentRender.cameraPosition, cameraPosition);
}

void Sahara::DisplayProgram::setFocus(const bool focus)
{
    program().setUniformValue(_fragmentRender.focus, static_cast<GLint>(focus));
}


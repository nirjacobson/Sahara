#include "program.h"


Sahara::Program::Program()
{

}

Sahara::Program::~Program()
{

}

GLfloat* Sahara::Program::qVector3DToArray(const QVector3D& vector) const
{
    GLfloat* data = new GLfloat[3];

    data[0] = vector.x();
    data[1] = vector.y();
    data[2] = vector.z();

    return data;
}

GLfloat* Sahara::Program::qVector4DToArray(const QVector4D& vector) const
{
    GLfloat* data = new GLfloat[4];

    data[0] = vector.x();
    data[1] = vector.y();
    data[2] = vector.z();
    data[3] = vector.w();

    return data;
}

GLfloat* Sahara::Program::qMatrix4x4ToArray(const QMatrix4x4& matrix) const
{
    GLfloat* data = new GLfloat[16];

    for (int i = 0; i < 4; i++) {
        data[i * 4 + 0] = matrix.column(i).x();
        data[i * 4 + 1] = matrix.column(i).y();
        data[i * 4 + 2] = matrix.column(i).z();
        data[i * 4 + 3] = matrix.column(i).w();
    }

    return data;
}

QOpenGLShaderProgram& Sahara::Program::program()
{
    return _program;
}

void Sahara::Program::layout(Sahara::WithVertexBuffers &wvb)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    for (VertexBufferDict::iterator i = wvb.vertexBuffers().begin(); i != wvb.vertexBuffers().end(); i++) {
        GLint location = _program.attributeLocation(i.key());
        if (location >= 0) {
            i.value().bind();
            _program.enableAttributeArray(location);
            glFuncs.glVertexAttribPointer(static_cast<GLuint>(location), i.value().stride(), GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
            i.value().release();
        }

        GLenum error = glGetError();
        assert(error == GL_NO_ERROR || error == GL_INVALID_OPERATION);
    }
}

void Sahara::Program::unlayout(Sahara::WithVertexBuffers& wvb)
{
    for (VertexBufferDict::iterator i = wvb.vertexBuffers().begin(); i != wvb.vertexBuffers().end(); i++) {
        GLint location = _program.attributeLocation(i.key());
        if (location >= 0) {
            _program.disableAttributeArray(location);
        }

        assert(glGetError() == GL_NO_ERROR);
    }
}

#include "openglgrid.h"

Sahara::OpenGLGrid::OpenGLGrid(const int length)
    : _length(length)
    , _xAxis('x', length)
    , _yAxis('y', length)
    , _zAxis('z', length)
{
    initPositionBuffer();
    initColorBuffer();
}

int Sahara::OpenGLGrid::length() const
{
    return _length;
}

Sahara::OpenGLAxis&Sahara::OpenGLGrid::xAxis()
{
    return _xAxis;
}

Sahara::OpenGLAxis&Sahara::OpenGLGrid::yAxis()
{
    return _yAxis;
}

Sahara::OpenGLAxis&Sahara::OpenGLGrid::zAxis()
{
    return _zAxis;
}

void Sahara::OpenGLGrid::initPositionBuffer()
{
    int numFloats = 2 * _length * 4 * 3;
    GLfloat* data = new GLfloat[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    QVector3D topLeft, bottomLeft, topRight, bottomRight;
    QList<QVector3D> vertices;

    for (int i = 0; i < _length; i++) {
        topLeft = {
            -(_length / 2.0f) + i,
            0,
            -(_length / 2.0f),
        };
        bottomLeft = {
            -(_length / 2.0f) + i,
            0,
            (_length / 2.0f),
        };
        topRight = {
            -(_length / 2.0f) + i + 1,
            0,
            -(_length / 2.0f),
        };
        bottomRight = {
            -(_length / 2.0f) + i + 1,
            0,
            (_length / 2.0f),
        };

        vertices = {
            topLeft, bottomLeft, bottomRight, topRight
        };

        for (int j = 0; j < vertices.size(); j++) {
            for (int k = 0; k < 3; k++) {
                data[dataIndex++] = vertices[j][k];
            }
        }
    }

    for (int i = 0; i < _length; i++) {
        topLeft = {
            -(_length / 2.0f),
            0,
            -(_length / 2.0f) + i,
        };
        bottomLeft = {
            -(_length / 2.0f),
            0,
            -(_length / 2.0f) + i + 1,
        };
        topRight = {
            (_length / 2.0f),
            0,
            -(_length / 2.0f) + i,
        };
        bottomRight = {
            (_length / 2.0f),
            0,
            -(_length / 2.0f) + i + 1,
        };

        vertices = {
            topLeft, bottomLeft, bottomRight, topRight
        };

        for (int j = 0; j < vertices.size(); j++) {
            for (int k = 0; k < 3; k++) {
                data[dataIndex++] = vertices[j][k];
            }
        }
    }

    OpenGLVertexBuffer positionBuffer;
    positionBuffer.setStride(3);
    positionBuffer.write(data, numFloats);

    delete [] data;

    addVertexBuffer("position", positionBuffer);
}

void Sahara::OpenGLGrid::initColorBuffer()
{
    int vertices = 2 * _length * 4;
    int numFloats = vertices * 3;
    GLfloat* data = new GLfloat[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < 3; j++) {
            data[dataIndex++] = 0.5;
        }
    }

    OpenGLVertexBuffer colorBuffer;
    colorBuffer.setStride(3);
    colorBuffer.write(data, numFloats);

    delete [] data;

    addVertexBuffer("color", colorBuffer);
}

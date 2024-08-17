#include "openglaxis.h"

Sahara::OpenGLAxis::OpenGLAxis(const char axis, const int length)
    : _axis(axis)
    , _length(length)
{
    initPositionBuffer();
    initColorBuffer();
}

void Sahara::OpenGLAxis::initPositionBuffer()
{
    int numFloats = 6 * 4 * 3;
    GLfloat* data = new GLfloat[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    QVector3D topLeft, bottomLeft, topRight, bottomRight;
    QList<QVector3D> vertices;

    QMatrix4x4 faceRotation;
    QMatrix4x4 axisRotation;
    if (_axis == 'y') {
        axisRotation.rotate(90, {0, 0, 1});
    } else if (_axis == 'z') {
        axisRotation.rotate(90, {0, 1, 0});
    }

    // Back face
    topLeft = {
        -_length/2.0f, 0.02f, -0.02f
    };
    bottomLeft = {
        -_length/2.0f, -0.02f, -0.02f
    };
    topRight = {
        -_length/2.0f, 0.02f, 0.02f
    };
    bottomRight = {
        -_length/2.0f, -0.02f, 0.02f
    };

    vertices = {
        topLeft, bottomLeft, topRight, bottomRight
    };

    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = axisRotation.map(vertices[i]);
        for (int j = 0; j < 3; j++) {
            data[dataIndex++] = vertices[i][j];
        }
    }

    // Front face
    faceRotation = QMatrix4x4();
    faceRotation.rotate(180, {0, 1, 0});
    topLeft = faceRotation.map(topLeft);
    bottomLeft = faceRotation.map(bottomLeft);
    topRight = faceRotation.map(topRight);
    bottomRight = faceRotation.map(bottomRight);

    vertices = {
        topLeft, bottomLeft, topRight, bottomRight
    };

    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = axisRotation.map(vertices[i]);
        for (int j = 0; j < 3; j++) {
            data[dataIndex++] = vertices[i][j];
        }
    }

    // Top face
    topLeft = {
        -_length/2.0f, 0.02f, -0.02f
    };
    bottomLeft = {
        -_length/2.0f, 0.02f, 0.02f
    };
    topRight = {
        _length/2.0f, 0.02f, -0.02f,
    };
    bottomRight = {
        _length/2.0f, 0.02f, 0.02f,
    };

    vertices = {
        topLeft, bottomLeft, topRight, bottomRight
    };

    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = axisRotation.map(vertices[i]);
        for (int j = 0; j < 3; j++) {
            data[dataIndex++] = vertices[i][j];
        }
    }

    // Remaining faces
    faceRotation = QMatrix4x4();

    for (int i = 0; i < 3; i++) {
        faceRotation.rotate(90, {1, 0, 0});
        topLeft = faceRotation.map(topLeft);
        bottomLeft = faceRotation.map(bottomLeft);
        topRight = faceRotation.map(topRight);
        bottomRight = faceRotation.map(bottomRight);

        vertices = {
            topLeft, bottomLeft, topRight, bottomRight
        };

        for (int j = 0; j < vertices.size(); j++) {
            vertices[j] = axisRotation.map(vertices[j]);
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

void Sahara::OpenGLAxis::initColorBuffer()
{
    int vertices = 6 * 4;
    int numFloats = vertices * 3;
    GLfloat* data = new GLfloat[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    QVector3D color;
    switch(_axis) {
        case 'x':
            color = {1, 0, 0};
            break;
        case 'y':
            color= {0, 1, 0};
            break;
        case 'z':
            color = {0, 0, 1};
            break;
    }

    for (int i = 0; i < vertices; i++) {
        data[dataIndex++] = color.x();
        data[dataIndex++] = color.y();
        data[dataIndex++] = color.z();
    }

    OpenGLVertexBuffer colorBuffer;
    colorBuffer.setStride(3);
    colorBuffer.write(data, numFloats);

    delete [] data;

    addVertexBuffer("color", colorBuffer);
}

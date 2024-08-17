#include "openglcameradisplay.h"


Sahara::OpenGLCameraDisplay::OpenGLCameraDisplay()
{
    initBuffers();
}

void Sahara::OpenGLCameraDisplay::initBuffers()
{
    QList<float> positions = {
      1, 1, -2, 1, -1, -2, -1, -0.9999998, -2, -0.9999997, 1, -2, -1.3411e-7, -1.78814e-7, 0, 0.8000001, 1.266667, -2, -0.7999997, 1.266667, -2, 1.78814e-7, 2.066667, -2
    };

    QList<float> normals = {
      0.8944273, 0, 0.4472137, 0, -0.8944273, 0.4472134, -0.8944274, 2.13248e-7, 0.4472135, 2.13248e-7, 0.8944272, 0.4472137, 0, 0, -1
    };

    QList<int> elements = {
      0, 0, 4, 0, 1, 0, 1, 1, 4, 1, 2, 1, 2, 2, 4, 2, 3, 2, 4, 3, 0, 3, 3, 3, 5, 4, 6, 4, 7, 4
    };

    int vertices = elements.size() / 2;
    int numFloats = vertices * 3;
    GLfloat* positionData = new GLfloat[static_cast<unsigned long>(numFloats)];
    GLfloat* normalData = new GLfloat[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    for (int i = 0; i < vertices; i++) {
      int positionIndex = elements[i * 2 + 0];
      int normalIndex = elements[i * 2 + 1];

      QList<float> position = positions.mid(positionIndex * 3, 3);
      QList<float> normal = normals.mid(normalIndex * 3, 3);

      for (int j = 0; j < 3; j++) {
        positionData[dataIndex * 3 + j] = position[j];
        normalData[dataIndex * 3 + j] = normal[j];
      }

      dataIndex++;
    }

    OpenGLVertexBuffer positionBuffer;
    positionBuffer.setStride(3);
    positionBuffer.write(positionData, numFloats);

    OpenGLVertexBuffer normalBuffer;
    normalBuffer.setStride(3);
    normalBuffer.write(normalData, numFloats);

    delete [] positionData;
    delete [] normalData;

    addVertexBuffer("position", positionBuffer);
    addVertexBuffer("normal", normalBuffer);
}

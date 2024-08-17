#include "vulkangrid.h"

Sahara::VulkanGrid::VulkanGrid(QVulkanWindow* window, const int length)
    : _vulkanWindow(window)
    , _length(length)
    , _xAxis(window, 'x', length)
    , _yAxis(window, 'y', length)
    , _zAxis(window, 'z', length)
{
    initPositionBuffer();
    initColorBuffer();
}

int Sahara::VulkanGrid::length() const
{
    return _length;
}

::Sahara::VulkanAxis& Sahara::VulkanGrid::xAxis()
{
    return _xAxis;
}

::Sahara::VulkanAxis& Sahara::VulkanGrid::yAxis()
{
    return _yAxis;
}

::Sahara::VulkanAxis& Sahara::VulkanGrid::zAxis()
{
    return _zAxis;
}

int Sahara::VulkanGrid::count() const
{
    return 2 * 5 * _length;
}

void Sahara::VulkanGrid::initPositionBuffer()
{
    int numFloats = 2 * _length * 5 * 3;
    float* data = new float[static_cast<unsigned long>(numFloats)];
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
            topLeft, bottomLeft, bottomRight, topRight, topLeft
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
            topLeft, bottomLeft, bottomRight, topRight, topLeft
        };

        for (int j = 0; j < vertices.size(); j++) {
            for (int k = 0; k < 3; k++) {
                data[dataIndex++] = vertices[j][k];
            }
        }
    }

    VulkanVertexBuffer* positionBuffer = new VulkanVertexBuffer(_vulkanWindow);
    positionBuffer->write(data, numFloats * sizeof(float), 3);

    delete [] data;

    addVertexBuffer("position", positionBuffer);
}

void Sahara::VulkanGrid::initColorBuffer()
{
    int vertices = 2 * _length * 5;
    int numFloats = vertices * 3;
    float* data = new float[static_cast<unsigned long>(numFloats)];
    int dataIndex = 0;

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < 3; j++) {
            data[dataIndex++] = 0.5;
        }
    }

    VulkanVertexBuffer* colorBuffer = new VulkanVertexBuffer(_vulkanWindow);
    colorBuffer->write(data, numFloats * sizeof(float), 3);

    delete [] data;

    addVertexBuffer("color", colorBuffer);
}

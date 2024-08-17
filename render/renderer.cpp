#include "renderer.h"

Sahara::Renderer::Renderer()
    : _showGrid(false)
    , _showAxes(true)
    , _showLights(true)
    , _showCameras(true)
{

}

bool Sahara::Renderer::showGrid() const
{
    return _showGrid;
}

void Sahara::Renderer::showGrid(const bool visible)
{
    _showGrid = visible;
}

bool Sahara::Renderer::showAxes() const
{
    return _showAxes;
}

void Sahara::Renderer::showAxes(const bool visible)
{
    _showAxes = visible;
}

bool Sahara::Renderer::showLights() const
{
    return _showLights;
}

void Sahara::Renderer::showLights(const bool visible)
{
    _showLights = visible;
}

bool Sahara::Renderer::showCameras() const
{
    return _showCameras;
}

void Sahara::Renderer::showCameras(const bool visible)
{
    _showCameras = visible;
}

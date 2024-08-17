#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>

namespace Sahara {
    class Renderer : public QObject
    {
        Q_OBJECT

    public:
        Renderer();
        virtual ~Renderer() { }

        bool showGrid() const;
        void showGrid(const bool visible);
        bool showAxes() const;
        void showAxes(const bool visible);
        bool showLights() const;
        void showLights(const bool visible);
        bool showCameras() const;
        void showCameras(const bool visible);

    private:
        bool _showGrid;
        bool _showAxes;
        bool _showLights;
        bool _showCameras;

    };
}

#endif // RENDERER_H

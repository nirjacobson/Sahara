#ifndef FPSPANEL_H
#define FPSPANEL_H

#include <QElapsedTimer>

#include "panel.h"

namespace Sahara {
    class Renderer;

    class FPSPanel : public Panel
    {
    public:
        FPSPanel(Renderer* renderer);

        // Panel interface
    private:
        Renderer* _renderer;
        QElapsedTimer _timer;

        bool update(QPainter &painter);
    };
}

#endif // FPSPANEL_H

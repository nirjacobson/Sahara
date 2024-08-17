#ifndef FPSPANEL_H
#define FPSPANEL_H

#include <QElapsedTimer>

#include "panel.h"

namespace Sahara {
    class VulkanRenderer;

    class FPSPanel : public Panel
    {
    public:
        FPSPanel(VulkanRenderer* renderer);

        // Panel interface
    private:
        VulkanRenderer* _renderer;
        QElapsedTimer _timer;

        bool update(QPainter &painter);
    };
}

#endif // FPSPANEL_H

#include "fpspanel.h"
#include "render/vulkanrenderer.h"

Sahara::FPSPanel::FPSPanel(VulkanRenderer *renderer)
    : Panel(renderer, 128, 16)
    , _renderer(renderer)
{
    _timer.start();
}

bool Sahara::FPSPanel::update(QPainter &painter)
{
    if (_timer.elapsed() < 250) return false;

    QColor transparency;
    transparency.setAlpha(0);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(QPoint(), size()), transparency);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QString text = QString("%1 FPS").arg(QString::number(_renderer->fps(), 'f', 2));

    painter.setBrush(Qt::black);
    painter.drawText(QPoint(0, size().height()), text);

    _timer.restart();

    return true;
}

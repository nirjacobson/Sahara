#ifndef SAHARA_SURFACE_H
#define SAHARA_SURFACE_H

#include <QMap>

#include "surface.h"
#include "../../../render/vulkanwithvertexbuffers.h"
#include "source.h"

namespace Sahara {

    class VulkanSurface : public Surface, public VulkanWithVertexBuffers
    {
        friend class JSON;

        public:
            VulkanSurface(QVulkanWindow *window, const SourceDict& sourceDict, const QString& material);

            void generateVertexBuffer(const Input::Semantic input);

        private:
            QVulkanWindow* _vulkanWindow;
    };

}

#endif // SAHARA_SURFACE_H

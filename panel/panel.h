#ifndef PANEL_H
#define PANEL_H

#include <QPainter>

#include "render/withvertexbuffers.h"
#include "render/withuniform.h"

namespace Sahara {

    class Renderer;

    class Panel : public WithVertexBuffers, public WithUniform
    {
        public:
            Panel(Renderer* renderer, int width, int height);
            ~Panel();

            const QSize& size() const;

            void setPosition(int x, int y);

            const QPoint& position();

        private:
            Renderer* _renderer;
            QPoint _position;
            QSize _size;

            QImage _image;
            VkImage _vkImage;
            VkImageView _vkImageView;
            VkDeviceMemory _vkImageMemory;
            QList<VkDescriptorSet> _descriptorSets;

            void generateVertexBuffers();
            virtual bool update(QPainter& painter) = 0;

            // WithUniform interface
        public:
            const QList<VkDescriptorSet> &descriptorSets() const;
            void updateUniform(const uint32_t currentFrame);
    };

}

#endif // PANEL_H

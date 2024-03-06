#ifndef SAHARA_IMAGE_H
#define SAHARA_IMAGE_H

#include <QImage>

#include "asset.h"
#include "vulkanutil.h"
#include "../../render/withuniform.h"

namespace Sahara
{
    class Renderer;

    class Image : public Asset, public WithUniform
    {
        friend class JSON;

        public:
            Image(Renderer* renderer, const QString& id, const QString& uri);
            ~Image();

            const QList<VkDescriptorSet>& descriptorSets() const;

            const QString& uri() const;

            void updateUniform() const {};

        private:
            QString _uri;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkImageView _imageView;
            QList<VkDescriptorSet> _descriptorSets;
    };

}

#endif // SAHARA_IMAGE_H

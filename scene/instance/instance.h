#ifndef SAHARA_INSTANCE_H
#define SAHARA_INSTANCE_H

#include <QMap>
#include <QMatrix4x4>

#include "../asset/material.h"
#include "../../common/volume.h"

namespace Sahara
{
    typedef QMap<QString, Material*> MaterialDict;

    class Instance
    {
        friend class JSON;

        protected:
            Instance(const MaterialDict& materials, const QMatrix4x4& transform);

        public:
            virtual ~Instance();

            QStringList materials() const;
            Material& getMaterial(const QString& name);
            void addMaterial(const QString& name, Material* material);

            const QMatrix4x4& transform() const;

            int focusSurface() const;
            void setFocusSurface(const int idx);

            virtual Volume volume() const = 0;

        private:
            MaterialDict _materials;
            QMatrix4x4 _transform;

            int _focusSurface;
    };

}

#endif // SAHARA_INSTANCE_H

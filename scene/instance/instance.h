#ifndef SAHARA_INSTANCE_H
#define SAHARA_INSTANCE_H

#include <QMap>
#include <QMatrix4x4>

#include "../asset/material.h"

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

            const QMatrix4x4& transform() const;

        private:
            MaterialDict _materials;
            QMatrix4x4 _transform;
    };

}

#endif // SAHARA_INSTANCE_H

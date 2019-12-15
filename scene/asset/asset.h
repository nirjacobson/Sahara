#ifndef SAHARA_ASSET_H
#define SAHARA_ASSET_H

#include <QString>

namespace Sahara
{

    class Asset
    {
        public:
            Asset(const QString& id);
            virtual ~Asset();

            const QString& id() const;

        private:
            QString _id;
    };

}

#endif // SAHARA_ASSET_H

#ifndef IMAGE_H
#define IMAGE_H

#include "Sahara_global.h"

#include "asset.h"

namespace Sahara {
    class SAHARA_EXPORT Image : public Asset
    {
        friend class JSON;

    public:
        Image(const QString& id, const QString& uri);
        virtual ~Image() { }

        const QString& uri() const;

    protected:
        QString _uri;
    };
}

#endif // IMAGE_H

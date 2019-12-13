#ifndef SAHARA_SOURCE_H
#define SAHARA_SOURCE_H

#include <QList>
#include <QMap>

namespace Sahara {

    class Source
    {
        public:
            Source(const QList<float> data, const int stride);

            int count() const;
            int stride() const;
            QList<float> at(const int i) const;

        private:
            QList<float> _data;
            int _stride;
    };

    typedef QMap<QString, Source*> SourceDict;

}

#endif // SAHARA_SOURCE_H

#ifndef SAHARA_VOLUME_H
#define SAHARA_VOLUME_H

#include <QVector3D>

namespace Sahara {

    class Volume
    {
        friend class JSON;

        public:
            Volume();
            Volume(const QVector3D& lowerVertex, const QVector3D& upperVertex);

            const QVector3D& lowerVertex() const;
            const QVector3D& upperVertex() const;
            float width();
            float height();
            float length();

            bool intersects(const QVector3D& point) const;

        private:
            QVector3D _lowerVertex;
            QVector3D _upperVertex;
    };

}

#endif // SAHARA_VOLUME_H

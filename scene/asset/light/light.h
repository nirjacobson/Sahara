#ifndef SAHARALIGHT_H
#define SAHARALIGHT_H

#include <QColor>

namespace Sahara
{

    class Light
    {
        public:
            Light(const QColor& color);

            const QColor& color() const;
            void setColor(const QColor& color);

        private:
            QColor _color;
    };

}

#endif // SAHARALIGHT_H

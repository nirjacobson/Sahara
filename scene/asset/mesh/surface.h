#ifndef SURFACE_H
#define SURFACE_H

#include "../../../Sahara_global.h"

#include "source.h"

namespace Sahara {
    class SAHARA_EXPORT Surface
    {
        friend class JSON;

    public:

        class Input {
        public:
            enum Semantic {
                POSITION,
                NORMAL,
                TEXCOORD,
                COLOR,
                JOINTS,
                WEIGHTS,
                UNDEFINED
            };

            Input();
            Input(const QString& source, const int offset);

            const QString& source() const;
            int offset() const;

            static QString semanticToString(const Semantic semantic);
            static Semantic semanticFromString(const QString& semantic);

        private:
            QString _source;
            int _offset;
        };

        Surface(const SourceDict& sourceDict, const QString& material);
        virtual ~Surface() { };

        int count() const;

        const QString& material() const;

        QList<Input::Semantic> inputs() const;
        void setInput(const Input::Semantic semantic, const QString& source, const int offset);

        int offset(const Input::Semantic semantic) const;

        const QList<int>& elements() const;
        void setElements(const QList<int>& elements);

        int triangles() const;

        virtual void generateVertexBuffer(const Input::Semantic input) = 0;

    protected:
        const SourceDict& _sources;
        QString _material;
        QMap<Input::Semantic, Input> _inputs;
        QList<int> _elements;
    };
}

#endif // SURFACE_H

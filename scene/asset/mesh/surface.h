#ifndef SAHARA_SURFACE_H
#define SAHARA_SURFACE_H

#include <QMap>

#include "../../../render/withvertexbuffers.h"
#include "mesh.h"

namespace Sahara {

    class Surface : public WithVertexBuffers
    {
        public:

            class Input {
                public:
                    enum Semantic {
                        POSITION,
                        NORMAL,
                        TEXCOORD,
                        COLOR,
                        BONES,
                        WEIGHTS
                    };

                    Input();
                    Input(const QString& source, const int offset);

                    const QString& source() const;
                    int offset() const;

                    static QString semanticToString(const Semantic semantic);

                private:
                    QString _source;
                    int _offset;
            };

            Surface(Mesh& mesh, const QString& material);

            const QString& material() const;

            QList<Input::Semantic> inputs() const;
            QString source(const Input::Semantic semantic) const;
            int offset(const Input::Semantic semantic) const;
            void setInput(const Input::Semantic semantic, const QString& source, const int offset);

            const QList<int>& elements() const;
            void setElements(const QList<int>& elements);

            void generateVertexBuffer(const Input::Semantic input);

        private:

            Mesh& _mesh;
            QString _material;
            QMap<Input::Semantic, Input> _inputs;
            QList<int> _elements;

    };

}

#endif // SAHARA_SURFACE_H

#ifndef SAHARA_SURFACE_H
#define SAHARA_SURFACE_H

#include <QMap>

#include "../../../render/withvertexbuffers.h"
#include "source.h"

namespace Sahara {

    class Surface : public WithVertexBuffers
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

            Surface(QVulkanWindow *window, const SourceDict& sourceDict, const QString& material);

            int count() const;

            const QString& material() const;

            QList<Input::Semantic> inputs() const;
            void setInput(const Input::Semantic semantic, const QString& source, const int offset);

            int offset(const Input::Semantic semantic) const;

            const QList<int>& elements() const;
            void setElements(const QList<int>& elements);

            int triangles() const;

            void generateVertexBuffer(const Input::Semantic input);

        private:
            QVulkanWindow* _vulkanWindow;
            const SourceDict& _sources;
            QString _material;
            QMap<Input::Semantic, Input> _inputs;
            QList<int> _elements;
    };

}

#endif // SAHARA_SURFACE_H

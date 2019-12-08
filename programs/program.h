#ifndef SAHARA_PROGRAM_H
#define SAHARA_PROGRAM_H

#include <QtOpenGL>
#include <QOpenGLShaderProgram>

#include "../render/withvertexbuffers.h"

namespace Sahara
{

    class Program
    {
        public:
            Program();
            virtual ~Program();

            virtual void bind() = 0;
            virtual void release() = 0;

        protected:
            GLfloat* qVector3DToArray(const QVector3D& vector) const;
            GLfloat* qVector4DToArray(const QVector4D& vector) const;
            GLfloat* qMatrix4x4ToArray(const QMatrix4x4& matrix) const;

            QOpenGLShaderProgram& program();

            void layout(WithVertexBuffers& wvb);
            void unlayout(WithVertexBuffers& wvb);

        private:
            QOpenGLShaderProgram _program;
    };

}

#endif // SAHARA_PROGRAM_H

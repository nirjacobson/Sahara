#ifndef SAHARA_DISPLAYPROGRAM_H
#define SAHARA_DISPLAYPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

namespace Sahara
{

    class DisplayProgram
    {
        public:
            DisplayProgram();
            ~DisplayProgram();

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;
    };

}

#endif // SAHARA_DISPLAYPROGRAM_H

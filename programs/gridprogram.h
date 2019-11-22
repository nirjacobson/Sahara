#ifndef SAHARA_GRIDPROGRAM_H
#define SAHARA_GRIDPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

namespace Sahara
{

    class GridProgram
    {
        public:
            GridProgram();
            ~GridProgram();

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;
    };

}

#endif // SAHARA_GRIDPROGRAM_H

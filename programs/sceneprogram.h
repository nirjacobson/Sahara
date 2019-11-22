#ifndef SAHARA_SCENEPROGRAM_H
#define SAHARA_SCENEPROGRAM_H

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

namespace Sahara
{

    class SceneProgram
    {
        public:
            SceneProgram();
            ~SceneProgram();

        private:
            QOpenGLShader* _vertexShader;
            QOpenGLShader* _fragmentShader;
            QOpenGLShaderProgram* _program;
    };

}

#endif // SAHARA_SCENEPROGRAM_H

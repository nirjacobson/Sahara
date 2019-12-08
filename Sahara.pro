QT += gui xml opengl

TEMPLATE = lib
DEFINES += SAHARA_LIBRARY

QMAKE_CXXFLAGS += -std=c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    render/renderer.cpp \
    common/transform.cpp \
    render/vertexbuffer.cpp \
    programs/displayprogram.cpp \
    programs/gridprogram.cpp \
    programs/program.cpp \
    programs/sceneprogram.cpp \
    scene/asset/animation.cpp \
    scene/asset/animationclip.cpp \
    scene/asset/armature.cpp \
    scene/asset/asset.cpp \
    scene/asset/bone.cpp \
    scene/asset/camera.cpp \
    scene/asset/controller.cpp \
    scene/asset/image.cpp \
    scene/asset/light.cpp \
    scene/asset/material.cpp \
    scene/asset/mesh.cpp \
    scene/asset/pointlight.cpp \
    scene/asset/surface.cpp \
    scene/instance/controllerinstance.cpp \
    scene/instance/instance.cpp \
    scene/instance/meshinstance.cpp \
    scene/model.cpp \
    scene/node/node.cpp \
    scene/node/nodeitem.cpp \
    scene/node/volume.cpp \
    scene/scene.cpp \
    widgets/modelwidget.cpp \
    widgets/scenewidget.cpp \
    render/withvertexbuffers.cpp

HEADERS += \
    Sahara_global.h \
    render/renderer.h \
    common/transform.h \
    render/vertexbuffer.h \
    programs/displayprogram.h \
    programs/gridprogram.h \
    programs/program.h \
    programs/sceneprogram.h \
    scene/asset/animation.h \
    scene/asset/animationclip.h \
    scene/asset/armature.h \
    scene/asset/asset.h \
    scene/asset/bone.h \
    scene/asset/camera.h \
    scene/asset/controller.h \
    scene/asset/image.h \
    scene/asset/light.h \
    scene/asset/material.h \
    scene/asset/mesh.h \
    scene/asset/pointlight.h \
    scene/asset/surface.h \
    scene/instance/controllerinstance.h \
    scene/instance/instance.h \
    scene/instance/meshinstance.h \
    scene/model.h \
    scene/node/node.h \
    scene/node/nodeitem.h \
    scene/node/volume.h \
    scene/scene.h \
    widgets/modelwidget.h \
    widgets/scenewidget.h \
    render/withvertexbuffers.h

INCLUDEPATH += \
    ../QCollada

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    programs/glsl/display.fsh \
    programs/glsl/display.vsh \
    programs/glsl/grid.fsh \
    programs/glsl/grid.vsh \
    programs/glsl/scene.fsh \
    programs/glsl/scene.vsh

RESOURCES += \
    shaders.qrc

FORMS += \
    widgets/modelwidget.ui

cmake_minimum_required(VERSION 3.14)

project(Sahara LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Xml Gui Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml Gui Widgets OpenGL OpenGLWidgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("../QCollada")
include_directories("/Users/nir/MoltenVK/Package/Release/MoltenVK/include")

add_library(Sahara SHARED
  Sahara_global.h
  sahara.cpp
  sahara.h
  common/transform.cpp
  common/transform.h
  common/volume.cpp
  common/volume.h
  json/json.cpp
  json/json.h
  programs/displayprogram.h
  programs/gridprogram.cpp
  programs/gridprogram.h
  programs/program.cpp
  programs/program.h
  programs/sceneprogram.cpp
  programs/sceneprogram.h
  render/display/openglcameradisplay.cpp
  render/display/openglcameradisplay.h
  render/display/opengldisplay.h
  render/display/openglpointlightdisplay.cpp
  render/display/openglpointlightdisplay.h
  render/grid/openglaxis.cpp
  render/grid/openglaxis.h
  render/grid/openglgrid.cpp
  render/grid/openglgrid.h
  render/openglrenderer.cpp
  render/openglrenderer.h
  render/openglvertexbuffer.cpp
  render/openglvertexbuffer.h
  render/openglwithvertexbuffers.cpp
  render/openglwithvertexbuffers.h
  scene/asset/light/ambientlight.cpp
  scene/asset/light/ambientlight.h
  scene/asset/light/light.cpp
  scene/asset/light/light.h
  scene/asset/light/pointlight.cpp
  scene/asset/light/pointlight.h
  scene/asset/mesh/openglmesh.cpp
  scene/asset/mesh/openglmesh.h
  scene/asset/mesh/source.cpp
  scene/asset/mesh/source.h
  scene/asset/mesh/openglsurface.cpp
  scene/asset/mesh/openglsurface.h
  scene/asset/animation.cpp
  scene/asset/animation.h
  scene/asset/animationclip.cpp
  scene/asset/animationclip.h
  scene/asset/armature.cpp
  scene/asset/armature.h
  scene/asset/asset.cpp
  scene/asset/asset.h
  scene/asset/camera.cpp
  scene/asset/camera.h
  scene/asset/controller.cpp
  scene/asset/controller.h
  scene/asset/openglimage.cpp
  scene/asset/openglimage.h
  scene/asset/joint.cpp
  scene/asset/joint.h
  scene/asset/openglmaterial.cpp
  scene/asset/openglmaterial.h
  scene/instance/instance.cpp
  scene/instance/instance.h
  scene/instance/openglinstancecontroller.cpp
  scene/instance/openglinstancecontroller.h
  scene/instance/instancemesh.cpp
  scene/instance/instancemesh.h
  scene/node/node.cpp
  scene/node/node.h
  scene/node/nodeitem.cpp
  scene/node/nodeitem.h
  scene/openglmodel.cpp
  scene/openglmodel.h
  scene/openglscene.cpp
  scene/openglscene.h
  widgets/scenewidget/cameracontrol.cpp
  widgets/scenewidget/cameracontrol.h
  widgets/scenewidget/openglscenewidget.cpp
  widgets/scenewidget/openglscenewidget.h
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  shaders.qrc
  widgets/scenewidget/cameracontrol.cpp
  widgets/scenewidget/cameracontrol.h
  widgets/scenewidget/openglscenewidget.cpp
  widgets/scenewidget/openglscenewidget.h
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  pipelines/animatedpipeline.cpp
  pipelines/animatedpipeline.h
  pipelines/displaypipeline.cpp
  pipelines/displaypipeline.h
  pipelines/gridpipeline.cpp
  pipelines/gridpipeline.h
  pipelines/panelpipeline.cpp
  pipelines/panelpipeline.h
  pipelines/pipeline.cpp
  pipelines/pipeline.h
  pipelines/scenepipeline.cpp
  pipelines/scenepipeline.h
  vulkanutil.cpp vulkanutil.h
  render/vulkanvertexbuffer.cpp
  render/vulkanvertexbuffer.h
  render/vulkanwithvertexbuffers.cpp
  render/vulkanwithvertexbuffers.h
  render/vulkanrenderer.cpp
  render/vulkanrenderer.h
  panel/fpspanel.cpp
  panel/fpspanel.h
  panel/panel.cpp
  panel/panel.h
  scene/asset/vulkanimage.cpp
  scene/asset/vulkanimage.h
  render/vulkanwithuniform.cpp
  render/vulkanwithuniform.h
  scene/asset/vulkanmaterial.cpp
  scene/asset/vulkanmaterial.h
  scene/instance/vulkaninstancecontroller.cpp
  scene/instance/vulkaninstancecontroller.h
  scene/vulkanscene.cpp
  scene/vulkanscene.h
  widgets/scenewidget/vulkanscenewidget.cpp
  widgets/scenewidget/vulkanscenewidget.h
  widgets/scenewidget/vulkanscenewindow.cpp
  widgets/scenewidget/vulkanscenewindow.h
  scene/vulkanmodel.cpp
  scene/vulkanmodel.h
  scene/asset/mesh/vulkanmesh.cpp
  scene/asset/mesh/vulkanmesh.h
  scene/asset/mesh/vulkansurface.cpp
  scene/asset/mesh/vulkansurface.h
  scene/asset/mesh/mesh.h
  scene/asset/mesh/mesh.cpp
  scene/asset/mesh/surface.h
  scene/asset/mesh/surface.cpp
  scene/asset/material.h
  scene/asset/material.cpp
  scene/instance/instancecontroller.h
  scene/instance/instancecontroller.cpp
  scene/model.h
  scene/model.cpp
  scene/asset/image.h
  scene/asset/image.cpp
  scene/scene.h
  scene/scene.cpp
  widgets/scenewidget/scenewidget.h
  widgets/scenewidget/scenewidget.cpp
  render/renderer.h
  render/renderer.cpp
  render/grid/vulkanaxis.cpp
  render/grid/vulkanaxis.h
  render/grid/vulkangrid.cpp
  render/grid/vulkangrid.h
  render/display/vulkancameradisplay.cpp
  render/display/vulkancameradisplay.h
  render/display/vulkandisplay.h
  render/display/vulkanpointlightdisplay.cpp
  render/display/vulkanpointlightdisplay.h
  programs/spir-v/animated.vert.spv
  programs/spir-v/display.frag.spv
  programs/spir-v/display.vert.spv
  programs/spir-v/grid.frag.spv
  programs/spir-v/grid.vert.spv
  programs/spir-v/panel.frag.spv
  programs/spir-v/panel.vert.spv
  programs/spir-v/scene.frag.spv
  programs/spir-v/scene.vert.spv
  programs/glsl/120/display.fsh
  programs/glsl/120/display.vsh
  programs/glsl/120/grid.fsh
  programs/glsl/120/grid.vsh
  programs/glsl/120/scene.fsh
  programs/glsl/120/scene.vsh
  programs/glsl/300/display.fsh
  programs/glsl/300/display.vsh
  programs/glsl/300/grid.fsh
  programs/glsl/300/grid.vsh
  programs/glsl/300/scene.fsh
  programs/glsl/300/scene.vsh
  programs/displayprogram.cpp
)

if (LINUX)
    target_link_libraries(Sahara PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
elseif (WIN32)
    target_link_libraries(Sahara PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        "${CMAKE_CURRENT_SOURCE_DIR}/../QCollada/build/Desktop-${CMAKE_BUILD_TYPE}/QCollada.lib")
elseif (APPLE)
    target_link_libraries(Sahara PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        "${CMAKE_CURRENT_SOURCE_DIR}/../QCollada/build/Desktop-${CMAKE_BUILD_TYPE}/libQCollada.dylib")

endif()

target_compile_definitions(Sahara PRIVATE SAHARA_LIBRARY)

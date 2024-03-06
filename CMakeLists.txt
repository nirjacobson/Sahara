cmake_minimum_required(VERSION 3.14)

project(Sahara LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Xml Gui Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml Gui Widgets OpenGL OpenGLWidgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("../QCollada")

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
  render/display/cameradisplay.cpp
  render/display/cameradisplay.h
  render/display/display.cpp
  render/display/display.h
  render/display/pointlightdisplay.cpp
  render/display/pointlightdisplay.h
  render/grid/axis.cpp
  render/grid/axis.h
  render/grid/grid.cpp
  render/grid/grid.h
  render/renderer.cpp
  render/renderer.h
  render/vertexbuffer.cpp
  render/vertexbuffer.h
  render/withvertexbuffers.cpp
  render/withvertexbuffers.h
  scene/asset/light/ambientlight.cpp
  scene/asset/light/ambientlight.h
  scene/asset/light/light.cpp
  scene/asset/light/light.h
  scene/asset/light/pointlight.cpp
  scene/asset/light/pointlight.h
  scene/asset/mesh/mesh.cpp
  scene/asset/mesh/mesh.h
  scene/asset/mesh/source.cpp
  scene/asset/mesh/source.h
  scene/asset/mesh/surface.cpp
  scene/asset/mesh/surface.h
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
  scene/asset/image.cpp
  scene/asset/image.h
  scene/asset/joint.cpp
  scene/asset/joint.h
  scene/asset/material.cpp
  scene/asset/material.h
  scene/instance/instance.cpp
  scene/instance/instance.h
  scene/instance/instancecontroller.cpp
  scene/instance/instancecontroller.h
  scene/instance/instancemesh.cpp
  scene/instance/instancemesh.h
  scene/node/node.cpp
  scene/node/node.h
  scene/node/nodeitem.cpp
  scene/node/nodeitem.h
  scene/model.cpp
  scene/model.h
  scene/scene.cpp
  scene/scene.h
  widgets/scenewidget/cameracontrol.cpp
  widgets/scenewidget/cameracontrol.h
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  shaders.qrc
  widgets/scenewidget/cameracontrol.cpp
  widgets/scenewidget/cameracontrol.h
  widgets/scenewidget/scenewidget.cpp
  widgets/scenewidget/scenewidget.h
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  widgets/modelwidget.cpp
  widgets/modelwidget.h
  widgets/modelwidget.ui
  pipelines/displaypipeline.h pipelines/displaypipeline.cpp
  programs/glsl/display.frag
  programs/glsl/display.vert
  programs/glsl/grid.frag
  programs/glsl/grid.vert
  programs/glsl/scene.frag
  programs/glsl/scene.vert
  programs/spir-v/display.frag.spv
  programs/spir-v/display.vert.spv
  programs/spir-v/grid.frag.spv
  programs/spir-v/grid.vert.spv
  programs/spir-v/scene.frag.spv
  programs/spir-v/scene.vert.spv
  pipelines/pipeline.h pipelines/pipeline.cpp
  pipelines/gridpipeline.h pipelines/gridpipeline.cpp
  pipelines/scenepipeline.h pipelines/scenepipeline.cpp
  vulkanutil.h vulkanutil.cpp
  render/withuniform.h render/withuniform.cpp
  widgets/scenewidget/scenewindow.h widgets/scenewidget/scenewindow.cpp
  widgets/scenewidget/scenewidget.h widgets/scenewidget/scenewidget.cpp

)

target_link_libraries(Sahara PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

target_compile_definitions(Sahara PRIVATE SAHARA_LIBRARY)

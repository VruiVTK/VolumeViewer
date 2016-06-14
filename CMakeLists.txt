CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF(POLICY CMP0020)
  CMAKE_POLICY(SET CMP0020 NEW)
ENDIF()

PROJECT(VolumeViewer)

set(CMAKE_MODULE_PATH
  "${VolumeViewer_SOURCE_DIR}/CMake/Modules"
  ${CMAKE_MODULE_PATH}
)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Use c++11:
set(CMAKE_CXX_STANDARD 11)

INCLUDE(FindPkgConfig)

IF(NOT VRUI_PKGCONFIG_DIR)
  FIND_PATH(VRUI_PKGCONFIG_DIR Vrui.pc)
ENDIF()
SET(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${VRUI_PKGCONFIG_DIR}")

PKG_SEARCH_MODULE(VRUI REQUIRED Vrui>=3000000)
STRING(REPLACE "-framework;" "-framework " VRUI_LDFLAGS "${VRUI_LDFLAGS}")
ADD_DEFINITIONS(${VRUI_CFLAGS})

# Find VTK
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

IF (VTK_VERSION VERSION_LESS "6.2")
  MESSAGE(FATAL_ERROR "Require VTK version 6.2 or higher")
ENDIF ()

IF (${VTK_RENDERING_BACKEND} STREQUAL "OpenGL")
  FIND_PACKAGE (GLEW REQUIRED)
  IF (NOT GLEW_FOUND)
    MESSAGE (FATAL_ERROR "Glew required. Please set GLEW_DIR")
  ENDIF ()
  INCLUDE_DIRECTORIES (${GLEW_INCLUDE_DIR})
ENDIF ()

# Find vtkVRUI
find_package(vtkVRUI REQUIRED)
include_directories(${vtkVRUI_INCLUDE_DIRS})

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
INCLUDE(InstallRequiredSystemLibraries)

SET(${PROJECT_NAME}_SRCS
  BaseLocator.cpp
  ClippingPlane.cpp
  ClippingPlaneLocator.cpp
  ColorMap.cpp
  ColorMapCallbackData.cpp
  ColorMapChangedCallbackData.cpp
  Contours.cpp
  ControlPoint.cpp
  ControlPointChangedCallbackData.cpp
  ExampleVTKReader.cpp
  FreeSliceLocator.cpp
  Gaussian.cpp
  Isosurfaces.cpp
  main.cpp
  RGBAColor.cpp
  ScalarWidget.cpp
  ScalarWidgetCallbackData.cpp
  ScalarWidgetChangedCallbackData.cpp
  ScalarWidgetControlPoint.cpp
  ScalarWidgetControlPointChangedCallbackData.cpp
  ScalarWidgetStorage.cpp
  Slices.cpp
  Storage.cpp
  SwatchesWidget.cpp
  TransferFunction1D.cpp
  volApplicationState.cpp
  volContextState.cpp
  volGeometry.cpp
  volReader.cpp
  )

ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  ${vtkVRUI_LIBRARIES}
  ${VTK_LIBRARIES}
  "${VRUI_LDFLAGS}"
)

IF (${VTK_RENDERING_BACKEND} STREQUAL "OpenGL")
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GLEW_LIBRARY})
ENDIF ()

INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

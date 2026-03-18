add_library(${PROJECT_NAME}_VIEWER)

target_include_directories(${PROJECT_NAME}_VIEWER
        PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/deps"
)

# configure sources
file(GLOB SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)

# add sources to the library
target_sources(${PROJECT_NAME}_VIEWER
        PRIVATE
        ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}_VIEWER
    PUBLIC
        ${VTK_LIBRARIES}
        ${OpenCASCADE_LIBRARIES}
)

# Use VTK auto-initialization for rendering backends
if (TARGET vtkCommonCore)
    vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}_VIEWER
        MODULES ${VTK_LIBRARIES}
    )
endif()

add_executable(${PROJECT_NAME}_MAIN
        main.cpp
)

target_include_directories(${PROJECT_NAME}_MAIN
        PRIVATE
        SYSTEM ${OpenCASCADE_INCLUDE_DIR}
        "${CMAKE_SOURCE_DIR}/deps"
        "${CMAKE_SOURCE_DIR}/src"
)

# Link VTK and OpenCASCADE
target_link_libraries(${PROJECT_NAME}_MAIN 
    PRIVATE 
    ${OpenCASCADE_LIBRARIES}
    ${VTK_LIBRARIES}
)

# Use VTK auto-initialization for rendering backends
if (TARGET vtkCommonCore)
    vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}_MAIN
        MODULES ${VTK_LIBRARIES}
    )
endif()
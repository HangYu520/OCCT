add_executable(${PROJECT_NAME}_MAIN
        main.cpp
)

target_include_directories(${PROJECT_NAME}_MAIN
        PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/deps"
)

target_link_libraries(OCCT_MAIN
    PRIVATE
        ${PROJECT_NAME}_VIEWER
        ${PROJECT_NAME}_IO
        ${OpenCASCADE_LIBRARIES}
)

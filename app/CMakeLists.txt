add_executable(${PROJECT_NAME}_MAIN
        main.cpp
)

target_include_directories(${PROJECT_NAME}_MAIN
        PRIVATE
        SYSTEM ${OpenCASCADE_INCLUDE_DIR}
        "${CMAKE_SOURCE_DIR}/deps"
        "${CMAKE_SOURCE_DIR}/src"
)

# Add linker options
target_link_libraries(${PROJECT_NAME}_MAIN PRIVATE ${OpenCASCADE_LIBRARIES})
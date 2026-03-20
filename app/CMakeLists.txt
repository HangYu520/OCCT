# ------------- MAIN ----------------

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
        ${PROJECT_NAME}_UTILS
        ${OpenCASCADE_LIBRARIES}
)

# ------------- TASK ----------------

add_executable(${PROJECT_NAME}_TASK
        task.cpp
)

target_include_directories(${PROJECT_NAME}_TASK
        PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/deps"
)

target_link_libraries(${PROJECT_NAME}_TASK
    PRIVATE
        ${PROJECT_NAME}_IO
        ${PROJECT_NAME}_UTILS
        ${OpenCASCADE_LIBRARIES}
)

# ------------- MAKE BOTTLE ----------------

add_executable(${PROJECT_NAME}_BOTTLE
        mkBottle.cpp
)

target_include_directories(${PROJECT_NAME}_BOTTLE
        PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/deps"
)

target_link_libraries(OCCT_BOTTLE
    PRIVATE
        ${PROJECT_NAME}_VIEWER
        ${PROJECT_NAME}_IO
        ${PROJECT_NAME}_UTILS
        ${OpenCASCADE_LIBRARIES}
)
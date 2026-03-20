add_library(${PROJECT_NAME}_UTILS)

target_include_directories(${PROJECT_NAME}_UTILS
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
target_sources(${PROJECT_NAME}_UTILS
        PRIVATE
        ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}_UTILS
    PUBLIC
        ${OpenCASCADE_LIBRARIES}
)

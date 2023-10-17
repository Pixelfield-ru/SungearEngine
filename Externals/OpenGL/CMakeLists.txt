project(gl46)

add_library(${PROJECT_NAME} STATIC include/glad/glad.h src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC include)
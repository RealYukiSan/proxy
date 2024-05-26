cmake_minimum_required(VERSION 3.24)
project(proxy)

set(CMAKE_C_FLAGS_DEBUG "-Wall -DVERBOSE -Wextra -g")
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O3 -DNDEBUG")

# Enet Module
file(GLOB ENET_SRC "${PROJECT_SOURCE_DIR}/thirdparty/enet/*.c")
add_library(Enet STATIC ${ENET_SRC})
target_include_directories(Enet PUBLIC "${PROJECT_SOURCE_DIR}/thirdparty/enet/include")

# TLSE Module 
file(GLOB TLSE_SRC "${PROJECT_SOURCE_DIR}/thirdparty/tlse/*.c")
add_library(Tlse STATIC ${TLSE_SRC})
target_include_directories(Tlse PUBLIC "${PROJECT_SOURCE_DIR}/thirdparty/tlse")

# sha256 Module
file(GLOB SHA256_SRC "${PROJECT_SOURCE_DIR}/thirdparty/sha256/*.c")
add_library(Sha256 STATIC ${SHA256_SRC})
target_include_directories(Sha256 PUBLIC "${PROJECT_SOURCE_DIR}/thirdparty/sha256")


# Project Setup
file(GLOB_RECURSE PROJECT_SRC 
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/core/*.c"
    "${PROJECT_SOURCE_DIR}/src/packet/*.c"
    "${PROJECT_SOURCE_DIR}/src/utils/*.c")
add_executable(${PROJECT_NAME} ${PROJECT_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE Enet Tlse Sha256 pthread)


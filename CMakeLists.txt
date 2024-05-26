cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "minipro-gui")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME} LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

#### Set up Conan and build dependencies #### https://github.com/conan-io/cmake-conan ##################################
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

#set(CONAN_SYSREQUIRES_MODE "enabled")
conan_cmake_configure(REQUIRES
#        qt/6.6.3
        BASIC_SETUP CMAKE_TARGETS
        GENERATORS CMakeToolchain CMakeDeps)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})
########################################################################################################################

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

file(GLOB H_FILES "src/*.h")
file(GLOB CPP_FILES "src/*.cpp")
file(GLOB INCLUDE "src/include/*.*")

add_executable(${PROJECT_NAME} ${H_FILES} ${CPP_FILES} ${INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core
        Qt::Gui
        Qt::Widgets)
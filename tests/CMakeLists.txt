cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "minipro-gui")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME} LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)

qt_standard_project_setup()

file(GLOB H_FILES "../src/*.h")
file(GLOB CPP_FILES "../src/*.cpp")
file(GLOB TEST_FILES "*.cpp")

qt_add_executable(${PROJECT_NAME}
        ${H_FILES}
        ../src/MainWindow.cpp
        ../src/HexView.cpp
        ${TEST_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Test
        Qt6::Widgets
)

install(TARGETS ${PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
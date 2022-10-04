cmake_minimum_required(VERSION 3.5)

add_subdirectory(QCustomPlot-library-master)
project(Calculator_v2 VERSION 0.1 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 COMPONENTS Gui REQUIRED)

set(TS_FILES Calculator_v2_en_US.ts)

set(PROJECT_SOURCES
        main.cc
        View_module/mainwindow.h
        View_module/mainwindow.cc
        View_module/mainwindow.ui
        View_module/calculator_mode.h
        View_module/calculator_mode.cc
        View_module/calculator_mode.ui
        View_module/credit_mode.h
        View_module/credit_mode.cc
        View_module/credit_mode.ui
        View_module/deposit_mode.h
        View_module/deposit_mode.cc
        View_module/deposit_mode.ui
        View_module/withdrawal.h
        View_module/withdrawal.cc
        View_module/withdrawal.ui
        View_module/replenishment.h
        View_module/replenishment.cc
        View_module/replenishment.ui
        View_module/qcplotic.h
        View_module/qcplotic.cc
        View_module/qcplotic.ui

        Model_module/calculator_model.h
        Model_module/calculator.h
        Model_module/calculator.cc
        Model_module/credit_calculator.h
        Model_module/credit_calculator.cc
        Model_module/deposit_calculator.h
        Model_module/deposit_calculator.cc
        Model_module/date_calculator.h
        Model_module/date_calculator.cc

        Controller_module/calculator_controller.h
        Controller_module/calculator_controller.cc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Calculator_v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Calculator_v2 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Calculator_v2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Calculator_v2
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Calculator_v2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE qcustomplot)
target_compile_definitions(${PROJECT_NAME} PRIVATE QCUSTOMPLOT_USE_LIBRARY)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui)

set_target_properties(Calculator_v2 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Calculator_v2)
endif()

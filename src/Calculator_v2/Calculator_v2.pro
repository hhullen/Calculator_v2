QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS=-finline-functions

MODEL_DIR = ../Calculator_v2/Model_module
VIEW_DIR = ../Calculator_v2/View_module
CONTROLLER_DIR = ../Calculator_v2/Controller_module
INCLUDE_DIR = ../Calculator_v2/QCustomPlot-library-master/lib

INCLUDEPATH += $$MODEL_DIR $$VIEW_DIR $$CONTROLLER_DIR $$INCLUDE_DIR

ICON = Resources/photosurface.icns

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cc \
    $$VIEW_DIR/mainwindow.cc \
    $$VIEW_DIR/calculator_mode.cc \
    $$VIEW_DIR/credit_mode.cc \
    $$VIEW_DIR/deposit_mode.cc \
    $$VIEW_DIR/withdrawal.cc \
    $$VIEW_DIR/replenishment.cc \
    $$VIEW_DIR/qcplotic.cc \
    $$MODEL_DIR/calculator.cc \
    $$MODEL_DIR/credit_calculator.cc \
    $$MODEL_DIR/deposit_calculator.cc \
    $$MODEL_DIR/date_calculator.cc \
    $$CONTROLLER_DIR/calculator_controller.cc \
    $$INCLUDE_DIR/qcustomplot.cpp

HEADERS += \
    $$VIEW_DIR/mainwindow.h \
    $$VIEW_DIR/calculator_mode.h \
    $$VIEW_DIR/credit_mode.h \
    $$VIEW_DIR/deposit_mode.h \
    $$VIEW_DIR/withdrawal.h \
    $$VIEW_DIR/replenishment.h \
    $$VIEW_DIR/qcplotic.h \
    $$MODEL_DIR/calculator_model.h \
    $$MODEL_DIR/calculator.h \
    $$MODEL_DIR/credit_calculator.h \
    $$MODEL_DIR/deposit_calculator.h \
    $$MODEL_DIR/date_calculator.h \
    $$CONTROLLER_DIR/calculator_controller.h \
    $$INCLUDE_DIR/qcustomplot.h \

FORMS += \
    $$VIEW_DIR/mainwindow.ui \
    $$VIEW_DIR/calculator_mode.ui \
    $$VIEW_DIR/credit_mode.ui \
    $$VIEW_DIR/deposit_mode.ui \
    $$VIEW_DIR/withdrawal.ui \
    $$VIEW_DIR/replenishment.ui \
    $$VIEW_DIR/qcplotic.ui

TRANSLATIONS += \
    Calculator_v2_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

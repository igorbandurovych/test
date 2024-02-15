QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    IconStorage.cpp \
    OperatorDelegate.cpp \
    OperatorEditor.cpp \
    OperatorEditorViewModel.cpp \
    OperatorModel.cpp \
    OperatorModelItem.cpp \
    OperatorView.cpp \
    PlusButton.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Constants.h \
    IconStorage.h \
    MainWindow.h \
    OperatorDelegate.h \
    OperatorEditor.h \
    OperatorEditorViewModel.h \
    OperatorModel.h \
    OperatorModelItem.h \
    OperatorView.h \
    PlusButton.h

FORMS += \
    MainWindow.ui \
    OperatorEditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

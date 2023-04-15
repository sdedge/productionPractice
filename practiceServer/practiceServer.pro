QT       += core gui
QT += network    #   network - используется для работы с сетевыми классами

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/frames/cardFrame/I_cardframe.cpp \
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.cpp \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.cpp \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.cpp \
    components/frames/cardFrame/changeIPLineEditFrame/change_IP_line_edit_frame.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    components/frames/cardFrame/I_cardframe.h \
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.h \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.h \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.h \
    components/frames/cardFrame/changeIPLineEditFrame/change_IP_line_edit_frame.h \
    mainwindow.h \
    server.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

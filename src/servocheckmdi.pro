TARGET = servoCheckMDI

TEMPLATE = app

QT += widgets
QT += serialport

CONFIG += c++11

unix {
    INCLUDEPATH += "/home/cord/Dev/OpenSceneGraph-3.6.3/include/"
    QMAKE_LIBDIR += "/home/cord/Dev/OpenSceneGraph-3.6.3-build/lib/"
    LIBS += -losg -losgDB -losgGA -losgViewer
}

win32 {
    INCLUDEPATH += "C:\Program Files\OpenSceneGraph\include"
    QMAKE_LIBDIR += "C:\Program Files\OpenSceneGraph\lib"
    LIBS += osgd.lib osgDBd.lib osgGAd.lib osgViewerd.lib
}

requires(qtConfig(tableview))
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
                ArticulatedPart.h \
                ChannelModel.h \
                ChannelWindow.h \
                DoubleSpinBoxDelegate.h \
                OsgWidget.h \
                PartModel.h \
                PartSpec.h \
                PartsWindow.h \
                ProgressBarDelegate.h \
                ServoModel.h \
                ServoWindow.h \
                ViewerWindow.h \
                data.h \
                mdichild.h \
                mdichildbase.h \
                serialportreader.h \
                settingsdialog.h
SOURCES       = main.cpp \
                ArticulatedPart.cpp \
                ChannelModel.cpp \
                ChannelWindow.cpp \
                DoubleSpinBoxDelegate.cpp \
                OsgWidget.cpp \
                PartModel.cpp \
                PartSpec.cpp \
                PartsWindow.cpp \
                ServoModel.cpp \
                ServoWindow.cpp \
                ViewerWindow.cpp \
                mainwindow.cpp \
                mdichild.cpp \
                mdichildbase.cpp \
                serialportreader.cpp \
                settingsdialog.cpp
RESOURCES     = mdi.qrc

# install
target.path = "C:\Users\cordj\Dev\ServoCheckMDI"
INSTALLS += target

FORMS += \
    settingsdialog.ui

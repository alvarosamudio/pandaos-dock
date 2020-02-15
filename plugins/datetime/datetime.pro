include (../../interfaces/interfaces.pri)

QT              += widgets svg
TEMPLATE         = lib
CONFIG          += plugin c++11 link_pkgconfig

TARGET          = $$qtLibraryTarget(datetime)
#DESTDIR          = $$_PRO_FILE_PWD_/../
DISTFILES       += datetime.json

HEADERS += \
    datetimeplugin.h \
    datetimewidget.h

SOURCES += \
    datetimeplugin.cpp \
    datetimewidget.cpp

target.path = $${PREFIX}/lib/pandaos-dock/plugins/
INSTALLS += target

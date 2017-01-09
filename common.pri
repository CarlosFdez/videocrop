CONFIG += c++14

# Extra requirements for building
# - libvlc dll files in path
# - plugins folder in path
# - Release only, dunno how to get it working in debug

## qtav
INCLUDEPATH += $$PWD/dependencies/qtav/include/
LIBS += -L$$PWD/dependencies/qtav/lib -lQt5AVWidgets -lQt5AV

CONFIG += c++14

# Extra requirements for building
# - libvlc dll files in path
# - plugins folder in path
# - Release only, dunno how to get it working in debug

## qtav (includes and libraries)
INCLUDEPATH += $$PWD/dependencies/qtav/include/
LIBS += -L$$PWD/dependencies/qtav/lib -lQt5AVWidgets -lQt5AV

# Copy binaries to output (based on http://stackoverflow.com/a/39234363)
copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD/dependencies/bin)\" \"$$shell_path($$OUT_PWD)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

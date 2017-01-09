# VideoCrop

A gui application used to crop videos on keyframes without retranscoding to maintain quality, remuxing the video into an mp4. Its similar to one of Avidemux's use case, but supports exporting multiple intervals simultaneously and comes with video playback features. Written in C++ using Qt.

This is my first real project with C++, so any help or advice would be appreciated. This was developed for Windows but does not use any Windows specific libraries, so with some work it could be cross-platform.

### Dependencies
Uses QtAV for viewing the video and ffmpeg to crop and remux.

For QtAV, venture over to http://www.qtav.org/ and get the files by installing the software and enabling development files (or building it yourself). Dump the binaries, lib, and include files into the dependencies folder. You may have to rename the debug lib files on Windows (QtAV1.lib to QtAV5d.lib and QtAVWidgets1.lib to Qt5AVWidgetsd.lib).

For ffmpeg, download the binaries from https://www.ffmpeg.org/download.html, put them in a directory somewhere, and include them in your system path. Or use your installed package manager (like apt or homebrew) if you're on a good platform.

### Compiling
Open up QtCreator, run QMake, and build. I could not get it to run in debug mode, so you may have to run in release mode.

### Todo
* Fix debug mode, currently only works in release mode.
* Use an actual architecture meant for GUIs. Could be MVC, could be a state machine, could be whatever.
* Implement options
* Add more buttons for existing features
* Make the screenshot button actually work
* Figure out how to unit test C++ code (and guis)

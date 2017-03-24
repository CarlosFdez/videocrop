# VideoCrop

A gui application used to crop videos on keyframes without retranscoding to maintain quality, remuxing the video into an mp4. Its similar to one of Avidemux's use case, but supports exporting multiple intervals simultaneously and comes with video playback features. Written in C++ using Qt.

This is my first real project with C++, so any help or advice would be appreciated. This was developed for Windows but does not use any Windows specific libraries, so with some work it could be cross-platform.

**Website**: https://carlosfdez.github.io/videocrop/ \
**Trello Board**: https://trello.com/b/ZucBimnC/videocrop

### Dependencies
Uses QtAV for viewing the video and ffmpeg to crop and remux. Uses C++14 and Qt 5.7

For QtAV, venture over to http://www.qtav.org/ and get the files by installing the software and enabling development files (or building it yourself). Dump the binaries, lib, and include files into the dependencies folder. You may have to rename the debug lib files on Windows (QtAV1.lib to QtAV5d.lib and QtAVWidgets1.lib to Qt5AVWidgetsd.lib).

For ffmpeg, download the binaries from https://www.ffmpeg.org/download.html, put them in a directory somewhere, and include them in your system path. Or use your installed package manager (like apt or homebrew) if you're on a good platform. Add the ffmpeg executable to the bin folder.

### Compiling
Open up QtCreator, run QMake, and build. I could not get it to run in debug mode, so you may have to run in release mode.

#### Deployment (Windows)
Run a command prompt in the output folder. If qt is in your system path, run `windeployqt <executableName>` to automatically add the qt binaries as well. Aftewards, copy all exe and dll files to an output folder.

#### Deployment (Other platforms)
If someone wishes to take this on, please let me know. I wrote this application to crop my own gameplay recordings to store before video editing, which means that I use it on the same platform I play games on (Windows). I have no personal need to run it on Linux, and I do not own a Mac.

### Todo
* Improve code (requires help from an actual C++ developer)
* Improve deployment scheme, maybe learn how to create a build server
* Fix debug mode, currently only works in release mode.
* Figure out and implement unit test for C++ and Qt
* The rest are in Trello

A simple project written for myself that crops videos on keyframes to maintain quality. Its intended to be similar to Avidemux but with better support for intervals and better previewing features. Written in C++ and Qt.

This is my first ever real project with C++, so any help or advice would be incredibly appreciated. This was developed for Windows but with some work on the linking it could be cross-platform.

### Dependencies
Uses QtAV for viewing the video and ffmpeg to perform the cutting.

For ffmpeg, download the binaries from https://www.ffmpeg.org/download.html, put them in a directory somewhere, and include them in your system path. Or use the package manager if you're on a good platform.

For QtAV, venture over to http://www.qtav.org/ and get the files by installing the software and enabling development files (or building it yourself). Dump the binaries, lib, and include files into the dependencies folder.

### Compiling
Open up QtCreator, run QMake, and build. I could not get it to run in debug mode, so you may have to run in release mode.
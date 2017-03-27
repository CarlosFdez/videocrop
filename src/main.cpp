#include "mainwindow.h"
#include <QApplication>
#include <stdlib.h>
#include <string>
#include <memory>
#include <iostream>

#include <QMediaPlayer>
#include <QVideoWidget>

#include <QtAVWidgets>
#include <QtAV>

using namespace std;
using namespace QtAV;

/*
 * Defines a Qt Application. Created to implement global error catching.
 */
class VideoCropApplication : public QApplication
{
public:
    VideoCropApplication(int& argc, char** argv): QApplication(argc, argv) {}
    bool notify(QObject* receiver, QEvent* event)
    {
        // todo: this doesn't seem to actually be catching errors, figure this out later
        bool done = true;
        try
        {
            done = QApplication::notify(receiver, event);
        } catch (const std::exception& ex) {
            qDebug() << "std exception";
        } catch (...) {
            qDebug() << "unknown exception";
        }
        return done;
    }
};

int main(int argc, char *argv[])
{
    try
    {
        QtAV::Widgets::registerRenderers();

        VideoCropApplication a(argc, argv);

        MainWindow w;

        if (argc > 1)
        {
            w.openFile(argv[argc-1]);
        }

        w.show();

        try
        {
            return a.exec();
        }
        catch (...)
        {
            qDebug() << "Unknown exception while executing application";
        }
    }
    catch (...)
    {
        cerr << "Unknown exception while setting up application";
        return -1;
    }
}

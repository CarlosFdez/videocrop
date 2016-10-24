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

//extern "C" {
//    #include <libavcodec/avcodec.h>
//    #include <libavformat/avformat.h>
//    #include <libswscale/swscale.h>
//}

//shared_ptr<AVFormatContext> load_context(const string& filename)
//{
//    AVFormatContext *pFormatCtx = nullptr;

//    if (avformat_open_input(&pFormatCtx, filename.c_str(), nullptr, nullptr) < 0)
//        throw "could not open file";

//    return shared_ptr<AVFormatContext>(pFormatCtx, avformat_free_context);
//}

class VideoCropApplication : public QApplication
{
public:
    VideoCropApplication(int& argc, char** argv): QApplication(argc, argv) {}
    bool notify(QObject* receiver, QEvent* event)
    {
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
        //av_register_all();
        QtAV::Widgets::registerRenderers();

        VideoCropApplication a(argc, argv);


//        QString pluginPath = "C:\\bin\\plugins";
//        VlcCommon::setPluginPath(pluginPath);

//        string filePath = "F:\\Videos\\OBS\\dark souls 2\\output\\mcduff.mp4";
//        shared_ptr<AVFormatContext> ctx = load_context(filePath);
//        cout << "filename" << ctx->filename << endl;
//        cout << "start time" << ctx->start_time << endl;
//        cout << "duration" << ctx->duration << endl;
//        cout << "num tracks" << ctx->nb_streams << endl;

        MainWindow w;
        w.show();

        try
        {
            return a.exec();
        }
        catch (...)
        {
            qDebug() << "Unknown exception";
        }
    }
    catch (...)
    {
        cerr << "Unknown exception";
        return -1;
    }
}

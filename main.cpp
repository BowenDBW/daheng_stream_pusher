#include "daheng_stream_pusher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DahengStreamPusher w;
    w.show();

    return a.exec();
}

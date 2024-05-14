#include "daheng_stream_pusher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    daheng_stream_pusher w;
    w.show();

    return a.exec();
}

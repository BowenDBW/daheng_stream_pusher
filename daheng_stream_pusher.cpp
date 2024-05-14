#include "daheng_stream_pusher.h"

void debug() {
    QMessageBox::information(nullptr, "debug",
        QString::number(CameraReader::getInstance().getServerCount()));

    std::vector<std::string>* names = CameraReader::getInstance().getServerNames();
    QMessageBox::information(nullptr, "debug",
        QString::fromStdString(names->at(0)));
}


daheng_stream_pusher::daheng_stream_pusher(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    debug();
}

daheng_stream_pusher::~daheng_stream_pusher()
{

}

#include "Drowsypick_tool.h"
#include <QtWidgets/QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Drowsypick_tool w;
	w.setWindowIcon(QIcon("./image/favicon.ico"));

    w.show();
    return a.exec();
}

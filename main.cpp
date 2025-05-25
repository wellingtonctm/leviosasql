#include "leviosa.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Leviosa w;
    w.show();
    return a.exec();
}

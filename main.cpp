#include "leviosa.h"
#include "sessionmanagerdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Leviosa w;
    // SessionManagerDialog w;
    w.show();
    return a.exec();
}

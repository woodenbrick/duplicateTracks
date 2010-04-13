#include <QApplication>
#include "duplicatetracks.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DuplicateTrackWindow* gui = new DuplicateTrackWindow();
    gui->show();
    return a.exec();
}

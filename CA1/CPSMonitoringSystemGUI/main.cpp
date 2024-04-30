#include "cpsapplication.h"
#include "cpswebsocket.h"
int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    CPS::Application application;
    application.show();

    return qApplication.exec();
}

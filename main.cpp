#include <QApplication>
#include <QtWidgets>

#include "windowload.h"
#include "mainwindow.h"
#include "windowresults.h"
#include "load.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
     //Load::loadAFile("/Users/narjissezaki/Desktop/mrna/test.csv",true);

    //creating the folders necessary to the use of the program
    system("mkdir \"Download\"");

   /* MainWindow fenetre;*/
    MainWindow fenetre;
        fenetre.show();
        return app.exec();

    return 0;
}

/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Implementacia funkcie main
  */

#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include "model/dataholder.h"

/** Simulation default speed of time change */
#define SIMULATION_OFFSET 2

/**
 * @brief main Hlavny program, ktoru spusta a vytvara aplikaciu
 * @param argc Pocet argumentov
 * @param argv Zoznam argumentov
 * @return stav ukoncenia
 */
int main(int argc, char *argv[])
{

    DataHolder *model = new DataHolder(34190);
    QApplication a(argc, argv);
    MainWindow w;
    w.setModel(model);
    w.show();
    return a.exec();
}

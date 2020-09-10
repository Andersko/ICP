/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Trieda MainWindow reprezentujuca okno aplikacie (styl zobrazenia, poloha a vlastnosti jednotlivych grafickych casti aplikacie, ...)
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "model/dataholder.h"
#include "controller/mapscene.h"
#include "controller/itineraryscene.h"
#include <QPushButton>

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class Class prepresenting graphic window of application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow Konstruktor pre triedu MainWindow
     * @param parent Parent
     */
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    /**
     * @brief setModel sets data model to window
     * @param model data model
     */
    void setModel(DataHolder *model);

public slots:

    /**
     * @brief notify notify all components that needs to be updated when timer ticks
     */
    void notify();

    /**
     * @brief resetTime resets time to basic value
     */
    void resetTime();

    /**
     * @brief zoomIn zoom in
     */
    void zoomIn();

    /**
     * @brief zoomOut zoom out
     */
    void zoomOut();

    /**
     * @brief processMapClick activate componets that needs to be activated when map is clicked
     */
    void processMapClick();

    /**
     * @brief processBusClick activate componets that needs to be activated when bus is clicked
     * @param bus clicked bus
     */
    void processBusClick(Spoj* bus);

    /**
     * @brief changeSpeed changes speed of time in app
     */
    void changeSpeed();

    /**
     * @brief trafficChanged changes traffic value of road
     * @param text traffic value
     */
    void trafficChanged(QString text);

    /**
     * @brief roadClose handles close road button clicks
     */
    void roadClose();

    /**
     * @brief throwErrorForUser throws error for user in bottom of window
     * @param msg error message
     */
    void throwErrorForUser(const QString msg);

private:

    /**
     * @brief ui ui
     */
    Ui::MainWindow *ui;

    /**
     * @brief model data model
     */
    DataHolder *model;

    /**
     * @brief map graphics view map
     */
    QGraphicsView *map;

    /**
     * @brief itinerary graphics view itinerary
     */
    QGraphicsView *itinerary;

    /**
     * @brief sceneMap graphics scene map
     */
    MapScene *sceneMap;

    /**
     * @brief sceneItinerary graphics scene itinerary
     */
    ItineraryScene *sceneItinerary;

    /**
     * @brief time time
     */
    int time = 0;

    /**
     * @brief timeOffset time offest
     */
    int timeOffset = 1; // Casovy posun o sekundy

    /**
     * @brief actualSpeed actual speed of app
     */
    QPushButton *actualSpeed;

    /**
     * @brief setTime sets application actual time
     */
    void setTime();

    /**
     * @brief setListData sets data list
     */
    void setListData();

    /**
     * @brief timer timer
     */
    QTimer *timer;

    /**
     * @brief showBusStops shows bus stops when bus is clicked
     * @param connection clicked bus
     */
    void showBusStops(Spoj *connection);

    /**
     * @brief replaceModelWithStops shows stops
     * @param stops stops
     */
    void replaceModelWithStops(QVector<std::tuple<QString, QString>> stops);
};

#endif // MAINWINDOW_H

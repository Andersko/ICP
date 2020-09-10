/**
  * @file
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Trieda MapScene reprezentujuca cast okna aplikacie zobrazujuceho itinerator spoju
  */

#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "model/dataholder.h"
#include "model/linka.h"
#include "model/road.h"
#include "model/spoj.h"
#include <QMouseEvent>
#include <QString>
#include <tuple>

/**
 * @brief The lastSelectedItem enum information of what type of item was last selected item
 */
enum lastSelectedItem {
    NOTHING,
    BUS,
    ROAD
};

/**
 * @brief The roadCloseMode enum information of in which mode scene is
 */
enum roadCloseMode {
    NORMAL,
    CLOSED,
    DETOUR
};

/**
 * @brief The MapScene class implements graphics scene for map
 */
class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * @brief MapScene Konstruktor pre triedu MapScene
     * @param parent Parent
     */
    explicit MapScene(QObject* parent = 0);

    /**
     * @brief setActiveBuses sets active buses
     * @param buses active buses
     */
    void setActiveBuses(QVector<Spoj*> buses);

    /**
     * @brief init scene initialization to paint map roads and stops
     * @param roads roads of map
     * @param model pointer to data model
     */
    void init(QVector<Road*> roads, DataHolder* model);

    /**
     * @brief updateMap update active buses on map based on their coordinates
     */
    void updateMap();

    /**
     * @brief setMode sets road close mode
     * @param mode mode
     */
    void setMode(roadCloseMode mode);

    /**
     * @brief getMode gets road close mode
     * @return current mode
     */
    roadCloseMode getMode();

    /**
     * @brief mousePressEvent handles mouse clicks on the scene
     * @param event mouse event
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:

    /**
     * @brief removePreviousEvent handles last mouse event before new one is precessed
     */
    void removePreviousEvent();

    /**
     * @brief processActualEvent precesses actual mouse event
     * @param event mouse event
     */
    void processActualEvent(QGraphicsSceneMouseEvent* event);

    /**
     * @brief escapeRoadCloseMode manage termination of road close mode if something goes wrong
     */
    void escapeRoadCloseMode();

    /**
     * @brief paintRoad repaint road color if was updated
     * @param road
     */
    void paintRoad(Road* road);

    /**
     * @brief model pointer to data model
     */
    DataHolder* model;

    /**
     * @brief lastSelected information (enum) of last selected item
     */
    lastSelectedItem lastSelected = NOTHING;

    /**
     * @brief mode information (enum) of actual mode
     */
    roadCloseMode mode = NORMAL;

    /**
     * @brief roads map roads
     */
    QVector<Road*> roads;

    /**
     * @brief activeBuses active buses
     */
    QVector<Spoj*> activeBuses;

    /**
     * @brief lastRoad saves last selected road
     */
    Road* lastRoad;

    /**
     * @brief lastBus saves last selected bus
     */
    Spoj* lastBus;

    /**
     * @brief roadsItems saves all road graphics items in graphic scene
     */
    QVector<QGraphicsLineItem*> roadsItems;

    /**
     * @brief activeBusesItems saves all bus graphics items in graphic scene
     */
    QVector<QGraphicsEllipseItem*> activeBusesItems;

    /**
     * @brief closedRoads tuple of one closed route (closed roads : detour roads)
     */
    std::tuple<QVector<Road*>, QVector<Road*>> closedRoads;

    /**
     * @brief allClosedRoads vector of all active tuples of closed routes
     */
    QVector<std::tuple<QVector<Road*>, QVector<Road*>>> allClosedRoads;

public slots:

    /**
     * @brief roadClose slot that handles change of road close modes
     */
    void roadClose();

    /**
     * @brief roadCloseCancel slot that handles cancel of already done road close by user
     */
    void roadCloseCancel();

signals:

    /**
     * @brief mapClicked emits that map was clicked
     */
    void mapClicked();

    /**
     * @brief busClicked emits that bus was clicked
     * @param bus clicked bus
     */
    void busClicked(Spoj* bus);

    /**
     * @brief roadClicked emits that road was clicked
     * @param text traffic level of road
     */
    void roadClicked(const QString text);

    /**
     * @brief roadClickedClosed emits that closed road was clicked
     */
    void roadClickedClosed();

    /**
     * @brief roadClickedDetour emits that detour road was clicked
     */
    void roadClickedDetour();

    /**
     * @brief cancelRoadClose emits that road close mode is force escaping
     */
    void cancelRoadCloseMode();

    /**
     * @brief throwErrorForUser throws error for user in bottom of window
     * @param msg error msg
     */
    void throwErrorForUser(QString msg);
};

#endif // MAPSCENE_H

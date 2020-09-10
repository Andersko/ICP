/**
  * @file
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Implementacia triedy MapScene reprezentujuca cast okna aplikacie zobrazujuceho hlavnu mapu
  */

#include "mapscene.h"
#include <math.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

/** Universal size of pen for MapScene */
#define PEN_SIZE 6 // universal size of items in scene

MapScene::MapScene(QObject *parent) : QGraphicsScene(parent)
{
    // pass
}

void MapScene::init(QVector<Road*> roads, DataHolder* model) {

    this->model = model;

    this->roads = roads;

    int x1, x2, y1, y2;

    /* roads paint */
    for (const auto &road : this->roads){
        x1 = road->getStart().x;
        x2 = road->getEnd().x;
        y1 = road->getStart().y;
        y2 = road->getEnd().y;

        auto line = addLine(x1, y1, x2, y2);
        line->setPen(QPen(Qt::black,PEN_SIZE));
        this->roadsItems.append(line);
    }

    /* stops paint */
    for (const auto &road : this->roads) {
        x1 = road->getStart().x;
        x2 = road->getEnd().x;
        y1 = road->getStart().y;
        y2 = road->getEnd().y;

        QRegExp reg("Z.*");

        if ( reg.exactMatch(road->getStart().name)){
            addEllipse(x1 - PEN_SIZE, y1 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::darkBlue), Qt::blue);
        }
        if ( reg.exactMatch(road->getEnd().name)){
            addEllipse(x2 - PEN_SIZE, y2 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::darkBlue), Qt::blue);
        }
    }
}

void MapScene::updateMap() {

    // removes not up to date buses from last update
    for ( const auto &item: this->activeBusesItems ) {
        removeItem(item);
        delete item;
    }

    activeBusesItems.clear();

    int x, y;

    /* actaul buses symbols paint */
    for ( const auto &spoj: this->activeBuses ) {
        x = spoj->x;
        y = spoj->y;

        if ( spoj->isSelected() ){
            activeBusesItems.append(addEllipse(x - PEN_SIZE * 2, y - PEN_SIZE * 2, PEN_SIZE * 4, PEN_SIZE * 4, QPen(Qt::darkBlue), Qt::cyan));
        } else {
            activeBusesItems.append(addEllipse(x - PEN_SIZE * 2, y - PEN_SIZE * 2, PEN_SIZE * 4, PEN_SIZE * 4, QPen(Qt::black), spoj->getColor()));
        }
    }
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    removePreviousEvent();
    processActualEvent(event);
    updateMap();

    // call base class overrides method to take care of some importants of which this override does not
    QGraphicsScene::mousePressEvent(event);
}

void MapScene::removePreviousEvent() {

    if ( this->lastSelected ) {
        if ( this->lastSelected == BUS ) {
            for ( auto road : this->lastBus->getConnectionRoadsShort() ) {
                this->paintRoad(road);
            }

            this->lastBus->setSelect(false);
        }

        if ( this->lastSelected == ROAD ) {
            this->lastRoad->setSelect(false);
            this->paintRoad(this->lastRoad);
        }

        this->lastSelected = NOTHING;
    }
}

void MapScene::processActualEvent(QGraphicsSceneMouseEvent *event) {

    if ( ! items(event->scenePos()).empty() ) {
        /* some item was clicked */

        QGraphicsItem* item = items(event->scenePos()).first();

        if ( this->mode == NORMAL ) {
            /* road close mode is off */

            if ( item->type() == 6 ) {
                /* clicked item is road */

                Road* road = roads.at(roadsItems.indexOf(dynamic_cast<QGraphicsLineItem*>(item)));
                road->setSelect(true);
                this->paintRoad(road);

                this->lastSelected = ROAD;
                this->lastRoad = road;

                if ( std::get<0>(road->getRoadClose()) ) {
                    emit roadClickedClosed();
                }
                else {
                    emit roadClicked(QString::number(road->getTraffic()));
                }

                if ( std::get<1>(road->getRoadClose()) ) {
                    emit roadClickedDetour();
                }

            } else if ( item->type() == 4 && dynamic_cast<QGraphicsEllipseItem*>(item)->rect().height() == PEN_SIZE * 4 ) {
                /* clicked item is bus */

                Spoj* bus = activeBuses.at(activeBusesItems.indexOf(dynamic_cast<QGraphicsEllipseItem*>(item)));

                bus->setSelect(true);
                this->lastSelected = BUS;
                this->lastBus = bus;

                // color bus roads without paintRoad() cause roads are not selected
                for ( auto road : bus->getConnectionRoadsShort() ) {
                    dynamic_cast<QGraphicsLineItem*>(roadsItems.at(roads.indexOf(road)))->setPen(QPen(Qt::cyan,PEN_SIZE));
                }

                emit busClicked(bus);
            }
        }

        else if ( this->mode == CLOSED ) {
            /* road close mode is on */

            if ( item->type() == 6 ) {

                Road* road = roads.at(roadsItems.indexOf(dynamic_cast<QGraphicsLineItem*>(item)));

                if ( std::get<0>(road->getRoadClose()) ) {
                    /* road already blocked */
                    this->escapeRoadCloseMode();
                    emit throwErrorForUser("Nemozno zablokovat jednu cestu viac krat");
                    return;
                }

                if ( std::get<1>(road->getRoadClose()) ) {
                    /* trying to block road that is already detour */
                    this->escapeRoadCloseMode();
                    emit throwErrorForUser("Nemozno zablokovat obchadzkovu trasu");
                    return;
                }

                QString previousStart;
                QString previousEnd;
                QString actualStart;
                QString actualEnd;

                if ( !std::get<0>(closedRoads).isEmpty() ) {
                    previousStart = std::get<0>(closedRoads).last()->getStartName();
                    previousEnd = std::get<0>(closedRoads).last()->getEndName();
                    actualStart = road->getStartName();
                    actualEnd = road->getEndName();
                }

                // short-circuit evaluation used to avoid second condition if first road is added
                // control if road is connected to last chosed
                if ( std::get<0>(closedRoads).isEmpty() || (!previousStart.compare(actualStart) || !previousStart.compare(actualEnd) || !previousEnd.compare(actualStart) || !previousEnd.compare(actualEnd)) ) {
                    if ( std::get<0>(closedRoads).indexOf(road) < 0 ) {
                        /* item is selected first time */

                        road->setRoadClose(std::make_tuple(1, 0));
                        std::get<0>(closedRoads).append(road);

                        dynamic_cast<QGraphicsLineItem*>(item)->setPen(QPen(Qt::red,PEN_SIZE));
                    }
                }

                else {
                    /* clicked road is not connected to last clicked */
                    this->escapeRoadCloseMode();
                    emit throwErrorForUser("Zvolena cesta sa nespaja s poslednou zvolenou");
                    return;
                }
            }

            else {
                /* clicked somewhere else than road while road close mode is on, exiting road close mode */
                this->escapeRoadCloseMode();
                return;
            }
        }

        else if ( this->mode == DETOUR ) {
            /* road close mode is on */

            if ( item->type() == 6 ) {

                Road* road = roads.at(roadsItems.indexOf(dynamic_cast<QGraphicsLineItem*>(item)));

                QString previousStart;
                QString previousEnd;
                QString actualStart;
                QString actualEnd;

                if ( !std::get<1>(closedRoads).isEmpty() ) {
                    previousStart = std::get<1>(closedRoads).last()->getStartName();
                    previousEnd = std::get<1>(closedRoads).last()->getEndName();
                    actualStart = road->getStartName();
                    actualEnd = road->getEndName();
                }

                // short-circuit evaluation used to avoid second condition if first road is added
                if ( std::get<1>(closedRoads).isEmpty() || (!previousStart.compare(actualStart) || !previousStart.compare(actualEnd) || !previousEnd.compare(actualStart) || !previousEnd.compare(actualEnd)) ) {
                    if ( std::get<0>(closedRoads).indexOf(road) < 0 && std::get<1>(closedRoads).indexOf(road) < 0 ) {
                        /* item is selected first time */

                        road->setRoadClose(std::make_tuple(0, 1));
                        std::get<1>(closedRoads).append(road);

                        dynamic_cast<QGraphicsLineItem*>(item)->setPen(QPen(Qt::green,PEN_SIZE));
                    }
                    else if ( std::get<1>(closedRoads).indexOf(road) >= 0 ) {
                        /* clicked road is already selected as detour in actual road closing */
                        this->escapeRoadCloseMode();
                        emit throwErrorForUser("Nemozno zvolit cestu ako obchadzku dva krat v jednom uzatvarani ciest");
                        return;
                    }
                    else {
                        /* if was clicked closed road from actual road closing, nothing happens */
                        return;
                    }
                }

                else {
                    /* clicked somewhere else than road while road close mode is on, exiting road close mode */
                    this->escapeRoadCloseMode();
                    emit throwErrorForUser("Zvolena cesta sa nespaja s poslednou zvolenou");
                    return;
                }
            }

            else {
                /* clicked somewhere else than road while road close mode is on, exiting road close mode */
                this->escapeRoadCloseMode();
                return;
            }
        }
    }
    else {
        /* map was clicked */

        emit mapClicked();

        if ( this->mode == CLOSED || this->mode == DETOUR ) {
            /* clicked on map while road close mod is on, exiting road close mode */
            this->escapeRoadCloseMode();
            return;
        }
    }
}

void MapScene::roadClose() {

    if ( this->mode == NORMAL ) {
        /* user confirmed closed roads and chose detour */

        // control if any roads was chosen
        if ( std::get<1>(this->closedRoads).isEmpty() ) {
            this->escapeRoadCloseMode();
            emit throwErrorForUser("Je potrebne zvolit minimalne jednu cestu obchadzky");
            return;
        }

        if ( std::get<0>(this->closedRoads).size() == 1 && std::get<1>(this->closedRoads).size() == 1 ) {
            this->escapeRoadCloseMode();
            emit throwErrorForUser("Nie je mozne vytvorit uzavretie s obchadzkou s menej ako 3 zvolenymi cestami");
            return;
        }

        QString closedFirstStart = std::get<0>(this->closedRoads).first()->getStartName();
        QString closedFirstEnd = std::get<0>(this->closedRoads).first()->getEndName();
        QString closedLastStart = std::get<0>(this->closedRoads).last()->getStartName();
        QString closedLastEnd = std::get<0>(this->closedRoads).last()->getEndName();
        QString detourFirstStart = std::get<1>(this->closedRoads).first()->getStartName();
        QString detourFirstEnd = std::get<1>(this->closedRoads).first()->getEndName();
        QString detourLastStart = std::get<1>(this->closedRoads).last()->getStartName();
        QString detourLastEnd = std::get<1>(this->closedRoads).last()->getEndName();

        if ( ! (
           ((!closedFirstStart.compare(detourFirstStart) ||
             !closedFirstStart.compare(detourFirstEnd) ||
             !closedFirstEnd.compare(detourFirstStart) ||
             !closedFirstEnd.compare(detourFirstEnd))
             &&
            (!closedLastStart.compare(detourLastStart) ||
             !closedLastStart.compare(detourLastEnd) ||
             !closedLastEnd.compare(detourLastStart) ||
             !closedLastEnd.compare(detourLastEnd)))
             ||
           ((!closedFirstStart.compare(detourLastStart) ||
             !closedFirstStart.compare(detourLastEnd) ||
             !closedFirstEnd.compare(detourLastStart) ||
             !closedFirstEnd.compare(detourLastEnd))
             &&
            (!closedLastStart.compare(detourFirstStart) ||
             !closedLastStart.compare(detourFirstEnd) ||
             !closedLastEnd.compare(detourFirstStart) ||
             !closedLastEnd.compare(detourFirstEnd)))
           ))
        {
            /* closed roads and detour roads is not connected properly */
            this->escapeRoadCloseMode();
            emit throwErrorForUser("Nevhodne zvolene cesty");
            return;
        }


        // change color of detour roads
        for ( Road* road : std::get<1>(closedRoads) ) {
            QGraphicsLineItem* roadItem = roadsItems.at(roads.indexOf(road));
            roadItem->setPen(QPen(Qt::darkGreen,PEN_SIZE));
        }

        for ( Linka* linka : model->getAllConnections() ) {
            /* iteration through all lines */
            for ( auto road : linka->getRoads() ) {
                /* iteration through all roads of current line */

                Road* onlyRoad = std::get<0>(road);

                for ( Road* road : std::get<0>(this->closedRoads) ) {
                    /* iteration through all closed roads by user */

                    if ( ! onlyRoad->getName().compare(road->getName()) ) {
                        /*
                        * this condition evaluate as true only once in every iteration of last two fors
                        * at the end it will break out of both
                        */

                        /* route of current line contains at least one closed road, line's buses will be processed */

                        for( Spoj* bus : linka->getConnections() ) {
                            /* iteration through all line's buses */

                            QVector<std::tuple<Road*, int>> oldConnectionRoads = bus->getConnectionRoads();
                            QVector<std::tuple<Road*, int>> newConnectionRoads;

                            std::tuple<Road*, int> previousRoad = make_tuple(nullptr, 0);

                            // flag to prevent multiple detour inserts if multiple roads was closed by user
                            bool detourAlreadyInserted = false;
                            // information about last road from original route used to decide if detour should be inserted upside down
                            Road* previousRoadOriginal;

                            for ( std::tuple<Road*, int> road : oldConnectionRoads ) {
                                /* iteration through all roads of current bus */

                                // flag that sets false if current road of current bus is closed, therefore this original road won't be inserted
                                bool originalRoad = true;

                                for ( Road* closedRoad : std::get<0>(this->closedRoads) ) {
                                    /* iteration through all closed roads by user - again */

                                    if ( ! std::get<0>(road)->getName().compare(closedRoad->getName()) ) {
                                        /* closed road on bus route was found */

                                        if ( !detourAlreadyInserted ) {
                                            /* closed road will be replaced with detour, if detour wasn't already inserted by another found closed road */

                                            int direction;

                                            QString originalRoadStart = previousRoadOriginal->getStartName();
                                            QString originalRoadEnd = previousRoadOriginal->getEndName();
                                            QString newRoadStart = std::get<1>(this->closedRoads).first()->getStartName();
                                            QString newRoadEnd = std::get<1>(this->closedRoads).first()->getEndName();

                                            if ( !originalRoadStart.compare(newRoadStart) || !originalRoadStart.compare(newRoadEnd) || !originalRoadEnd.compare(newRoadStart) || !originalRoadEnd.compare(newRoadEnd) ) {
                                                for ( Road* road : std::get<1>(this->closedRoads) ) {
                                                    /* this cycle determinate direction of every road in new buses route and insert it to new bus route */

                                                    Road* previousRoadShort = std::get<0>(previousRoad);

                                                    if ( previousRoadShort ) {
                                                        if ( !road->getStartName().compare(previousRoadShort->getEndName()) || !road->getEndName().compare(previousRoadShort->getStartName())) {
                                                            direction = std::get<1>(previousRoad);
                                                        }
                                                        else {
                                                            direction = std::get<1>(previousRoad) * (-1);
                                                        }
                                                    }
                                                    else {
                                                        QRegExp reg("Z.*");

                                                        if ( reg.exactMatch(road->getStartName()) ) {
                                                            direction = 1;
                                                        }
                                                        else {
                                                            direction = -1;
                                                        }
                                                    }

                                                    std::tuple<Road*, int> longRoad;
                                                    longRoad = make_tuple(road, direction);
                                                    newConnectionRoads.append(longRoad);

                                                    previousRoad = longRoad;
                                                }
                                            }

                                            else {
                                                for ( int i = std::get<1>(this->closedRoads).size() - 1; i >= 0; i-- ) {
                                                    /* this cycle determinate direction of every road in new buses route and insert it to new bus route */
                                                    /* it also reverse order in which were roads selected by user */

                                                    Road* road = std::get<1>(this->closedRoads).at(i);
                                                    Road* previousRoadShort = std::get<0>(previousRoad);

                                                    if ( previousRoadShort ) {
                                                        if ( !road->getStartName().compare(previousRoadShort->getEndName()) || !road->getEndName().compare(previousRoadShort->getStartName())) {
                                                            direction = std::get<1>(previousRoad);
                                                        }
                                                        else {
                                                            direction = std::get<1>(previousRoad) * (-1);
                                                        }
                                                    }
                                                    else {
                                                        QRegExp reg("Z.*");

                                                        if ( reg.exactMatch(road->getStartName()) ) {
                                                            direction = 1;
                                                        }
                                                        else {
                                                            direction = -1;
                                                        }
                                                    }

                                                    std::tuple<Road*, int> longRoad;
                                                    longRoad = make_tuple(road, direction);
                                                    newConnectionRoads.append(longRoad);

                                                    previousRoad = longRoad;
                                                }
                                            }

                                            detourAlreadyInserted = true;
                                        }

                                        originalRoad = false;
                                        break;
                                    }
                                }

                                if ( originalRoad ) {
                                    /* any closed roads wasn't match with original road so original road is inserted */

                                    newConnectionRoads.append(road);
                                    previousRoad = road;
                                }

                                previousRoadOriginal = std::get<0>(road);
                            }

                            // save new route
                            bus->assignRoads(newConnectionRoads);
                        }

                        goto doubleBreak_1; // break from two fors
                    }
                }
            }

            doubleBreak_1:;
        }

        allClosedRoads.append(closedRoads);
    }

    else if ( this->mode == CLOSED ) {
        /* user is choosing closed roads */

        // reset data from last road close
        std::get<0>(closedRoads).clear();
        std::get<1>(closedRoads).clear();

        QGraphicsLineItem* lastRoadItem = roadsItems.at(roads.indexOf(this->lastRoad));
        lastRoadItem->setPen(QPen(Qt::red,PEN_SIZE));

        this->lastRoad->setSelect(false);
        this->lastSelected = NOTHING;
        this->lastRoad->setRoadClose(std::make_tuple(1, 0));

        std::get<0>(closedRoads).append(this->lastRoad);
    }

    else if ( this->mode == DETOUR ) {
        /* user is choosing detour roads */

        // change color of closed roads
        for ( Road* road : std::get<0>(closedRoads) ) {
            QGraphicsLineItem* roadItem = roadsItems.at(roads.indexOf(road));
            roadItem->setPen(QPen(Qt::gray,PEN_SIZE));
        }
    }
}

void MapScene::roadCloseCancel() {

    this->lastRoad->setSelect(false);
    this->lastSelected = NOTHING;

    for ( auto closedRoute : this->allClosedRoads ) {
        for ( Road* closedRoad : std::get<0>(closedRoute) ) {
            if ( ! closedRoad->getName().compare(this->lastRoad->getName()) ) {
                /* found a closed rout that user want to cancel */

                this->closedRoads = closedRoute;
                this->escapeRoadCloseMode();

                for ( Linka* linka : model->getAllConnections() ) {
                    /* iteration through all lines */
                    for ( auto road : linka->getRoads() ) {
                        /* iteration through all roads of current line */

                        Road* onlyRoad = std::get<0>(road);

                        if ( ! closedRoad->getName().compare(onlyRoad->getName()) ) {

                            for( Spoj* bus : linka->getConnections() ) {
                                bus->assignRoads(linka->getRoads());
                            }

                            goto doubleBreak_2;
                        }
                    }
                }

                doubleBreak_2:;

                this->allClosedRoads.removeOne(closedRoute);
                goto doubleBreak_3;
            }
        }
    }

    doubleBreak_3:;
}

void MapScene::escapeRoadCloseMode() {

    this->mode = DETOUR;
    emit cancelRoadCloseMode();

    // repaint closed roads
    for ( Road* road : std::get<0>(this->closedRoads) ) {
        road->setRoadClose(std::make_tuple(-1, 0));
        this->paintRoad(road);
    }

    // repaint detour roads
    for ( Road* road : std::get<1>(this->closedRoads) ) {
        road->setRoadClose(std::make_tuple(0, -1));
        this->paintRoad(road);
    }
}

void MapScene::paintRoad(Road* road) {

    QGraphicsLineItem* roadItem = roadsItems.at(roads.indexOf(road));

    if ( road->isSelected() ) {
        roadItem->setPen(QPen(Qt::cyan,PEN_SIZE));
    }
    else if ( std::get<0>(road->getRoadClose()) ) {
        roadItem->setPen(QPen(Qt::gray,PEN_SIZE));
    }
    else if ( std::get<1>(road->getRoadClose()) ) {
        roadItem->setPen(QPen(Qt::darkGreen,PEN_SIZE));
    }
    else if ( road->getTraffic() != 0 ) {

        if ( road->getTraffic() >= 10 ) {
            roadItem->setPen(QPen(Qt::red,PEN_SIZE));
        }
        else {
            roadItem->setPen(QPen(QColor("orange"),PEN_SIZE));
        }
    }
    else {
        roadItem->setPen(QPen(Qt::black,PEN_SIZE));
    }
}

void MapScene::setActiveBuses(QVector<Spoj *> buses){
    this->activeBuses = buses;
}

roadCloseMode MapScene::getMode() {
    return this->mode;
}

void MapScene::setMode(roadCloseMode mode) {
    this->mode = mode;
}

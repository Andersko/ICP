/**
  * @file
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Implementacia triedy ItineraryScene reprezentujuca cast okna aplikacie zobrazujuceho itinerator spoju
  */

#include "itineraryscene.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QDebug>

/** Univerzal size of item in scene */
#define PEN_SIZE 5 

ItineraryScene::ItineraryScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::lightGray);
}

void ItineraryScene::update(Spoj* bus) {

// -------------------------------------------------
//  main itinerary line paint

    // remove all last items from previous show() from scene
    for ( auto item : items() ) {
        removeItem(item);
        delete item;
    }

    // main itinerary line
    addLine(0, 0, 1000, 0)->setPen(QPen(Qt::blue, PEN_SIZE));



// -------------------------------------------------
//  stops, names and bus symbol paint

    // sum of all road's lenghts
    int busLenght = 0;

    for ( Road* road : bus->getConnectionRoadsShort() ) {
        busLenght += road->getLength();
    }

    // sum of ratios of roads lenght, equal 1000 (main itinerary line lenght) after upcoming for cycle finishes
    qreal LengthRatioCount = 0;

    QString startName;
    QString endName;
    QString PreviousStartName = "";
    QString PreviousEndName = "";
    bool verticalTextPosition = false; // stop name up/down switch --- false -> up, true -> down
    QRegExp reg("Z.*");

    // first stop and its name has to be paint manually
    addEllipse(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::blue), Qt::blue)->setZValue(1);
    startName = bus->getConnectionRoadsShort().first()->getStartName();
    endName = bus->getConnectionRoadsShort().first()->getEndName();
    if ( reg.exactMatch(startName) ) {
        addText(startName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
    }
    else {
        addText(endName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
    }

    // draws ellipses representing stops and their names
    for ( Road* road : bus->getConnectionRoadsShort() ) {

        // offset for current road in ratio to bus all road connections roads
        qreal offset = (road->getLength() / (qreal)busLenght) * 1000;

        if ( road->getTraffic() > 0 && road->getTraffic() < 10 ) {
            addLine(LengthRatioCount, 0, LengthRatioCount + offset, 0)->setPen(QPen(QColor("orange"), PEN_SIZE));
        }
        else if ( road->getTraffic() >= 10 ) {
            addLine(LengthRatioCount, 0, LengthRatioCount + offset, 0)->setPen(QPen(Qt::red, PEN_SIZE));
        }

        startName = road->getStartName();
        endName = road->getEndName();

        if ( reg.exactMatch(endName) && (!endName.compare(PreviousStartName) || !endName.compare(PreviousEndName)) ) {

            addEllipse(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::blue), Qt::blue)->setZValue(1);

            if ( verticalTextPosition ) {
                addText(endName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
                verticalTextPosition = ! verticalTextPosition;
            }
            else {
                addText(endName)->setPos(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE * 2 - 25);
                verticalTextPosition = ! verticalTextPosition;
            }
        }
        else if ( reg.exactMatch(startName) && (!startName.compare(PreviousStartName) || !startName.compare(PreviousEndName)) ) {

            addEllipse(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::blue), Qt::blue)->setZValue(1);

            if ( verticalTextPosition ) {
                addText(startName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
                verticalTextPosition = ! verticalTextPosition;
            }
            else {
                addText(startName)->setPos(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE * 2 - 25);
                verticalTextPosition = ! verticalTextPosition;
            }

        }

        // bus symbol paint
        if ( ! road->getName().compare(bus->getActualPosition()->getName()) ) {
            addEllipse(LengthRatioCount + offset * bus->BusPositionOnRoad() - PEN_SIZE * 2, 0 - PEN_SIZE * 2, PEN_SIZE * 4, PEN_SIZE * 4, QPen(Qt::black), bus->getColor())->setZValue(2);
        }

        LengthRatioCount += offset;

        PreviousStartName = startName;
        PreviousEndName = endName;
    }

    // last stop and its name has to be paint manually
    addEllipse(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE, PEN_SIZE * 2, PEN_SIZE * 2, QPen(Qt::blue), Qt::blue)->setZValue(1);
    startName = bus->getConnectionRoadsShort().last()->getStartName();
    endName = bus->getConnectionRoadsShort().last()->getEndName();
    if ( reg.exactMatch(startName) ) {
        if ( verticalTextPosition ) {
            addText(startName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
        }
        else {
            addText(startName)->setPos(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE * 2 - 25);
        }
    }
    else {
        if ( verticalTextPosition ) {
            addText(endName)->setPos(LengthRatioCount - PEN_SIZE, 0 + PEN_SIZE * 2);
        }
        else {
            addText(endName)->setPos(LengthRatioCount - PEN_SIZE, 0 - PEN_SIZE * 2 - 25);
        }
    }
}

void ItineraryScene::ditch() {
    for ( auto item : items() ) {
        removeItem(item);
        delete item;
    }
}

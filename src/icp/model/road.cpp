/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Implementacia triedy Road reprezentuje cestu na mape
  */

#include "road.h"
#include <QDebug>
#include <QStringList>

Road::Road(QString record)
{
    QStringList records = record.split(" ");

    this->start.name = records[0];
    this->start.x = records[1].toInt();
    this->start.y = records[2].toInt();

    this->end.name = records[3];
    this->end.x = records[4].toInt();
    this->end.y = records[5].toInt();

    this->length = records[6].toInt();

    this->roadClose = std::make_tuple(0, 0);
}

QString Road::getStartName(){
    return this->start.name;
}

QString Road::getEndName(){
    return this->end.name;
}

Point Road::getEnd(){
    return this->end;
}

Point Road::getStart(){
    return this->start;
}

void Road::setTraffic(float delay){
    // Delay in seconds //
    this->traffic = delay;
}

int Road::getTraffic(){
    return this->traffic;
}

int Road::getLength(){
    /* Pre testovacie ucely, je dlzka cesty v sekundach */
    return this->length * LENGTH_CONVERTOR;
}

QString Road::getName(){
    return this->start.name + "-" + this->end.name;
}

void Road::setSelect(bool selected){
    this->selected = selected;
}

bool Road::isSelected(){
    return this->selected;
}

void Road::setRoadClose(std::tuple<int, int> roadClose) {
    this->roadClose = std::make_tuple(std::get<0>(this->roadClose) + std::get<0>(roadClose), std::get<1>(this->roadClose) + std::get<1>(roadClose));
}

std::tuple<int, int> Road::getRoadClose() {
    return this->roadClose;
}

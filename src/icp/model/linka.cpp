/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Implementacia triedy Linka reprezentuje autobusovu linku
  */

#include "linka.h"

#include <QDebug>


Linka::Linka(QString record)
{
    QStringList records = record.split(" ");

    this->id = records[0].toInt();
    this->beginStop = records[1];
    this->endStop = records[2];

    QStringList stops = records[3].split("-");
    for ( int i = 0; i < stops.size(); i++){
        QString stop = stops[i];
        this->middleStops.append(stop);
    }

    QStringList routes = records[4].split("-");

    for ( int i = 0; i < routes.size(); i++ ){
        QString route = routes[i];
        this->routeWay.append(route);
    }

    this->connectionColor = QColor(rand() % 200 + 1, rand() % 200 +1, rand() % 200 + 1);
}

void Linka::notify(int time){
    for ( auto const &spoj : this->busConnections ){
        spoj->setTime(time);
        spoj->notify();
    }
}

QVector<std::tuple<QString, Spoj*>> Linka::getSchedule(){
    QVector<std::tuple<QString, Spoj*>> schedule;

    for ( const auto &spoj : this->busConnections ){
        schedule.append(std::make_tuple(spoj->getSchedule(), spoj));
    }

    return schedule;
}

QVector<Spoj*> Linka::getActiveBuses(){
    QVector<Spoj*> activeBuses;

    for ( const auto &spoj : this->busConnections ){
        if ( spoj->isActive() ){
            activeBuses.append(spoj);
        }
    }

    return activeBuses;
}

void Linka::addConnection(Spoj *newSpoj){
    this->busConnections.append(newSpoj);
    newSpoj->addColor(this->connectionColor);
}

void Linka::addRoad(std::tuple<Road *, int> newRoad){
    this->roads.append(newRoad);
}

QVector<std::tuple<Road*, int>> Linka::getRoads(){
    return this->roads;
}

QVector<Spoj*> Linka::getConnections(){
    return this->busConnections;
}

QString Linka::getBeginStop(){
    return this->beginStop;
}

QString Linka::getEndStop(){
    return this->endStop;
}

QColor Linka::getColor(){
    return this->connectionColor;
}

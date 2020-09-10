/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Implementacia triedy Spoj a jej metod.
  */


#include "spoj.h"

#include <QDebug>

Spoj::Spoj(QString record, int time)
{
    QStringList records = record.split(" ");

    this->time = time;
    this->busId = records[0].toInt();
    this->startTime = records[1];

    /* Rozdelenie startovacieho casu pre vypocet startovacieho casu v sekundach pre system */
    QStringList startTimeSplit = this->startTime.split(":");
    this->startTimeValue = startTimeSplit[0].toInt() * 3600 + startTimeSplit[1].toInt() * 60;

    if ( time >= this->startTimeValue ){
        /* Simulacia zacala v case, kedy autobus uz je na ceste */
        this->active = true;
    }
}

void Spoj::calculateEndTime(){
    int travelTime = 0;

    for ( const auto &road : this->connectionRoads ){
        travelTime += std::get<0>(road)->getLength() + std::get<0>(road)->getTraffic();
    }

    this->endTimeValue = travelTime + this->startTimeValue;

    /* Prevod sekund na "HH:MM" format */
    int hours = (int)(this->endTimeValue / 3600.0);
    int minutes = (int)((this->endTimeValue % 3600) / 60.0);

    this->endTime = QString::number(hours) + ":" + QString::number(minutes);
}

void Spoj::notify(){
    this->timeOnRoad = 0;

    if ( this->time >= this->startTimeValue && !this->active && this->time < this->endTimeValue){
        /* Spoj je na zaklade systemoveho casu spusteny */
        this->active = true;
        for ( const auto &road: this->connectionRoads ){
            this->timeOnRoad += std::get<0>(road)->getLength() + std::get<0>(road)->getTraffic();

            // Pozrie aktualnu poziciu na mape vzhladom na cestu
            // Z pozicie vzhaldom na cestu sa vypocita podla casu
            // posun na danej ceste
            if ( this->time <= this->startTimeValue + this->timeOnRoad ){
                this->actualPosition = road;
                this->calculateCoordinates();
                return;
            }
        }
    } else if ( this->active && this->time < this->endTimeValue ){
        /* Spoj vyhlada svoju aktualnu cestu, na ktorej sa podla casu nachadza */
        for ( const auto &road: this->connectionRoads ){
            this->timeOnRoad += std::get<0>(road)->getLength() + std::get<0>(road)->getTraffic();

            // Pozrie aktualnu poziciu na mape vzhladom na cestu
            // Z pozicie vzhaldom na cestu sa vypocita podla casu
            // posun na danej ceste
            if ( this->time <= this->startTimeValue + this->timeOnRoad ){
                this->actualPosition = road;
                this->calculateCoordinates();
                return;
            }
        }

    } else if ( this->active ){
        /* Spoj podla systemoveho casu skoncil svoju premavku */
        this->active = false;
    }
}

qreal Spoj::BusPositionOnRoad() {

    int timeOnActualRoad = this->time - (this->timeOnRoad + this->startTimeValue) + std::get<0>(this->actualPosition)->getLength() + std::get<0>(this->actualPosition)->getTraffic();
    int roadLength = std::get<0>(this->actualPosition)->getLength() + std::get<0>(this->actualPosition)->getTraffic();

    // vracia podiel prejdenej dlzky cesty delenej celkovou dlzkou cesty
    return timeOnActualRoad / (qreal)roadLength;
}

void Spoj::calculateCoordinates(){
    this->calculateEndTime();   // Prepocet koncoveho casu pri aktualizacii casu, v pripade novych dopravnych obmedzeni

    int timeOnActualRoad = (this->timeOnRoad + this->startTimeValue) - std::get<0>(this->actualPosition)->getLength() - std::get<0>(this->actualPosition)->getTraffic();

    Road *road = std::get<0>(this->actualPosition);
    int direction = std::get<1>(this->actualPosition);

    Point roadStart = road->getStart();
    Point roadEnd = road->getEnd();

    qreal dx = 0;
    qreal dy = 0;
    qreal roadLenght = road->getLength() + road->getTraffic();

    if ( roadStart.x != roadEnd.x && roadStart.y != roadEnd.y ){
        /* Pohyb po diagonale */
        if ( direction < 0 ){
            /* Pohyb po smere definovanej cesty */
            dx = (roadStart.x - roadEnd.x) / roadLenght;
            dx *= (this->time - timeOnActualRoad);

            dy = (roadStart.y - roadEnd.y) / roadLenght;
            dy *= (this->time - timeOnActualRoad);

        } else {
            /* Pohyb po opacnom smere definovanej cesty */
            dx = (roadEnd.x - roadStart.x) / roadLenght;
            dx *= (this->time - timeOnActualRoad);

            dy = (roadEnd.y - roadStart.y) / roadLenght;
            dy *= (this->time - timeOnActualRoad);
        }

    } else if ( roadStart.x != roadEnd.x ){
        /* Horizontalny pohyb */
        if ( direction < 0 ){
            /* Pohyb po smere definovanej cesty */
            dx = (roadStart.x - roadEnd.x) / roadLenght;
            dx *= (this->time - timeOnActualRoad);

        } else {
            /* Pohyb po opacnom smere definovanej cesty */
            dx = (roadEnd.x - roadStart.x) / roadLenght;
            dx *= (this->time - timeOnActualRoad);

        }

    } else {
        /* Vertikalny pohyb */
        if ( direction < 0 ){
            /* Pohyb po smere definovanej cesty */
            dy = (roadStart.y - roadEnd.y) / roadLenght;
            dy *= (this->time - timeOnActualRoad);
        } else {
            /* Pohyb po opacnom smere definovanej cesty */
            dy = (roadEnd.y - roadStart.y) / roadLenght;
            dy *= (this->time - timeOnActualRoad);

        }
    }

    if ( direction > 0 ){
        /* Pohyb po smere definovanej cesty => Zacina sa pocitat od definovaneho zaciatku cesty */
        this->x = std::get<0>(this->actualPosition)->getStart().x + dx;
        this->y = std::get<0>(this->actualPosition)->getStart().y + dy;
    } else {
        /* Pohyb po opacnom smere definovanej cesty => Zacina sa pocitat od definovaneho konca cesty */
        this->x = std::get<0>(this->actualPosition)->getEnd().x + dx;
        this->y = std::get<0>(this->actualPosition)->getEnd().y + dy;
    }
}

void Spoj::assignRoads(QVector<std::tuple<Road *, int>> roads){
    this->connectionRoads = roads;
    this->calculateEndTime();
}

QString Spoj::getSchedule(){
    return this->startTime + "-" + this->endTime;
}

bool Spoj::isActive(){
    return this->active;
}

bool Spoj::isSelected(){
    return this->selected;
}

void Spoj::setSelect(bool select){
    this->selected = select;
}

void Spoj::addColor(QColor connectionColor){
    this->color = connectionColor;
}

QColor Spoj::getColor(){
    return this->color;
}

QVector<std::tuple<QString, int>> Spoj::getStops(){
    QString pointName;
    int stopTime = this->startTimeValue;
    QVector<std::tuple<QString, int>> stops;

    for ( const auto &road: this->connectionRoads ){
        stopTime += std::get<0>(road)->getLength() + std::get<0>(road)->getTraffic();
        pointName = std::get<0>(road)->getEndName();
        if ( QRegExp("Z.*").exactMatch(pointName) ){
            stops.append(std::make_tuple(pointName, stopTime));
        }
    }


    return stops;
}

QVector<Road*> Spoj::getConnectionRoadsShort(){

    QVector<Road*> roads;

    for ( const auto &road : this->connectionRoads ){
        roads.append(std::get<0>(road));
    }

    return roads;
}

QVector<std::tuple<Road*, int>> Spoj::getConnectionRoads() {
    return this->connectionRoads;
}

Road* Spoj::getActualPosition() {
    return std::get<0>(this->actualPosition);
}

void Spoj::setTime(int time) {
    this->time = time;
}

int Spoj::getTime() {
    return this->time;
}

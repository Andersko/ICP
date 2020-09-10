/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Trieda Linka reprezentuje autobusovu linku
  */

#ifndef LINKA_H
#define LINKA_H

#include <QVector>
#include <QString>
#include <QColor>
#include "model/spoj.h"
#include "model/road.h"

using namespace std;

/**
 * @brief Trieda Linka predstavuje autobusovu Linku, ktora spravuje svoje spoje na mape
 */
class Linka
{
    /**
     * @brief busConnections Pole objektov reprezentujucich spoj pre danu linku
     */
    QVector <Spoj*>busConnections;

    /**
     * @brief roads Pole dvojic, ktore predstavuju cestu po ktorej sa dana linka pohybuje so smerom na danej ceste
     */
    QVector<std::tuple<Road*, int>> roads;

    /**
     * @brief beginStop Zaciatocna zastavka danej linky
     */
    QString beginStop;

    /**
     * @brief endStop Konecna zastavka danej linky
     */
    QString endStop;

    /**
     * @brief connectionColor Farba pre odlisenie liniek na mape
     */
    QColor connectionColor;
public:

    /**
     * @brief id Cislo danej linky
     */
    int id;

    /**
     * @brief middleStops vsetky zastavky, ktorymi dana linka prejde
     */
    QVector<QString> middleStops;

    /**
     * @brief routeWay Vsetky body na mape, ktorymi dana linka prechadza
     */
    QVector<QString> routeWay;

    /**
     * @brief Linka Konstruktor pre triedu Linka
     * @param record Zaznam danej linky zo vstupneho suboru
     */
    Linka(QString record);

    /**
     * @brief notify Notifikacna funkcia, ktora propaguje zmenu casu svojim spojom
     * @param time Aktualny systemovy cas
     */
    void notify(int time);

    /**
     * @brief addConnection Prida nove spojenie do linky
     * @param newSpoj Novy spoj pre danu linku
     */
    void addConnection(Spoj *newSpoj);

    /**
     * @brief addRoad Prida novu cestu do zoznamu ciest pre linku
     * @param newRoad Nova ceste so smerom pre danu linku
     */
    void addRoad(std::tuple<Road *,int> newRoad);

    /**
     * @brief getConnections Zisti vsetky spoje pre danu linku
     * @return Spoje ktore patri pod linku
     */
    QVector<Spoj*> getConnections();

    /**
     * @brief getRoads Zisti vsetky cesty, po ktorych dana linka prechadza
     * @return Dvojicu cesta a smer na ceste na danej linke
     */
    QVector<std::tuple<Road*, int>> getRoads();

    /**
     * @brief getSchedule Informuje o rozpise spojov danej linky
     * @return Pole o dvojiciach casu a spoju
     */
    QVector<std::tuple<QString, Spoj*>> getSchedule();

    /**
     * @brief getActiveBuses Zisti aktivne autobusy, ktore su na cestach na danej linke
     * @return Pole aktivnych autobusov na danej linke
     */
    QVector<Spoj*> getActiveBuses();

    /**
     * @brief getBeginStop Oznami nazov svojej pociatocnej zastavky
     * @return Nazov pociatocnej zastavky
     */
    QString getBeginStop();

    /**
     * @brief getEndStop Oznami nazov svojej konecnej zastavky
     * @return Nazov konecne zastavky
     */
    QString getEndStop();

    /**
     * @brief getColor Vrati hodnotu farby pre danu linku
     * @return QColor farba linky
     */
    QColor getColor();
};

#endif // LINKA_H

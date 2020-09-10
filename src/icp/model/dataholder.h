/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Trieda reprezentujuca model z MVC architektury. Poskytuje vsetky potrebne data k fungovani simulacii autobusovej dopravy
  */

#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include "model/spoj.h"
#include "model/linka.h"
#include "model/road.h"

#include <map>
#include <QFile>
#include <QDebug>

/**
 * @brief Provides data from model about all busses, its routes, times etc.
 */
class DataHolder
{
    /** @brief linky Maps id of connections into class representation of bus connection */
    map <int, Linka*> linky;

    /** @brief Roads List of all roads displayed on map*/
    vector <Road*> roads;
public:

    /**
     * @brief DataHolder Konstruktor trieda DataHolder
     * @param startingTime Zacinajuci cas simulacie
     */
    DataHolder(int startingTime);

    /**
     * @brief Najde a vrati objekt reprezentujuci cesty na zaklade 2 bodov, ktore urcuju smer cesty
     * @param from Nazov bodu/zastavky, z ktorej dany spoj vychadza
     * @param to Nazov bodu/zastavky, do ktorej dany spoj vchadza
     * @return dvojicu <cesta, smer_cesty>.
     */
    std::tuple<Road*, int> findRoute(QString from, QString to);

    /**
     * @brief matchRoads Spoji cesty na mape s cestami, po ktorych je pohyb liniek.
     */
    void matchRoads();

    /**
     * @brief timeChanged Funkcia, ktora upozorni na zmenu casu vsetky potrebne casti dat, ktore po zmene casu zmenia svoj stav
     * @param time Aktualny cas
     */
    void timeChanged(int time);

    /**
     * @brief getAllRoads Spristupni vsetky cesty pre mapovy podklad
     * @return Pole objektov reprezentujucich cesty
     */
    QVector<Road*> getAllRoads();

    /**
     * @brief getActiveBuses Najde vsetky aktivne spoje, ktore premavaju na mape
     * @return Vsetky aktivne spoje na mape
     */
    QVector<Spoj*> getActiveBuses();

    /**
     * @brief getConnection Najde autobusovy linku podla ID
     * @param id ID hladanej linky
     * @return Linka so vstupnym ID
     */
    Linka *getConnection(int id);

    /**
     * @brief getAllConnections Preda vsetky linky na mape
     * @return Vsetky linky na mape
     */
    QVector<Linka*> getAllConnections();
};

#endif // DATAHOLDER_H

/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Implementacia triedy DataHolder reprezentuje model v MVC
  */

#include "dataholder.h"

DataHolder::DataHolder(int startingTime)
{

    /* Subory v Qt resources, ktore obsahuju linky, cesty a spoje v specifikovanej forme */
    QFile fileLinky(":/text_files/txt_files/linky.txt");
    QFile fileSpoje(":/text_files/txt_files/spoje.txt");
    QFile fileCesty(":/text_files/txt_files/cesty.txt");

    QByteArray line;    // Riadok pre citanie zo suboru
    Linka *newLinka;
    Spoj *newSpoj;
    Road *newRoad;

    if ( !fileLinky.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        qDebug() << "Cant open file";
        return;
    }

    if ( !fileSpoje.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        qDebug() << "Cant open file";
        return;
    }

    if ( !fileCesty.open( QIODevice::ReadOnly | QIODevice::Text ) ){
        qDebug() << "cant open Cesty";
        return;
    }

    while ( !fileLinky.atEnd() ){
        line = fileLinky.readLine();

        /* Potrebne odstaranit newline z QByteArray */
        newLinka = new Linka(QString(line).remove(QRegExp("\\n")));

        this->linky[newLinka->id] = newLinka;
    }

    while ( !fileSpoje.atEnd() ){
        line = fileSpoje.readLine();
        newSpoj = new Spoj(QString(line).remove(QRegExp("\\n")), startingTime);

        this->linky[newSpoj->busId]->addConnection(newSpoj);
    }

    while ( !fileCesty.atEnd() ){
        line = fileCesty.readLine();
        newRoad = new Road(QString(line));

        this->roads.push_back(newRoad);
    }


    fileLinky.close();
    fileSpoje.close();
    fileCesty.close();

    this->matchRoads();
}

std::tuple<Road *, int> DataHolder::findRoute(QString from, QString to){
    QString start;
    QString end;

    for ( unsigned int i = 0; i < this->roads.size(); i++ ){
        start = this->roads[i]->getStartName();
        end = this->roads[i]->getEndName();

        if ( start == from && end == to ){
            /* Hladana cesta je v smere, v ktorom bola definovana v subore */
            return std::make_tuple(this->roads[i], 1);
        } else if ( start == to && end == from ){
            /* Hladana cesta je v opacnom smere, v akom bola definovana v subore */
            return std::make_tuple(this->roads[i], -1);
        }
    }
}

void DataHolder::matchRoads(){
    std::tuple<Road*, int> road;
    Linka *linka;

    for( auto const &linkaPair : this->linky ){
        linka = linkaPair.second;

        for ( QVector<QString>::iterator iter = linka->routeWay.begin(), prev = linka->routeWay.end(); iter != linka->routeWay.end(); prev = iter, ++iter ){
            /* prev obsahuje predchadzajuci bod cesty z danej linky */

            if ( iter == linka->routeWay.begin() ){
                /* Priradovanie ciest zacina az od druheho bodu cesty danej linky */
                continue;
            } else {

                /* Najde cestu z danych bodov a priradi ju danej linkej do ktorej patri */
                road = this->findRoute(*prev, *iter);
                linka->addRoad(road);
            }
        }


        for ( const auto &spoj : linka->getConnections() ){
            /* Priradi spojom danej linky cestu po ktorej sa budu pohybovat */
            spoj->assignRoads(linka->getRoads());
        }
    }
}

void DataHolder::timeChanged(int time){
    /* Upozornenie na zmenu casu simulacie */

    for(auto const &linka: this->linky ){
        linka.second->notify(time);
    }
}

QVector<Road*> DataHolder::getAllRoads(){
    QVector<Road*> roads = QVector<Road*>::fromStdVector(this->roads);

    if ( roads[0] == nullptr ){
        qDebug() << "NULL";
    }

    return roads;
}

QVector<Spoj*> DataHolder::getActiveBuses(){
    QVector<Spoj*> allActiveBuses;
    QVector<Spoj*> activeLinkaBuses;

    for ( auto const &linka: this->linky ){
        activeLinkaBuses = (linka.second)->getActiveBuses();

        for ( auto const &spoj: activeLinkaBuses ){
            allActiveBuses.append(spoj);
        }
    }

    return allActiveBuses;
}

Linka *DataHolder::getConnection(int id){
    return this->linky[id];
}

QVector<Linka*> DataHolder::getAllConnections(){
    QVector<Linka*> connections;

    for ( const auto &linka: this->linky){
        connections.append(linka.second);
    }

    return connections;
}






























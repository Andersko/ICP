/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Definicia triedy Spoj a jej metod.
  */


#ifndef SPOJ_H
#define SPOJ_H

#include <QVector>
#include <QString>
#include <tuple>
#include <QColor>

#include "model/road.h"

/**
 * @brief The Spoj class Trieda reprezentujuca spoj na mape.
 */
class Spoj
{
    /**
     * @brief timeOnRoad Cas, ktory je dany spoj na ceste
     */
    int timeOnRoad = 0;

    /**
     * @brief active Indikator aktivnosti spoju
     */
    bool active = false;

    /**
     * @brief selected Indikator oznacenia spoju na mape
     */
    bool selected = false;

    /**
     * @brief color Farba linky ktora sa vykresli spoj na mape
     */
    QColor color;
public:
    /**
     * @brief x Aktualna pozicia na X osi na mape
     */
    int x = 0;

    /**
     * @brief y Aktualna pozicia na Y osi na mape
     */
    int y = 0;

    /**
     * @brief time Systemovy cas pritomny pri poslednej aktualizacii
     */
    int time;

    /**
     * @brief Spoj Konstruktor, ktory zo zaznamu priradi vlastnosti daneho spoju
     * @param record Zaznam zo suboru o danom spoji
     * @param time Aktualny systemovy cas
     */
    Spoj(QString record, int time);

    /**
     * @brief notify Notifikacia pri zmene casu o vypocitani novych udajov
     */
    void notify();

    /**
     * @brief BusPositionOnRoad Vypocita prejdenu cast aktualnej cesty spoju v percentach v tvare desatinneho cisla v rozmedzi 0-1
     * @return prejdeny usok cesty
     */
    qreal BusPositionOnRoad ();

    // testing purpose scope
    /**
     * @brief busId ID linky po ktorej dany spoj premava
     */
    int busId;

    /**
     * @brief startTime Startovaci cas v tvar "HH:MM"
     */
    QString startTime;

    /**
     * @brief startTimeValue Startovaci cas v sekundach pre systemovy cas
     */
    int startTimeValue;

    /**
     * @brief endTime Cas konca spoju v tvare "HH:MM"
     */
    QString endTime;

    /**
     * @brief endTimeValue Cas konca spoju v sekundach pre systemovy cas
     */
    int endTimeValue;

    /**
     * @brief actualPosition Aktualna cesta na ktorej sa dany spoj nachadza, sluzi pre vypocet pozicie v zavislosti na case
     */
    std::tuple<Road*,int> actualPosition;

    /**
     * @brief connectionRoads Cesty po ktorych dany spoj premava so smerom na danych cestach
     */
    QVector<std::tuple<Road*, int>> connectionRoads;

    /**
     * @brief getConnectionRoadsShort Ziska cesty po ktorych dany spoj premava bez definovaneho smeru na danych cestach
     * @return Cesty po ktorych dany spoj premava bez definovaneho smeru na danych cestach
     */
    QVector<Road*> getConnectionRoadsShort();

    /**
     * @brief getConnectionRoads Ziska cesty po ktorych dany spoj premava
     * @return Cesty po ktorych dany spoj premava
     */
    QVector<std::tuple<Road*, int>> getConnectionRoads();

    /**
     * @brief getActualPosition Ziska aktualnu cestu na ktorej sa spoj nachadza
     * @return Cesta na ktorej sa spoj nachadza
     */
    Road* getActualPosition();

    /**
     * @brief calculateCoordinates Vypocet suradnic na zaklade aktualneho casu
     */
    void calculateCoordinates();

    /**
     * @brief assignRoads Priradi cesty z linky ku spoju
     * @param roads Cesty danej linky po ktorych bude spoj premavat
     */
    void assignRoads(QVector<std::tuple<Road*, int>> roads);

    /**
     * @brief calculateEndTime Vypocet koncoveho casu daneho spoja na zaklade dlzky cesty spolu so zdrzanim
     */
    void calculateEndTime();

    /**
     * @brief getSchedule Informuje o casovom rozpise daneho spoju
     * @return Casovu reprezentaciu spoju v tvare "HH:MM - HH:MM"
     */
    QString getSchedule();

    /**
     * @brief isActive Zistenie aktivnosti spoja
     * @return Hodnotu aktivnosti daneho spoja
     */
    bool isActive();

    /**
     * @brief setSelect Nastavi hodnotu oznacenia na mape
     * @param select Hodnota oznacenia
     */
    void setSelect(bool select);

    /**
     * @brief isSelected Informuje o stave oznacenia
     * @return Hodnotu oznacenia
     */
    bool isSelected();

    /**
     * @brief addcolor Prida farbu linky pre vykreslenie
     * @param connectionColor Farba otcovskej linky
     */
    void addColor(QColor connectionColor);

    /**
     * @brief getColor Oznami farbu otcovskej linky
     * @return Farba linky
     */
    QColor getColor();

    /**
     * @brief setTime Ulozi cas z poslednej aktualizacie spoju na mape
     * @param time Aktualny systemovy cas
     */
    void setTime(int time);

    /**
     * @brief getTime Ziska cas z poslednej aktualizacie spoju
     * @return cas z poslednej aktualizacie spoju
     */
    int getTime();

    /**
     * @brief getStops Ziska zastavky
     * @return zastavky
     */
    QVector<std::tuple<QString,int>> getStops();

    /**
     * @brief getRoads Ziska cesty
     * @return cesty
     */
    QVector<Road *> getRoads();
};

#endif // SPOJ_H

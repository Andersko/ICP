/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Definicia triedy Road a jej metod.
  */

#ifndef ROAD_H
#define ROAD_H

#include <QString>
#include <tuple>


/** Default length of road */
#define LENGTH_CONVERTOR 1

/**
 * @brief The Point struct Struktura reprezentujuca bod na mape
 */
struct Point{
    /**
     * @brief name Meno daneho bodu
     */
    QString name;

    /**
     * @brief x Pozicia na ose X
     */
    int x;

    /**
     * @brief y Pozicia na ose Y
     */
    int y;
};

/**
 * @brief Trieda reprezentujuca cestu na mape. Sklada sa z 2 bodov (zaciatok, koniec)
 */
class Road
{
    /**
     * @brief start Bod, ktorym dana cesta zacina
     */
    Point start;

    /**
     * @brief end Bod, ktorym dana cesta konci
     */
    Point end;

    /**
     * @brief traffic Modifikator stupna dopravnej premavky na danej ceste ( vytvaranie meskani )
     */
    float traffic = 0;

    /**
     * @brief length Dlzka cesty v casovych jednotkach ( sekundy )
     */
    int length;


    /**
     * @brief selected Indikator oznacenia cesty na mape
     */
    bool selected = false;

    /**
     * @brief roadClose informacia o tom ci (a na kolkych miestach) cesta nie je uzavreta ci nie je pouzita ako obchadzka,
     */
    std::tuple<int, int> roadClose;

public:
    /**
     * @brief Road Konstruktor pre triedu Road
     * @param record Zaznam o ceste zo vstupneho suboru
     */
    Road(QString record);

    /**
     * @brief getStartName Spristupni nazov zaciatocneho bodu
     * @return Meno zaciatocneho bodu
     */
    QString getStartName();

    /**
     * @brief getEndName Spristupni nazov konecneho bodu
     * @return Meno konecneho bodu
     */
    QString getEndName();

    /**
     * @brief getStart Spristupni zaciatocny bod cesty
     * @return Zaciatocny bod cesty
     */
    Point getStart();

    /**
     * @brief getEnd Spristupni konecny body cesty
     * @return Konecny bod cesty
     */
    Point getEnd();

    /**
     * @brief getName
     * @return
     */
    QString getName();

    /**
     * @brief setTraffic
     * @param delay
     */
    void setTraffic(float delay);

    /**
     * @brief getTraffic
     * @return
     */
    int getTraffic();

    /**
     * @brief getLength
     * @return
     */
    int getLength();

    /**
     * @brief setSelect Nastavi hodnotu oznacenia cesty na mape
     * @param selected Hodnota oznacenia
     */
    void setSelect(bool selected);

    /**
     * @brief isSelected Oznami hodnotu oznacenia cesty
     * @return Boolean hodnota oznacenia
     */
    bool isSelected();

    /**
     * @brief setRoadClose Nastavi hodnoty uzavretia a obchadzky cesty
     */
    void setRoadClose(std::tuple<int, int> roadClose);

    /**
     * @brief getRoadClose Vrati hodnoty uzavretia a obchadzky cesty
     * @return Hodnoty uzavretia a obchadzky cesty
     */
    std::tuple<int, int> getRoadClose();
};

#endif // ROAD_H

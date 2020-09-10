/**
  * @file
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Trieda ItineraryScene reprezentujuca cast okna aplikacie zobrazujuceho itinerator spoju
  */

#ifndef ITINERARYSCENE_H
#define ITINERARYSCENE_H

#include <QGraphicsScene>
#include "model/spoj.h"

/**
 * @brief The ItineraryScene class implements graphics scene for itinerary
 */
class ItineraryScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * @brief ItineraryScene Konstruktor pre triedu ItineraryScene
     * @param parent Parent
     */
    explicit ItineraryScene(QObject *parent = 0);

public slots:

    /**
     * @brief update updates and shows itinerary
     * @param bus seleted bus
     */
    void update(Spoj* bus);

    /**
     * @brief ditch ditch itinerary
     */
    void ditch();
};

#endif // ITINERARYSCENE_H

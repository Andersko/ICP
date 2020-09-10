/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Trieda ConnectionItem zobrazujuca udaje o zvolenej polozke
  */

#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QWidget>
#include "model/linka.h"

namespace Ui {
class ConnectionItem;
}

/**
 * @brief The ConnectionItem class Implementuje kontroler pre rozhranie spoju na mape.
 */
class ConnectionItem : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief ConnectionItem Konstruktor triedy ConnectionItem
     * @param parent Rodic zdedenej triedy
     */
    explicit ConnectionItem(QWidget *parent = 0);
    ~ConnectionItem();

    /**
     * @brief setData Sets data from representing class to UI labels as Id, stops, color
     * @param linka Class holding data about bus on map
     */
    void setData(Linka *linka);

    /**
     * @brief setData
     * @param stop
     * @param time
     */
    void setData(QString stop, QString time);

private:
    Ui::ConnectionItem *ui;

    /**
     * @brief data Class holding data about bus.
     */
    Linka *data;
};

#endif // CONNECTIONITEM_H

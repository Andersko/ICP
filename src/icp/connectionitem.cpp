/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  *
  * Implementacia triedy ConnectionItem zobrazujucej udaje o zvolenej polozke
  */


#include "connectionitem.h"
#include "ui_connectionitem.h"

ConnectionItem::ConnectionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionItem)
{
    ui->setupUi(this);
}

ConnectionItem::~ConnectionItem()
{
    delete ui;
}

void ConnectionItem::setData(Linka *linka){
    this->data = linka;

    ui->connectionId->setText("Linka #" + QString::number(this->data->id));
    ui->connectionId->setStyleSheet("color: " + linka->getColor().name() + ";");
    ui->ConnectionStops->setText(this->data->getBeginStop() + "-" + this->data->getEndStop());
}

void ConnectionItem::setData(QString stop, QString time){
    ui->connectionId->setText(stop);
    ui->ConnectionStops->setText(time);
}

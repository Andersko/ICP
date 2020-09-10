/**
  * @file
  * @author Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
  * @author Andrej Pavlovic <xpavlo14@stud.fit.vutbr.cz>
  *
  * Implementacia triedy MainWindow reprezentujuca okno aplikacie (styl zobrazenia, poloha a vlastnosti jednotlivych grafickych casti aplikacie, ...)
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model/dataholder.h"
#include "connectionitem.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

/** Default starting time in seconds */
#define STARTING_TIME 34200

/** Speed in seconds as normal speed */
#define NORMAL_SPEED 1000

/** Speed in seconds as 2x speed */
#define TWO_TIME_SPEED 500

/** Speed in seconds as 3x speed */
#define THREE_TIME_SPEED 250

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ui set up
    ui->setupUi(this);
    this->time = STARTING_TIME;


    // graphics scenes and timer set up
    this->timer = new QTimer(this);
    this->map = ui->map;
    this->itinerary = ui->itinerary;
    this->sceneMap = new MapScene(this->map);
    this->map->setScene(this->sceneMap);
    this->sceneItinerary = new ItineraryScene(this->itinerary);
    this->itinerary->setScene(this->sceneItinerary);


    // widgeds set up
    ui->trafficLevelText1->hide();
    ui->trafficLevelText2->hide();
    ui->trafficLevel->hide();
    ui->roadCloseText->hide();
    ui->roadCloseCancel->hide();
    ui->roadCloseCancel->setStyleSheet("QLabel { color : red; }");
    ui->roadClose->hide();
    actualSpeed = ui->oneTimeSpeed;


    // all signals and slots connetcions
    connect(timer, &QTimer::timeout, this, &MainWindow::notify);

    connect(sceneMap, &MapScene::mapClicked, sceneItinerary, &ItineraryScene::ditch);
    connect(sceneMap, &MapScene::mapClicked, this, &MainWindow::processMapClick);
    connect(sceneMap, &MapScene::mapClicked, ui->trafficLevel, &QLineEdit::hide);
    connect(sceneMap, &MapScene::mapClicked, ui->trafficLevelText1, &QLabel::hide);
    connect(sceneMap, &MapScene::mapClicked, ui->trafficLevelText2, &QLabel::hide);
    connect(sceneMap, &MapScene::mapClicked, ui->roadClose, &QPushButton::hide);
    connect(sceneMap, &MapScene::mapClicked, ui->roadCloseText, &QLabel::hide);
    connect(sceneMap, &MapScene::mapClicked, ui->roadCloseCancel, &QLabel::hide);

    connect(sceneMap, &MapScene::roadClicked, sceneItinerary, &ItineraryScene::ditch);
    connect(sceneMap, &MapScene::roadClicked, ui->trafficLevel, &QLineEdit::show);
    connect(sceneMap, &MapScene::roadClicked, ui->trafficLevel, &QLineEdit::setText);
    connect(sceneMap, &MapScene::roadClicked, ui->trafficLevelText1, &QLabel::show);
    connect(sceneMap, &MapScene::roadClicked, ui->trafficLevelText2, &QLabel::show);
    connect(sceneMap, &MapScene::roadClicked, ui->roadClose, &QPushButton::show);
    connect(sceneMap, &MapScene::roadClicked, ui->roadCloseText, &QLabel::hide);
    connect(sceneMap, &MapScene::roadClicked, ui->roadCloseCancel, &QLabel::hide);

    connect(sceneMap, &MapScene::roadClickedClosed, sceneItinerary, &ItineraryScene::ditch);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->trafficLevel, &QLineEdit::hide);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->trafficLevelText1, &QLabel::hide);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->trafficLevelText2, &QLabel::hide);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->roadClose, &QLabel::hide);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->roadCloseText, &QLabel::hide);
    connect(sceneMap, &MapScene::roadClickedClosed, ui->roadCloseCancel, &QPushButton::show);

    connect(sceneMap, &MapScene::roadClickedDetour, ui->roadClose, &QPushButton::hide);

    connect(sceneMap, &MapScene::busClicked, sceneItinerary, &ItineraryScene::update);
    connect(sceneMap, &MapScene::busClicked, this, &MainWindow::processBusClick);
    connect(sceneMap, &MapScene::busClicked, ui->trafficLevel, &QLineEdit::hide);
    connect(sceneMap, &MapScene::busClicked, ui->trafficLevelText1, &QLabel::hide);
    connect(sceneMap, &MapScene::busClicked, ui->trafficLevelText2, &QLabel::hide);
    connect(sceneMap, &MapScene::busClicked, ui->roadClose, &QPushButton::hide);
    connect(sceneMap, &MapScene::busClicked, ui->roadCloseText, &QLabel::hide);
    connect(sceneMap, &MapScene::busClicked, ui->roadCloseCancel, &QLabel::hide);

    connect(sceneMap, &MapScene::cancelRoadCloseMode, this, &MainWindow::roadClose);
    connect(sceneMap, &MapScene::cancelRoadCloseMode, ui->trafficLevel, &QLineEdit::hide);
    connect(sceneMap, &MapScene::cancelRoadCloseMode, ui->trafficLevelText1, &QLabel::hide);
    connect(sceneMap, &MapScene::cancelRoadCloseMode, ui->trafficLevelText2, &QLabel::hide);
    connect(sceneMap, &MapScene::cancelRoadCloseMode, ui->roadCloseText, &QPushButton::hide);
    connect(sceneMap, &MapScene::cancelRoadCloseMode, ui->roadCloseCancel, &QPushButton::hide);

    connect(ui->roadClose, &QPushButton::clicked, this, &MainWindow::roadClose);
    connect(ui->roadClose, &QPushButton::clicked, sceneMap, &MapScene::roadClose);
    connect(ui->roadClose, &QPushButton::clicked, ui->trafficLevel, &QPushButton::hide);
    connect(ui->roadClose, &QPushButton::clicked, ui->trafficLevelText1, &QLabel::hide);
    connect(ui->roadClose, &QPushButton::clicked, ui->trafficLevelText2, &QLabel::hide);

    connect(ui->roadCloseCancel, &QPushButton::clicked, sceneMap, &MapScene::roadCloseCancel);
    connect(ui->roadCloseCancel, &QPushButton::clicked, ui->roadCloseCancel, &QPushButton::hide);

    connect(sceneMap, &MapScene::throwErrorForUser, this, &MainWindow::throwErrorForUser);
    connect(ui->trafficLevel, &QLineEdit::textChanged, this, &MainWindow::trafficChanged);
    connect(ui->oneTimeSpeed, &QPushButton::clicked, this, &MainWindow::changeSpeed);
    connect(ui->twoTimeSpeed, &QPushButton::clicked, this, &MainWindow::changeSpeed);
    connect(ui->threeTimeSpeed, &QPushButton::clicked, this, &MainWindow::changeSpeed);
    connect(ui->resetTime, &QPushButton::clicked, this, &MainWindow::resetTime);
    connect(ui->zoomIn, &QPushButton::clicked, this, &MainWindow::zoomIn);
    connect(ui->zoomOut, &QPushButton::clicked, this, &MainWindow::zoomOut);


    // timer start
    timer->start(this->timeOffset * 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModel(DataHolder *model){

    this->model = model;
    this->sceneMap->init(this->model->getAllRoads(), this->model);
    this->setListData();
}

void MainWindow::setListData(){
    QVector <Linka*> connections = this->model->getAllConnections();

    for ( const auto &linka: connections ){
        QListWidgetItem *item =  new QListWidgetItem();
        item->setSizeHint(QSize(100,50));
        ConnectionItem *connectionItem = new ConnectionItem();
        connectionItem->setData(linka);

        ui->linkyList->addItem(item);
        ui->linkyList->setItemWidget(item, connectionItem);
    }
}

void MainWindow::notify(){
    this->time += this->timeOffset;
    this->model->timeChanged(this->time);
    this->setTime();

    QVector<Spoj*> active = this->model->getActiveBuses();

    this->sceneMap->setActiveBuses(active);
    this->sceneMap->updateMap();

    // find selected bus
    for ( auto bus : active ) {
        if ( bus->isSelected() ) {
            this->sceneItinerary->update(bus);
            break;
        }
    }
}

void MainWindow::resetTime() {
    this->time = STARTING_TIME;
    notify();
}

void MainWindow::zoomIn() {
   map->scale(1.25,1.25);
}

void MainWindow::zoomOut() {
   map->scale(0.8,0.8);
}

void MainWindow::processMapClick(){
    ui->linkyList->clear();
    this->setListData();
}

void MainWindow::processBusClick(Spoj *bus){
    this->showBusStops(bus);
}

void MainWindow::changeSpeed(){
    QPushButton *btn = qobject_cast<QPushButton*>(QObject::sender());

    QPalette palette = this->actualSpeed->palette();
    palette.setColor(QPalette::Button, QColor(Qt::white));
    this->actualSpeed->setAutoFillBackground(true);
    this->actualSpeed->setPalette(palette);
    this->actualSpeed->update();

    if ( btn->text() == "1x" ){
        this->actualSpeed = btn;
        this->timer->setInterval(NORMAL_SPEED);
    } else if ( btn->text() == "2x" ){
        this->actualSpeed = btn;
        this->timer->setInterval(TWO_TIME_SPEED);
    } else {
        this->actualSpeed = btn;
        this->timer->setInterval(THREE_TIME_SPEED);
    }

    palette = this->actualSpeed->palette();
        palette.setColor(QPalette::Button, QColor(Qt::red));
        this->actualSpeed->setAutoFillBackground(true);
        this->actualSpeed->setPalette(palette);
        this->actualSpeed->update();
}

void MainWindow::setTime(){
    QString hours = QString::number((int)(this->time / 3600));
    int minutesValue = (int)((this->time % 3600) / 60);
    QString minutes = minutesValue < 10 ? "0" + QString::number(minutesValue) : QString::number(minutesValue);

    ui->hourLabel->setText(hours + ":" + minutes);
}

void MainWindow::showBusStops(Spoj *connection){
    QVector<std::tuple<QString, int>> stops = connection->getStops();
    int hours;
    int minutes;
    QString minutesString;

    QVector<std::tuple<QString, QString>> stopsWithTime;

    for ( const auto &stop: stops ){
        hours = (int)(std::get<1>(stop) / 3600);
        minutes = (int)((std::get<1>(stop) % 3600) / 60);

        minutesString = minutes < 10 ? ( "0" + QString::number(minutes) ) : QString::number(minutes);
        stopsWithTime.append(std::make_tuple(std::get<0>(stop), QString::number(hours) + ":" + minutesString));
    }


    this->replaceModelWithStops(stopsWithTime);
}

void MainWindow::replaceModelWithStops(QVector<std::tuple<QString, QString>> stops){
    ui->linkyList->clear();

    for ( const auto &stop: stops ){
        QListWidgetItem *item =  new QListWidgetItem();
        item->setSizeHint(QSize(100,50));
        ConnectionItem *connectionItem = new ConnectionItem();
        connectionItem->setData(std::get<0>(stop), std::get<1>(stop));

        ui->linkyList->addItem(item);
        ui->linkyList->setItemWidget(item, connectionItem);
    }
}

void MainWindow::trafficChanged(QString text) {
    for (auto road : this->model->getAllRoads()) {
        if ( road->isSelected() ) {
            road->setTraffic(text.toFloat());
            break;
        }
    }
}

void MainWindow::roadClose() {

    if ( sceneMap->getMode() == NORMAL ) {

        sceneMap->setMode(CLOSED);
        ui->roadCloseText->setStyleSheet("QLabel { color : red; }");
        ui->roadCloseText->setText("Vyberte uzavrete cesty");
        ui->roadCloseText->setVisible(true);
        ui->roadClose->setText("Potvrdit vyber");
    }

    else if ( sceneMap->getMode() == CLOSED ) {
        sceneMap->setMode(DETOUR);
        ui->roadCloseText->setStyleSheet("QLabel { color : green; }");
        ui->roadCloseText->setText("Vyberte obchadzkove cesty");
        ui->roadCloseText->setVisible(true);
        ui->roadClose->setText("Potvrdit vyber");
    }

    else if ( sceneMap->getMode() == DETOUR ) {
        sceneMap->setMode(NORMAL);
        ui->roadCloseText->setVisible(false);
        ui->roadClose->setText("Uzavriet cestu");
        ui->roadClose->setVisible(false);
    }
}

void MainWindow::throwErrorForUser(QString msg) {
    ui->roadCloseText->setText(msg);
    ui->roadCloseText->setStyleSheet("QLabel { color : red; }");
    ui->roadCloseText->setVisible(true);
}

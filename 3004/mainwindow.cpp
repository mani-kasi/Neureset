#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //device on by default
    power = true;

    //set up rest of vars
    menuIndex = 0;
    battery = 100;
    charging = false;
    auxPlug = true;
    timer = 29;
    progressValue = 0;
    pause = false;
    stop = false;
    play = false;

    //connect all the slots
    ui->setupUi(this);
    device = new Device();
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::Power);
    connect(ui->charger, &QPushButton::clicked, this, &MainWindow::Charge);
    connect(ui->auxcord, &QPushButton::clicked, this, &MainWindow::Aux);
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::Start);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::Pause);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::Stop);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::MenuButton);
    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::MenuUp);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::MenuDown);
    connect(ui->selectButton, &QPushButton::clicked, this, &MainWindow::MenuEnter);

    connect(device, SIGNAL(sendBlueLightSignal()), this, SLOT (BlueLight()));
    connect(device, SIGNAL(sendGreenLightSignal()), this, SLOT (GreenLight()));
    connect(device, SIGNAL(sendRedLightSignal()), this, SLOT (RedLight()));
    connect(device, SIGNAL(updateSessionLogs()), this, SLOT (SessionLogs()));
    connect(device, SIGNAL(sendProgress()), this, SLOT (updateProgress()));
    connect(device, SIGNAL(progressDone()), this, SLOT (progressComplete()));


    connect(ui->timeEdit, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    QTimer* timeUpdate = new QTimer(this);
    connect(timeUpdate, &QTimer::timeout, this, &MainWindow::updateTime);
    timeUpdate->start(1000);

    //init the menu labels (options) to access them easier
    menuLabels[0] = ui->newSessionLabel;
    menuLabels[1] = ui->sessionLogLabel;
    menuLabels[2] = ui->dateLabel;
    menuLabels[3] = ui->testLabel;

    //shouldnt be able to press pause or stop before session starts
    ui->stopButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
}






MainWindow::~MainWindow()
{
    delete ui;
}

//LIGHTS - looks like device will do this.
//turns red light on or off
void MainWindow::RedLight(){
    QWidget *redWidget = ui->redWidget;
    if(redWidget->palette().color(QPalette::Background) == Qt::black){
        redWidget->setStyleSheet("background-color: red;");
    }
    else{
        redWidget->setStyleSheet("background-color: black;");
    }
}

//turns blue light on or off
void MainWindow::BlueLight(){
    QWidget *blueWidget = ui->blueWidget;
    if(blueWidget->palette().color(QPalette::Background) == Qt::black){
        blueWidget->setStyleSheet("background-color: blue;");
    }
    else{
        blueWidget->setStyleSheet("background-color: black;");
    }
}


//turns green light on or off
void MainWindow::GreenLight(){
    QWidget *greenWidget = ui->greenWidget;
    if(greenWidget->palette().color(QPalette::Background) == Qt::black){
        greenWidget->setStyleSheet("background-color: green;");
    }
    else{
        greenWidget->setStyleSheet("background-color: black;");
    }
}


//MENU OPTION FUNCTIONS
//starts a new session, pass in current date.
void MainWindow::NewSession(QDateTime const &dateTime){
    play = true;
    ui->stackedWidget->setCurrentWidget(ui->page_2);

        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->menuButton->setEnabled(false);

        ui->pauseButton->setEnabled(true);
        ui->stopButton->setEnabled(true);

    device->newSession(dateTime);
}


//display the session logs, get the session logs from device, loop through them to show them all, remember index of the one user chooses to open that one on pc.
//will get called everytime a new session is done.
void MainWindow::SessionLogs(){
    //enable all the buttons that were disabled when the a new session was started
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->selectButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->menuButton->setEnabled(true);

    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    Session** sessions = device->getSessions();
    QStringList stringList;

    for (int i = 0; sessions[i] != NULL; i++){
        QDateTime startTime = sessions[i]->getStartTime();
        stringList << startTime.toString();
    }

    QStringListModel* model = new QStringListModel(stringList, this);

    ui->listView->setModel(model);
}


//DISPLAY PC
//display a session's log on the pc screen, shows date of session, and
void MainWindow::PCScreen(Session* session){
    ui->pcTitle1->setText("Start Date:");
    ui->pcTitle2->setText("End Date:");
    ui->pcTitle3->setText("Overall Baseline Start:");
    ui->pcTitle4->setText("Overall Baseline End:");
    ui->pcSessionTitle->setText("Session Data:");


    ui->pcStart->setText(session->getStartTime().toString());
    ui->pcEnd->setText(session->getEndTime().toString());
    ui->pcBaselineStart->setText(QString::number(session->getOverallBaselineStart()));
    ui->pcBaselineEnd->setText(QString::number(session->getOverallBaselineEnd()));
}

void MainWindow::chooseElectrode(){
    QStringList stringList;

    for (int i = 0; i != NUM_ELECTRODES; i++){
        stringList << QString::number(i+1);

}
    QStringListModel* model = new QStringListModel(stringList, this);

    ui->listView_2->setModel(model);
}

//uncomment after adding getElectrodes() function and plotWidget to page_7 on ui

//void MainWindow::updateTestUi(int id){

//    qInfo("PLOT GRAPH");

//    QVector<Electrode*>* electrodes = device->getElectrodes();
//    Electrode* electrode = (*electrodes)[id];

//    QVector<double> treatmentData = electrode->getTreatmentData();
//    QVector<double> dominantWaveform = electrode->getBaselineWaveform();


//    QVector<double> x(treatmentData.size());
//    for (int i = 0; i < x.size(); ++i) {
//        x[i] = i / 128.0;
//    }
//    //This plots  both but I think we will only plot treated waveform?
//    // Set up the custom plot
//    QCustomPlot *customPlot = ui->plotWidget;
//    QFont legendFont = customPlot->legend->font();
//    legendFont.setPointSize(8); // Set a smaller font size (default is usually 9 or 10)
//    customPlot->legend->setFont(legendFont);

//    customPlot->addGraph(); // Treated waveform graph
//    customPlot->graph(0)->setData(x, treatmentData);
//    customPlot->graph(0)->setName("Treated Waveform");
//    customPlot->graph(0)->setPen(QPen(Qt::blue));

//    customPlot->addGraph();
//    customPlot->graph(1)->setData(x, dominantWaveform);
//    customPlot->graph(1)->setName("Dominant Waveform");
//    customPlot->graph(1)->setPen(QPen(Qt::red));


//    customPlot->rescaleAxes();


//    customPlot->xAxis->scaleRange(1.4, customPlot->xAxis->range().center());
//    customPlot->yAxis->scaleRange(1.4, customPlot->yAxis->range().center());


//    customPlot->xAxis->setLabel("Time (s)");
//    customPlot->yAxis->setLabel("Amplitude");


//    customPlot->legend->setVisible(true);

//    customPlot->replot();



//}








//SLOTS:

//Power Button, switches stacked screen to black to show device is turned off, tells device to turn off
void MainWindow::Power()
{
    // check if device is on or off, sends accordingly
    if(power == true){
        if(play){
            Stop();
            stop = true;
        }
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        device->power(false);
        power = false;
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->menuButton->setEnabled(false);
    }
    else if( battery <= 0){
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        device->power(false);
        power = false;
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->menuButton->setEnabled(false);
    }
    else{
        device->power(true);
        power = true;
        if(stop){
            qInfo("STOP IS TRUE SO DOING STOP PAGE FOR TURN ON");
            ui->stackedWidget->setCurrentWidget(ui->page_stop);
            ui->upButton->setEnabled(false);
            ui->downButton->setEnabled(false);
            ui->selectButton->setEnabled(false);
            ui->playButton->setEnabled(false);
            ui->stopButton->setEnabled(false);
            ui->pauseButton->setEnabled(false);
            ui->menuButton->setEnabled(false);
        }
        else{
            qInfo("STOP IS FALSE");
            MenuButton();
            ui->upButton->setEnabled(true);
            ui->downButton->setEnabled(true);
            ui->selectButton->setEnabled(true);
            ui->playButton->setEnabled(true);
            ui->stopButton->setEnabled(false);
            ui->pauseButton->setEnabled(false);
            ui->menuButton->setEnabled(true);
        }
    }

}

//Charger Plugged in:
void MainWindow::Charge()
{
    //if statement check weather or not it plugged in, does charge function accordingly
    if(charging){
        charging = false;
        ui->label_6->setText("Charger OFF");
    }
    else{
        battery = 100;
        ui->label_6->setText("Charger ON");
        ui->batteryLabel->setText(QString::number(battery) + "%");
        ui->lowBatteryLabel->setText("");
        charging = true;
    }
}


//AUX Plugged in:
void MainWindow::Aux()
//change after to make them look disabled.
{
    if(auxPlug){
        ui->auxLabel->setText("Aux Cord is plugged out - Plug in to use device!");
        ui->label_5->setText("Headsets OFF");
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        auxPlug = false;
        device->pauseSession();
    }
    else{
        ui->auxLabel->setText("");
        ui->label_5->setText("Headsets ON");
        ui->upButton->setEnabled(true);
        ui->downButton->setEnabled(true);
        ui->selectButton->setEnabled(true);
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        auxPlug = true;
        device->resumeSession();
    }
}

//Start Button:
void MainWindow::Start()
{
    //enable pause button if its disable
    //passes the current date time to new session
    NewSession(ui->dateTimeEdit->dateTime());
}

//Pause Button:
void MainWindow::Pause()
{
    //disable pause button til session is unpaused
    //device->pauseSession();

    if(!pause){
        ui->stackedWidget->setCurrentWidget(ui->page_pause);
        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->menuButton->setEnabled(false);

        ui->pauseButton->setEnabled(true);
        pause = true;
        device->pauseSession();
    }
    else{
        ui->stopButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        pause = false;
        device->resumeSession();
    }
}

//Stop Button:
void MainWindow::Stop()
{
    //enable start button and pause button if one was disable
    stop = true;
    ui->stackedWidget->setCurrentWidget(ui->page_stop);
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->menuButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    device->stopSession();
}

//menu list button:
void MainWindow::MenuButton()
{
    if(power){
        ui->stackedWidget->setCurrentWidget(ui->page);
        ui->stopButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->upButton->setEnabled(true);
        ui->downButton->setEnabled(true);
        ui->selectButton->setEnabled(true);
        ui->playButton->setEnabled(true);
    }
}

//menu up:
void MainWindow::MenuUp()
{
    if(ui->stackedWidget->currentWidget()->objectName() == "page"){
        //if statement to change current menuIndex label to black again
        menuLabels[menuIndex]->setStyleSheet("background: transparent; border: none; color: black");
        if (menuIndex > 0){
            menuIndex--;
        }
        //update the new menu Index label to show its selected by changing to red.
        menuLabels[menuIndex]->setStyleSheet("background: transparent; border: none; color: red");
    }
}

//menu down:
void MainWindow::MenuDown()
{
    //if statement to change current menuIndex label to black again
    menuLabels[menuIndex]->setStyleSheet("background: transparent; border: none; color: black;");
    if (menuIndex < 3){
        menuIndex++;
    }
    //update the new menu Index label to show its selected by changing to red.
    menuLabels[menuIndex]->setStyleSheet("background: transparent; border: none; color: red;");
}

//menu enter/select button:
void MainWindow::MenuEnter()
{
    //chooses the selected option on the main menu
    if(ui->stackedWidget->currentWidget()->objectName() == "page"){
        if(charging == false){
            battery -= 5;
            ui->batteryLabel->setText(QString::number(battery) + "%");
        }
        //checks if battery is low and gives user a warning
        if(battery <= 5 && battery > 0){
            ui->lowBatteryLabel->setText("LOW BATTERY - Please Charge!");
        }
        //checks if battery is dead and turns off
        else if(battery <= 0){
            ui->lowBatteryLabel->setText("");
            Power();
        }
        //if not switches screen.
        else{
            switch(menuIndex){
                case 0:
                    Start();
                    break;
                case 1:
                    ui->stackedWidget->setCurrentWidget(ui->page_5);
                    ui->upButton->setEnabled(false);
                    ui->downButton->setEnabled(false);
                    ui->playButton->setEnabled(false);
                    ui->pauseButton->setEnabled(false);
                    ui->stopButton->setEnabled(false);
                    break;
                case 2:
                    ui->stackedWidget->setCurrentWidget(ui->page_4);
                    ui->upButton->setEnabled(false);
                    ui->downButton->setEnabled(false);
                    ui->playButton->setEnabled(false);
                    ui->pauseButton->setEnabled(false);
                    ui->stopButton->setEnabled(false);
                    break;
                case 3:
                    ui->stackedWidget->setCurrentWidget(ui->page_6);
                    ui->upButton->setEnabled(false);
                    ui->downButton->setEnabled(false);
                    ui->playButton->setEnabled(false);
                    ui->pauseButton->setEnabled(false);
                    ui->stopButton->setEnabled(false);
                    break;
            }
        }

    }

    //if they are choosing to view a session on the session logs page
    else if(ui->stackedWidget->currentWidget()->objectName() == "page_5"){
        QModelIndex currIndex = ui->listView->currentIndex();
        int selectedIndex = currIndex.row();
        if(device->getNumSessions() != 0 && selectedIndex != 256){
            PCScreen(device->getSessions()[selectedIndex]);
        }
    }

    else if(ui->stackedWidget->currentWidget()->objectName() == "page_6"){
        //qInfo("TEST GRAPH ENTER BUTTON PRESSED");
    }
}

//TIME:
void MainWindow::onTimeChanged(const QTime &time)
{
    QDate currDate = ui->dateTimeEdit->date();
    ui->dateTimeEdit->setDateTime(QDateTime(currDate, time));
}

void MainWindow::onDateChanged(const QDate &date)
{
    QTime currTime = ui->dateTimeEdit->time();
    ui->dateTimeEdit->setDateTime(QDateTime(date, currTime));
}

void MainWindow::updateTime(){
    QDateTime currTime = ui->dateTimeEdit->dateTime();
    currTime = currTime.addSecs(60);
    ui->dateTimeEdit->setDateTime(currTime);
}


void MainWindow::updateProgress(){
    //get the current time, then subtract for each electrode
    timer -= 1.380952380952381;

    int timerInt = static_cast<int>(timer);
    QString newTime = (timerInt < 10) ? QString("0%1").arg(timerInt) : QString::number(timerInt);

    ui->timerLabel->setText("0:" + newTime);

    progressValue += 476;
    ui->progressBar->setValue(progressValue / 100);
}

void MainWindow::progressComplete(){
    timer = 29;
    ui->timerLabel->setText("0:29");
    progressValue = 0;
    ui->progressBar->setValue(progressValue / 100);
    if(pause){
        Pause();
    }
    //turn off stop if it hasnt yet
    stop = false;
    play = false;
    MenuButton();
}

//void MainWindow::on_listView_2_doubleClicked(const QModelIndex &index)
//{
//       ui->stackedWidget->setCurrentWidget(ui->page_7);
//       QModelIndex currIndex = ui->listView_2->currentIndex();
//       int selectedIndex = currIndex.row();
//       updateTestUi(selectedIndex);
//}

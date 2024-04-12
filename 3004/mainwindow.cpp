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

    //init the menu labels (options) to access them easier
    menuLabels[0] = ui->newSessionLabel;
    menuLabels[1] = ui->sessionLogLabel;
    menuLabels[2] = ui->dateLabel;
    menuLabels[3] = ui->testLabel;


}






MainWindow::~MainWindow()
{
    delete ui;
}


//reset function that will get called after session is done, just brings back device state to beginning (main menu), we can call this after we run new session
void MainWindow::Reset(){

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
    ui->stackedWidget->setCurrentWidget(ui->page_2);

        ui->upButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->selectButton->setEnabled(false);
        ui->playButton->setEnabled(false);
        ui->menuButton->setEnabled(false);

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

    qInfo("in SessionLogs");
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
    qInfo("in pcScreen now");
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








//SLOTS:

//Power Button, switches stacked screen to black to show device is turned off, tells device to turn off
void MainWindow::Power()
{
    // check if device is on or off, sends accordingly
    if(power == true){
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        device->power(false);
        power = false;
    }
    else if( battery <= 0){
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        device->power(false);
        power = false;
    }
    else{
        ui->stackedWidget->setCurrentWidget(ui->page);
        device->power(true);
        power = true;
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
        ui->stopButton->setEnabled(true);
        ui->pauseButton->setEnabled(true);
        auxPlug = true;
        device->resumeSession();
    }
}

//Start Button:
void MainWindow::Start()
{
    //disable start button til session is done?
    //enable pause button if its disable
    //passes the current date time to new session
    NewSession(ui->dateTimeEdit->dateTime());
}

//Pause Button:
void MainWindow::Pause()
{
    //disable pause button til session is unpaused
    device->pauseSession();
}

//Stop Button:
void MainWindow::Stop()
{
    //enable start button and pause button if one was disable
    //device->stopSession();
}

//menu list button:
void MainWindow::MenuButton()
{
    if(power){
        ui->stackedWidget->setCurrentWidget(ui->page);
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
                    qInfo("switching to session screen");
                    break;
                case 2:
                    ui->stackedWidget->setCurrentWidget(ui->page_4);
                    break;
                case 3:
                    ui->stackedWidget->setCurrentWidget(ui->page_6);
                    break;
            }
        }

    }

    //if they are choosing to view a session on the session logs page
    else if(ui->stackedWidget->currentWidget()->objectName() == "page_5"){
        qInfo("session listView selected");
        QModelIndex currIndex = ui->listView->currentIndex();
        int selectedIndex = currIndex.row();
        if(device->getNumSessions() != 0 && selectedIndex != 256){
            PCScreen(device->getSessions()[selectedIndex]);
        }
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


void MainWindow::updateProgress(){
    //get the current time, then subtract for each electrode
    timer -= 1.380952380952381;
    qInfo("CALLED UPDATE PROGRESS");

    int timerInt = static_cast<int>(timer);
    QString newTime = (timerInt < 10) ? QString("0%1").arg(timerInt) : QString::number(timerInt);

    ui->timerLabel->setText("0:" + newTime);

    progressValue += 476;
    ui->progressBar->setValue(progressValue / 100);
}

void MainWindow::progressComplete(){
    qInfo("RESETING NOW");
    timer = 29;
    ui->timerLabel->setText("0:29");
    progressValue = 0;
    ui->progressBar->setValue(progressValue / 100);
    MenuButton();
}

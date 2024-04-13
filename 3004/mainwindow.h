#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "device.h"
#include <QLabel>
#include <QStringListModel>
#include <QDebug>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //menu options
    void NewSession(QDateTime const &dateTime);

    //pc screen
    void PCScreen(Session* session);

    //graph
    //void updateTestUi(int id);
    void chooseElectrode();



private:
    Ui::MainWindow *ui;
    bool power;
    int menuIndex;
    QLabel *menuLabels[4];
    int battery;
    bool charging;
    bool auxPlug;
    Device *device;
    double timer;
    int progressValue;
    bool pause;
    bool stop;
    bool play;

public slots:
    //active the lights
    void RedLight();
    void BlueLight();
    void GreenLight();

    //load session logs
    void SessionLogs();

    //power and charge
    void Power();
    void Charge();
    void Aux();


    //play, pause, stop buttons
    void Start();
    void Stop();
    void Pause();

    //menu buttons
    void MenuButton();
    void MenuUp();
    void MenuDown();
    void MenuEnter();

    //date and time:
    void onTimeChanged(const QTime &time);
    void onDateChanged(const QDate &date);
    void updateTime();

    //update progress and time during session
    void updateProgress();
    void progressComplete();

    //void on_listView_2_doubleClicked(const QModelIndex &index);
};
#endif // MAINWINDOW_H

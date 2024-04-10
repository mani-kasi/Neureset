#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "Device.h"
#include <QLabel>
#include <QStringListModel>
#include

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //reset function that will get called after session is done, just brings back device state to beginning (main menu)
    void Reset();

    //active the lights
    void RedLight();
    void BlueLight();
    void GreenLight();

    //menu options
    void NewSession();
    void SessionLogs();

    //pc screen
    void PCScreen(int sessionExample);

private:
    Ui::MainWindow *ui;
    bool power;
    int menuIndex;
    QLabel *menuLabels[4];
    int battery;
    bool charging;
    bool auxPlug;
    int testLogs[10];
    //Device eeg;

private slots:
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


    //test functions
    void Lights();

};
#endif // MAINWINDOW_H

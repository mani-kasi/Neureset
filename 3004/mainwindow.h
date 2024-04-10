#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "device.h"
#include <QLabel>
#include <QStringListModel>

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

    //menu options
    void NewSession(QDateTime const &dateTime);
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
    Device *device;

public slots:
    //active the lights
    void RedLight();
    void BlueLight();
    void GreenLight();

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

};
#endif // MAINWINDOW_H

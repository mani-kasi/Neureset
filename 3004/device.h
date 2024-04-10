#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <session.h>
#include <electrode.h>
#include <QTimer>
#define MAX_SESSIONS 30
#define NUM_ELECTRODES 21

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    ~Device();
    void newSession();
    void power(bool on);
    void calculateOverallBaseline();
    void calculateBaseline(int site);
    void saveSession();
    void batteryCharge();

private:
    Session* sessions[MAX_SESSIONS];
    Session* curSession;
    Electrode* electrodes[NUM_ELECTRODES];
    int beforeBaseline;
    int afterBaseline;
    bool charging;
    int battery;
    int dateTime;
    int contactTimer;
    int numSessions;
    QTimer* pauseTimer;
    bool currentlyOn;

private slots:
    void pauseSession();
    void resumeSession();
    void decrementTime();

signals:
    void sendBlueLightSignal();
    void sendRedLightSignal();
    void sendGreenLightSignal();
};

#endif // DEVICE_H

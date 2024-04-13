#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <session.h>
#include <electrode.h>
#include <QTimer>
#include <QVector>
#include <QEventLoop>
#define MAX_SESSIONS 30
#define NUM_ELECTRODES 21

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    ~Device();
    void newSession(QDateTime const &dateTime);
    void power(bool on);
    void saveSession();
    void resumeSession();
    void pauseSession();
    void stopSession();
    void delay(int);

    Session** getSessions();
    int getNumSessions();
    QVector<Electrode*>* getElectrodes();

private:
    Session* sessions[MAX_SESSIONS];
    Session* curSession;
    QVector<Electrode*> electrodes;
    int beforeBaseline;
    int afterBaseline;
    int dateTime;
    int contactTimer;
    int numSessions;
    QTimer* pauseTimer;

private slots:

    void decrementTime();

signals:
    void sendBlueLightSignal();
    void sendRedLightSignal();
    void sendGreenLightSignal();
    void sendProgress();
    void progressDone();
    void updateSessionLogs();
};

#endif // DEVICE_H

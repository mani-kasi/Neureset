#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

enum State {
    STARTUP, CONTACT, TREATMENT, LOST_CONTACT
};

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);

private:
    int overallBaseline;
    State state;
    bool charging;
    int battery;
    int dateTime;
    int contactTimer;


signals:

};

#endif // DEVICE_H

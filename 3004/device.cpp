#include "device.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    pauseTimer = new QTimer();

    //Connect QTimer decrement function
    connect(pauseTimer, &QTimer::timeout, this, &Device::decrementTime);

    numSessions = 0;
    power(true);
}

Device::~Device() {
    for (int i = 0; i < numSessions; i++) {
        delete sessions[i];
    }

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        delete electrodes[i];
    }
}

void Device::newSession(QDateTime const &dateTime) {
    qInfo("device new session function just called");

    if (numSessions+1 == MAX_SESSIONS) {
        return;
    }


    for (int i = 0; i < NUM_ELECTRODES; i++) {
        electrodes.append(new Electrode());
    }

    curSession = new Session(nullptr, dateTime, electrodes);

    emit sendBlueLightSignal();


    //Need baseline at start of session
    curSession->startSession();

    emit sendProgress();
    delay(5000);


    //Treatment on each electrode
    for (int i = 0; i < NUM_ELECTRODES; i++) {   
        emit sendGreenLightSignal();
        emit sendProgress();
        qInfo("Applying treatment for electrode %d:",i);
        electrodes[i]->applyTreatment();
        electrodes[i]->applyTreatment();
        electrodes[i]->applyTreatment();
        electrodes[i]->applyTreatment();
        delay(1000);

        //if paused then something needs to happen
    }

    //Need baseline at end of session
    curSession->setOverallBaselineEnd();
    emit sendProgress();
    delay(5000);

    saveSession();
    curSession = NULL;
}

void Device::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void Device::decrementTime() {
    contactTimer -= 60;
    if (contactTimer <= 0) {
        power(false);
    }
}

void Device::pauseSession() {
    pauseTimer->start(1000);
    emit sendRedLightSignal();
    //set state to paused
}

void Device::resumeSession() {
    if (pauseTimer->isActive()) {
        pauseTimer->stop();
    }
    emit sendBlueLightSignal();
}

void Device::power(bool on) {
    if (on) {
        //This is five minutes in seconds
        contactTimer = 300;
    }
    else {
        for (int i = 0; i < NUM_ELECTRODES; i++) {
            delete electrodes[i];
        }
        //Somehow stop flow of program
    }
}

void Device::saveSession() {
    sessions[numSessions++] = curSession;
}

void Device::stopSession() {
    delete curSession;
    //Stop flow of program
}


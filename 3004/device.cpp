#include "device.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    //Connect QTimer decrement function
    //connect(pauseTimer, &QTimer::timeout, this, SLOT (decrementTime()));
    //There is an int dateTime, not sure how to format that
    numSessions = 0;
    pauseTimer = new QTimer();
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
    if (numSessions+1 == MAX_SESSIONS) {
        return;
    }

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        electrodes[i] = new Electrode();
    }

    //curSession = new Session();

    emit sendBlueLightSignal();

    calculateOverallBaseline();

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        emit sendGreenLightSignal();
        //electrodes[i]->treatment()
        //if paused then

    }
    calculateOverallBaseline();
    saveSession();
    curSession = NULL;
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

void Device::calculateOverallBaseline() {

}

void Device::calculateBaseline(int site) {

}

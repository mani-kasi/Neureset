#include "device.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    //Connect QTimer decrement function
    connect(pauseTimer, &QTimer::timeout, this, SLOT (decrementTime()));
    charging = false;
    battery = 100;
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

void Device::newSession() {
    if (numSessions+1 == MAX_SESSIONS) {
        return;
    }

    curSession = new Session();

    sendBlueLightSignal();

    calculateOverallBaseline();

    for (int i = 0; i < NUM_ELECTRODES; i++) {
        sendGreenLightSignal();
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
    sendRedLightSignal();
    //set state to paused
}

void Device::resumeSession() {
    if (pauseTimer->isActive()) {
        pauseTimer->stop();
    }
    sendBlueLightSignal();
}

void Device::power(bool on) {
    if (on) {
        currentlyOn = true;
        //This is five minutes in seconds
        contactTimer = 300;
    }
    else {
        currentlyOn = false;
        //Somehow stop flow of program
    }
}

void Device::saveSession() {
    sessions[numSessions++] = curSession;
}

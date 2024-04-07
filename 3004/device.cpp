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
    state = CONTACT;

    curSession = new Session();

    sendBlueLightSignal();

    calculateOverallBaseline();

    state = TREATMENT;
    for (int i = 0; i < NUM_ELECTRODES; i++) {
        sendGreenLightSignal();
        //electrodes[i]->treatment()

    }
    calculateOverallBaseline();
    saveSession();
}

void Device::changeSession(Session* session) {

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
}

void Device::resumeSession() {
    if (pauseTimer->isActive()) {
        pauseTimer->stop();
    }
    sendBlueLightSignal();
}

void Device::power(bool on) {
    if (on) {
        state = STARTUP;
        //This is five minutes in seconds
        contactTimer = 300;
    }
    else {
        //Somehow stop flow of program
    }
}

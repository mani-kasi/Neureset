#include "device.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    state = STARTUP;
    charging = false;
    battery = 100;
    //This is five minutes in seconds
    contactTimer = 300;
    //There is an int dateTime, not sure how to format that
}

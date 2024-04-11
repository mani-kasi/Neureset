#include "session.h"



Session::Session(QObject *parent, const QDateTime& startTime,  Electrode** electrodes)
    : QObject{parent}
{
    this->startTime = startTime;
    this->electrodes = electrodes;
}

void Session::startSession(){
    qInfo("Session started");
    running = true;
    overallBaselineStart = calcOverallBaseline();
}

void Session::setOverallBaselineEnd(){
    qInfo("Session end baseline calculated");
    overallBaselineEnd = calcOverallBaseline();
}

void Session::pauseSession(){
    paused = true;
}

void Session::resumeSession(){
    paused = false;
}

void Session::stopSession(){
    running = false;
    paused = false;
    overallBaselineStart = 0;
    overallBaselineEnd = 0;
}

void Session::setStartTime(const QDateTime& startTime) {
    this->startTime = startTime;
}

void Session::setEndTime(const QDateTime& endTime) {
    this->endTime = endTime;
}

QDateTime Session::getStartTime()const{
    cout<<this->startTime.toString("yyyy-MM-dd HH:mm:ss").toStdString()<<endl;
    return startTime;
}
QDateTime Session::getEndTime() const{
    cout<<this->endTime.toString("yyyy-MM-dd HH:mm:ss").toStdString()<<endl;
    return endTime;
}

double Session::getOverallBaselineStart() const{
    return overallBaselineStart;
}


double Session::getOverallBaselineEnd() const{
    return overallBaselineEnd;
}

double Session::getSessionProgress() const{
    return sessionProgress;
}
double Session::calcOverallBaseline(){


    double tempSum = 0;
    for(int i = 0; i<NUM_ELECTRODES; i++){
        tempSum = tempSum + electrodes[i]->getDominantFrequency();
    }
    double tempDomAvg = tempSum/NUM_ELECTRODES;

    return tempDomAvg;
}

bool Session::isSessionRunning() const{
    return running;
}
bool Session::isSessionPaused() const{
    return paused;
}

string Session::print() {
    string temp = "DateTime: "+ startTime.toString("yyyy-MM-dd HH:mm:ss").toStdString()+"\n Overall Baseline Start: "+to_string(overallBaselineStart)+"\n Overall Baseline Start: "+ to_string(overallBaselineEnd);
    return temp;
}

void Session::onContactLost(){}
void Session::onContactReestablished(){}
void Session::onPauseTimeout(){}

#include "session.h"



Session::Session(QObject *parent, const QDateTime& startTime,  QVector<Electrode*>electrodes)
    : QObject{parent}
{
    this->startTime = startTime;
    if(!startTime.isValid()){
        this->startTime.setDate(QDate(2024, 4, 12));
        this->startTime.setTime(QTime(7, 0));
    }
    this->electrodes = electrodes;
}

void Session::startSession(){
    qInfo("Session started!");
    running = true;
    for(int i = 0; i<electrodes.length(); i++){
        electrodes[i]->generateBaselineData();
    }
    overallBaselineStart = calcOverallBaseline();
}

void Session::setOverallBaselineEnd(){
    qInfo("Session end and baseline calculated!");
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
    return startTime;
}
QDateTime Session::getEndTime() const{
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
    for(int i = 0; i<electrodes.length(); i++){
        tempSum = tempSum + electrodes[i]->getDominantFrequency();
    }
    double tempDomAvg = tempSum/electrodes.length();

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

#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <iostream>
#include "electrode.h"
using namespace std;


#define NUM_ELECTRODES 21
#define PAUSE_TIMEOUT 300

class Session : public QObject {
    Q_OBJECT

public:
    Session(QObject* parent, const QDateTime&, Electrode** );

    void setStartTime(const QDateTime&);
    void setEndTime(const QDateTime&);

    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();

    bool isSessionRunning() const;
    bool isSessionPaused() const;
    double getSessionProgress() const;

    QDateTime getStartTime() const;
    QDateTime getEndTime() const;

    double getOverallBaselineStart() const;
    double getOverallBaselineEnd() const;
    double calcOverallBaseline();

    void setOverallBaselineEnd();

    string print();

signals:
    void sessionStarted();
    void sessionPaused();
    void sessionResumed();
    void sessionStopped();
    void contactLost();
    void contactReestablished();
    void sessionErased();
    void progressUpdated(double progress);

private slots:
    void onContactLost();
    void onContactReestablished();
    void onPauseTimeout();

private:
    Electrode** electrodes;
    bool running = false;
    bool paused = false;
    QDateTime startTime;
    QDateTime endTime;
    double overallBaselineStart;
    double overallBaselineEnd;
    double sessionProgress=0;

};

#endif // SESSION_H

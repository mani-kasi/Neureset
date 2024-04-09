#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include "electrode.h"

class Session : public QObject {
    Q_OBJECT

public:
    Session(QObject* parent = nullptr);

    void startSession();
    void pauseSession();
    void resumeSession();
    void stopSession();

    bool isSessionRunning() const;
    bool isSessionPaused() const;
    double getSessionProgress() const;
    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    QVector<double> getOverallBaselineStart() const;
    QVector<double> getOverallBaselineEnd() const;

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
    QVector<Electrode*> electrodes;
    bool running;
    bool paused;
    QDateTime startTime;
    QDateTime endTime;
    QVector<double> overallBaselineStart;
    QVector<double> overallBaselineEnd;

    const int NUM_ELECTRODES = 21;
    const int PAUSE_TIMEOUT = 300; // 5 minutes in seconds
};

#endif // SESSION_H

//#include "session.h"


//// Initialize the static variable
//int Session::nextSessionID = 1;

//Session::Session(int id, const QDateTime& start)
//    : startTime(start)
//{
//    if (id == -1) {
//        sessionID = nextSessionID++;
//    } else {
//        sessionID = id;
//    }
//    // Other initialization code if needed
//}


//Session::Session(QObject* parent)
//    : QObject(parent)
//    , running(false)
//    , paused(false)
//{
//    for (int i = 0; i < NUM_ELECTRODES; ++i) {
//        electrodes.append(new Electrode());
//    }
//}

//void Session::startSession()
//{
//    if (!running) {
//        running = true;
//        paused = false;
//        startTime = QDateTime::currentDateTime();
//        endTime = QDateTime();

//        // Calculate overall baseline at the start of the session
//        calculateOverallBaseline(overallBaselineStart);

//        // Connect signals and slots for contact loss and reestablishment
//        for (Electrode* electrode : electrodes) {
//            connect(electrode, &Electrode::contactLost, this, &Session::onContactLost);
//            connect(electrode, &Electrode::contactReestablished, this, &Session::onContactReestablished);
//        }

//        emit sessionStarted();
//        startTreatment();
//    }
//}

//void Session::pauseSession()
//{
//    if (running && !paused) {
//        paused = true;
//        emit sessionPaused();

//        // Start the pause timeout timer
//        QTimer::singleShot(PAUSE_TIMEOUT * 1000, this, &Session::onPauseTimeout);
//    }
//}

//void Session::resumeSession()
//{
//    if (running && paused) {
//        paused = false;
//        emit sessionResumed();
//    }
//}

//void Session::stopSession()
//{
//    if (running) {
//        running = false;
//        paused = false;
//        endTime = QDateTime::currentDateTime();

//        // Calculate overall baseline at the end of the session
//        calculateOverallBaseline(overallBaselineEnd);

//        emit sessionStopped();
//    }
//}

//bool Session::isSessionRunning() const
//{
//    return running;
//}

//bool Session::isSessionPaused() const
//{
//    return paused;
//}

//double Session::getSessionProgress() const
//{
//    if (running) {
//        // Calculate and return the session progress based on time or number of electrodes treated
//        // You can customize this based on your specific progress calculation logic
//        return 0.0;
//    }
//    return 0.0;
//}

//QDateTime Session::getStartTime() const
//{
//    return startTime;
//}

//QDateTime Session::getEndTime() const
//{
//    return endTime;
//}

//QVector<double> Session::getOverallBaselineStart() const
//{
//    return overallBaselineStart;
//}

//QVector<double> Session::getOverallBaselineEnd() const
//{
//    return overallBaselineEnd;
//}

//void Session::onContactLost()
//{
//    if (running && !paused) {
//        pauseSession();
//        emit contactLost();
//    }
//}

//void Session::onContactReestablished()
//{
//    if (running && paused) {
//        resumeSession();
//        emit contactReestablished();
//    }
//}

//void Session::onPauseTimeout()
//{
//    if (running && paused) {
//        stopSession();
//        emit sessionErased();
//    }
//}

//void Session::calculateOverallBaseline(QVector<double>& baseline)
//{
//    baseline.clear();
//    for (Electrode* electrode : electrodes) {
//        electrode->generateBaselineData(60, Electrode::SAMPLE_RATE);
//        baseline.append(electrode->getDominantFrequency());
//    }
//}

//void Session::startTreatment()
//{
//    for (Electrode* electrode : electrodes) {
//        electrode->generateBaselineData(60, Electrode::SAMPLE_RATE);
//        electrode->applyTreatment(1, Electrode::OFFSET_FREQUENCY, Electrode::TREATMENT_INTERVAL);
//        // Emit a signal to indicate that treatment is being delivered for this electrode
//        // You can connect this signal to update the UI accordingly
//    }
//    stopSession();
//}

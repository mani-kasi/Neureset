#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QVector>
#include <cmath>
#include <random>

class Electrode {
public:
    Electrode();

    void generateBaselineData();
    void applyTreatment();

    QVector<double> getBaselineData() const;
    QVector<double> getTreatmentData() const;
    double getDominantFrequency() const;
    QVector<double> getBaselineWaveform() const;


private:
    double generateRandomFrequency(double minFreq, double maxFreq);
    double generateRandomAmplitude();
    double calculateDominantFrequency();

    QVector<double> baselineData;
    QVector<double> treatmentData;
    double dominantFrequency;

    // Constants
    const int SAMPLE_RATE = 60;
    const int BASELINE_DURATION = 60; // 1 minute for baseline
    const int TREATMENT_DURATION = 1; // 1 second for treatment
    const int TREATMENT_INTERVAL = SAMPLE_RATE / 16; // 1/16 second intervals
    const double OFFSET_FREQUENCY = 5.0;

    double f1, A1, f2, A2, f3, A3, f4, A4;
};

#endif // ELECTRODE_H

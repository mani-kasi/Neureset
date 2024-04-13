#include "electrode.h"
#include <chrono>
#include <iostream>

Electrode::Electrode()
    : dominantFrequency(0.0), f1(0.0), A1(0.0), f2(0.0), A2(0.0), f3(0.0), A3(0.0), f4(0.0), A4(0.0) {}

double Electrode::generateRandomFrequency(double minFreq, double maxFreq) {
    static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dis(minFreq, maxFreq);
    return dis(gen);
}

double Electrode::generateRandomAmplitude() {
    static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

double Electrode::calculateDominantFrequency() {
    double numerator = f1 * A1 * A1 + f2 * A2 * A2 + f3 * A3 * A3 + f4 * A4 * A4;
    double denominator = A1 * A1 + A2 * A2 + A3 * A3 + A4 * A4;
    return numerator / denominator;
}

void Electrode::generateBaselineData() {
    int numSamples = BASELINE_DURATION * SAMPLE_RATE;
    baselineData.resize(numSamples);

    f1 = generateRandomFrequency(0.1, 3.0);
    A1 = generateRandomAmplitude();
    f2 = generateRandomFrequency(3.5, 7.5);
    A2 = generateRandomAmplitude();
    f3 = generateRandomFrequency(7.5, 13.0);
    A3 = generateRandomAmplitude();
    f4 = generateRandomFrequency(14.0, 30.0);
    A4 = generateRandomAmplitude();

    for (int i = 0; i < numSamples; ++i) {
        double t = i / static_cast<double>(SAMPLE_RATE);
        baselineData[i] = A1 * std::sin(2 * M_PI * f1 * t) +
                          A2 * std::sin(2 * M_PI * f2 * t) +
                          A3 * std::sin(2 * M_PI * f3 * t) +
                          A4 * std::sin(2 * M_PI * f4 * t);
    }

    dominantFrequency = calculateDominantFrequency();
}

void Electrode::applyTreatment() {


    int numSamples = TREATMENT_DURATION * SAMPLE_RATE;
    treatmentData = baselineData.mid(0, numSamples);

    for (int start = 0; start < numSamples; start += TREATMENT_INTERVAL) {
        int end = std::min(start + TREATMENT_INTERVAL, numSamples);
        double modulatedFrequency = dominantFrequency + OFFSET_FREQUENCY;

        for (int i = start; i < end; ++i) {
            double t = i / static_cast<double>(SAMPLE_RATE);
            treatmentData[i] *= std::sin(2 * M_PI * modulatedFrequency * t);
        }

        // Update the dominant frequency for the next interval
        dominantFrequency = modulatedFrequency;
    }
    //std::cout << std::to_string(dominantFrequency) << std::endl;


}

QVector<double> Electrode::getBaselineData() const {
    return baselineData;
}

QVector<double> Electrode::getTreatmentData() const {
    return treatmentData;
}

double Electrode::getDominantFrequency() const {
    return dominantFrequency;
}

QVector<double> Electrode::getBaselineWaveform() const {
    QVector<double> dominantWaveform(baselineData.size());
    double tStep = 1.0 / SAMPLE_RATE;
    for (int i = 0; i < dominantWaveform.size(); ++i) {
        double t = i * tStep;
        dominantWaveform[i] = std::sin(2 * M_PI * dominantFrequency * t);
    }
    return dominantWaveform;
}




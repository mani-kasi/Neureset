#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "electrode.h"
#include "qcustomplot.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Electrode electrode;
    electrode.generateBaselineData();
    electrode.applyTreatment();
    electrode.applyTreatment();
    electrode.applyTreatment();
    electrode.applyTreatment();

    QVector<double> treatmentData = electrode.getTreatmentData();
    QVector<double> dominantWaveform = electrode.getBaselineWaveform();

    QVector<double> x(treatmentData.size());
    for (int i = 0; i < x.size(); ++i) {
        x[i] = i / 128.0;
    }
    //This plots  both but I think we will only plot treated waveform?
    // Set up the custom plot
    QCustomPlot *customPlot = ui->plotWidget;
    customPlot->addGraph(); // Treated waveform graph
    customPlot->graph(0)->setData(x, treatmentData);
    customPlot->graph(0)->setName("Treated Waveform");
    customPlot->graph(0)->setPen(QPen(Qt::blue));

    customPlot->addGraph();
    customPlot->graph(1)->setData(x, dominantWaveform);
    customPlot->graph(1)->setName("Dominant Waveform");
    customPlot->graph(1)->setPen(QPen(Qt::red));


    customPlot->rescaleAxes();


    customPlot->xAxis->scaleRange(1.4, customPlot->xAxis->range().center());
    customPlot->yAxis->scaleRange(1.4, customPlot->yAxis->range().center());


    customPlot->xAxis->setLabel("Time (s)");
    customPlot->yAxis->setLabel("Amplitude");


    customPlot->legend->setVisible(true);

    customPlot->replot();


}

MainWindow::~MainWindow()
{
    delete ui;
}


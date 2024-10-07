//
// Created by Nik Menshikov on 05.10.2024.
//


#include "harmonic_vibration_and_meander.h"
#include "ui_harmonic_vibration_and_meander.h"
#include "fftw3.h"
#include <cmath>


HarmonicVibrationAndMeander::HarmonicVibrationAndMeander(QWidget *parent) : QWidget(parent),
                                                                            ui(new Ui::HarmonicVibrationAndMeander)
{
    ui->setupUi(this);
    [[maybe_unused]] auto conn0 = connect(ui->spinBox, &QSpinBox::valueChanged, this,
                                          &HarmonicVibrationAndMeander::setupGraph);
    [[maybe_unused]] auto conn = connect(ui->comboBox, &QComboBox::currentIndexChanged, this,
                                         &HarmonicVibrationAndMeander::setupGraph);

    ui->comboBox->addItem("Гармонический сигнал");
    ui->comboBox->addItem("Меандр");
    ui->comboBox->addItem("Спектр Гармонического сигнала");
    ui->comboBox->addItem("Спектр Меандра");
}

HarmonicVibrationAndMeander::~HarmonicVibrationAndMeander()
{
    delete ui;
}

std::vector<double> qVectorToStdVector(QVector<double> &signal)
{
    std::vector result(signal.begin(), signal.end());
    return result;
}

void HarmonicVibrationAndMeander::setupGraph()
{
    x.resize(101);
    y.resize(101);
    const int currentIndex = ui->comboBox->currentIndex();
    ui->customPlot->clearGraphs();

    const double frequency = ui->spinBox->value();
    constexpr double amplitude = 1;
    constexpr int pointCount = 101;

    if (currentIndex == 0 || currentIndex == 1) {
        for (int i = 0; i < pointCount; ++i) {
            x[i] = i / 50.0 - 1;

            if (currentIndex == 0) {
                //harmonic vibration
                y[i] = amplitude * sin(2 * M_PI * frequency * x[i]);
                ui->customPlot->yAxis->setRange(-1, 1);
            } else {
                y[i] = (sin(2 * M_PI * frequency * x[i]) >= 0) ? 1 : 0; //meander
                ui->customPlot->yAxis->setRange(-0.1, 1.1);
            }
        }

        ui->customPlot->xAxis->setLabel("Время (с)");
        ui->customPlot->yAxis->setLabel("Амплитуда");
        ui->customPlot->xAxis->setRange(-1, 1);


    } else if (currentIndex == 2 || currentIndex == 3) {
        std::vector<double> signal(pointCount);
        for (int i = 0; i < pointCount; ++i) {
            if (currentIndex == 2) {
                signal[i] = amplitude * sin(2 * M_PI * frequency * i / 50.0);
            } else {
                signal[i] = (sin(2 * M_PI * frequency * i / 50) >= 0) ? 1 : 0;
            }
        }

        const QVector<double> spectrum = getSpectrum(signal);
        x.clear();
        x.resize(spectrum.size());
        for (int i = 0; i < spectrum.size(); ++i) {
            x[i] = i * (1.0 / pointCount * 50.0);
        }
        y.clear();
        y.resize(spectrum.size());
        y = spectrum;

        ui->customPlot->xAxis->setLabel("Частота (Гц)");
        ui->customPlot->yAxis->setLabel("Амплитуда");
        ui->customPlot->xAxis->setRange(0, 50);
        ui->customPlot->yAxis->setRange(0, *std::max_element(y.begin(), y.end()) + 10);
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot->replot();
}

QVector<double> HarmonicVibrationAndMeander::getSpectrum(const std::vector<double> &signal)
{
    const size_t N = signal.size();

    auto *in = static_cast<double *>(fftw_malloc(sizeof(double) * N));
    auto *out = static_cast<fftw_complex *>(fftw_malloc(sizeof(fftw_complex) * N));

    for (int i = 0; i < N; ++i) {
        in[i] = signal[i];
    }

    const auto plan = fftw_plan_dft_r2c_1d(static_cast<int>(N), in, out, FFTW_MEASURE);
    fftw_execute(plan);

    QVector<double> spectrum(static_cast<int>(N) / 2);

    for (int i = 0; i < N / 2; i++) {
        double real = out[i][0];
        double imagine = out[i][1];
        spectrum[i] = sqrt(pow(real, 2) + pow(imagine, 2));
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return spectrum;
}

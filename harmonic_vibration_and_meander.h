//
// Created by Nik Menshikov on 05.10.2024.
//

#ifndef HARMONIC_VIBRATION_AND_MEANDER_H
#define HARMONIC_VIBRATION_AND_MEANDER_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui
{
    class HarmonicVibrationAndMeander;
}

QT_END_NAMESPACE

class HarmonicVibrationAndMeander : public QWidget
{
    Q_OBJECT

public:
    explicit HarmonicVibrationAndMeander(QWidget *parent = nullptr);

    ~HarmonicVibrationAndMeander() override;

    void setupGraph();

private:
    Ui::HarmonicVibrationAndMeander *ui;
    QVector<double> x, y;
    static QVector<double> getSpectrum(const std::vector<double> &signal);
};


#endif //HARMONIC_VIBRATION_AND_MEANDER_H

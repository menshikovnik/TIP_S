#include "harmonic_vibration_and_meander.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    HarmonicVibrationAndMeander harmonicVibrationAndMeander;
    harmonicVibrationAndMeander.show();
    return QApplication::exec();
}

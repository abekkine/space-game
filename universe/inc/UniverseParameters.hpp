#ifndef UNIVERSE_PARAMETERS_HPP_
#define UNIVERSE_PARAMETERS_HPP_

struct UniverseParameters {

    int octaveCount;
    double frequency;
    double x, y;
    double minValue;
    double stepSize;
    double zIndex;

    double numMs; // M class
    double numKs; // K class
    double numGs; // G class
    double numFs; // F class
    double numAs; // A class
    double numBs; // B class
    double numOs; // O class
    double numNSs; // Neutron star
    double numBHs; // Black hole

    UniverseParameters()
    : octaveCount(1)
    , frequency(1.648)
    , x(-14.507)
    , y(13.707)
    , minValue(0.637)
    , stepSize(0.529)
    , zIndex(0.0)
    , numMs(256.0)
    , numKs(128.0)
    , numGs(64.0)
    , numFs(32.0)
    , numAs(16.0)
    , numBs(8.0)
    , numOs(4.0)
    , numNSs(2.0)
    , numBHs(1.0)
    {}
};

#endif // UNIVERSE_PARAMETERS_HPP_

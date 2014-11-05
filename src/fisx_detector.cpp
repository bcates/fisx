#include "fisx_detector.h"
#include <math.h>
#include <stdexcept>

namespace fisx
{

Detector::Detector(const std::string & name, const double & density, const double & thickness, \
                  const double & funnyFactor): Layer(name, density, thickness, funnyFactor)
{
    this->diameter =  0.0;
    this->distance = 10.0;
    this->escapePeakEnergyThreshold = 0.010,
    this->escapePeakIntensityThreshold = 1.0e-7;
    this->escapePeakNThreshold = 4;
    this->escapePeakAlphaIn = 90.;
    this->escapePeakCache.clear();
}

void Detector::setMaterial(const std::string & materialName)
{
    this->escapePeakCache.clear();
    this->Layer::setMaterial(materialName);
}

void Detector::setMaterial(const Material & material)
{
    this->escapePeakCache.clear();
    this->Layer::setMaterial(material);
}

void Detector::setMinimumEscapePeakEnergy(const double & energy)
{
    this->escapePeakEnergyThreshold = energy;
    this->escapePeakCache.clear();
}

void Detector::setMinimumEscapePeakIntensity(const double & intensity)
{
    this->escapePeakIntensityThreshold = intensity;
    this->escapePeakCache.clear();
}

void Detector::setMaximumNumberOfEscapePeaks(const int & nPeaks)
{
    this->escapePeakNThreshold = nPeaks;
    this->escapePeakCache.clear();
}


double Detector::getActiveArea() const
{
    double pi;
    pi = acos(-1.0);
    return (0.25 * pi) * (this->diameter * this->diameter);
}

void Detector::setActiveArea(const double & area)
{
    double pi;
    pi = acos(-1.0);
    if (area < 0)
    {
        throw std::invalid_argument("Negative detector area");
    }
    this->diameter = 2.0 * sqrt(area/pi);
}

void Detector::setDiameter(const double & diameter)
{
    if (diameter < 0)
    {
        throw std::invalid_argument("Negative detector diameter");
    }
    this->diameter = diameter;
}

void Detector::setDistance(const double & distance)
{
    if (distance <= 0)
    {
        throw std::invalid_argument("Negative detector distance");
    }
    this->distance = distance;
}

const double & Detector::getDiameter() const
{
    return this->diameter;
}

const double & Detector::getDistance() const
{
    return this->distance;
}

std::map<std::string, std::map<std::string, double> > Detector::getEscape(const double & energy,
                                                            const Elements & elementsLibrary,
                                                            const std::string & label,
                                                            const int & update)
{
    if (update != 0)
        this->escapePeakCache.clear();
    if (label.size())
    {
        if (this->escapePeakCache.find(label) == this->escapePeakCache.end())
        {
            // calculate it
            this->escapePeakCache[label] = elementsLibrary.getEscape(this->getComposition(elementsLibrary), \
                                                                     energy, \
                                                                     this->escapePeakEnergyThreshold, \
                                                                     this->escapePeakIntensityThreshold, \
                                                                     this->escapePeakNThreshold, \
                                                                     this->escapePeakAlphaIn);
        }
        return this->escapePeakCache[label];
    }
    else
    {
        return elementsLibrary.getEscape(this->getComposition(elementsLibrary), \
                                         energy, \
                                         this->escapePeakEnergyThreshold, \
                                         this->escapePeakIntensityThreshold, \
                                         this->escapePeakNThreshold, \
                                         this->escapePeakAlphaIn);
    }
}

} // namespace fisx

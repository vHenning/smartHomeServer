#ifndef RAYLEIGH_H
#define RAYLEIGH_H

#include <ctime>

namespace Rayleigh
{
    static const double NANO = 0.000000001;
    static const double RED_WAVELENGTH   = 660 * NANO;
    static const double GREEN_WAVELENGTH = 525 * NANO;
    static const double BLUE_WAVELENGTH  = 465 * NANO;

    /**
     * Get transmission factor of light with given wavelength through atmosphere at given zenith angle
     *
     * @param zenith Zenith angle [rad]. 90° - elevation.
     * @param wavelength Wavelength of light [m]
     * @return Transmission factor. 1 = Full intensity (not amplitude) is transmitted. 0 = no light is transmitted.
     */
    double getTransmission(double zenith, double wavelength);

    /**
     * Get zenith angle of sun at given position and time
     *
     * @param lat Latitude [deg]
     * @param lan Longitude [deg]
     * @param time Time [UTC]
     * @return Zenith angle (zenith = 1 - elevation) [deg]
     */
    double getZenith(double lat, double lan, std::tm time);

    /**
     * Convert given time to two julian doubles.
     * The unit of the returned doubles is days since "julian epoch".
     * 
     * @param time In: time object
     */
    double getJulian(std::tm time);
} // namespace Rayleigh

#endif // RAYLEIGH_H
#ifndef RAYLEIGH_H
#define RAYLEIGH_H

#include <vector>
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

    /**
     * Get the spectral radiance of a black body for the given wavelength and temperature
     *
     * @param lambda Wavelength [m]
     * @param T Temperature [K]
     * @return Spectral radiance [W/(m² * Hz * sr)]
     */
    double planck(double lambda, double T);

    /**
     * Estimate color temperature based on given radiances for given wavelengths
     *
     * @param wavelengths Array of wavelengths [m]
     * @param intensities Array of spectral radiances [W/(m² * Hz * sr)]
     * @param size Size of both arrays (they must be the same size)
     * @param T_init Initial guess for the color temperature [K]
     * @return Estimated color temperature [K]
     */
    double levenberg_marquardt(double wavelengths[], double intensities[], int size, double T_init);

    /**
     * Estimate color temperature based on given radiances for given wavelengths. Only consider the ratio of the given values (normalize them).
     *
     * @param wavelengths Array of wavelengths [m]
     * @param intensities Array of spectral radiance ratios [-]
     * @param size Size of both arrays (they must be the same size)
     * @param T_init Initial guess for the color temperature [K]
     * @return Estimated color temperature [K]
     */
    double levenberg_marquardt_normal(double wavelengths[], double intensities[], int size, double T_init);

    /**
     * Get color temperature of light received by the sun shining through the atmosphere at the given zenith angle.
     *
     * @param zenith The angle between the sun and zenith (sunset/sunrise = 90°, noon = 0°) [rad]
     * @return Color temperature [K]
     */
    double getColorTemperature(double zenith);

    /**
     * Get RGB values of sunlight shining through the atmosphere at given zenith angle.
     *
     * @param The angle between the sun and zenith (sunset/sunrise = 90°, noon = 0°) [rad]
     * @return Vector containing red, green and blue values (between 0 and 1)
     */
    std::vector<double> getRGB(double zenith);

} // namespace Rayleigh

#endif // RAYLEIGH_H
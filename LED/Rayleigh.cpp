#include "Rayleigh.h"

#include <math.h>
#include "aaplus/AA+.h"

#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)

double Rayleigh::getTransmission(double zenith, double wavelength)
{
    static const double FACTOR = 0.0084 * pow(10, -6 * 4);
    double tau = FACTOR / (cos(zenith) * pow(wavelength, 4));
    return exp(-tau);
}

double Rayleigh::getJulian(std::tm time)
{
    int year = time.tm_year + 1900;
    int month = time.tm_mon + 1;
    int day = time.tm_mday;
    int hour = time.tm_hour;
    int minute = time.tm_min;

    return CAADate(year, month, day, hour, minute, 0, true).Julian();
}

double Rayleigh::getZenith(double lat, double lon, std::tm time)
{
    double julian = getJulian(time);

    // Get Ecliptic Sun coordinates
    double eclipticLatitude = CAASun::ApparentEclipticLatitude(julian, false);
    double eclipticLongitude = CAASun::ApparentEclipticLongitude(julian, false);

    // Ecliptic to equatorial: In: lon, lat, earth tilt (obliquity of ecliptic)
    // Return: X [right ascension decimal hours] y [ecliptic latitude, degrees]
    double earthTilt = CAANutation::TrueObliquityOfEcliptic(julian);
    CAA2DCoordinate equatorial = CAACoordinateTransformation::Ecliptic2Equatorial(eclipticLongitude, eclipticLatitude, earthTilt);

    // Equatorial to horizontal: In: Local Hour angle [decimal hours, westwards, from south], declination [degrees], latitude of position [degrees]
    // Return: X [azimuth, degrees west of south], Y [altitude, degrees]
    double gst = CAASidereal::ApparentGreenwichSiderealTime(julian);
    double localHourAngle = gst - CAACoordinateTransformation::DegreesToHours(-lon) - equatorial.X;
    CAA2DCoordinate horizontal = CAACoordinateTransformation::Equatorial2Horizontal(localHourAngle, equatorial.Y, lat);

    double elevation = horizontal.Y;
    double zenith = 90 - elevation;

    return zenith;
}

double Rayleigh::planck(double lambda, double T) {
    const double h = 6.626e-34;  // Planck's constant (Joule*second)
    const double c = 3.0e8;      // Speed of light (m/s)
    const double k = 1.381e-23;  // Boltzmann's constant (Joule/Kelvin)

    double exponent = (h * c) / (lambda * k * T);
    return (2.0 * h * c * c) / (pow(lambda, 5) * (exp(exponent) - 1));
}

double Rayleigh::levenberg_marquardt(double wavelengths[], double intensities[], int size, double T_init) {
    const double epsilon = 1e-6; // Convergence threshold
    double T = T_init;
    double lambda, model, diff, J, JTJ, JTF, damping = 1.0;
    int max_iter = 1000;

    for (int iter = 0; iter < max_iter; iter++) {
        JTF = 0.0;
        JTJ = 0.0;

        for (int i = 0; i < size; i++) {
            lambda = wavelengths[i];
            model = planck(lambda, T);
            diff = intensities[i] - model;

            // Compute Jacobian (dI/dT)
            double dIdT = (planck(lambda, T + epsilon) - model) / epsilon;

            // Compute elements of normal equation
            JTF += dIdT * diff;
            JTJ += dIdT * dIdT;
        }

        if (fabs(JTF) < epsilon) break; // Convergence check

        // Update rule: T_new = T_old + delta_T
        double delta_T = JTF / (JTJ + damping);
        T += delta_T;

        if (fabs(delta_T) < epsilon) break; // Check for convergence
    }

    return T;
}

double Rayleigh::levenberg_marquardt_normal(double wavelengths[], double intensities[], int size, double T_init) {
    const double epsilon = 1e-6; // Convergence threshold
    double T = T_init;
    // double lambda, model, diff, J, JTJ, JTF, damping = 1.0;
    double lambda, model, diff, J, JTJ, JTF, damping = 0.00001;
    int max_iter = 1000;

    // Normalize intensities
    double maxIntensity = 0;
    int maxIndex = 0;
    for (int i = 0; i < size; ++i)
    {
        if (intensities[i] > maxIntensity)
        {
            maxIntensity = intensities[i];
            maxIndex = i;
        }
    }
    for (int i = 0; i < size; ++i)
    {
        intensities[i] = intensities[i] / maxIntensity;
    }

    for (int iter = 0; iter < max_iter; iter++) {
        JTF = 0.0;
        JTJ = 0.0;

        double maxPlanck = planck(wavelengths[maxIndex], T);

        for (int i = 0; i < size; i++) {
            lambda = wavelengths[i];
            model = planck(lambda, T) / maxPlanck;
            diff = intensities[i] - model;

            // Compute Jacobian (dI/dT)
            double dIdT = ((planck(lambda, T + epsilon) / maxPlanck) - model) / epsilon;

            // Compute elements of normal equation
            JTF += dIdT * diff;
            JTJ += dIdT * dIdT;
        }

        if (fabs(JTF) < epsilon)
        {
            printf("Jacobian converged after %d iterations\n", iter);
            break;
        }

        // Update rule: T_new = T_old + delta_T
        double delta_T = JTF / (JTJ + damping);
        T += delta_T;

        if (fabs(delta_T) < epsilon)
        {
            printf("Temperature converged after %d iterations\n", iter);
            break;
        }
    }

    return T;
}

double Rayleigh::getColorTemperature(double zenith)
{
    double** intensities = new double*[3];

    intensities[0] = new double[2];
    intensities[0][0] = Rayleigh::RED_WAVELENGTH;
    intensities[0][1] = Rayleigh::getTransmission(zenith, Rayleigh::RED_WAVELENGTH);
    intensities[1] = new double[2];
    intensities[1][0] = Rayleigh::GREEN_WAVELENGTH;
    intensities[1][1] = Rayleigh::getTransmission(zenith, Rayleigh::GREEN_WAVELENGTH);
    intensities[2] = new double[2];
    intensities[2][0] = Rayleigh::BLUE_WAVELENGTH;
    intensities[2][1] = Rayleigh::getTransmission(zenith, Rayleigh::BLUE_WAVELENGTH);

    double justIntensities[3];
    double justWavelengths[3];

    for (int i = 0; i < 3; ++i)
    {
        justIntensities[i] = intensities[i][1];
        justWavelengths[i] = intensities[i][0];
    }

    double startTemp = 4750;
    return Rayleigh::levenberg_marquardt_normal(justWavelengths, justIntensities, 3, startTemp);
}

std::vector<double> Rayleigh::getRGB(double zenith)
{
    std::vector<double> result;
    result.push_back(getTransmission(zenith, RED_WAVELENGTH));
    result.push_back(getTransmission(zenith, GREEN_WAVELENGTH));
    result.push_back(getTransmission(zenith, BLUE_WAVELENGTH));

    return result;
}

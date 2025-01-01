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

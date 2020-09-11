#include <cmath>
#include <iostream>

#include "R2Graph/R2Graph.h"
#include "R3Graph/R3Graph.h"

const double EARTH_RADIUS = 6378000.0;
const double PI = 3.14159265358979323846;
const double PI_2 = PI * 0.5;
const double TO_RAD = PI / 180.0;
const double TO_DEG = 1 / TO_RAD;

using namespace std;
using namespace R3Graph;

R2Point getMapProj(const R2Point& map, const R2Point& point);

int main()
{
    R2Point m, p;
    while (true)
    {
        cout << "Enter map position (lat, lon) and point on the map (x, y):" << endl;
        cin >> m >> p;
        if (!cin.good())
            break;

        R2Point pointProj = getMapProj(m, p);
        cout << "Point projection on the Earth surface: " << pointProj << endl;
    }

    return 0;
}

R2Point getMapProj(const R2Point& map, const R2Point& point)
{
    // latitude and longitude of the map center
    double mLat = map.x * TO_RAD, mLon = map.y * TO_RAD;

    // pre-calculate sines and cosines
    double sinLat = sin(mLat),
           sinLon = sin(mLon),
           cosLat = cos(mLat),
           cosLon = cos(mLon);

    // map center vector
    R3Vector m = EARTH_RADIUS * R3Vector(cosLat * cosLon,
                                         cosLat * sinLon,
                                         sinLat);

    // basis vector of the Earth Z axis
    R3Vector ez = R3Vector(0, 0, 1);

    // basis vectors of the map X, Y axes
    R3Vector emx = ez.vectorProduct(m).normalized();
    R3Vector emy = m.vectorProduct(emx).normalized();

    // vector of the given point on the map, in Earth basis
    R3Vector pm = emx * point.x + emy * point.y;

    // vector of the given point, in Earth basis
    R3Vector p = m + pm;

    // scale vector to the Earth surface
    p = EARTH_RADIUS * p.normalized();

    // latitude and longitude of the determined point
    double pLat = PI_2 - p.angle(ez),
           pLon = atan2(p.y, p.x);

    return R2Point(pLat, pLon) * TO_DEG;
}
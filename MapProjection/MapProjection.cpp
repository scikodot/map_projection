#include <cmath>

#include "R2Graph/R2Graph.h"
#include "R3Graph/R3Graph.h"

const double EARTH_RADIUS = 6378000.0;
const double PI = 3.14159265358979323846;
const double TO_RAD = PI / 180.0;
const double TO_DEG = 1 / TO_RAD;

using namespace std;
using namespace R3Graph;

R2Point getMapProj(const R2Point& map, const R2Point& point);
R2Point toLatLon(const R3Vector& v);

int main()
{
    R2Point m, p;
    while (true)
    {
        cout << "Enter map position (lat, lon) and point on the map (x, y):" << endl;
        cin >> m >> p;
        if (cin.bad())
            break;

        R2Point pointProj = getMapProj(m, p);
        cout << "Point projection on the Earth surface: " << pointProj << endl;
    }

    return 0;
}

R2Point getMapProj(const R2Point& map, const R2Point& point)
{
    // latitude (b) and longitude (a) of the map center
    double b = map.x * TO_RAD, a = map.y * TO_RAD;

    // projections of the point on the map axes
    double rx = point.x, ry = point.y;

    // map center vector
    R3Vector m = EARTH_RADIUS * R3Vector(cos(b) * cos(a), 
                                         cos(b) * sin(a), 
                                         sin(b));

    // vector of the given point on the map, in Earth basis
    R3Vector vm = R3Vector(-rx * sin(a) - ry * sin(b) * cos(a), 
                            rx * cos(a) - ry * sin(b) * sin(a), 
                            ry * cos(b));

    // point vector, in Earth basis
    R3Vector v = m + vm;
    
    // scale vector to the Earth surface
    R3Vector vn = EARTH_RADIUS * v.normalized();

    return toLatLon(vn);
}

R2Point toLatLon(const R3Vector& v)
{
    // define basis vector of Z axis
    R3Vector ez = R3Vector(0, 0, 1);

    // determine latitude and longitude of the desired point
    double lat = 0.5 * PI - v.angle(ez);
    double lon = atan2(v.y, v.x);
    
    return R2Point(lat, lon) * TO_DEG;
}
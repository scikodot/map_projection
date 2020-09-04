#include <cmath>

#include "R2Graph/R2Graph.h"
#include "R3Graph/R3Graph.h"

const double EARTH_RADIUS = 6378000.0;
const double PI = 3.14159265358979323846;

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
    // define map in the initial state, at the (0, 0) point on the Earth surface
    R3Vector v = R3Vector(EARTH_RADIUS, point.x, point.y);  // point position on the map in Earth basis
    R3Vector vn = EARTH_RADIUS * v.normalized();  // scale vector to the Earth surface

    // convert point radius-vector to (lat, lon) position on Earth surface,
    // and move it by the map offset from the (0, 0) point
    return toLatLon(vn) + map;
}

R2Point toLatLon(const R3Vector& v)
{
    // define basis vectors
    R3Vector ex = R3Vector(1, 0, 0), 
             ey = R3Vector(0, 1, 0), 
             ez = R3Vector(0, 0, 1);

    // determine latitude and longitude of the desired point
    double lat = 0.5 * PI - v.angle(ez);
    double lon = atan2(v.y, v.x);
    
    return 180.0 / PI * R2Point(lat, lon);
}
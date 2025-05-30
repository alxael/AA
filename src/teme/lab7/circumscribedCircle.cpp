#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <iomanip>

using namespace std;

using Point = pair<long long, long long>;
const double eps = 1e-9;

double distanceSquared(const Point &p1, const Point &p2)
{
    double dx = p1.first - p2.first;
    double dy = p1.second - p2.second;
    return dx * dx + dy * dy;
}

double distance(const Point &p1, const Point &p2)
{
    return sqrt(distanceSquared(p1, p2));
}

string insideCircumscribedCircle(const Point &p1, const Point &p2, const Point &p3, const Point &p)
{
    double a = distance(p1, p2);
    double b = distance(p1, p3);
    double c = distance(p2, p3);

    double det = 2 * (p1.first * (p2.second - p3.second) + p2.first * (p3.second - p1.second) + p3.first * (p1.second - p2.second));
    if (fabs(det) < eps)
        return "DEGENERATE_TRIANGLE";

    double xp = ((p1.first * p1.first + p1.second * p1.second) * (p2.second - p3.second) +
                 (p2.first * p2.first + p2.second * p2.second) * (p3.second - p1.second) +
                 (p3.first * p3.first + p3.second * p3.second) * (p1.second - p2.second)) /
                det;

    double yp = ((p1.first * p1.first + p1.second * p1.second) * (p3.first - p2.first) +
                 (p2.first * p2.first + p2.second * p2.second) * (p1.first - p3.first) +
                 (p3.first * p3.first + p3.second * p3.second) * (p2.first - p1.first)) /
                det;

    double s = (a + b + c) / 2;
    double area = sqrt(s * (s - a) * (s - b) * (s - c));
    if (fabs(area) < eps)
        return "DEGENRATE_TRIANGLE";
    double radius = (a * b * c) / (4 * area);

    double dx = p.first - xp;
    double dy = p.second - yp;
    double distanceSquared = dx * dx + dy * dy;
    double radiusSquared = radius * radius;

    if (fabs(distanceSquared - radiusSquared) < eps)
        return "BOUNDARY";
    else if (distanceSquared < radiusSquared)
        return "INSIDE";
    else
        return "OUTSIDE";
}

int main()
{
    Point p1, p2, p3, p;
    int pointCount;

    cin >> p1.first >> p1.second >> p2.first >> p2.second >> p3.first >> p3.second;
    cin >> pointCount;

    for (int index = 1; index <= pointCount; index++)
    {
        cin >> p.first >> p.second;
        cout << insideCircumscribedCircle(p1, p2, p3, p) << '\n';
    }
    return 0;
}

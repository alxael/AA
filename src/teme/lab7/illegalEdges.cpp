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

int insideCircumscribedCircle(const Point &p1, const Point &p2, const Point &p3, const Point &p)
{
    double a = distance(p1, p2);
    double b = distance(p1, p3);
    double c = distance(p2, p3);

    double det = 2 * (p1.first * (p2.second - p3.second) + p2.first * (p3.second - p1.second) + p3.first * (p1.second - p2.second));
    if (fabs(det) < eps)
        return 0;

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
        return 0;
    double radius = (a * b * c) / (4 * area);

    double dx = p.first - xp;
    double dy = p.second - yp;
    double distanceSquared = dx * dx + dy * dy;
    double radiusSquared = radius * radius;

    if (distanceSquared < radiusSquared)
        return 1; // INSIDE
    else
        return -1; // OUTSIDE
}

int main()
{
    Point p1, p2, p3, p4;

    cin >> p1.first >> p1.second >> p2.first >> p2.second;
    cin >> p3.first >> p3.second >> p4.first >> p4.second;

    int p1p3 = insideCircumscribedCircle(p1, p2, p3, p4);
    int p2p4 = insideCircumscribedCircle(p2, p3, p4, p1);

    cout << "AC: " << (p1p3 == 1 ? "ILLEGAL" : "LEGAL") << '\n';
    cout << "BD: " << (p2p4 == 1 ? "ILLEGAL" : "LEGAL") << '\n';
    return 0;
}

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

using Point = pair<long long, long long>;
vector<Point> points;
long long INF;

long long orientation(Point p1, Point p2, Point p3)
{
    return (p2.first - p1.first) * (p3.second - p2.second) - (p2.second - p1.second) * (p3.first - p2.first);
}
bool isPointOnSegment(Point p1, Point p2, Point p3)
{
    return orientation(p1, p2, p3) == 0 &&
           min(p1.first, p2.first) <= p3.first &&
           p3.first <= max(p1.first, p2.first) &&
           min(p1.second, p2.second) <= p3.second &&
           p3.second <= max(p1.second, p2.second);
}
bool doPointsIntersect(Point p1, Point p2, Point p3, Point p4)
{
    int d1 = orientation(p1, p2, p3) > 0;
    int d2 = orientation(p1, p2, p4) > 0;
    int d3 = orientation(p3, p4, p1) > 0;
    int d4 = orientation(p3, p4, p2) > 0;
    if (d1 != d2 && d3 != d4)
        return 1;

    return (isPointOnSegment(p1, p2, p3) && d1 == 0) ||
           (isPointOnSegment(p1, p3, p4) && d2 == 0) ||
           (isPointOnSegment(p3, p4, p1) && d3 == 0) ||
           (isPointOnSegment(p3, p4, p2) && d4 == 0);
}
int pointsInsideCount(Point p)
{
    int pointsCount = points.size();
    for (int index = 0; index < pointsCount; index++)
        if (isPointOnSegment(points[index], points[(index + 1) % pointsCount], p))
            return -1;

    Point auxPoint = {INF + 1, p.second + 1};

    int count = 0;
    for (int index = 0; index < pointsCount; index++)
    {
        if (doPointsIntersect(points[index], points[(index + 1) % pointsCount], p, auxPoint))
            count++;
    }
    return count;
}

int main()
{
    int pointsCount;
    Point point;

    cin >> pointsCount;
    cin >> point.first >> point.second;
    INF = point.first;
    points.push_back(point);

    for (int index = 1; index < pointsCount; index++)
    {
        cin >> point.first >> point.second;
        INF = max(INF, point.first);
        points.push_back(point);
    }

    cin >> pointsCount;
    for (int index = 0; index < pointsCount; index++)
    {
        cin >> point.first >> point.second;
        int result = pointsInsideCount(point);
        if (result == -1)
            cout << "BOUNDARY" << endl;
        else
        {
            if (result % 2)
                cout << "INSIDE" << endl;
            else
                cout << "OUTSIDE" << endl;
        }
    }
    return 0;
}
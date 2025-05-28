#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

using Point = pair<long long, long long>;
Point start;

long long orientation(const Point &p1, const Point &p2, const Point &p3)
{
    return (p2.first - p1.first) * (p3.second - p2.second) - (p2.second - p1.second) * (p3.first - p2.first);
}

int pointOnSegment(const Point &p1, const Point &p2, const Point &p3)
{
    if (orientation(p1, p2, p3) == 0)
    {
        if (
            min(p1.first, p2.first) <= p3.first && p3.first <= max(p1.first, p2.first) &&
            min(p1.second, p2.second) <= p3.second && p3.second <= max(p1.second, p2.second))
        {
            return 1;
        }
    }
    return 0;
}
bool intersection(const Point &p1, const Point &p2, const Point &p3, const Point &p4)
{
    int det1 = orientation(p1, p2, p3) > 0;
    int det2 = orientation(p1, p2, p4) > 0;
    int det3 = orientation(p3, p4, p1) > 0;
    int det4 = orientation(p3, p4, p2) > 0;

    if (det1 != det2 && det3 != det4)
        return 1;

    if (
        (pointOnSegment(p1, p2, p3) && det1 == 0) ||
        (pointOnSegment(p1, p2, p4) && det2 == 0) ||
        (pointOnSegment(p3, p4, p1) && det3 == 0) ||
        (pointOnSegment(p3, p4, p2) && det4 == 0))
    {
        return 1;
    }

    return 0;
}

vector<Point> points;
long long maxX = -1e18;

int insidePolygon(const Point &p)
{
    for (int index = 0; index < points.size(); index++)
        if (pointOnSegment(points[index], points[(index + 1) % points.size()], p))
            return -1;

    Point point = {maxX + 1, p.second + 1};

    int cnt = 0;
    for (int index = 0; index < points.size(); index++)
        if (intersection(points[index], points[(index + 1) % points.size()], p, point))
            cnt++;

    return cnt;
}

signed main()
{
    int pointCount, testCount;
    Point point;

    cin >> pointCount;
    for (int index = 0; index < pointCount; index++)
    {
        cin >> point.first >> point.second;
        points.push_back(point);
        maxX = max(maxX, point.first);
    }

    cin >> testCount;
    for (int index = 0; index < testCount; index++)
    {
        cin >> point.first >> point.second;
        int answer = insidePolygon(point);
        if (answer == -1)
            cout << "BOUNDARY";
        else
        {
            if (answer % 2)
                cout << "INSIDE";
            else
                cout << "OUTSIDE";
        }
        cout << endl;
    }
    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

using Point = pair<long long, long long>;
Point start;

long long orientation(Point p1, Point p2, Point p3)
{
    return (p2.first - p1.first) * (p3.second - p2.second) - (p2.second - p1.second) * (p3.first - p2.first);
}
bool compare(const Point &p1, const Point &p2)
{
    return ((p1.first < p2.first) || (p1.first == p2.first && p1.second < p2.second));
}
bool compare_convex(const Point &p1, const Point &p2)
{
    long long value = orientation(start, p1, p2);
    if (value != 0)
        return value > 0;
    long long dx1 = p1.first - start.first, dy1 = p1.second - start.second;
    long long dx2 = p2.first - start.first, dy2 = p2.second - start.second;
    return (dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2);
}

int main()
{
    int pointCount;
    vector<Point> points;
    Point point;

    cin >> pointCount;
    for (int index = 1; index <= pointCount; index++)
    {
        cin >> point.first >> point.second;
        points.push_back(point);
    }

    sort(points.begin(), points.end(), compare);
    start = points[0];

    sort(points.begin() + 1, points.end(), compare_convex);

    vector<Point> stack;
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    for (int index = 2; index < points.size(); index++)
    {
        while (stack.size() >= 2 && orientation(stack[stack.size() - 2], stack[stack.size() - 1], points[index]) <= 0)
            stack.pop_back();
        stack.push_back(points[index]);
    }
    cout << stack.size() << endl;
    for (auto value : stack)
        cout << value.first << ' ' << value.second << endl;
    return 0;
}
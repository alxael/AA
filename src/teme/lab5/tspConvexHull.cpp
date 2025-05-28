#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <map>

using namespace std;

const long long INF = 1e18;
const double eps = 1e-9;
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
bool compareConvex(const Point &p1, const Point &p2)
{
    long long value = orientation(start, p1, p2);
    if (value != 0)
        return value > 0;
    long long dx1 = p1.first - start.first, dy1 = p1.second - start.second;
    long long dx2 = p2.first - start.first, dy2 = p2.second - start.second;
    return (dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2);
}

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

double getBestEdge(const Point &p1, const Point &p2, const Point &p3)
{
    return distance(p1, p3) + distance(p3, p2) - distance(p1, p2);
}

double calculateGainRatio(const Point &p1, const Point &p2, const Point &p3)
{
    double d = distance(p1, p2);
    if (d < eps)
        return INF;
    return (distance(p1, p3) + distance(p3, p2)) / d;
}

int main()
{
    int pointCount;
    Point point;

    cin >> pointCount;

    vector<Point> points;
    vector<double> bestGain(pointCount + 5, 0);
    vector<int> bestEdge(pointCount + 5, 0);

    for (int index = 0; index < pointCount; index++)
    {
        cin >> point.first >> point.second;
        points.push_back(point);
    }

    sort(points.begin(), points.end(), compare);
    start = points[0];

    sort(points.begin() + 1, points.end(), compareConvex);

    vector<pair<Point, int>> stack;
    stack.push_back(make_pair(points[0], 0));
    stack.push_back(make_pair(points[1], 1));
    for (int index = 2; index < pointCount; index++)
    {
        while (stack.size() >= 2 && orientation(stack[stack.size() - 2].first, stack[stack.size() - 1].first, points[index]) <= 0)
            stack.pop_back();
        stack.push_back(make_pair(points[index], index));
    }

    map<int, bool> inHull;
    vector<int> remaining;
    stack.push_back(stack[0]);

    for (auto value : stack)
        inHull[value.second] = true;

    for (int index = 0; index < pointCount; index++)
        if (inHull.find(index) == inHull.end())
            remaining.push_back(index);

    stack.pop_back();
    for (auto remainingIndex : remaining)
    {
        bestGain[remainingIndex] = INF;
        Point p3 = points[remainingIndex];

        for (int index = 0; index < stack.size(); index++)
        {
            int nextIndex = (index + 1) % stack.size();
            Point p1 = stack[index].first, p2 = stack[nextIndex].first;

            double gain = getBestEdge(p1, p2, p3);
            if (gain < bestGain[remainingIndex])
            {
                bestGain[remainingIndex] = gain;
                bestEdge[remainingIndex] = index;
            }
        }
    }

    while (!remaining.empty())
    {
        int bestRemaining = -1;
        double minRatio = INF;

        for (auto remainingIndex : remaining)
        {
            int index = bestEdge[remainingIndex];
            int nextIndex = (index + 1) % stack.size();
            Point p1 = stack[index].first, p2 = stack[nextIndex].first, p3 = points[remainingIndex];

            double gainRatio = calculateGainRatio(p1, p2, p3);
            if (gainRatio < minRatio)
            {
                minRatio = gainRatio;
                bestRemaining = remainingIndex;
            }
            else if (fabs(gainRatio - minRatio) < eps && bestRemaining != -1)
            {
                double currentDistance = distance(p1, p3) + distance(p3, p2);
                int previousIndex = bestEdge[bestRemaining];
                int previousNextIndex = (previousIndex + 1) % stack.size();
                double previousDistance = distance(stack[previousIndex].first, points[bestRemaining]) + distance(points[bestRemaining], stack[previousNextIndex].first);
                if (currentDistance < previousDistance)
                {
                    minRatio = gainRatio;
                    bestRemaining = remainingIndex;
                }
            }
        }

        int index = bestEdge[bestRemaining];

        stack.insert(stack.begin() + index + 1, make_pair(points[bestRemaining], bestRemaining));
        remaining.erase(find(remaining.begin(), remaining.end(), bestRemaining));

        for (auto remainingIndex : remaining)
        {
            bestGain[remainingIndex] = INF;
            Point p3 = points[remainingIndex];

            for (int index = 0; index < stack.size(); index++)
            {
                int nextIndex = (index + 1) % stack.size();
                Point p1 = stack[index].first, p2 = stack[index].first;

                double gain = getBestEdge(p1, p2, p3);
                if (gain < bestGain[remainingIndex])
                {
                    bestGain[remainingIndex] = gain;
                    bestEdge[remainingIndex] = index;
                }
            }
        }
    }

    stack.push_back(stack[0]);

    int bottomLeftIndex = 0;
    for (int index = 1; index < stack.size(); index++)
    {
        if (stack[index].first.first < stack[bottomLeftIndex].first.first ||
            (stack[index].first.first == stack[bottomLeftIndex].first.first &&
             stack[index].first.second < stack[bottomLeftIndex].first.second))
        {
            bottomLeftIndex = index;
        }
    }

    if (bottomLeftIndex > 0)
    {
        vector<pair<Point, int>> tour;
        for (int index = bottomLeftIndex; index < stack.size(); index++)
            tour.push_back(stack[index]);
        for (int index = 1; index <= bottomLeftIndex; index++)
            tour.push_back(stack[index]);
        stack = tour;
    }

    for (auto value : stack)
        cout << value.first.first << ' ' << value.first.second << endl;

    return 0;
}
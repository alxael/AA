#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int orientation(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3)
{
    return (p2.first - p1.first) * (p3.second - p2.second) - (p2.second - p1.second) * (p3.first - p2.first);
}

int main()
{
    vector<pair<int, int>> points;
    pair<long, long> point;
    int pointCount;

    cin >> pointCount;
    for (int index = 0; index < pointCount; index++)
    {
        cin >> point.first >> point.second;
        points.push_back(point);
    }
    points.push_back(points[0]);

    int leftCount = 0, rightCount = 0, straightCount = 0;
    for (int index = 1; index < pointCount; index++)
    {
        int value = orientation(points[index - 1], points[index + 1], points[index]);
        if (value == 0)
            straightCount++;
        else if (value < 0)
            leftCount++;
        else
            rightCount++;
    }

    cout << leftCount << ' ' << rightCount << ' ' << straightCount;
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>

using namespace std;

using Point = pair<long long, long long>;

bool isXMonotone(const vector<Point> &polygon)
{
    if (polygon.size() < 3)
        return true;
    int leftmost = 0, rightmost = 0;
    for (int index = 1; index < polygon.size(); index++)
    {
        if (polygon[index].first < polygon[leftmost].first ||
            (polygon[index].first == polygon[leftmost].first && polygon[index].second < polygon[leftmost].second))
            leftmost = index;
        if (polygon[index].first > polygon[rightmost].first ||
            (polygon[index].first == polygon[rightmost].first && polygon[index].second > polygon[rightmost].second))
            rightmost = index;
    }
    if (leftmost == rightmost)
        return true;
    vector<Point> upperChain;
    int current = leftmost;
    do
    {
        upperChain.push_back(polygon[current]);
        current = (current + 1) % polygon.size();
    } while (current != rightmost);
    upperChain.push_back(polygon[rightmost]);
    for (size_t index = 1; index < upperChain.size(); index++)
        if (upperChain[index].first < upperChain[index - 1].first)
            return false;
    vector<Point> lowerChain;
    current = rightmost;
    do
    {
        lowerChain.push_back(polygon[current]);
        current = (current + 1) % polygon.size();
    } while (current != leftmost);
    lowerChain.push_back(polygon[leftmost]);
    for (size_t index = 1; index < lowerChain.size(); index++)
        if (lowerChain[index].first > lowerChain[index - 1].first)
            return false;
    return true;
}

bool isYMonotone(const vector<Point> &polygon)
{
    if (polygon.size() < 3)
        return true;
    int topmost = 0, bottommost = 0;
    for (int index = 1; index < polygon.size(); index++)
    {
        if (polygon[index].second > polygon[topmost].second ||
            (polygon[index].second == polygon[topmost].second && polygon[index].first < polygon[topmost].first))
            topmost = index;
        if (polygon[index].second < polygon[bottommost].second ||
            (polygon[index].second == polygon[bottommost].second && polygon[index].first > polygon[bottommost].first))
            bottommost = index;
    }
    if (topmost == bottommost)
        return true;
    vector<Point> leftChain;
    int current = topmost;
    do
    {
        leftChain.push_back(polygon[current]);
        current = (current + 1) % polygon.size();
    } while (current != bottommost);
    leftChain.push_back(polygon[bottommost]);
    for (size_t index = 1; index < leftChain.size(); index++)
        if (leftChain[index].second > leftChain[index - 1].second)
            return false;
    vector<Point> rightChain;
    current = bottommost;
    do
    {
        rightChain.push_back(polygon[current]);
        current = (current + 1) % polygon.size();
    } while (current != topmost);
    rightChain.push_back(polygon[topmost]);
    for (size_t index = 1; index < rightChain.size(); index++)
        if (rightChain[index].second < rightChain[index - 1].second)
            return false;
    return true;
}

int main()
{
    int pointCount;
    vector<Point> polygon;
    Point point;

    cin >> pointCount;
    for (int index = 1; index <= pointCount; index++)
    {
        cin >> point.first >> point.second;
        polygon.push_back(point);
    }

    cout << (isXMonotone(polygon) ? "YES" : "NO") << '\n';
    cout << (isYMonotone(polygon) ? "YES" : "NO") << '\n';
    return 0;
}

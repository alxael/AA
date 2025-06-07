#include <iostream>
#include <vector>
using namespace std;

using Point = pair<int, int>;

bool nonDecreasing(const vector<Point> &chain, int coordIndex)
{
    for (size_t i = 0; i + 1 < chain.size(); i++)
    {
        int curr = (coordIndex == 0) ? chain[i].first : chain[i].second;
        int next = (coordIndex == 0) ? chain[i + 1].first : chain[i + 1].second;
        if (curr > next)
            return false;
    }
    return true;
}

bool isMonotone(const vector<Point> &points, int coordIndex)
{
    int n = (int)points.size();
    int minIndex = 0, maxIndex = 0;
    for (int i = 1; i < n; i++)
    {
        if ((coordIndex == 0 && points[i].first < points[minIndex].first) ||
            (coordIndex == 1 && points[i].second < points[minIndex].second))
        {
            minIndex = i;
        }
        if ((coordIndex == 0 && points[i].first > points[maxIndex].first) ||
            (coordIndex == 1 && points[i].second > points[maxIndex].second))
        {
            maxIndex = i;
        }
    }

    vector<Point> chain1;
    for (int i = minIndex; i != maxIndex; i = (i + 1) % n)
    {
        chain1.push_back(points[i]);
    }

    vector<Point> chain2;
    for (int i = minIndex; i != maxIndex; i = (i - 1 + n) % n)
    {
        chain2.push_back(points[i]);
    }

    return nonDecreasing(chain1, coordIndex) && nonDecreasing(chain2, coordIndex);
}

int main()
{
    int n;
    Point point;

    cin >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; i++)
    {
        cin >> points[i].first >> points[i].second;
    }

    cout << (isMonotone(points, 0) ? "YES" : "NO") << '\n';
    cout << (isMonotone(points, 1) ? "YES" : "NO") << '\n';

    return 0;
}
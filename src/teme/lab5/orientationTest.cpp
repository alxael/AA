#include <iostream>
#include <utility>
using namespace std;

using Point = pair<long long, long long>;

long long orientation(Point p1, Point p2, Point p3)
{
    return (p2.first - p1.first) * (p3.second - p2.second) - (p2.second - p1.second) * (p3.first - p2.first);
}

int main()
{
    int testCount;
    Point p1, p2, p3;

    cin >> testCount;

    for (int testIndex = 1; testIndex <= testCount; testIndex++)
    {
        cin >> p1.first >> p1.second >> p2.first >> p2.second >> p3.first >> p3.second;

        long long result = orientation(p1, p2, p3);

        if (result == 0)
            cout << "TOUCH";
        else if (result > 0)
            cout << "LEFT";
        else
            cout << "RIGHT";

        cout << endl;
    }
    return 0;
}
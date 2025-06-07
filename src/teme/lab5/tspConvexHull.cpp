#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

using Point = pair<long long, long long>;

double dist(const Point &p1, const Point &p2)
{
    double dx = p1.first - p2.first, dy = p1.second - p2.second;
    return sqrt(dx * dx + dy * dy);
}

double tourDistance(const vector<Point> &t)
{
    double d = 0;
    int n = t.size();
    for (int i = 0; i < n; i++)
        d += dist(t[i], t[(i + 1) % n]);
    return d;
}

void twoOpt(vector<Point> &t)
{
    int n = t.size();
    bool improved = true;
    while (improved)
    {
        improved = false;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 2; j < n; j++)
            {
                if (i == 0 && j == n - 1)
                    continue;
                double d1 = dist(t[i], t[i + 1]) + dist(t[j], t[(j + 1) % n]);
                double d2 = dist(t[i], t[j]) + dist(t[i + 1], t[(j + 1) % n]);
                if (d2 + 1e-9 < d1)
                {
                    reverse(t.begin() + i + 1, t.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
}

int main()
{
    int n;
    cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; i++)
        cin >> pts[i].first >> pts[i].second;

    double bestD = 1e300;
    vector<Point> bestTour;
    int tries = (n <= 200 ? n : 10);

    for (int seed = 0; seed < tries; seed++)
    {
        vector<bool> used(n, false);
        vector<Point> tour;
        tour.reserve(n);
        int cur = seed;
        tour.push_back(pts[cur]);
        used[cur] = true;
        for (int step = 1; step < n; step++)
        {
            int nxt = -1;
            double bd = 1e300;
            for (int j = 0; j < n; j++)
                if (!used[j])
                {
                    double d = dist(tour.back(), pts[j]);
                    if (d < bd)
                    {
                        bd = d;
                        nxt = j;
                    }
                }
            tour.push_back(pts[nxt]);
            used[nxt] = true;
        }

        twoOpt(tour);
        double d = tourDistance(tour);
        if (d < bestD)
        {
            bestD = d;
            bestTour = tour;
        }
    }

    int start = 0;
    for (int i = 1; i < n; i++)
    {
        if (bestTour[i].first < bestTour[start].first ||
            (bestTour[i].first == bestTour[start].first &&
             bestTour[i].second < bestTour[start].second))
            start = i;
    }

    for (int i = 0; i < n; i++)
    {
        auto &p = bestTour[(start + i) % n];
        cout << p.first << " " << p.second << "\n";
    }
    cout << bestTour[start].first << " " << bestTour[start].second << "\n";

    return 0;
}

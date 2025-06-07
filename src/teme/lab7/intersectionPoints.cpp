#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using Point = pair<int, int>;
using Event = tuple<int, int, int, int>;

class AIB
{
private:
    vector<int> bits;

public:
    AIB(int n) : bits(n + 1, 0) {}
    void update(int index, int value)
    {
        while (index < (int)bits.size())
        {
            bits[index] += value;
            index += index & (-index);
        }
    }
    int sum(int index)
    {
        int answer = 0;
        while (index > 0)
        {
            answer += bits[index];
            index -= index & (-index);
        }
        return answer;
    }

    int rangeSum(int left, int right)
    {
        if (left > right)
            return 0;
        return sum(right) - sum(left - 1);
    }
};

bool compare(const Event &e1, const Event &e2)
{
    if (get<0>(e1) != get<0>(e2))
        return get<0>(e1) < get<0>(e2);
    return get<1>(e1) < get<1>(e2);
}

int getYId(const vector<int> &ys, int y)
{
    return int(lower_bound(ys.begin(), ys.end(), y) - ys.begin()) + 1;
}

int main()
{
    int n;
    Point p1, p2;

    cin >> n;
    vector<pair<Point, Point>> segments;
    for (int i = 0; i < n; i++)
    {
        cin >> p1.first >> p1.second >> p2.first >> p2.second;
        segments.push_back(make_pair(p1, p2));
    }

    vector<Event> events;
    vector<int> ys;
    for (auto &[p1, p2] : segments)
    {
        if (p1.second == p2.second)
        {
            if (p1.first > p2.first)
                swap(p1.first, p2.first);

            events.emplace_back(p1.first, 0, p1.second, p1.second);
            events.emplace_back(p2.first, 2, p1.second, p1.second);
            ys.push_back(p1.second);
        }
        else
        {
            if (p1.second > p2.second)
                swap(p1.second, p2.second);
            events.emplace_back(p1.first, 1, p1.second, p2.second);
            ys.push_back(p1.second);
            ys.push_back(p2.second);
        }
    }

    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    sort(events.begin(), events.end(), compare);

    AIB aib((int)ys.size());
    long long result = 0;

    for (auto &[x, t, first, second] : events)
    {
        if (t == 0)
            aib.update(getYId(ys, second), 1);
        else if (t == 2)
            aib.update(getYId(ys, second), -1);
        else
        {
            int left = getYId(ys, second) + 1;
            int right = getYId(ys, second) - 1;
            if (left <= right)
                result += aib.rangeSum(left, right);
        }
    }

    cout << result << "\n";

    return 0;
}
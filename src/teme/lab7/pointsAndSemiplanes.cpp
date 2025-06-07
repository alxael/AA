#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<double> left, right, down, up;

    for (int i = 0; i < n; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == 0)
        {
            double y0 = -1.0 * c / b;
            if (b > 0)
                up.push_back(y0);
            else
                down.push_back(y0);
        }
        else
        {
            double x0 = -1.0 * c / a;
            if (a > 0)
                right.push_back(x0);
            else
                left.push_back(x0);
        }
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    sort(down.begin(), down.end());
    sort(up.begin(), up.end());

    int m;
    cin >> m;
    cout << fixed << setprecision(6);

    while (m--)
    {
        double x, y;
        cin >> x >> y;

        auto itLeft = lower_bound(left.begin(), left.end(), x);
        if (itLeft == left.begin())
        {
            cout << "NO" << '\n';
            continue;
        }
        --itLeft;
        double leftValue = *itLeft;

        auto itRight = upper_bound(right.begin(), right.end(), x);
        if (itRight == right.end())
        {
            cout << "NO" << '\n';
            continue;
        }
        double rightValue = *itRight;

        auto itDown = lower_bound(down.begin(), down.end(), y);
        if (itDown == down.begin())
        {
            cout << "NO" << '\n';
            continue;
        }
        --itDown;
        double downValue = *itDown;

        auto itUp = upper_bound(up.begin(), up.end(), y);
        if (itUp == up.end())
        {
            cout << "NO" << '\n';
            continue;
        }
        double upValue = *itUp;

        if (leftValue < x && x < rightValue && downValue < y && y < upValue)
            cout << "YES" << '\n'
                 << (rightValue - leftValue) * (upValue - downValue) << '\n';
        else
            cout << "NO" << '\n';
    }

    return 0;
}
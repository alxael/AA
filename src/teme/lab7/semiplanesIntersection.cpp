#include <iostream>
#include <limits>

using namespace std;

const double LIMIT = numeric_limits<double>::infinity();

int main()
{
    int n;
    cin >> n;
    double xMin = -LIMIT, xMax = LIMIT;
    double yMin = -LIMIT, yMax = LIMIT;

    for (int i = 0; i < n; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == 0)
        {
            double yVal = -1.0 * c / b;
            if (b > 0)
            {
                if (yVal < yMax)
                    yMax = yVal;
            }
            else
            {
                if (yVal > yMin)
                    yMin = yVal;
            }
        }
        else
        {
            double xVal = -1.0 * c / a;
            if (a > 0)
            {
                if (xVal < xMax)
                    xMax = xVal;
            }
            else
            {
                if (xVal > xMin)
                    xMin = xVal;
            }
        }
    }

    if (xMin > xMax || yMin > yMax)
        cout << "VOID" << '\n';
    else if (xMin != -LIMIT && xMax != LIMIT && yMin != -LIMIT && yMax != LIMIT)
        cout << "BOUNDED" << '\n';
    else
        cout << "UNBOUNDED" << '\n';

    return 0;
}
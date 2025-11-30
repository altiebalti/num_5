// Лабораторна робота номер 5, варіант номер 3 
// виконав студент групи ТК-31
// Тимченко Олександр

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

struct SplineTuple {
    double a, b, c, d, x;
};

double f(double x) {
    double ch = cosh(2 * x);
    return 8.0 / (ch * ch);
}

vector<SplineTuple> buildSpline(const vector<double>& x, const vector<double>& y, int n) {
    vector<SplineTuple> splines(n);

    for (int i = 0; i < n; ++i) {
        splines[i].x = x[i];
        splines[i].a = y[i];
    }

    vector<double> h(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        h[i] = x[i + 1] - x[i];
    }

    vector<double> alpha(n - 1);
    vector<double> l(n + 1);
    vector<double> mu(n);
    vector<double> z(n + 1);

    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < n - 1; ++i) {
        l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (3.0 * (splines[i + 1].a - splines[i].a) / h[i] - 
                3.0 * (splines[i].a - splines[i - 1].a) / h[i - 1]) / l[i] - 
                (h[i - 1] * z[i - 1]) / l[i];
    }

    l[n - 1] = 1.0;
    z[n - 1] = 0.0;
    splines[n - 1].c = 0.0;

    for (int j = n - 2; j >= 0; --j) {
        splines[j].c = z[j] - mu[j] * splines[j + 1].c;
        splines[j].b = (splines[j + 1].a - splines[j].a) / h[j] - 
                       h[j] * (splines[j + 1].c + 2.0 * splines[j].c) / 3.0;
        splines[j].d = (splines[j + 1].c - splines[j].c) / (3.0 * h[j]);
    }

    return splines;
}

int main() {
    cout << fixed << setprecision(4);

    double start = -4.0;
    double end = 4.0;
    int n;

    cout << "Enter the number of interpolation points (for example, 10): ";
    cin >> n;

    if (n < 2) {
        cout << "Error: the number of points must be >= 2." << endl;
        return 1;
    }

    vector<double> x(n), y(n);
    double step = (end - start) / (n - 1);

    cout << "\n--- Interpolation Points ---\n";
    cout << "x\t\tf(x)\n";
    for (int i = 0; i < n; ++i) {
        x[i] = start + i * step;
        y[i] = f(x[i]);
        cout << x[i] << "\t" << y[i] << endl;
    }

    vector<SplineTuple> spline = buildSpline(x, y, n);

    cout << "\n--- Spline Coefficients ---\n";
    cout << "i\tXi\ta_i\tb_i\tc_i\td_i\n";
    for (int i = 0; i < n - 1; ++i) {
        cout << i << "\t" 
             << spline[i].x << "\t"
             << spline[i].a << "\t"
             << spline[i].b << "\t"
             << spline[i].c << "\t"
             << spline[i].d << endl;
    }

    ofstream outFile("spline_data.csv");
    outFile << "x,original_f(x),spline_S(x)\n";
    
    double plotStep = 0.05; 
    for (double currX = start; currX <= end; currX += plotStep) {
        double originalY = f(currX);
        double splineY = 0;

        int i = 0;
        for (int k = 0; k < n - 1; ++k) {
            if (currX >= x[k]) {
                i = k;
            }
        }
        if (currX >= x[n-1]) i = n - 2;

        double dx = currX - spline[i].x;
        splineY = spline[i].a + spline[i].b * dx + spline[i].c * dx * dx + spline[i].d * dx * dx * dx;

        outFile << currX << "," << originalY << "," << splineY << "\n";
    }
    
    outFile.close();
    cout << "\nData for the graph has been saved to the file 'spline_data.csv'.\n";
    cout << "Open this file in Excel or Python for visualization.\n";

    return 0;
}
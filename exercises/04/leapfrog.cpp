#include <omp.h>

#include <cmath>
#include <iostream>
#include <new>

using namespace std;

#define N_X 500

int main() {
    int x_0; int t_0; int x_f; int t_f; int n_t;
    double delta_x; double delta_t;

    double* x = new double[N_X];
    double* u = new double[N_X];
    double* v = new double[N_X];
    double* a = new double[N_X];


#pragma omp parallel
    {
        x_0 = t_0 = 0;
        x_f = t_f = 10;
        delta_x = ((x_f - x_0) / (N_X - 1.0));
        delta_t = delta_x / 2;
        n_t = (int)((t_f - t_0) / delta_t) + 1;

#pragma omp parallel for
        for (int i = 0; i < N_X; i++) {
            x[i] = (x_0 + delta_x * i);
            u[i] = exp(-pow(x[i] - 5.0, 2));
        }
    }

    double delta_tl = 0.0;

    for (int t = 0; t < n_t; t++) {
        delta_tl = (t == t_0) ? delta_t / 2 : delta_t;

        for (int i = 0, j = 1; i < N_X; i++, j++) {
            if (j < N_X - 1) {
                a[j] = (u[j - 1] + u[j + 1] - 2 * u[j]) / pow(delta_x, 2);
            }

            v[i] += a[i] * delta_tl;
            u[i] += v[i] * delta_t;
        }
    }

#pragma omp paralellel for
    for (int i = N_X / 2 - 10; i < N_X / 2 + 10; i++) {
        cout << "x = " << x[i] << ", u = " << u[i] << endl;
    }

    delete[] x; delete[] u; delete[] v; delete[] a;

    return 0;
}
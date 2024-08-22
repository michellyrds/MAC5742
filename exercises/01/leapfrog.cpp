#include <cmath>
#include <iostream>

using namespace std;

int main(){
    const int n_x = 500;

    int x_0; int t_0;
    int x_f; int t_f;

    x_0 = t_0 = 0;
    x_f = t_f = 10;

    double delta_x = ((x_f - x_0)/(n_x - 1.0));
    double delta_t = delta_x/2;

    int n_t = (int) ((t_f - t_0)/ delta_t) + 1;

    double x[n_x] = {0.0};
    double u[n_x] = {0.0};

    for(int i = 0; i < n_x; i++){
        x[i] = (x_0 + delta_x*i);
        u[i] = exp(-pow(x[i]-5.0, 2));
    }

    double v[n_x]; double a[n_x];

    v[n_x] = {0.0};
    a[n_x] = {0.0};

    double delta_tl = 0.0;
    for(int t = 0; t < n_t; t++){
        delta_tl = delta_t;
        
        if(t == t_0){
            delta_tl/=2;
        };

        for(int i = 1; i < n_x-1; i++){
            a[i] = (u[i-1] + u[i+1] - 2*u[i])/pow(delta_x, 2);
        }

        for(int i = 0; i < n_x; i++){
            v[i] += a[i]*delta_tl;
            u[i] += v[i]*delta_t;

        }

        int start = n_x/2 - 10;
        int stop = n_x/2 + 10;

        for(int i = start; i < stop; i++){
            cout << "x = " << x[i] << ", u = " << u[i] << endl;
        }
    }
}
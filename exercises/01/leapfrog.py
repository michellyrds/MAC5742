import numpy as np

def compare(x):
    return -np.exp(-((x - 5)**2))


def compute():
    n_x = 500
    x_0 = t_0 = 0
    x_f = t_f = 10

    delta_x = (x_f - x_0)/(n_x - 1)
    delta_t = delta_x/2

    n_t = ((t_f - t_0)/ delta_t) + 1

    x, u = [0] * n_x, [0] * n_x
    for i in range(n_x):
        x[i] = (x_0 + delta_x*i)
        u[i] = np.exp(-((x[i] - 5)**2))


    v, a = [0] * n_x, [0] * n_x


    delta_tl = 0
    for t in range(int(n_t)):
        delta_tl = delta_t

        if t == t_0:
            delta_tl/=2

        for i in range(1, n_x-1):
            a[i] = (u[i-1] + u[i+1] - 2*u[i])/(delta_x**2)

        for i in range(n_x):
            v[i] = v[i] + a[i]*delta_tl
            u[i] = u[i] + v[i]*delta_t


    start=int(n_x/2 - 10)
    stop=int(n_x/2 + 10)
    for i in range(start, stop):
        print(f"x = {x[i]}, u = {u[i]}")

if __name__ == "__main__": 
    compute()

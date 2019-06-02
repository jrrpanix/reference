import matplotlib.pyplot as plt
import numpy as np

def PolynomialFit(x, y):
    # Fit using Vandermonde Matrix, Condition number can explode
    assert len(x) == len(y)
    N = len(x)
    A = np.ones((N,N))
    for i in range(1, N):
        A[:, i] = x
        x = x * x
    return np.linalg.solve(A, y), np.linalg.cond(A)

def PolynomialEval(p, x):
    # Evaluate Polynomial Fit, via Horners Rule
    # p - polynomial coefficients, x - xvalues
    i, v = len(p) - 1, None
    while i > 0:
        if v is None :
            v = p[i]*x
        else:
            v = x*(p[i] + v)
            i = i -1
    v += p[i]
    return v

def CubicFit(x, y):
    # cubic spline with continuity conditions
    d = len(x) -1
    a = np.zeros((4*d,4*d))
    s = np.zeros(4*d)
    r = 0
    for i in range(d):
        b=4*i
        a[r,b] = 1
        s[r] = y[r]
        r += 1
    b = (d-1)*4
    dk = x[d] - x[d-1]
    a[r, b:]= [1, dk, dk**2, dk**3]
    s[r] = y[r]

    r += 1
    for i in range(1,d):
        b = 4*(i-1)
        dx = x[i] - x[i-1]
        a[r,b:b+8] = [1, dx, dx**2, dx**3, -1, 0, 0, 0]
        a[r+1,b:b+8] = [0, 1, 2*dx, 3*dx**2, 0, -1, 0, 0]
        a[r+2,b:b+8] = [0, 0, 2, 6*dx, 0, 0, -2, 0]
        r += 3
    a[r,0:4] = [0, 0, 2, 0]
    b = 4*(d-1)
    a[r+1,b:b+4] = [0, 0, 2, 0]
    return np.linalg.solve(a,s), np.linalg.cond(a)

def CubicEval(x, c, xi):
    # Evaluate the cubic spline                                                                                                                                   
    #                                                                                                                                                             
    #    x - original x knot points                                                                                                                               
    #    c - vector of spline coefficits                                                                                                                          
    #   xi - x value being evaluated     
    def leval(x, c, xi, i):
        dx = xi - x[i-1]
        b = 4*(i-1)
        return c[b] + c[b+1]*dx + c[b+2]*dx**2 + c[b+3]*dx**3
    
    d = len(x) - 1
    for i in range(1,d):
        if xi < x[i]:
            return leval(x, c, xi, i)
    return leval(x, c, xi, d)




    



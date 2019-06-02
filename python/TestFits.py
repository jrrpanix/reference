import argparse
import matplotlib.pyplot as plt
import numpy as np

from fits import CubicFit, CubicEval, PolynomialFit, PolynomialEval

Fits = [(lambda x : np.exp(-0.5*x*x), r'$\exp{-\frac{1}{2}x^2}$'),
        (np.sin, r'$\sin x$')]


def PlotCubic(x,y, xr, yr):
    plt.plot(x,y, 'ro' , label="points")
    plt.plot(xr, yr, label="fit")
    plt.title('Cubic Spline')
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.legend()
    plt.show()

def CubicRun(x, y):
    xr = np.arange(x[0],x[-1]+0.1,0.1)
    c , cn = CubicFit(x,y)
    yr = np.array([CubicEval(x, c, xr[i]) for i in range(len(xr))])
    PlotCubic(x, y, xr, yr)

def CubicTest(FX):
    fX, fStr = FX[0], FX[1] 
    x = np.array([i/6 + i/12 for i in range(-12,12)])
    CubicRun(x, fX(x))

def SplineTest(x, y):
    CubicRun(np.array(x), np.array(y))


def PolynomialTest(FX):
    fX, fStr = FX[0], FX[1] 

    xp = np.arange(-2,2,0.01)
    yp = fX(xp)

    x = np.arange(-2,2,0.45)
    y = fX(x)
    p, cond = PolynomialFit(x,y)
    yhat = np.array([PolynomialEval(p, xp[i]) for i in range(0,len(xp))])

    plt.plot(xp, yp, label=fStr)
    plt.plot(xp, yhat,label=r'$fit$')
    plt.legend()
    plt.xlabel(r'$x$')
    plt.ylabel(r'$f(x)$')
    plt.title(fStr)
    plt.show()



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Various Mathematical Fits')
    parser.add_argument('-f','--fit', choices=['poly', 'cubic', 'spline'])
    parser.add_argument('-i','--ix', type=int, default=0)
    parser.add_argument('-x','--x', nargs='+', default=[-12,-6.1,-3, -0.5, 1.1, 4, 8,12])
    parser.add_argument('-y','--y', nargs='+', default=[3,-14, 10, 0, 16,-2, 12, -2])
    args = parser.parse_args()
    ix = args.ix

    if args.fit == 'poly':
        PolynomialTest(Fits[ix])
    elif args.fit == 'cubic':
        CubicTest(Fits[ix])
    elif args.fit == 'spline':
        x, y  = [float(x) for x in args.x], [float(y) for y in args.y]
        asset len(x) == len(y)
        SplineTest(x, y)


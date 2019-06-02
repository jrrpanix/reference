import matplotlib.pyplot as plt
import numpy as np
from fits import CubicFit, CubicEval
import argparse

#
# Fit some yield curves from 2019
#


data = np.array([[20190301,2.44,2.52,2.55,2.55,2.54,2.56,2.67,2.76,3.13],
                 [20190401,2.43,2.46,2.41,2.33,2.29,2.31,2.4,2.49,2.89],
                 [20190531,2.35,2.35,2.21,1.95,1.9,1.93,2.03,2.14,2.58]])


def FitYields(tc=0):
    x0 = np.array([0.25,0.5,1.0,2.0,3.0,5.0,7.0,10.0,30.0])
    xr = np.arange(x0[0],x0[-1+tc]+0.1,0.1)
    y = np.array([2.43,2.46,2.41,2.33,2.29,2.31,2.4,2.49,2.89])
    rows, cols = data.shape
    for i in range(rows):
        date = str(int(data[i,0]))
        x = x0[:tc] if tc != 0 else x0
        y = data[i, 1:tc] if tc !=0 else data[i, 1:]
        print(y)
        c , cn = CubicFit(x,y)
        yr = np.array([CubicEval(x, c, xr[i]) for i in range(len(xr))])
        plt.plot(x, y, 'o')
        plt.plot(xr, yr, label=date)
    plt.title("Historical Yield Curves")
    plt.xlabel("term")
    plt.ylabel("yield")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Various Mathematical Fits')
    parser.add_argument('-b','--b', type=int, default=0)
    args = parser.parse_args()
    # set -b to various negative numbers to chop off higher terms 
    # b=-1 will remove 30
    # b=-2 will remove 30,10
    # b=-3 will remove 30,10,7 etc.
    FitYields(tc=args.b)

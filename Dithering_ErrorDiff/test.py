import numpy as np

test = np.array([[2,16,3,16], 
                [10,6,11,7],
                [4,14,1,15],
                [12,8,9,5]])

test2 = np.array([[2,6,4],
                  [5,0,1],
                  [8,3,7]])

test3 = np.array([[0,1],
                  [3,1]])

test4 = np.array([[0,8,2,10], 
                [12,4,14,6],
                [3,11,1,9],
                [15,7,13,5]])

test4 = np.array([[0,8,2,10], 
                [12,4,14,6],
                [3,11,1,9],
                [15,7,13,5]])

scalar = 1/16
scalar2 = 1/9
scalar3 = 1/4

res = np.multiply(test, scalar)
res2 = np.multiply(test2, scalar2)
res3 = np.multiply(test3, scalar3)
res4= np.multiply(test4, scalar)

print(np.sum(res4)/16)


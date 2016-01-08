import numpy as np

imax = 3
jmax = 3

A = np.eye(imax*jmax)*4 + np.eye(imax*jmax, k=1)*-1 + np.eye(imax*jmax, k=-1)*-1 + np.eye(imax*jmax, k=imax)*-1 + np.eye(imax*jmax, k=-imax)*-1

B = np.eye(imax*jmax)*-1 + np.eye(imax*jmax, k=1)

BT = np.transpose(B)

ZERO = np.zeros((imax*jmax,jmax*imax))

print(A)
print(B)
print(BT)
print(ZERO)

C = np.append(A,B,axis=1)
D = np.append(BT,ZERO,axis=1)

M = np.append(C,D,axis=0)

print(M)

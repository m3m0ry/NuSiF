import sys
import numpy as np
import scipy.sparse.linalg as spla
import matplotlib.pyplot as plt


def constructM(imax, jmax):
    Du = np.eye(imax*jmax)*4 + np.eye(imax*jmax, k=1)*-1 + np.eye(imax*jmax, k=-1)*-1 + np.eye(imax*jmax, k=imax)*-1 + np.eye(imax*jmax, k=-imax)*-1

    for x in range(1,jmax):
        Du[imax*x, imax*x-1] = 0
        Du[imax*x-1, imax*x] = 0
    for x in range(1,jmax+1):
        Du[imax*x-1, imax*x-1] = 0

    for x in range(0,imax*jmax):
        count = 0
        for y in range(0,imax*jmax):
            if( Du[x,y] <= -1 ):
                count = count + 1
        if(Du[x,x] != 0 and count < 4):
            Du[x,x] = 5

    print(Du)

    Dv = np.eye(imax*jmax)*4 + np.eye(imax*jmax, k=1)*-1 + np.eye(imax*jmax, k=-1)*-1 + np.eye(imax*jmax, k=imax)*-1 + np.eye(imax*jmax, k=-imax)*-1
    #set first and last of every diagonal set to 5
    for x in range(0,imax):
        Dv[jmax*x, jmax*x] = 5
    for x in range(1,imax+1):
        Dv[jmax*x-1,jmax*x-1] = 5
    #Last imax cols are 0
    for x in range(imax*jmax-imax,imax*jmax):
        for y in range(0,imax*jmax):
            Dv[y,x] = 0
    #Set some on +-1 diagonal to 0
    for x in range(1,jmax):
        Dv[imax*x, imax*x-1] = 0
        Dv[imax*x-1, imax*x] = 0

    print(Dv)

    nhx = np.eye(imax*jmax)*-1 + np.eye(imax*jmax, k=1)
    for x in range(1,imax):
        nhx[x*jmax-1,x*jmax] = 0
    for x in range(1,imax+1):
        nhx[x*jmax-1,x*jmax-1] = 0

    print(nhx)

    nhy = np.eye(imax*jmax)*-1 + np.eye(imax*jmax, k=imax)
    for x in range(1,imax+1):
        nhy[imax*jmax-x,imax*jmax-x] = 0

    print(nhy)

    ZERO = np.zeros((imax*jmax,jmax*imax))

    A1 = np.append(Du, ZERO, axis=1)
    A2 = np.append(ZERO, Dv, axis=1)
    A = np.append(A1,A2, axis=0)

    B = np.append(nhx, nhy, axis=0)

    nhxT = np.eye(imax*jmax)*-1 + np.eye(imax*jmax,k=1)
    for x in range(1,imax+1):
        nhxT[jmax*x-1,jmax*x-1] = 0
        nhxT[jmax*x-2,jmax*x-1] = 0
    for x in range(1,imax):
        nhxT[jmax*x-1,jmax*x] = 0

    print(nhxT)
    nhyT = np.eye(imax*jmax)*-1 + np.eye(imax*jmax,k=imax)
    for x in range(imax*jmax-imax,imax*jmax):
        for y in range(0,imax*jmax):
            nhyT[y,x] = 0
    print(nhyT)


    BT = np.append(nhxT, nhyT, axis=1)

    M1 = np.append(A,B,axis=1)

    M3 = np.append(BT,ZERO,axis=1)

    M = np.append(M1,M3,axis=0)
    return M

def rhs(imax, jmax):
    u = np.zeros(imax*jmax)
    for x in range(2,imax+1):
        u[imax*jmax - x] = 2
    v = np.zeros(imax*jmax)
    p = np.zeros(imax*jmax)
    rhs = np.append(u,v)
    return np.append(rhs,p)




def VTKwrite(u,v,p,imax,jmax,fileName="output.vtk"):
    f = open(fileName, 'w')
    f.write("# vtk DataFile Version 4.0\n")
    f.write("Nusif VTK output\n")
    f.write("ASCII\n")
    f.write("DATASET STRUCTURED_POINTS\n")
    f.write("DIMENSIONS " + str(imax-1) + " " + str(jmax-1) + " 1\n")
    f.write("ORIGIN 0 0 0\n")
    f.write("SPACING 1 1 1\n") #deltas are = 1
    f.write("POINT_DATA " + str((imax-1)*(jmax-1)) + "\n")
    f.write("\n")
    f.write("VECTORS velocity double\n" )
    for j in range(0,jmax-1):
        for i in range(0,imax-1):
            uS = 0.5 * (u[i,j] + u[i+1,j])
            vS = 0.5 * (v[i,j] + v[i,j+1])
            f.write(str.format("{0:.3f}",uS) + " " + str.format("{0:.3f}",vS) + " 0\n")
    f.write("\n")

    f.write("SCALARS pressure double 1\n")
    f.write("LOOKUP_TABLE default\n")
    for j in range(0,jmax-1):
        for i in range(0,imax-1):
            f.write(str(p[i,j]) + "\n")

def main(argv):
    np.set_printoptions(linewidth=200,edgeitems = 20*15)
    imax = int(argv[0])
    jmax = int(argv[1])
    M = constructM(imax,jmax)
    print(M)

    rhs1 = rhs(imax,jmax)
    print(rhs1)
    return
    
    sol = spla.gmres(M, rhs1)[0]
    [u,v,p] = np.split(sol,3)
    u = np.reshape(u,(imax,jmax))
    v = np.reshape(v,(imax,jmax))
    p = np.reshape(p,(imax,jmax))

    print(u)
    
    y,x = np.mgrid[0:imax, 0:jmax]
    
    VTKwrite(u,v,p,imax,jmax)

    plt.streamplot(x,y,u,v)
    plt.show()


if __name__ == "__main__":
    main(sys.argv[1:])

import sys
import numpy as np
import scipy.sparse.linalg as spla
import matplotlib.pyplot as plt

def pos(i,j,width):
    return i+j*width


def constructM(imax, jmax):
    #Du = np.eye((imax+1) * (jmax+2))*4 + np.eye((imax+1) * (jmax+2), k=1)*-1 + np.eye((imax+1)*(jmax+2), k=-1)*-1 + np.eye((imax+1)*(jmax+2), k=(imax+1))*-1 + np.eye((imax+1)*(jmax+2), k=-(imax+1))*-1
    Du = np.zeros(((imax+1)*(jmax+2),(imax+1)*(jmax+2)))
    for i in range(0,imax+1):
        for j in range(0,jmax+2):
            if i == 0:
                Du[pos(i,j,imax+1),pos(i,j,imax+1)] = 1
            elif i == imax:
                Du[pos(i,j,imax+1),pos(i,j,imax+1)] = 1
            elif j == 0:
                Du[pos(i,j,imax+1),pos(i,j,imax+1)] = 1
                Du[pos(i,j,imax+1),pos(i,j+1,imax+1)] = 1
            elif j == jmax+1:
                Du[pos(i,j,imax+1),pos(i,j,imax+1)] = 1
                Du[pos(i,j,imax+1),pos(i,j-1,imax+1)] = 1
            else:
                Du[pos(i,j,imax+1),pos(i,j,imax+1)] = 4
                Du[pos(i,j,imax+1),pos(i+1,j,imax+1)] = -1
                Du[pos(i,j,imax+1),pos(i-1,j,imax+1)] = -1
                Du[pos(i,j,imax+1),pos(i,j+1,imax+1)] = -1
                Du[pos(i,j,imax+1),pos(i,j-1,imax+1)] = -1
    


    #print("Du:")
    #print(Du)

    #Dv = np.eye((imax+2)*(jmax+1))*4 + np.eye((imax+2)*(jmax+1), k=1)*-1 + np.eye((imax+2)*(jmax+1), k=-1)*-1 + np.eye((imax+2)*(jmax+1), k=(imax+2))*-1 + np.eye((imax+2)*(jmax+1), k=-(imax+2))*-1
    Dv = np.zeros(((imax+2)*(jmax+1),(imax+2)*(jmax+1)))
    for i in range(0,imax+2):
        for j in range(0,jmax+1):
            if j == 0:
                Dv[pos(i,j,imax+2),pos(i,j,imax+2)] = 1
            elif j == jmax:
                Dv[pos(i,j,imax+2),pos(i,j,imax+2)] = 1
            elif i == 0:
                Dv[pos(i,j,imax+2),pos(i,j,imax+2)] = 1
                Dv[pos(i,j,imax+2),pos(i+1,j,imax+2)] = 1
            elif i == imax+1:
                Dv[pos(i,j,imax+2),pos(i,j,imax+2)] = 1
                Dv[pos(i,j,imax+2),pos(i-1,j,imax+2)] = 1
            else:
                Dv[pos(i,j,imax+2),pos(i,j,imax+2)] = 4
                Dv[pos(i,j,imax+2),pos(i+1,j,imax+2)] = -1
                Dv[pos(i,j,imax+2),pos(i-1,j,imax+2)] = -1
                Dv[pos(i,j,imax+2),pos(i,j+1,imax+2)] = -1
                Dv[pos(i,j,imax+2),pos(i,j-1,imax+2)] = -1

    #print("Dv:")
    #print(Dv)

    ZeroD1 = np.zeros(((imax+1)*(jmax+2),(imax+2)*(jmax+1)))
    ZeroD2 = np.zeros(((imax+2)*(jmax+1),(imax+1)*(jmax+2)))
    #print("ZeroD1")
    #print(ZeroD1)

    A1 = np.append(Du, ZeroD1, axis=1)
    A2 = np.append(ZeroD2,Dv, axis=1)
    A3 = np.append(A1,A2, axis=0)
    #print("A3:")
    #print(A3)

    nhx = np.zeros(((imax+1)*(jmax+2),(imax+2)*(jmax+2)))
    for i in range(1,imax):
        for j in range(1,jmax+1):
            nhx[pos(i,j,imax+1),pos(i,j,imax+2)] = -1
            nhx[pos(i,j,imax+1),pos(i+1,j,imax+2)] = 1

    #print("nhx:")
    print(nhx)


    nhy = np.zeros(((imax+2)*(jmax+1),(imax+2)*(jmax+2)))
    for i in range(1,imax+1):
        for j in range(1,jmax):
            nhy[pos(i,j,imax+2),pos(i,j,imax+2)] = -1
            nhy[pos(i,j,imax+2),pos(i,j+1,imax+2)] = 1

    #print("nhy:")
    #print(nhy)

    ZeroN = np.zeros(((imax+2)*(jmax+2),(imax+2)*(jmax+2)))

    B = np.append(nhx,nhy,axis=0)
    #print(B.transpose())

    nhxT = np.zeros(((imax+2)*(jmax+2),(imax+1)*(jmax+2)))
    for i in range(1,imax+1):
        for j in range(1,jmax+1):
            nhxT[pos(i,j,imax+2),pos(i-1,j,imax+1)] = -1
            nhxT[pos(i,j,imax+2),pos(i,j,imax+1)] = 1
    #print("nhxT:")
    #print(nhxT)
    nhyT = np.zeros(((imax+2)*(jmax+2),(imax+2)*(jmax+1)))
    for i in range(1,imax+1):
        for j in range(0,jmax):
            nhyT[pos(i,j+1,imax+2),pos(i,j,imax+2)] = -1
            nhyT[pos(i,j+1,imax+2),pos(i,j+1,imax+2)] = 1
    #print("nhyT:")
    #print(nhyT)

    BT = np.append(nhxT,nhyT,axis=1)
    #BT = B.transpose()
    #print(BT)

    A = np.append(A3,B, axis=1)
    D = np.append(BT,ZeroN, axis=1)

    M = np.append(A,D,axis=0)

    return M

def rhs(imax, jmax):
    u = np.zeros((imax+1)*(jmax+2))
    for x in range(2,imax+1):
        u[(imax+1)*(jmax+2) - x] = 2
    v = np.zeros((imax+2)*(jmax+1))
    p = np.zeros((imax+2)*(jmax+2))
    rhs = np.append(u,v)
    return np.append(rhs,p)


def VTKwrite(u,v,p,imax,jmax,fileName="output.vtk"):
    f = open(fileName, 'w')
    f.write("# vtk DataFile Version 4.0\n")
    f.write("Nusif VTK output\n")
    f.write("ASCII\n")
    f.write("DATASET STRUCTURED_POINTS\n")
    f.write("DIMENSIONS " + str(imax) + " " + str(jmax) + " 1\n")
    f.write("ORIGIN 0 0 0\n")
    f.write("SPACING 1 1 1\n") #deltas are = 1
    f.write("POINT_DATA " + str((imax)*(jmax)) + "\n")
    f.write("\n")
    f.write("VECTORS velocity double\n" )
    for j in range(0,jmax):
        for i in range(0,imax):
            uS = 0.5 * (u[i,j+1] + u[i+1,j+1])
            vS = 0.5 * (v[i+1,j] + v[i+1,j+1])
            f.write(str.format("{0:.3f}",uS) + " " + str.format("{0:.3f}",vS) + " 0\n")
    f.write("\n")

    f.write("SCALARS pressure double 1\n")
    f.write("LOOKUP_TABLE default\n")
    for j in range(0,jmax):
        for i in range(0,imax):
            f.write(str(p[i+1,j+1]) + "\n")

def main(argv):
    np.set_printoptions(linewidth=200,edgeitems = 20*15)
    imax = int(argv[0])
    jmax = int(argv[1])
    M = constructM(imax,jmax)
    #print(M)

    rhs1 = rhs(imax,jmax)
    #print(rhs1)

    [sol,exit] = spla.gmres(M, rhs1,maxiter=10000)
    #[sol,exit] = spla.gmres(M, rhs1)
    if exit > 0:
        print("Convergence not achived. Iters: " + str(exit))
    elif exit < 0:
        print("Illegal input or breakdown of gmres")

    u = np.take(sol,range(0,(imax+1)*(jmax+2)))
    v = np.take(sol,range((imax+1)*(jmax+2),(imax+1)*(jmax+2)+(imax+2)*(jmax+1)))
    p = np.take(sol,range((imax+1)*(jmax+2)+(imax+2)*(jmax+1),(imax+1)*(jmax+2)+(imax+2)*(jmax+1)+(imax+2)*(jmax+2)))
    u = np.reshape(u,(imax+1,jmax+2),'F')
    v = np.reshape(v,(imax+2,jmax+1),'F')
    p = np.reshape(p,(imax+2,jmax+2),'F')

    VTKwrite(u,v,p,imax,jmax)
    #print(u)
    #print("---")
    #print("---")
    print(v)
    #print("---")
    #print("---")
    #print(p)
    
    y,x = np.mgrid[0:imax+1, 0:jmax+1]
    
    VTKwrite(u,v,p,imax,jmax)
    return

    plt.streamplot(x,y,u,v)
    plt.show()


if __name__ == "__main__":
    main(sys.argv[1:])

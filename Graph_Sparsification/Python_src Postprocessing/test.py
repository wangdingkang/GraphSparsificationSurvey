# import  networkx as nx
# from numpy import linalg as LA
#
# import numpy as np
#
# G = nx.Graph();
#
# for i in range(2):
#     G.add_node(i)
#
# G.add_edge(0, 1, weight=0.5)
#
# K = np.matrix('0.08 -0.08; -0.08 0.08')
#
#
# vals, vecs = LA.eig(K)
# print(vals)

from scipy.sparse import eye
from scipy.sparse.linalg import LinearOperator, eigsh, minres
import numpy as np

Ln = np.matrix('3 -2 -1 0; -2 4 -2 0; -1 -2 4 -1; 0 0 -1 1', dtype=float)
Ln = Ln / 100000
print(Ln)
# OPinv = LinearOperator(matvec=lambda v: minres(Ln, v, tol=1e-6)[0],
#                                shape=Ln.shape, dtype=Ln.dtype)
vals = eigsh(Ln, k=3, which='SM', tol=1e-6, return_eigenvectors=False) #OPinv=OPinv)

ret = []
for val in vals:
    ret.append(val)
ret.sort()
print(ret)
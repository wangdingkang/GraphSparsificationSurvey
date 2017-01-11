import networkx as nx
import scipy.sparse as ss
import numpy as np

if __name__ == '__main__':
    G = nx.read_edgelist('output from c++')

    A = nx.laplacian_matrix(G)

    L = ss.csgraph.laplacian(A)

    vals, vecs = ss.linalg.eigsh(L, k=3, which='BE')

    print(vals)

    print(ans)

import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"

if __name__ == '__main__':
    onlyfiles = [join(input_folder, f) for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        G = nx.read_edgelist(filename)

        # use normalized version instead.
        L = nx.normalized_laplacian_matrix(G)

        vals, vecs = ss.linalg.eigsh(L.asfptype(), k=3, which='BE')

        print(filename, ": ", vals)




import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_node = "sampled_nodes/node_"

if __name__ == '__main__':
    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))

        with open(output_node + filename, 'w+') as node_file:
            for node in G.nodes():
                node_file.write(node + '\n')

        # use normalized version instead.
        L = nx.normalized_laplacian_matrix(G)

        vals, vecs = ss.linalg.eigsh(L.asfptype(), k=5, which='BE')

        print(filename, ": ", vals)




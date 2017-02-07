import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_node = "sampled_nodes/node_"
output_eigen = "results/"

if __name__ == '__main__':
    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        print('Handling ' + filename + '...')

        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))
        G = max(nx.connected_component_subgraphs(G), key=len)

        print('Lcc of size ' + str(nx.number_of_nodes(G)))

        with open(output_node + filename, 'w+') as node_file:
            for node in G.nodes():
                node_file.write(node + '\n')

        # use normalized version instead.
        # L = nx.normalized_laplacian_matrix(G)

        cnt_node = nx.number_of_nodes(G)
        L = nx.laplacian_matrix(G)

        vals, vecs = ss.linalg.eigsh(L.asfptype(), k=6, which='LM', sigma=0, maxiter=2000)

        name = filename[filename.rfind('_'):]
        with open(output_eigen + "eigen" + name, 'a') as eigen_file:
            for v in vals:
                # if you use normalized one, comment the division of '/cnt_node'
                eigen_file.write('{0:.8f}'.format(v / cnt_node) + ' ')
            eigen_file.write(filename + '\n')



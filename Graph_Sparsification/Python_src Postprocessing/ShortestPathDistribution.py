import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"

if __name__ == '__main__':
    onlyfiles = [join(input_folder, f) for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        print(filename)

        G = nx.read_edgelist(filename)

        # ans = nx.average_shortest_path_length(G)

        for g in nx.connected_component_subgraphs(G):
            print(nx.average_shortest_path_length(g))





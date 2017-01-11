import networkx as nx
from os import listdir
from os.path import isfile, join

input_folder = "origin/"

if __name__ == "__main__":

    onlyfiles = [join(input_folder, f) for f in listdir(input_folder) if isfile(join(input_folder, f))]
    for filename in onlyfiles:
        G = nx.read_edgelist(filename)
        Gc = max(nx.connected_component_subgraphs(G), key=len)

        print(len(Gc))
        print(Gc.size())

        Go = nx.convert_node_labels_to_integers(Gc)

        nx.write_edgelist(Go, "processed/LCC_" + filename, data=False)
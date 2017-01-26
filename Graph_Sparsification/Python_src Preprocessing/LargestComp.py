import networkx as nx
from os import listdir
from os.path import isfile, join

input_folder = "origin/"
node_folder = "origin_position/node_"
node_output_folder = "processed_position/node_LCC_"

if __name__ == "__main__":

    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]
    for filename in onlyfiles:
        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))
        pos_path = node_folder + filename

        # means its a 3D model proximity graph, each node has its own coordinates.
        if(isfile(pos_path)):

            with open(pos_path) as myfile:
                for line in myfile:
                    content = line.split()
                    tpos = content[1] + " " + content[2] + " " + content[3]
                    # print(content, tpos)
                    G.add_node((content[0]), pos=tpos)

            Gc = max(nx.connected_component_subgraphs(G), key=len)
            Go = nx.convert_node_labels_to_integers(Gc)
            outgraph_path = "processed/LCC_" + filename
            with open(outgraph_path, 'w+') as file:
                file.write(str(nx.number_of_nodes(Go)) + '\n');
                for edge in Go.edges(data=True):
                    file.write(str(edge[0]) + " " + str(edge[1]) + " " + str(edge[2]['weight']) + '\n');
            # nx.write_weighted_edgelist(Go, "processed/LCC_" + filename)
            opos_path = node_output_folder + filename
            with open(opos_path, 'w+') as file:
                for node in Go.nodes(data=True):
                    file.write( str(node[0]) + ' ' + node[1]['pos'] + '\n' )

        # otherwise, it's just a graph from snap stanford.
        else:
            Gc = max(nx.connected_component_subgraphs(G), key=len)
            Go = nx.convert_node_labels_to_integers(Gc)
            outgraph_path = "processed/LCC_" + filename
            with open(outgraph_path, 'w+') as file:
                file.write(str(nx.number_of_nodes(Go)) + '\n');
                for edge in Go.edges(data=True):
                    file.write(str(edge[0]) + " " + str(edge[1]) + ' 1\n');
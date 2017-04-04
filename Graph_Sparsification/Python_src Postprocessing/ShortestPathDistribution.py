import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_sp = 'results/'

if __name__ == '__main__':
    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        print('processing ' + filename)

        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))



        ret = {}
        is_connected = False
        num_pair = 0;

        # compute the shortest path distances.
        if nx.is_connected(G):
            is_connected = True
            cnt_node = nx.number_of_nodes(G)
            num_pair = cnt_node * (cnt_node - 1) / 2


            for node in G:
                path_length=nx.single_source_dijkstra_path_length(G, node, weight='weight')
                # avg += sum(path_length.values())#
                for k, d in path_length.items():
                    if d in ret:
                        ret[d] += 1
                    else:
                        ret[d] = float(1)

            for k in ret.keys():
                ret[k] /= num_pair

        #compute the shortest path length distribution of its max component.
        else:
            g = max(nx.connected_component_subgraphs(G), key=len)
            cnt_node = nx.number_of_nodes(g)
            num_pair = cnt_node * (cnt_node - 1) / 2

            for node in g:
                path_length=nx.single_source_dijkstra_path_length(g, node, weight='weight')
                # avg += sum(path_length.values())#
                for k, d in path_length.items():
                    if d in ret:
                        ret[d] += 1
                    else:
                        ret[d] = float(1)

            for k in ret.keys():
                ret[k] /= num_pair

        name = filename[filename.rfind('_'):]
        with open(output_sp + 'SP_' + name, 'a') as sp_file:
            # sp_file.write('{0:.6f}'.format(ret) + ' ')
            # sp_file.write('{0:.6f}'.format(max_comp_avg_sp_length) + ' ')
            for k, d in ret:
                sp_file.write(str(k) + ':' + str(d) + ' ')
            if is_connected:
                sp_file.write('connected ')
            else:
                sp_file.write('unconnected ')
            sp_file.write(filename + '\n')

        # ans = nx.average_shortest_path_length(G)

        # output format:
        # avg_sp_length, max_comp_avg_sp_length, is_connected, filename;
        # num_pair = 0
        # max_comp_avg_sp_length = 0
        # for g in nx.connected_component_subgraphs(G):
        #     avg = nx.average_shortest_path_length(g, weight='weight')
        #     cnt = nx.number_of_nodes(g)
        #     ret += cnt * (cnt - 1) * avg
        #     num_pair += cnt * (cnt - 1)
        #     max_comp_avg_sp_length = max(max_comp_avg_sp_length, avg)
        #     cnt_comp += 1
        #     print(avg)
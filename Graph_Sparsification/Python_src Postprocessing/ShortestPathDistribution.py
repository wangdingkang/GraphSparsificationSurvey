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

        # ans = nx.average_shortest_path_length(G)

        # output format:
        # avg_sp_length, max_comp_avg_sp_length, is_connected, filename;
        num_pair = 0
        ret = 0
        max_comp_avg_sp_length = 0
        cnt_comp = 0
        for g in nx.connected_component_subgraphs(G):
            avg = nx.average_shortest_path_length(g)
            cnt = nx.number_of_nodes(g)
            ret += cnt * (cnt - 1) * avg
            num_pair += cnt * (cnt - 1)
            max_comp_avg_sp_length = max(max_comp_avg_sp_length, avg)
            cnt_comp += 1
            print(avg)


        ret /= num_pair

        name = filename[filename.rfind('_'):]
        with open(output_sp + 'SP_' + name, 'a') as sp_file:
            sp_file.write('{0:.6f}'.format(ret) + ' ')
            sp_file.write('{0:.6f}'.format(max_comp_avg_sp_length) + ' ')
            if cnt_comp == 1:
                sp_file.write('connected ')
            else:
                sp_file.write('unconnected ')
            sp_file.write(filename + '\n')
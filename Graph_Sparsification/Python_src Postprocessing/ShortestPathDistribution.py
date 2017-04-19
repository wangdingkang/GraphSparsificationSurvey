import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_sp = 'results/'
# suppose diameter won't surpass 1500
# ignore distances that larger than 100
cut_off = 25
iteration = 4
rets_apsp = {}

if __name__ == '__main__':
    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        print('processing ' + filename)

        suffix_name = filename[filename.rfind('_'):]
        prefix_name = filename[:filename.find('_', filename.find('_') + 1)]

        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))

        ret = [0.0 for i in range(cut_off + 1)]
        num_pair = 0;
        cnt = 0;

        # compute the shortest path distances.
        if not nx.is_connected(G):
            G = max(nx.connected_component_subgraphs(G), key=len)

        cnt_node = nx.number_of_nodes(G)
        print(str(cnt_node) + ' nodes.')
        num_pair = cnt_node * (cnt_node - 1)

        for node in G:
            if cnt % 100 == 0:
                print(str(cnt) + ' nodes processed.')
            path_length=nx.single_source_dijkstra_path_length(G, node, weight='weight', cutoff=cut_off)
            # print(path_length)
            # avg += sum(path_length.values())#
            for k, d in path_length.items():
                # print(str(d) + " " + str(int(d)) + " ")
                # if int(d) == 1:
                #    print(node + ' ' + k)
                ret[int(d)] += 1
            cnt += 1
        # distance to itself is 0, we need to zero it out.
        ret[0] = 0

        for k in range(1, cut_off + 1):
            ret[k] /= num_pair
        print(ret)
        if prefix_name in rets_apsp:
            rets_apsp[prefix_name] = [x + y for x, y in zip(ret, rets_apsp[prefix_name])]
        else:
            rets_apsp[prefix_name] = ret
        # print(rets_apsp[prefix_name])

    print('Finished, writing...')
    # output histogram
    with open(output_sp + 'asps' + suffix_name, 'a') as sp_file:
        for type in sorted(rets_apsp):
            for v in rets_apsp[type]:
                sp_file.write("{0:.4f}".format(v / iteration) + ' ')
            sp_file.write(type + '\n')

    # let matlab do the work.
    # output CUMULATIVE apsp distributions
    # with open(output_sp + 'apsp_cumul' + suffix_name, 'a') as sp_file:
    #     # sp_file.write('{0:.6f}'.format(ret) + ' ')
    #     # sp_file.write('{0:.6f}'.format(max_comp_avg_sp_length) + ' ')
    #     for type, vals in rets_apsp.items():
    #         sum = 0.0
    #         for v in vals:
    #             sum += v
    #             sp_file.write("{0:.4f}".format(sum / iteration) + ' ') #str(k) + ':' +
    #         sp_file.write(type + '\n')

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
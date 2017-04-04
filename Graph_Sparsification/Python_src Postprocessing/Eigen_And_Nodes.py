import networkx as nx
from scipy.sparse.linalg import eigsh
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_node = "sampled_nodes/node_"
output_eigen = "results/"
iterations = 10

suffix_name = ''
rets_Ln = {};
rets_Lp = {};

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

        # normalized version normalized by degree
        Ln = nx.normalized_laplacian_matrix(G)
        Ln = Ln.asfptype()
        # OPinv = LinearOperator(matvec=lambda v: minres(Ln, v, tol=1e-6)[0],
        #                        shape=Ln.shape, dtype=Ln.dtype)

        # normalized by graph size
        cnt_node = nx.number_of_nodes(G)
        Lp = nx.laplacian_matrix(G)
        Lp = Lp.asfptype()

        if suffix_name == '':
            suffix_name = filename[filename.rfind('_'):]
        prefix_name = filename[:filename.find('_')]
        print(prefix_name, cnt_node)

        # normalized by graph size
        vals = eigsh(Lp, k=6, which='SM', tol=1e-8, return_eigenvectors=False)
        ret = []
        for val in vals:
            ret.append(val / cnt_node)
        ret.sort()
        if prefix_name in rets_Lp:
            rets_Lp[prefix_name] = [x + y for x, y in zip(ret, rets_Lp[prefix_name])]
            print("Adding Lp... ", ret)
            print("Added Lp... ", rets_Lp[prefix_name])
        else:
            rets_Lp[prefix_name] = ret;
            print("Inserting Lp... ", ret)
            print("Inserted Lp... ", rets_Lp[prefix_name])

        # normalized version normalized by degree
        vals = eigsh(Ln, k=6, which='SM', tol=1e-8, return_eigenvectors=False)
        ret = []
        for val in vals:
            ret.append(val)
        ret.sort()
        if prefix_name in rets_Ln:
            rets_Ln[prefix_name] = [x + y for x, y in zip(ret, rets_Ln[prefix_name])]
            print("Adding Ln...", ret)
            print("Added Ln... ", rets_Ln[prefix_name])
        else:
            rets_Ln[prefix_name] = ret;
            print("Inserting Ln... ", ret)
            print("Inserted Ln... ", rets_Ln[prefix_name])


with open(output_eigen + "eigen_lp" + suffix_name, 'w') as eigen_file:
    for k, v in rets_Lp.items():
        # if you use normalized one, comment the division of '/cnt_node'
        for val in v:
            eigen_file.write('{0:.8f}'.format((val / iterations) if k != 'LCC' else val) + ' ')
        eigen_file.write(k + '\n')

with open(output_eigen + "eigen_ln" + suffix_name, 'w') as eigen_file:
    for k, v in rets_Ln.items():
        # if you use normalized one, comment the division of '/cnt_node'
        for val in v:
            eigen_file.write('{0:.8f}'.format((val / iterations) if k != 'LCC' else val) + ' ')
        eigen_file.write(k + '\n')
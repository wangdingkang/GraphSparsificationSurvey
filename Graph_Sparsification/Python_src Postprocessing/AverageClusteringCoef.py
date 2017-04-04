import networkx as nx
import scipy.sparse as ss
from os import listdir
from os.path import isfile, join

input_folder = "processed/"
output_ac = 'results/'

if __name__ == '__main__':
    onlyfiles = [f for f in listdir(input_folder) if isfile(join(input_folder, f))]

    for filename in onlyfiles:
        print('processing ' + filename)

        G = nx.read_edgelist(join(input_folder, filename), data=(('weight',float),))

        avg_c = nx.average_clustering(G, weight='weight')

        print(filename, avg_c)
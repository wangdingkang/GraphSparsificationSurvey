import networkx as nx

if __name__ == "__main__":

    G = nx.read_edgelist('CA-HepTh.txt')
    Gc = max(nx.connected_component_subgraphs(G), key=len)

    print(len(Gc))
    print(Gc.size())

    nx.write_edgelist(Gc, "CA-HepTh_LCC.txt")
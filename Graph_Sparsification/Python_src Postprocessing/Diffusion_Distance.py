import os
import numpy as np
import scipy.linalg as sl
import scipy.sparse as sparse
import scipy.spatial.distance as dist
import scipy.sparse.csgraph

INPUT_FILE_DIR = '../data/input'
OUTPUT_FILE_DIR = '../data/output'
EPS = 1e-20

class RelabelEngine():
    relabel_map = {}
    
    def __init__(self):
        self.relabel_map.clear()
        pass
    
    def new_label(self, old_label):
        if not self.relabel_map.has_key(old_label):
            self.relabel_map[old_label] = len(self.relabel_map)
        return self.relabel_map[old_label]

    def count(self):
        return len(self.relabel_map)

def diffusion_map(metric, epsilon, t, n_components, sparse_thresh=1e-8):
    """Compute the diffusion coordinates.

    Args:
        metric (ndarray): The distance matrix in square or condensed form.
        epsilon (float): Kernel scale parameter in exp(-d**2/(4 * epsilon))
        t (int): The number of timesteps. Smaller values favor local structure.
        n_components (int): The number of coordinates.

    Returns:
        ndarray: The points mapped to diffusion coodinates.

    """
    if metric.ndim == 1:
        metric = dist.squareform(metric)

    # construct the isotropic kernel
    kernel = np.exp(-metric ** 2 / (4 * float(epsilon)))

    # approximate the Laplace-Beltrami operator
    # normalization = np.sqrt(kernel.sum(axis=0))
    # diffusion_matrix = kernel / np.outer(normalization, normalization)
    # diffusion_matrix = np.eye(diffusion_matrix.shape[0]) - diffusion_matrix

    normalization = kernel.sum(axis=0)
    diffusion_matrix = kernel / normalization[:, None]

    # compute the singular value decomposition
#     u, s, v = np.linalg.svd(diffusion_matrix, n_components+1)
#     psi = u / u[:, 0, None]

    s, u = sl.eigh(diffusion_matrix, eigvals=(0, n_components + 1))
    psi = u / u[:, 0].reshape((u.shape[0], 1))
 
    # map to diffusion coordinates
    lambda_t = s[1:n_components + 1] ** t
    return psi[:, 1:n_components + 1] * lambda_t

def get_input_graph(input_files):
    edges = []
    # rename the nodes
    engine = RelabelEngine()
    for line in input_file.readlines():
        nodes = line.split()
        _nodes = []
        if len(nodes) <= 1:
            continue
        _nodes.append(engine.new_label(int(nodes[0])))
        _nodes.append(engine.new_label(int(nodes[1])))
        if len(nodes) > 2:
            _nodes.append(float(nodes[2]))
        edges.append(_nodes)

    print("Nodes: " + str(engine.count()) + " Edges: " + str(len(edges)))

    # create a empty adjacency matrix
    graph = np.zeros((engine.count(), engine.count()))
    # graph.fill(np.Inf)
    for pair in edges:
        if len(pair) > 2:
            graph[pair[0]][pair[1]] = pair[2]
            graph[pair[1]][pair[0]] = pair[2]
        else:
            graph[pair[0]][pair[1]] = 1
            graph[pair[1]][pair[0]] = 1            

    return graph

def get_largest_connected_componet(graph):
    n_components, component_array = scipy.sparse.csgraph.connected_components(graph)
    label_dict = {}
    max_count = 0
    max_index = 0

    for label in component_array:
        if label_dict.has_key(label):
            label_dict[label] += 1
        else:
            label_dict[label] = 1 

    for k in label_dict:
        if label_dict[k] > max_count:
            max_count = label_dict[k]
            max_index = k

    slice_index_x = []
    slice_index_y = []
    for i in range(len(graph)):
        if component_array[i] == max_index:
            slice_index_x.append([i])
            slice_index_y.append(i)

    print("Largest Connected Component Node: " + str(max_count))
    return graph[slice_index_x, slice_index_y]

def scale_out(f_input):
    min_element = float('inf') 

    it = np.nditer(f_input, flags=['multi_index'])
    while not it.finished:
        if it.multi_index[0] != it.multi_index[1] and it.value > EPS:
            min_element = min(min_element, it.value)
        it.iternext()
    
    f_input /= min_element
    f_input = np.around(f_input)

if __name__ == '__main__':
    for filename in os.listdir(INPUT_FILE_DIR):
        if (filename.endswith('~')):
            continue

        print('Performing calculation on for file ' + filename)
        input_file = open(os.path.join(INPUT_FILE_DIR, filename), 'r')
        # calculate the largest connected component
        print('Getting largest connect component.')
        graph = get_input_graph(input_file);
        graph = get_largest_connected_componet(graph)
        print('Done.')

        # calculate all pair shortest path
        print('Calculating distance.')
        distance = sparse.csgraph.shortest_path(graph, method='J')
        print('Done.')

        name_without_ext = os.path.splitext(filename)[0]
        new_filename = name_without_ext + "_APSP.txt";
        new_filename = os.path.join(OUTPUT_FILE_DIR, new_filename)
        np.savetxt(new_filename, distance, fmt='%.0f')
        continue;

        # compute diffusion map
        print('Getting diffusion map.')
        # compute the diffusion distance from embedding
        embedding = diffusion_map(distance, epsilon=10, t=10, n_components=50)
        print('Done.')
        # compute the diffusion distance from embedding
        print('Getting diffusion metric.')
        diffusion_metric = dist.squareform(dist.pdist(embedding))

        diffusion_distance = dist.squareform(dist.pdist(embedding))
        #scale_out(diffusion_distance)
        print('Done.')

        # save data to file
        print('Saving diffusion distance to file.')
        name_without_ext = os.path.splitext(filename)[0]
        new_filename = name_without_ext + "_DiffusionDistance.txt";
        new_filename = os.path.join(OUTPUT_FILE_DIR, new_filename)
        np.savetxt(new_filename, diffusion_distance, fmt='%.3f')
        print('All done.')

#         new_filename = os.path.splitext(os.path.basename(filename))[0] + "_APSP.txt";
#         new_filename = os.path.join(OUTPUT_FILE_DIR, new_filename)
#         np.savetxt(new_filename, distance, fmt='%.0f')

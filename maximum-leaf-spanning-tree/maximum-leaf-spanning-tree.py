import networkx as nx
import random
from networkx.algorithms.bridges import has_bridges, bridges
from networkx.algorithms.minors import contracted_edge
from networkx.algorithms.components import is_connected
from networkx.algorithms.dominating import is_dominating_set
from itertools import combinations, groupby
import matplotlib.pyplot as plt


NO_INSTANCE = 'The resulting graph does not have a size k connected dominating set!\nThe graph does not have a spanning tree with atleast k leaves!'
YES_INSTANCE = 'The resulting graph has a size k connected dominating set!\nThe graph has a spanning tree with atleast k leaves!'
k_leaves_subset = []


# Creates a random graph with specified number of nodes and edges.
def createRandomGraph(nodes, edges, k):
    graph = nx.Graph()
    graph.add_nodes_from(range(nodes))

    for i in range(1, nodes):
        neighbour = random.randint(0, i - 1)
        graph.add_edge(i, neighbour)

    while graph.number_of_edges() != edges:
        new_edge = random.sample(range(nodes), 2)
        graph.add_edge(*new_edge)

    print('Input: Graph with {} vertices and {} edges with k = {}.'.format(nodes, edges, k))
    return graph


# Removes 'useless' vertices from the graph
def reductionOne(graph):
    while True:
        reduced = False
        for node in graph.nodes():
            if graph.degree[node] == 2:
                v_neighbours = list(graph.neighbors(node))
                if graph.degree[v_neighbours[0]] == 2 and graph.degree[v_neighbours[1]] == 2:
                    graph.remove_node(node)
                    graph.add_edges(v_neighbours[0], v_neighbours[1])
                    reduced = True
                    break
        if not reduced:
            return graph


# Solves MLST by bruteforcing using Minimum connected dominating set
def solveMLST(graph, k):
    subsets = list(combinations(graph.nodes(), k))
    for subset in subsets:
        graph_subset = graph.copy()
        graph_subset.remove_nodes_from(n for n in graph if n in subset)
        if graph_subset.number_of_nodes() == 0:
            return False
        if is_connected(graph_subset) and is_dominating_set(graph, graph_subset.nodes()):
            global k_leaves_subset
            k_leaves_subset = subset
            return True
    return False


# The main algoruthm for MLST
def MLST(graph, k):
    greater_than_k  = True in [graph.degree[node] >= k for node in graph.nodes()]
    if is_connected(graph) and greater_than_k:
        # print('The graph has a vertex with degree greater than k.\nThe graph has a spanning tree with atleast k leaves!')
        solveMLST(graph, k)
        return True
        
    graph = reductionOne(graph)

    # 4 * (k + 2) * (k + 1) is the kernel
    if graph.number_of_nodes() >= 4 * (k + 2) * (k + 1):
        # print('The number of vertices in the resultant graph is more than 4(k + 2)(k + 1).\nThe graph has a spanning tree with atleast k leaves!')
        solveMLST(graph, k)
        return True

    result = solveMLST(graph, k)
    # print(YES_INSTANCE if result else NO_INSTANCE)
    return result

def printGraph(graph, has_k_leaves):
    plt.figure(figsize = (8, 5))

    if has_k_leaves:
        print('Leaves of some spanning tree with atleast k leaves', k_leaves_subset)
        node_list = list(graph.nodes())
        node_colour = []
        for i in range(graph.number_of_nodes()):
            if node_list[i] in k_leaves_subset:
                node_colour.append('#90ee90')
            else:
                node_colour.append('#9090ee')

        nx.draw(graph, nodelist = node_list, node_color = node_colour, with_labels = True, node_size = 500)
        plt.show()
    else:
        nx.draw(graph, node_color = '#9090ee', with_labels = True, node_size = 500)
        plt.show()        
    
    # nx.draw(graph, node_color = 'lightblue', with_labels = True, node_size = 500)


if __name__ == '__main__':    
    nodes = 5
    edges = 4
    k = 4
    graph = createRandomGraph(nodes, edges, k)
    has_k_leaves = MLST(graph, k)
    print('Output: Graph G has a spanning tree with k = {} leaves?: {}'.format(k, has_k_leaves))
    printGraph(graph, has_k_leaves)
    plt.show()
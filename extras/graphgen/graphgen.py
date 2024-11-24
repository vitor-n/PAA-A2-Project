import sys, os
import random
import csv
from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

def create_grid_graph(m, n):
    G = nx.Graph()
    
    for i in range(m):
        for j in range(n):
            linear_index = i * n + j
            if i < m - 1:
                # Add edge to the node below
                G.add_edge(linear_index, (i + 1) * n + j)
            if j < n - 1:
                # Add edge to the node to the right
                G.add_edge(linear_index, i * n + (j + 1))
    
    return G

def choose_seeds(G, N):
    """Choose N random node_seeds (nodes) from the graph G."""
    random.seed(42)
    nodes = list(G.nodes)
    node_seeds = random.sample(nodes, N)
    return node_seeds

def grow_regions(G, node_seeds):
    #TODO: Add edge data
    """Grow regions from the node_seeds until all edges are assigned to a region."""
    regions = {seed: [seed] for seed in node_seeds}
    edge_to_region = {}
    queue = list(node_seeds)
    visited = set(node_seeds)
    
    while queue:
        node = queue.pop(0)
        region = None
        for seed, nodes in regions.items():
            if node in nodes:
                region = seed
                break
        
        for neighbor in G.neighbors(node):
            edge = (node, neighbor)
            if edge not in edge_to_region:
                edge_to_region[edge] = region
                edge_to_region[(neighbor, node)] = region
                if neighbor not in visited:
                    visited.add(neighbor)
                    queue.append(neighbor)
                    regions[region].append(neighbor)
    
    return regions, edge_to_region

def create_subgraphs(G, edge_to_region):
    region_edges = {}
    for edge, region in edge_to_region.items():
        if region not in region_edges:
            region_edges[region] = []
        region_edges[region].append(edge)
    
    subgraphs = {}
    for region, edges in region_edges.items():
        subgraph = nx.Graph()
        subgraph.add_edges_from(edges)
        subgraphs[region] = subgraph
    
    return subgraphs

def plot_graph(G, edge_to_region, m, n):
    """Plot the graph G with edges colored by region and individual subgraphs."""
    pos = {}
    for i in range(m):
        for j in range(n):
            linear_index = i * n + j
            pos[linear_index] = (j, -i)
            
    edges = G.edges()
    colors = [edge_to_region[edge] for edge in edges]
    
    palette = [
        "#377eb8",  # blue
        "#ff7f00",  # orange
        "#4daf4a",  # green
        "#f781bf",  # pink
        "#a65628",  # brown
        "#984ea3",  # purple
        "#999999",  # gray
        "#e41a1c",  # red
        "#dede00"   # yellow
    ]
    
    unique_regions = list(set(colors))
    color_map = {region: palette[i % len(palette)] for i, region in enumerate(unique_regions)}
    edge_colors = [color_map[region] for region in colors]
    
    plt.figure(figsize=(6, 6))
    nx.draw(G, pos, edge_color=edge_colors, node_size=0, width=2, with_labels=False)
    plt.margins(0)
    plt.show()

def relabel_nodes(G):
    """Relabel nodes of the graph G to use linear indices."""
    mapping = {node: i for i, node in enumerate(G.nodes)}
    G = nx.relabel_nodes(G, mapping)
    return G

def save(edge_to_region, folder, m, n, N_REGIONS):
    os.makedirs(os.path.dirname(folder), exist_ok=True)

    with open(f"{folder}/city.cfg", "w") as f:
        f.write(f"rows={m}\n")
        f.write(f"cols={n}\n")
        f.write(f"regions={len(set(edge_to_region.values()))}\n")

    with open(f"{folder}/city-edges.csv", "w", newline="") as csvfile:
        fieldnames = ["node1", "node2", "length", "maxSpeed", "numBuildings", "region", "street"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        region_to_index = {region: i for i, region in enumerate(set(edge_to_region.values()))}
        
        writer.writeheader()
        for edge, region in edge_to_region.items():
            region = region_to_index[region]
            node1 = edge[0]
            node2 = edge[1]
            writer.writerow({"node1": node1, "node2": node2, "region": region})

def read(filename):
    """Read the edge-to-region mapping from a CSV file with linear indices."""
    edge_to_region = {}
    with open(filename, "r", newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            node1 = int(row["node1"])
            node2 = int(row["node2"])
            region = int(row["region"])
            edge_to_region[(node1, node2)] = region
    return edge_to_region

if __name__ == "__main__":

    N_ROWS = 100
    N_COLS = 100
    N_REGIONS = 10
    
    G = create_grid_graph(N_ROWS, N_COLS)

    node_seeds = choose_seeds(G, N_REGIONS)

    regions, edge_to_region = grow_regions(G, node_seeds)
    #edge_to_region = read("graph-regions.csv")
    #G = G.to_directed()

    save(edge_to_region, "data/city-1/", N_ROWS, N_COLS, N_REGIONS)
    
    #plot_graph(G, edge_to_region, N_ROWS, N_COLS)


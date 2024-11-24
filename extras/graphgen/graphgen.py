import sys, os
import random
import csv
import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

class GraphGenerator:
    def __init__(self):
        self.rows = None
        self.cols = None
        self.regions = None
        self.G = nx.Graph()
        self.edge_to_region = {}

    def relabel_nodes(self):
        """Relabel nodes of the graph G to use linear indices."""
        mapping = {node: i for i, node in enumerate(G.nodes)}
        self.G = nx.relabel_nodes(self.G, mapping)
        return G

    def create_grid_graph(self, m, n):
        self.G.clear()
        for i in range(m):
            for j in range(n):
                linear_index = i * n + j
                if i < m - 1:
                    # Add edge to the node below
                    self.G.add_edge(linear_index, (i + 1) * n + j)
                if j < n - 1:
                    # Add edge to the node to the right
                    self.G.add_edge(linear_index, i * n + (j + 1))

    def choose_seeds(self, N):
        """Choose N random node_seeds (nodes) from the graph G."""
        #random.seed(42)
        nodes = list(self.G.nodes)
        node_seeds = random.sample(nodes, N)
        return node_seeds

    def grow_regions(self, node_seeds):
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
            
            for neighbor in self.G.neighbors(node):
                edge = (node, neighbor)
                if edge not in edge_to_region:
                    edge_to_region[edge] = region
                    edge_to_region[(neighbor, node)] = region
                    if neighbor not in visited:
                        visited.add(neighbor)
                        queue.append(neighbor)
                        regions[region].append(neighbor)
        
        return regions, edge_to_region

    def create_subgraphs(self):
        region_edges = {}
        for edge, region in self.edge_to_region.items():
            if region not in region_edges:
                region_edges[region] = []
            region_edges[region].append(edge)
        
        subgraphs = {}
        for region, edges in region_edges.items():
            subgraph = nx.Graph()
            subgraph.add_edges_from(edges)
            subgraphs[region] = subgraph
        
        return subgraphs

    def gen_random_city(self, m, n, N_REGIONS):
        self.rows = m
        self.cols = n
        self.regions = N_REGIONS
        self.create_grid_graph(m, n)
        node_seeds = self.choose_seeds(N_REGIONS)
        _, self.edge_to_region = self.grow_regions(node_seeds)
        #self.subgraphs = self.create_subgraphs(edge_to_region)

    def read(self, folder):
        folder += "/" if not folder.endswith("/") else ""
        with open(f"{folder}city.cfg", "r") as f:
            for line in f:
                key, value = line.strip().split("=")
                if key == "rows":
                    self.rows = int(value)
                elif key == "cols":
                    self.cols = int(value)
                elif key == "regions":
                    self.regions = int(value)

        with open(f"{folder}city-edges.csv", "r", newline="") as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                node1 = int(row["node1"])
                node2 = int(row["node2"])
                region = int(row["region"])
                self.edge_to_region[(node1, node2)] = region
                self.G.add_edge(node1, node2)

    def save(self, folder):
        os.makedirs(os.path.dirname(folder), exist_ok=True)

        with open(f"{folder}/city.cfg", "w") as f:
            f.write(f"rows={self.rows}\n")
            f.write(f"cols={self.cols}\n")
            f.write(f"regions={len(set(self.edge_to_region.values()))}\n")

        with open(f"{folder}/city-edges.csv", "w", newline="") as csvfile:
            fieldnames = ["node1", "node2", "length", "maxSpeed", "numBuildings", "region", "street"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            region_to_index = {region: i for i, region in enumerate(set(self.edge_to_region.values()))}
            
            writer.writeheader()
            for edge, region in self.edge_to_region.items():
                region = region_to_index[region]
                node1 = edge[0]
                node2 = edge[1]
                writer.writerow({"node1": node1, "node2": node2, "region": region})

    def plot(self):
        """Plot the graph G with edges colored by region."""
        pos = {i * self.cols + j: (j, -i) for i in range(self.rows) for j in range(self.cols)}

        edges = self.G.edges()
        colors = [self.edge_to_region[edge] for edge in edges]

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
        nx.draw(self.G, pos, edge_color=edge_colors, node_size=0, width=2, with_labels=False)
        plt.margins(0)
        plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate or read city graph, plot and save data.")
    parser.add_argument("mode", choices=["gen", "read"], help="Mode of operation: 'gen' or 'read'", type=lambda s: s.lower())
    parser.add_argument("-f","--folder", type=str, required=True, help="Folder name to read from or save to")
    parser.add_argument("-o","--output", type=str, help="Output folder name to save the city graph (required in 'read' mode)")
    parser.add_argument("-p","--plot", action="store_true", help="Plot the city graph, can be quite slow for large graphs")

    args = parser.parse_args()

    city = GraphGenerator()

    args.folder += "/" if not args.folder.endswith("/") else ""

    if args.mode == "gen":
        m = int(input("Enter number of rows: "))
        n = int(input("Enter number of columns: "))
        n_regions = int(input("Enter number of regions: "))
        city.gen_random_city(m, n, n_regions)
        city.save(args.folder)

    elif args.mode == "read":
        city.read(args.folder)
        if args.output:
            city.save(args.output)
    
    if args.plot:
        city.plot()

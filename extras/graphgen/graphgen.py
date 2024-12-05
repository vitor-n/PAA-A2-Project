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
        self.num_rows = None
        self.num_cols = None
        self.num_regions = None
        self.graph = nx.Graph()
        self.edge_to_region = {}
        self.edge_to_street = {}
        self.edges = []
        self.pos = {}
        self.seeds = []
        self.subway = nx.Graph()
        self.subway_stations = []
        self.bus = nx.Graph()

    def relabel_nodes(self):
        """Relabel nodes of the graph G to use linear indices."""
        mapping = {node: i for i, node in enumerate(self.graph.nodes)}
        self.graph = nx.relabel_nodes(self.graph, mapping)

    def gen_grid_city(self, m, n):
        self.graph.clear()

        def add_edge(node1, node2, length, num_buildings, num_residentials, cost_to_escavate):
            self.graph.add_edge(node1, node2)
            self.graph.edges[node1, node2]["length"] = length
            self.graph.edges[node1, node2]["numBuildings"] = num_buildings
            self.graph.edges[node1, node2]["numResidentials"] = num_residentials
            self.graph.edges[node1, node2]["escavationCost"] = cost_to_escavate

        len_row = np.random.randint(5, 20, size=m) * 10
        len_col = np.random.randint(5, 20, size=n) * 10

        mean_cost_meter = 300
        deviation_cost_meter = 100

        diag_prob = 0

        for i in range(m):
            for j in range(n):
                node1 = i * n + j
                self.pos[node1] = (j, -i)
                if i < m - 1:
                    # Add edge to the node below
                    node2 = (i + 1) * n + j 
                    edge = (node1, node2)
                    self.graph.add_edge(node1, node2)

                    self.pos[node2] = (j, -i-1)


                    self.graph.edges[edge]["length"] = len_row[i]
            
                    num_buildings = len_row[i] // 10

                    numIndustrial = random.randint(0, num_buildings//5+1)
                    numCommercial = random.randint(1, num_buildings//3+1)
                    numTouristics = random.randint(0, num_buildings//4+1)
                    numResidentials = random.randint(num_buildings//4,num_buildings//2+1)
                    self.graph.edges[edge]["numResidentials"] = numResidentials
                    self.graph.edges[edge]["numIndustrial"] = numIndustrial
                    self.graph.edges[edge]["numCommercial"] = numCommercial
                    self.graph.edges[edge]["numTouristics"] = numTouristics
                    self.graph.edges[edge]["numBuildings"] = numCommercial + numIndustrial + numTouristics + numResidentials

                    self.graph.edges[edge]["escavationCost"] = len_row[i] * mean_cost_meter + random.randint(-deviation_cost_meter, deviation_cost_meter) // 100 * 100
                    
                    self.edges.append(edge)
                    self.edges.append((node2, node1))

                if j < n - 1:
                    # Add edge to the node to the right
                    node2 = i * n + (j + 1)
                    self.pos[node2] = (j+1, -i)
    
                    edge = (node1, node2)
                    self.graph.add_edge(node1, node2)
                    self.graph.edges[edge]["length"] = len_col[j]
                    num_buildings = len_col[i] // 10 - np.random.randint(0, len_col[i] // 20 - 1)

                    numIndustrial = random.randint(0, num_buildings//5)
                    numCommercial = random.randint(1, num_buildings//3)
                    numTouristics = random.randint(0, num_buildings//4)
                    numResidentials = random.randint(num_buildings//4,num_buildings//2)
                    self.graph.edges[edge]["numResidentials"] = numResidentials
                    self.graph.edges[edge]["numIndustrial"] = numIndustrial
                    self.graph.edges[edge]["numCommercial"] = numCommercial
                    self.graph.edges[edge]["numTouristics"] = numTouristics
                    self.graph.edges[edge]["numBuildings"] = numCommercial + numIndustrial + numTouristics + numResidentials
                    self.graph.edges[edge]["escavationCost"] = len_col[j] * mean_cost_meter + random.randint(-deviation_cost_meter, deviation_cost_meter) // 100 * 100
                    self.edges.append(edge)
                    self.edges.append((node2, node1))

                # diag_1 = random.randint(0, 1)

                # if i < m -1 and j < n - 1 and random.random() < diag_prob and diag_1 and node1 %2 == 0:
                #     # Add edge to the node to the right
                #     node2 = (i + 1) * n + (j + 1)
                #     edge = (node1, node2)
                #     self.graph.add_edge(node1, node2)
                #     self.graph.edges[edge]["length"] = np.sqrt(len_row[i]**2 + len_col[j]**2)
                #     num_buildings = len_col[j] // 10 - np.random.randint(0, len_col[j] // 10 - 1)
                #     self.graph.edges[edge]["numBuildings"] = num_buildings
                #     self.graph.edges[edge]["numResidentials"] = num_buildings - random.randint(num_buildings//5,num_buildings//2)
                #     self.graph.edges[edge]["escavationCost"] = np.sqrt(len_row[i]**2 + len_col[j]**2) * mean_cost_meter + random.randint(-deviation_cost_meter, deviation_cost_meter) // 100 * 100
                #     self.edges.append(edge)
                #     self.edges.append((node2, node1))

                # if i < m - 1 and j > 0 and random.random() < diag_prob and not diag_1 and node1 %2 == 0:
                #     node2 = (i + 1) * n + (j - 1)
                #     edge = (node1, node2)
                #     self.graph.add_edge(node1, node2)
                #     self.graph.edges[edge]["length"] = np.sqrt(len_row[i]**2 + len_col[j-1]**2)

                #     num_buildings = min(len_row[i], len_col[j-1]) // 10 - np.random.randint(0, min(len_row[i], len_col[j-1]) // 10 - 1)
                #     self.graph.edges[edge]["numBuildings"] = num_buildings
                #     self.graph.edges[edge]["numResidentials"] = num_buildings - random.randint(num_buildings//5, num_buildings//2)
                #     self.graph.edges[edge]["escavationCost"] = np.sqrt(len_row[i]**2 + len_col[j-1]**2) * mean_cost_meter + random.randint(-deviation_cost_meter, deviation_cost_meter) // 100 * 100

                #     self.edges.append(edge)
                #     self.edges.append((node2, node1))

    def choose_seeds(self, N):
        """Choose N random node_seeds (nodes) from the graph G."""
        #random.seed(42)
        nodes = list(self.graph.nodes)
        node_seeds = random.sample(nodes, N)
        self.seeds = node_seeds
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
            
            for neighbor in self.graph.neighbors(node):
                edge = (node, neighbor)
                if edge not in edge_to_region:
                    edge_to_region[edge] = region
                    edge_to_region[(neighbor, node)] = region
                    self.graph.edges[edge]["region"] = region
                    if neighbor not in visited:
                        visited.add(neighbor)
                        queue.append(neighbor)
                        regions[region].append(neighbor)
        
        return regions, edge_to_region

    def gen_streets(self, max_len=4):
        
        max_speed_list = [30, 50, 60, 70, 80, 90, 100]
        max_speed_weights = [0.1, 0.2, 0.2, 0.2, 0.2, 0.05, 0.05]

        max_speed = np.random.choice(max_speed_list, p=max_speed_weights)

        def update_street(node, neighbor, street_id, last_region, curr_len, curr_max_len, max_speed, start_node):
            if self.edge_to_region[(node, neighbor)] == last_region and curr_len < curr_max_len:
                curr_len += 1
            else:
                street_id += 2
                curr_max_len = random.randint(1, max_len)
                max_speed = np.random.choice(max_speed_list, p=max_speed_weights)
                curr_len = 0
                start_node = node
            self.edge_to_street[(node, neighbor)] = street_id
            self.graph.edges[(node, neighbor)]["maxSpeed"] = max_speed
            self.graph.edges[(node, neighbor)]["street"] = street_id
            self.graph.edges[(node, neighbor)]["streetStart"] = start_node
            last_region = self.edge_to_region[(node, neighbor)]

            return street_id, last_region, curr_len, curr_max_len, max_speed, start_node
        
        street_id_h = 0
        street_id_v = 1

        curr_len = 0
        curr_max_len = random.randint(1, max_len)
        last_region = self.edge_to_region[(0, 1)]
        start_node = 0
        for i in range(self.num_rows):
            for j in range(self.num_cols-1):
                node = i * self.num_cols + j
                right_node = i * self.num_cols + (j + 1)
                
                info = update_street(node, right_node, street_id_h, last_region, curr_len, curr_max_len, max_speed, start_node)
                street_id_h, last_region, curr_len, curr_max_len, max_speed, start_node = info

        curr_len = 0
        curr_max_len = random.randint(1, max_len)
        last_region = self.edge_to_region[(0, self.num_cols)]
        start_node = 0
        for j in range(self.num_cols):
            for i in range(self.num_rows-1):
                node = i * self.num_cols + j
                down_node = (i + 1) * self.num_cols + j

                info = update_street(node, down_node, street_id_v, last_region, curr_len, curr_max_len, max_speed, start_node)
                street_id_v, last_region, curr_len, curr_max_len, max_speed, start_node = info

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
        self.num_rows = m
        self.num_cols = n
        self.num_regions = N_REGIONS
        self.gen_grid_city(m, n)
        node_seeds = self.choose_seeds(N_REGIONS)
        _, self.edge_to_region = self.grow_regions(node_seeds)
        self.gen_streets()
        #self.subgraphs = self.create_subgraphs(edge_to_region)

    def read(self, folder):
        folder += "/" if not folder.endswith("/") else ""
        with open(f"{folder}city.cfg", "r") as f:
            for line in f:
                key, value = line.strip().split("=")
                if key == "rows":
                    self.num_rows = int(value)
                elif key == "cols":
                    self.num_cols = int(value)
                elif key == "regions":
                    self.num_regions = int(value)

        with open(f"{folder}city-edges.csv", "r", newline="") as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                node1 = int(row["node1"])
                node2 = int(row["node2"])
                region = int(row["region"])
                self.edge_to_region[(node1, node2)] = region
                self.graph.add_edge(node1, node2)
            
        with open(f"{folder}city-nodes.csv", "r", newline="") as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                node = int(row["node"])
                x = float(row["x"])
                y = float(row["y"])
                self.pos[node] = (x, y)

        city_subway = Path(f"{folder}subway-edges.csv")
        if city_subway.is_file():
            with open(city_subway, "r", newline="") as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    node1 = int(row["node1"])
                    node2 = int(row["node2"])
                    self.subway.add_edge(node1, node2)

        city_subway_stations = Path(f"{folder}subway-nodes.csv")
        if city_subway_stations.is_file():
            with open(city_subway_stations, "r", newline="") as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    node = int(row["station"])
                    self.subway_stations.append(node)

        city_bus = Path(f"{folder}bus-edges.csv")
        if city_bus.is_file():
            with open(city_bus, "r", newline="") as csvfile:
                reader = csv.DictReader(csvfile)
                for row in reader:
                    node1 = int(row["node1"])
                    node2 = int(row["node2"])
                    self.bus.add_edge(node1, node2)

    def save(self, folder):
        os.makedirs(os.path.dirname(folder), exist_ok=True)

        with open(f"{folder}/city.cfg", "w") as f:
            f.write(f"rows={self.num_rows}\n")
            f.write(f"cols={self.num_cols}\n")
            f.write(f"num_regions={self.num_regions}\n")
            f.write(f"num_nodes={len(self.graph.nodes)}\n")
            f.write(f"num_edges={len(self.graph.edges)}\n")

        with open(f"{folder}/city-edges.csv", "w", newline="") as csvfile:
            fieldnames = ["node1",
                          "node2",
                          "region",
                          "street",
                          "streetStart",
                          "length",
                          "maxSpeed",
                          "numBuildings",
                          "numResidentials",
                          "numIndustrial",
                          "numCommercial",
                          "numTouristics",
                          "escavationCost"
                          ]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            region_to_index = {region: i for i, region in enumerate(self.seeds)}
            
            writer.writeheader()
            for edge in self.edges:
                region = self.edge_to_region[edge]
                region = region_to_index[region]
                
                data = self.graph.edges[edge]
                street = self.edge_to_street.get(edge, -1)  # -1 for edges not part of any street
                writer.writerow({
                    "node1": edge[0],
                    "node2": edge[1],
                    "region": region_to_index[data.get("region", "@")],
                    "street": data.get("street", "@"),
                    "streetStart": data.get("streetStart", "@"),
                    "length": data.get("length", "@"),
                    "maxSpeed": data.get("maxSpeed", "@"),
                    "numBuildings": data.get("numBuildings", "@"),
                    "numResidentials": data.get("numResidentials", "@"),
                    "numIndustrial": data.get("numIndustrial", "@"),
                    "numCommercial": data.get("numCommercial", "@"),
                    "numTouristics": data.get("numTouristics", "@"),
                    "escavationCost": data.get("escavationCost", "@")
                })

        with open(f"{folder}/city-nodes.csv", "w", newline="") as csvfile:
            fieldnames = ["node", "isBusStop", "isMetroStation", "x", "y"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writeheader()
            for node, pos in self.pos.items():
                writer.writerow({"node": node,
                                 "isBusStop": 0,
                                 "isMetroStation": 0,
                                 "x": pos[0],
                                 "y": pos[1]})

    def plot(self):
        """Plot the graph G with edges colored by region."""
        edges = self.graph.edges()
        colors = [self.edge_to_region[edge] for edge in edges]

        palette = [
            "#FF8585",
            "#FFA77A",
            "#FFB974",
            "#E3FF78",
            "#99FF99",
            "#80FFD4",
            "#66D6E8",
            "#6699FF",
            "#8C75FF",
            "#FF99FF",
        ]

        unique_regions = list(set(colors))
        color_map = {region: palette[i % len(palette)] for i, region in enumerate(unique_regions)}
        edge_colors = [color_map[region] for region in colors]

        plt.figure(figsize=(6, 6))
        nx.draw(self.graph, self.pos, edge_color=edge_colors, node_size=0, width=3, with_labels=False)
        nx.draw(self.subway, self.pos, edge_color="#444444", node_size=0, width=3, with_labels=False)
        nx.draw(self.bus, self.pos, edge_color="#888888", node_size=0, width=3, with_labels=False)

        for station in self.subway_stations:
            plt.plot(self.pos[station][0], self.pos[station][1], 'o', color="#444444", markersize=10)

        if self.num_rows < 20 and self.num_cols < 20:
            for edge in self.graph.edges():
                sorted_edge = tuple(sorted(edge))
                if sorted_edge in self.edge_to_street:
                    street_id = self.edge_to_street[sorted_edge]
                    
                    x1, y1 = self.pos[edge[0]]
                    x2, y2 = self.pos[edge[1]]
                    mid_x = (x1 + x2) / 2
                    mid_y = (y1 + y2) / 2
                    
                    # Add street number label
                    plt.text(mid_x, mid_y, f'{street_id}', 
                            bbox=dict(facecolor='white', alpha=0.8, pad=0.5),
                            horizontalalignment='center',
                            verticalalignment='center',
                            fontsize=8)

        plt.margins(0)
        plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate or read city graph, plot and save the data.")
    parser.add_argument("mode", choices=["gen", "read"], help="Mode of operation: 'gen' or 'read'", type=lambda s: s.lower())
    parser.add_argument("folder", help="Folder to save if in 'gen' mode or read if in 'read' mode", type=str)
    parser.add_argument("-p","--plot", action="store_true", help="Plot the city graph, can be slow for large graphs")

    args = parser.parse_args()

    city = GraphGenerator()

    args.folder += "/" if not args.folder.endswith("/") else ""

    if args.mode == "gen":
        m = int(input("Enter number of rows: "))
        n = int(input("Enter number of columns: "))
        n_regions = int(input("Enter number of regions: "))
        city.gen_random_city(m, n, n_regions)
        city.save(args.folder)
        if args.plot:
            city.plot()

    elif args.mode == "read":
        city.read(args.folder)
        city.plot()

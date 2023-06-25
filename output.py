import matplotlib.pyplot as plt
import networkx as _nx

import os

if not os.path.exists('output'):
	os.makedirs('output')

A = _nx.Graph()
A.add_nodes_from(['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', ])
A.add_weighted_edges_from([('a', 'b', 1), ('a', 'd', 1), ('b', 'c', 1), ('b', 'd', 1), ('b', 'f', 1), ('c', 'e', 1), ('c', 'f', 1), ('d', 'e', 1), ('e', 'g', 1), ])
A.remove_nodes_from(['h', ])
A.remove_edges_from([('c', 'f'), ])
A_bfs_tree = _nx.bfs_tree(A, 'a')
A_bfs_edges = [tuple(sorted(edge)) for edge in _nx.utils.pairwise(_nx.shortest_path(A_bfs_tree, 'a', 'g'))]
A_edges_colors = []
for edge in A.edges:
	if edge in A_bfs_edges:
		A_edges_colors.append('red')
	else:
		A_edges_colors.append('black')
A_pos = _nx.spring_layout(A)
_nx.draw(A, with_labels=True, font_weight='bold', edge_color=A_edges_colors, node_color='white', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.4'), pos=A_pos)
_nx.draw_networkx_edge_labels(A, A_pos, edge_labels=_nx.get_edge_attributes(A, 'weight'))
plt.savefig('output/bfsAtoG.png')
plt.clf()

A_dfs_tree = _nx.dfs_tree(A, 'b')
A_dfs_edges = [tuple(sorted(edge)) for edge in _nx.utils.pairwise(_nx.shortest_path(A_dfs_tree, 'b', 'e'))]
A_edges_colors = []
for edge in A.edges:
	if edge in A_dfs_edges:
		A_edges_colors.append('red')
	else:
		A_edges_colors.append('black')
A_pos = _nx.spring_layout(A)
_nx.draw(A, with_labels=True, font_weight='bold', edge_color=A_edges_colors, node_color='white', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.4'), pos=A_pos)
_nx.draw_networkx_edge_labels(A, A_pos, edge_labels=_nx.get_edge_attributes(A, 'weight'))
plt.savefig('output/dfsBtoE.png')
plt.clf()

A_mst = _nx.minimum_spanning_tree(A)
A_mst_edges = [tuple(sorted([edge[0], edge[1]])) for edge in A_mst.edges(data=True)]
A_edges_colors = []
for edge in A.edges:
	if edge in A_mst_edges:
		A_edges_colors.append('red')
	else:
		A_edges_colors.append('black')
A_pos = _nx.spring_layout(A)
_nx.draw(A, with_labels=True, font_weight='bold', edge_color=A_edges_colors, node_color='white', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.4'), pos=A_pos)
_nx.draw_networkx_edge_labels(A, A_pos, edge_labels=_nx.get_edge_attributes(A, 'weight'))
plt.savefig('output/mstA.png')
plt.clf()


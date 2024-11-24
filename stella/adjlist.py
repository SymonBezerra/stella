import _stella

class AdjList(_stella.AdjList):
    def dfs(self, start: str):
        dfs_tree = AdjList()
        self._dfs_rec(start, dfs_tree)
        return self._dfs_rec(start, dfs_tree)

    def _dfs_rec(self, start, dfs_tree):
        adj = [edge for edge in self.edges.values() if (edge.n1.label == start or edge.n2.label == start) and not edge.label in dfs_tree.edges]
        for i in adj:
            next_node = i.n1 if i.n2.label == start else i.n2
            if not next_node in dfs_tree.nodes:
                dfs_tree.add_node(next_node)
                dfs_tree.add_edge(i)
                self._dfs_rec(next_node.label, dfs_tree)
        return dfs_tree
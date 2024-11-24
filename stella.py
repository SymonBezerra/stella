import _stella

class AdjList(_stella.AdjList):
    def dfs(self, start: str):
        dfs_tree = AdjList()
        visited = {v: False for v in self.nodes}
        
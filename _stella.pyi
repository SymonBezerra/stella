from abc import ABC, abstractmethod
from typing import Union

class Node:
    """
    Class for representing a node in a graph.

    `__init__(self, label: str)`
        Initialize a Node object with a label.

    Attributes
    -------
    `_label (str)`: string label for the Node object. Readonly via @property.

    Methods
    -------
    `__str__(self) -> str`
        Return a string representation of the Node object (i.e., its label).
    """

    def __init__(self, label: str): ...

    @property
    def label(self) -> str: return self._label

    def __str__(self) -> str: ...

class BaseEdge(ABC):
    """
    Abstract base class for representing the edge of a graph.
    Must not be instantiated.

    Attributes
    -------
    `_label (str)`: string label for the BaseEdge object. Readonly via @property.
    `_n1 (Node)`: first node of the edge. Readonly via @property.
    `_n2 (Node)`: second node of the edge. Readonly via @property.
    `_weight (int)`: represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    `__str__(self) -> str`
        Return a string representation of the Node object (i.e., its label).
    """

    def __str__(self) -> str: ...

    @property
    def label(self) -> str: return self._label

    @property
    def n1(self) -> Node: return self._n1

    @property
    def n2(self) -> Node: return self._n2

    @property
    def weight(self) -> int: return self._weight

class Edge(BaseEdge):
    """
    Class for representing a non-directed edge in a graph.
    Must be used in AdjList and AdjMatrix classes.

    Attributes
    -------
    `_label (str)`: string label for the BaseEdge object. Readonly via @property.
    `_n1 (Node)`: first node of the edge. Readonly via @property.
    `_n2 (Node)`: second node of the edge. Readonly via @property.
    `_weight (int)`: represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    `__str__(self) -> str`
        Return a string representation of the Node object (i.e., its label).
    """

class DirectedEdge(BaseEdge):
    """
    Class for representing a directed edge in a graph.
    Must be used in DirectedAdjList and DirectedAdjMatrix classes.

    Attributes
    -------
    `_label (str)`: string label for the BaseEdge object. Readonly via @property.
    `_n1 (Node)`: first node of the edge. Readonly via @property.
    `_n2 (Node)`: second node of the edge. Readonly via @property.
    `_weight (int)`: represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    `__str__(self) -> str`
        Return a string representation of the Node object (i.e., its label).
    """

class Graph(ABC):
    """
    Abstract base class for the definition of common methods for all graph classes.
    Must not be instantiated.

    Due to differences between adjacency lists and matrices, this class does not specify
    details on how the edges will be stored, just the common method interface.

    Attributes
    -------
    `_nodes (list[Node])`: contains all the nodes present in the Graph. Readonly via @property.

    Methods
    -------
    `add_edge(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_edge(edge: Edge)`
        Adds an existing edge to the graph.
    `add_node(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_node(node: Node)`
        Adds an existing node to the graph.
    `get_node(label: str)`
        Retrieves a Node object from the graph.
    """

    @abstractmethod
    def add_node(self, arg: Union[str, Node]) -> None:
        """stella
        Adds a Node object into the Graph. This method has two possible signatures:

        `add_node(arg: str)`
            Creates a brand new Node object with `arg` as its label.
        `add_node(arg: Node)`
            Adds an existing Node object to the graph.
        """
        ...

    @abstractmethod
    def add_edge(self, arg: Union[str, BaseEdge], n1: str=None, n2: str=None, weight: int=1) -> None:
        """
        Adds an Edge object into the graph. This method has two possible signatures:

        `add_edge(arg: BaseEdge)`
            Adds an existing BaseEdge object to the graph.

        `add_edge(arg: str, n1: str, n2: str, weight: int = 1)`
            Creates an Edge object with `arg` as its label, then searches for nodes inside
            the graph with `n1` and `n2` as its labels, with default weight 1.
        
        Raises
        -------
        `TypeError`: if an Edge object is added to a directed graph,
        or if an DirectedEdge object is added to a non-directed graph.
        """
        ...

    @abstractmethod
    def get_node(self, label: str) -> Union[Node, None]:
        """
        Returns a node from the graph. If the label is not found, a `None` value is returned.
        """

    @property
    def nodes(self) -> list[Node]: ...

class AdjList(Graph):
    """
    Class representation of a non-directed adjacency list.
    Implements a `dict[Edge]` for its Edge objects, with its labels as keys, for the edges.

    Attributes
    -------
    `_nodes (list[Node])`: contains all the nodes present in the AdjList. Readonly via @property.
    `_edges (dict[Edge])`: contains all the edges present in the AdjList. Readonly via @property.

    Methods
    -------
    `add_edge(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_edge(edge: Edge)`
        Adds an existing edge to the graph.
    `add_node(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_node(node: Node)`
        Adds an existing node to the graph.
    `get_node(label: str)`
        Retrieves a Node object from the graph.
    `get_edge(label: str)`
        Retrieves an Edge object from the graph.
    """

    @property
    def edges(self) -> dict[Edge]:
        """
        Returns the edges from the AdjList.
        """

    @property
    def nodes(self) -> list[Node]:
        """
        Returns the nodes from the AdjList.
        """

    @property
    def get_edge(self, label: str) -> Union[Edge, None]:
        """
        Retrieves an edge from the graph. If the label is not found, None is returned.
        """

class DirectedAdjList(AdjList):
    """
    Class representation of a directed adjacency list.
    Implements a `dict[DirectedEdge]` for its Edge objects, with its labels as keys, for the edges.

    Attributes
    -------
    `_nodes (list[Node])`: contains all the nodes present in the AdjList. Readonly via @property.
    `_edges (dict[DirectedEdge])`: contains all the edges present in the AdjList. Readonly via @property.

    Methods
    -------
    `add_edge(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_edge(edge: DirectedEdge)`
        Adds an existing edge to the graph.
    `add_node(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_node(node: Node)`
        Adds an existing node to the graph.
    `get_node(label: str)`
        Retrieves a Node object from the graph.
    `get_edge(label: str)`
        Retrieves an DirectedEdge object from the graph.
    """
    @property
    def get_edge(self, label: str) -> Union[DirectedEdge, None]: ...

class AdjMatrix(Graph):
    """
    Class representation of a non-directed adjacency matrix.
    Implements a non-squared `list[list[Edge]]` for its edge objects.

    Attributes
    -------
    `_nodes (list[Node])`: contains all the nodes present in the AdjList. Readonly via @property.
    `_edges (list[list[Edge]])`: contains all the edges present in the AdjList. Readonly via @property.

    Methods
    -------
    `add_edge(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_edge(edge: DirectedEdge)`
        Adds an existing edge to the graph.
    `add_node(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_node(node: Node)`
        Adds an existing node to the graph.
    `get_node(label: str)`
        Retrieves a Node object from the graph.
    """

    @property
    def edges(self) -> list[list[Edge]]:
        """
        Returns the edges matrix from the AdjMatrix.
        For the non-directed adjacency matrix, this module implements a non-squared 2D list,
        such as:
        * * * * *
        * * * *
        * * *
        * *
        *
        """

    @property
    def nodes(self) -> list[Node]:
        """
        Returns the nodes from the AdjMatrix.
        """

class DirectedAdjMatrix(AdjMatrix):
    """
    Class representation of a non-directed adjacency matrix.
    Implements a squared `list[list[DirectedEdge]]` for its edge objects.

    Attributes
    -------
    `_nodes (list[Node])`: contains all the nodes present in the AdjList. Readonly via @property.
    `_edges (list[list[DirectedEdge]])`: contains all the edges present in the AdjList. Readonly via @property.

    Methods
    -------
    `add_edge(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_edge(edge: DirectedEdge)`
        Adds an existing edge to the graph.
    `add_node(label: str)`
        Creates a new edge object and adds it to the graph.
    `add_node(node: Node)`
        Adds an existing node to the graph.
    `get_node(label: str)`
        Retrieves a Node object from the graph.
    """
    @property
    def edges(self) -> list[list[DirectedEdge]]:
        """
        Returns the edges matrix from the AdjMatrix.
        For the directed adjacency matrix, this module implements a squared 2D list,
        such as:
        * * * * *
        * * * * *
        * * * * *
        * * * * *
        * * * * *
        """
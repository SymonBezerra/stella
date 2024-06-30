from abc import ABC
from functools import singledispatchmethod
from typing import Union

class Node:
    """
    Class for representing a node in a graph.

    __init__(self, label: str)
        Initialize a Node object with a label.

    Attributes
    -------
    _label (str): string label for the Node object. Readonly via @property.

    Methods
    -------
    __str__(self) -> str
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
    _label (str): string label for the BaseEdge object. Readonly via @property.
    _n1 (Node): first node of the edge. Readonly via @property.
    _n2 (Node): second node of the edge. Readonly via @property.
    _weight (int): represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    __str__(self) -> str
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
    _label (str): string label for the BaseEdge object. Readonly via @property.
    _n1 (Node): first node of the edge. Readonly via @property.
    _n2 (Node): second node of the edge. Readonly via @property.
    _weight (int): represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    __str__(self) -> str
        Return a string representation of the Node object (i.e., its label).
    """

class DirectedEdge(BaseEdge):
    """
    Class for representing a directed edge in a graph.
    Must be used in DirectedAdjList and DirectedAdjMatrix classes.

    Attributes
    -------
    _label (str): string label for the BaseEdge object. Readonly via @property.
    _n1 (Node): first node of the edge. Readonly via @property.
    _n2 (Node): second node of the edge. Readonly via @property.
    _weight (int): represents the weight of the edge object. Default to 1. Readonly via @property.

    Methods
    -------
    __str__(self) -> str
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
    _nodes (list[Node]): contains all the nodes present in the Graph. Readonly via @property.

    Methods
    -------
    add_edge(label: str)
        Creates a new edge object and adds it to the graph.
    add_edge(edge: Edge)
        Adds an existing edge to the graph.
    add_node(label: str)
        Creates a new edge object and adds it to the graph.
    add_node(node: Node)
        Adds an existing node to the graph.
    """

    def add_node(arg: Union[str, Node]) -> None:
        """
        
        """

    def add_edge(arg: Union[str | Node], n1: str= None, n2: str=None, weight: int = None) -> None: ...

class AdjList(Graph):
    pass
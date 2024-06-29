#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>
#include <string>

using std::string;
using std::ostream;

#include "node.hpp"

namespace stella {
    class BaseEdge {
    public:
        const string label;
        const Node* n1;
        const Node* n2;
        const int weight;

        BaseEdge(string label, Node* n1, Node* n2, int weight);
        BaseEdge(string label, Node* n1, Node* n2);

        string getLabel() const;
        const Node* getN1() const;
        const Node* getN2() const;
        int getWeight() const;

        friend ostream& operator<<(ostream& os, BaseEdge* object);
    };

    class Edge: public BaseEdge {
    public:
        Edge(string label, Node* n1, Node* n2, int weight);
        Edge(string label, Node* n1, Node* n2);
    };

    class DirectedEdge: public Edge {
    public:
        DirectedEdge(string label, Node* n1, Node* n2, int weight);
        DirectedEdge(string label, Node* n1, Node* n2);
        friend std::ostream& operator<<(std::ostream& os, DirectedEdge* object);
    };
}

#endif
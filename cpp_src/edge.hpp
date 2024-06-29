#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;
using std::ostream;

#include "node.hpp"

namespace stella {
    class BaseEdge {
    public:
        const string label;
        const shared_ptr<Node> n1;
        const shared_ptr<Node> n2;
        const int weight;

        BaseEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight);
        BaseEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2);

        string getLabel() const;
        const shared_ptr<Node> getN1() const;
        const shared_ptr<Node> getN2() const;
        int getWeight() const;

        friend ostream& operator<<(ostream& os, BaseEdge* object);
    };

    class Edge: public BaseEdge {
    public:
        Edge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight);
        Edge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2);
    };

    class DirectedEdge: public Edge {
    public:
        DirectedEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight);
        DirectedEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2);
        friend std::ostream& operator<<(std::ostream& os, DirectedEdge* object);
    };
}

#endif
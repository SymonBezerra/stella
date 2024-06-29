#include "edge.hpp"

namespace stella {
    BaseEdge::BaseEdge(string label, Node* n1, Node* n2, int weight)
        : label(label), n1(n1), n2(n2), weight(weight) {}

    BaseEdge::BaseEdge(string label, Node* n1, Node* n2)
        : label(label), n1(n1), n2(n2), weight(1) {}

    string BaseEdge::getLabel() const {
        return label;
    }

    const Node* BaseEdge::getN1() const {
        return n1;
    }

    const Node* BaseEdge::getN2() const {
        return n2;
    }

    int BaseEdge::getWeight() const {
        return weight;
    }

    std::ostream& operator<<(std::ostream& os, BaseEdge* object) {
        os << object->label << ": " << object->n1->getLabel() << "-"
           << object->n2->getLabel() << "(" << object->weight << ")";
        return os;
    }

    Edge::Edge(string label, Node* n1, Node* n2, int weight)
        : BaseEdge(label, n1, n2, weight) {}

    Edge::Edge(string label, Node* n1, Node* n2)
        : BaseEdge(label, n1, n2) {}

    DirectedEdge::DirectedEdge(string label, Node* n1, Node* n2, int weight)
        : Edge(label, n1, n2, weight) {}

    DirectedEdge::DirectedEdge(string label, Node* n1, Node* n2)
        : Edge(label, n1, n2) {}

    std::ostream& operator<<(std::ostream& os, DirectedEdge* object) {
        os << object->label << ": " << object->n1->getLabel() << "->"
           << object->n2->getLabel() << "(" << object->weight << ")";
        return os;
    }
}
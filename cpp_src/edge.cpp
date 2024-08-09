#include "edge.hpp"

namespace stella {
    BaseEdge::BaseEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight)
        : label(label), n1(n1), n2(n2), weight(weight) {}

    BaseEdge::BaseEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2)
        : label(label), n1(n1), n2(n2), weight(1) {}

    string BaseEdge::getLabel() const {
        return label;
    }

    const shared_ptr<Node> BaseEdge::getN1() const {
        return n1;
    }

    const shared_ptr<Node> BaseEdge::getN2() const {
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

    bool operator==(const BaseEdge& first, const BaseEdge& second) {
        return (first.n1->getLabel() == second.n1->getLabel()
            || first.n1->getLabel() == second.n2->getLabel())
        && (first.n2->getLabel() == second.n2->getLabel()
            || first.n2->getLabel() == second.n1->getLabel());
    }

    bool operator!=(const BaseEdge& first, const BaseEdge& second) {
        return !(first == second);
    }

    bool operator>(const BaseEdge& first, const BaseEdge& second) {
        return first.weight > second.weight;
    }

    bool operator>=(const BaseEdge& first, const BaseEdge& second) {
        return first.weight >= second.weight;
    }

    bool operator<(const BaseEdge& first, const BaseEdge& second) {
        return first.weight < second.weight;
    }

    bool operator<=(const BaseEdge& first, const BaseEdge& second) {
        return first.weight <= second.weight;
    }

    Edge::Edge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight)
        : BaseEdge(label, n1, n2, weight) {}

    Edge::Edge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2)
        : BaseEdge(label, n1, n2) {}

    DirectedEdge::DirectedEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2, int weight)
        : Edge(label, n1, n2, weight) {}

    DirectedEdge::DirectedEdge(string label, shared_ptr<Node> n1, shared_ptr<Node> n2)
        : Edge(label, n1, n2) {}

    std::ostream& operator<<(std::ostream& os, DirectedEdge* object) {
        os << object->label << ": " << object->n1->getLabel() << "->"
           << object->n2->getLabel() << "(" << object->weight << ")";
        return os;
    }

    bool operator==(const DirectedEdge& first, const DirectedEdge& second) {
        return first.n1->getLabel() == second.n1->getLabel()
        && first.n2->getLabel() == second.n2->getLabel();
    }

    bool operator!=(const DirectedEdge& first, const DirectedEdge& second) {
        return !(first == second);
    }
}
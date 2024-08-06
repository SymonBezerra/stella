#include "node.hpp"

namespace stella {
    Node::Node(string label): label(label) {}

    string Node::getLabel() const {
        return label;
    }

    std::ostream& operator<<(std::ostream& os, Node* obj) {
        os << obj->label;
        return os;
    }

    bool operator==(Node& first, Node& second) {
        return first.label == second.label;
    }

    bool operator!=(Node& first, Node& second) {
        return first.label != second.label;
    }
}
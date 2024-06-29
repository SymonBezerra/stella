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
}
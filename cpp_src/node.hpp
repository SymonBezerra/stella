#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>

using std::string;

namespace stella {
    class Node {
    protected:
        const string label;
    public:
        Node(string label);
        string getLabel() const;
        friend std::ostream& operator<<(std::ostream& os, Node* obj);
    };
}

#endif
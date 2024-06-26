#ifndef CUSTOM_ADJLIST_H
#define CUSTOM_ADJLIST_H

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <exception>

#include "stella.hpp"

class CustomAdjList: public stella::AdjList<stella::Node, stella::Edge> {
    public:
        std::vector<std::pair<std::string, std::string>> nonAdjacentPairs() {
            std::vector<stella::Node*> nodes = this->getAllNodes();
            std::map<std::string, stella::Edge*> edges = this->getAllEdges();
            std::vector<std::pair<std::string, std::string>> allPairs;

            std::vector<std::pair<std::string, std::string>> nonAdjacentPairs;

            for (int i = 0; i < nodes.size(); i++) {
                for (int j = i + 1; j < nodes.size(); j++) {
                    allPairs.push_back(std::pair<std::string, std::string>(nodes[i]->getLabel(), nodes[j]->getLabel()));
                }
            }

            for (std::pair<std::string, std::string> pair: allPairs) {
                bool nonAdjacent = true;
                for (auto edge: edges) {
                    if (pair.first == edge.second->getN1()->getLabel() && pair.second == edge.second->getN2()->getLabel()
                    || pair.first == edge.second->getN2()->getLabel() && pair.second == edge.second->getN1()->getLabel()) {
                        nonAdjacent = false;
                    }
                }
                if (nonAdjacent) nonAdjacentPairs.push_back(pair);
            }
            return nonAdjacentPairs;
        }

        bool hasLoops() {
            for (std::pair<std::string, stella::Edge*> edge: getAllEdges()) {
                if (edge.second->getN1()->getLabel() == edge.second->getN2()->getLabel())
                    return true;
            }
            return false;
        }

        int degree(std::string label) {
            stella::Node* node = getNode(label);
            if (!node) throw std::invalid_argument("Label not present at graph");
            int degree = 0;
            for (std::pair<std::string, stella::Edge*> edge: getAllEdges()) {
                if (edge.second->getN1()->getLabel() == label) degree++;
                if (edge.second->getN2()->getLabel() == label) degree++;
            }
            return degree;
        }

        bool hasParallels() {
            std::vector<stella::Edge*> edgeList;
            for (std::pair<std::string, stella::Edge*> edge: getAllEdges())
                edgeList.push_back(edge.second);

            for (int i = 0; i < edgeList.size(); i++) {
                for (int j = i + 1; j < edgeList.size(); j++) {
                    if (edgeList[i]->getN1()->getLabel() == edgeList[j]->getN1()->getLabel()
                    && edgeList[i]->getN2()->getLabel() == edgeList[j]->getN2()->getLabel()
                    || edgeList[i]->getN1()->getLabel() == edgeList[j]->getN2()->getLabel()
                    && edgeList[i]->getN2()->getLabel() == edgeList[j]->getN1()->getLabel())
                        return true;
                }
            }

            return false;
        }

        std::vector<stella::Edge*> edgesOverNode(std::string label) {
            stella::Node* node = getNode(label);
            if (!node) throw std::invalid_argument("Label not present at graph");
            std::vector<stella::Edge*> edges;

            for (std::pair<std::string, stella::Edge*> edge: getAllEdges()) {
                if (edge.second->getN1()->getLabel() == label
                || edge.second->getN2()->getLabel() == label)
                    edges.push_back(edge.second);
            }
            return edges;
        }

        bool isComplete() {
            return !hasParallels() && !hasLoops() && nonAdjacentPairs().size() == 0;
        }
};

#endif
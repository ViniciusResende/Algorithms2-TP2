#include <algorithm>
#include <chrono>
#include <queue>
#include <limits>
#include <iostream>

#include "bnb_alg.hpp"

#define FLT_MAX 3.40282347e+38

class Node {
public:
    int level; // Level in the decision tree (which city is being considered)
    float pathCost; // Cost of the path up to this point
    float bound; // Lower bound on the cost of the best possible path through this node
    std::vector<int> path; // Current path

    // Constructor
    Node(int level, float pathCost, float bound, std::vector<int> path)
        : level(level), pathCost(pathCost), bound(bound), path(path) {}

    // Overload the < operator
    bool operator<(const Node& rhs) const {
        // Modify this comparison to fit your needs
        // This example prioritizes lower bound
        return this->bound > rhs.bound;
    }
};

std::vector<std::vector<float>>* computeMinPath(std::vector<std::vector<float>> graph) {
  int n = graph.size();
  std::vector<std::vector<float>>* minPath = new std::vector(n, std::vector<float>(2, 0));

  for(int i = 0; i < n; i++) {
    float minEdge = std::numeric_limits<float>::infinity();
    float secondMinEdge = std::numeric_limits<float>::infinity();
    for(int j = 0; j < n; j++) {
      if(i != j) {
        if(graph[i][j] < minEdge) {
          secondMinEdge = minEdge;
          minEdge = graph[i][j];
        } else if(graph[i][j] < secondMinEdge) {
          secondMinEdge = graph[i][j];
        }
      }
    }

    (*minPath)[i][0] = minEdge;
    (*minPath)[i][1] = secondMinEdge;
  }

  return minPath;
}

float getBound(std::vector<std::vector<float>> graph, std::vector<std::vector<float>>* minPath, int nextVertex, Node *previous) {
  if(previous == NULL) {
    float minPathWeight = 0;
    for(size_t i = 0; i < minPath->size(); i++) {
      minPathWeight += (*minPath)[i][0];
      minPathWeight += (*minPath)[i][1];
    }

    return minPathWeight / 2;
  }

  float newEdgeCost = graph[previous->path[previous->level-1]][nextVertex];
  float newBound = previous->bound * 2;

  if(newEdgeCost >= (*minPath)[nextVertex][1]) {
    newBound -= (*minPath)[nextVertex][1];
    newBound += newEdgeCost;
  }

  if(newEdgeCost >= (*minPath)[previous->path[previous->level-1]][1]) {
    newBound -= (*minPath)[previous->path[previous->level-1]][1];
    newBound += newEdgeCost;
  }

  return newBound / 2;
}

float calculateBound(std::vector<std::vector<float>>& adjMatrix, Node& node, Node& prevNode) {
    float bound = prevNode.bound;

    // Only calculate the minimum cost edge for the new city
    size_t newCity = node.path.back();
    float minCost = FLT_MAX;
    if(node.path.size() == adjMatrix.size()) {
      minCost = adjMatrix[newCity][node.path[0]];
    } else {
      for (size_t j = 0; j < adjMatrix[newCity].size(); j++) {
        if (adjMatrix[newCity][j] < minCost && newCity != j && std::find(node.path.begin(), node.path.end(), j) == node.path.end()) {
            minCost = adjMatrix[newCity][j];
        }
      }
    }    

    bound += minCost;

    return bound;
}

bool verifySequence(std::vector<int>& vec, int n) {
  std::vector<bool> exists(n, false);

  for (int num : vec) {
    if (num < 0 || num >= n) {
      return false; // number is out of range
    }
    if (exists[num]) {
      return false; // number is repeated
    }
    exists[num] = true;
  }

  // Check if all numbers from 0 to n-1 exist
  for (bool e : exists) {
    if (!e) {
      return false; // a number is missing
    }
  }

  return true;
}

void printVector(std::vector<int> vec) {
  for(size_t i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}

std::vector<int> branchAndBound(std::vector<std::vector<float>> graph) {
  int n = graph.size();
  std::priority_queue<Node> queue;
  std::vector<int> bestPath(n);
  float bestCost = FLT_MAX;

  Node root(1, 0, 0, {0});
  root.bound = calculateBound(graph, root, root);
  queue.push(root);

  // Start the timer
  auto startTime = std::chrono::steady_clock::now();

  while(!queue.empty()) {
    // Check if the execution time exceeds 30 minutes
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime).count();
    if (elapsedTime >= 30) {
      return {};
    }

    Node node = queue.top();
    queue.pop();

    if(node.level > n) {
      if(bestCost > node.pathCost) {
        bestCost = node.pathCost; 
        bestPath = node.path;
      }
    } else if(node.bound < bestCost) {
      if(node.level < n) {
        for(int k = 1; k < n; k++) {
          bool isNotInPath = std::find(node.path.begin(), node.path.end(), k) == node.path.end();      
          bool thereIsEdge = graph[node.path[node.level-1]][k] != 0;         
          Node childNode(node.level + 1, node.pathCost + graph[node.path.back()][k], 0, node.path);
          childNode.path.push_back(k);
          childNode.bound = calculateBound(graph, childNode, node);
          if(isNotInPath && thereIsEdge && (childNode.bound < bestCost)) {        
            queue.push(childNode);       
          }
        }
      } else {   
        bool isEveryVerticesVisited = verifySequence(node.path, n);
        Node childNode(node.level + 1, node.pathCost + graph[node.path.back()][0], node.bound, node.path);
        childNode.path.push_back(0);
        if(graph[node.path[n-1]][0] != 0 && (childNode.bound < bestCost) && isEveryVerticesVisited) {     
          queue.push(childNode);    
        }
      }
    }
  }

  return bestPath;
}
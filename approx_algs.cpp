#include <algorithm>
#include <stack>
#include <limits>

#include "approx_algs.hpp"

/**
 * @class MinHeap
 * @brief A class representing a minimum heap data structure.
 * 
 * The MinHeap class provides operations to push elements, pop the minimum element,
 * access the top element, check if the heap is empty, and get the size of the heap.
 */
class MinHeap {
private:
  std::vector<std::tuple<float, int, int>> data;

  struct comparator {
    bool operator()(const std::tuple<float, int, int>& a, const std::tuple<float, int, int>& b) {
      return std::get<0>(a) > std::get<0>(b);
    }
  };

public:
  /**
   * @brief Pushes an element into the heap.
   * 
   * @param val The element to be pushed into the heap.
   */
  void push(std::tuple<float, int, int> val) {
    data.push_back(val);
    std::push_heap(data.begin(), data.end(), comparator());
  }

  /**
   * @brief Removes the minimum element from the heap.
   */
  void pop() {
    std::pop_heap(data.begin(), data.end(), comparator());
    data.pop_back();
  }

  /**
   * @brief Returns the minimum element from the heap.
   * 
   * @return The minimum element from the heap.
   */
  std::tuple<float, int, int> top() {
    return data.front();
  }

  /**
   * @brief Checks if the heap is empty.
   * 
   * @return True if the heap is empty, false otherwise.
   */
  bool empty() {
    return data.empty();
  }

  /**
   * @brief Returns the size of the heap.
   * 
   * @return The size of the heap.
   */
  size_t size() {
    return data.size();
  }
};

/**
 * Calculates the minimum spanning tree (MST) of a given graph using Prim's algorithm.
 * 
 * @param graph The input graph represented as a 2D vector of floats.
 * @return The MST of the input graph represented as a 2D vector of floats.
 */
std::vector<std::vector<float>> prim_mst(const std::vector<std::vector<float>>& graph) {
  int numVertices = graph.size();
  std::vector<std::vector<float>> mst(numVertices, std::vector<float>(numVertices, 0));
  std::vector<bool> visited(numVertices, false);
  MinHeap heap;

  // (weight, vertex, parent)
  heap.push(std::make_tuple(0.0, 0, -1));

  while (!heap.empty()) {
    auto [weight, vertex, parent] = heap.top();
    heap.pop();
    if (visited[vertex]) {
      continue;
    }
    visited[vertex] = true;
    if (parent != -1) {
      mst[parent][vertex] = weight;
      mst[vertex][parent] = weight;
    }
    for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
      if (!visited[neighbor] && graph[vertex][neighbor] != 0) {
        heap.push(std::make_tuple(graph[vertex][neighbor], neighbor, vertex));
      }
    }
  }
  return mst;
}

/**
 * @brief Performs a preorder walk on a tree represented as an adjacency matrix.
 * 
 * This function takes a tree represented as a 2D vector of floats and performs a preorder walk on it.
 * It returns a vector of integers representing the order in which the vertices were visited during the walk.
 * 
 * @param tree The tree represented as an adjacency matrix.
 * @return std::vector<int> The order in which the vertices were visited during the walk.
 */
std::vector<int> tree_preorder_walk(const std::vector<std::vector<float>>& tree) {
  int numVertices = tree.size();
  std::vector<bool> visited(numVertices, false);
  std::stack<int> stack;
  std::vector<int> walk;

  stack.push(0);

  while (!stack.empty()) {
    int vertex = stack.top();
    stack.pop();
    if (visited[vertex]) continue;
    visited[vertex] = true;
    walk.push_back(vertex);
    for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
      if (tree[vertex][neighbor] != 0) {
        stack.push(neighbor);
      }
    }
  }
  return walk;
}

/**
 * @brief Represents a sequence of integers.
 *
 * This class provides a dynamic array-like container that can store a sequence of integers.
 * It supports various operations such as adding, removing, and accessing elements.
 */
std::vector<int> approximate_tsp(const std::vector<std::vector<float>>& graph) {
  // Calculate the MST
  std::vector<std::vector<float>> mst = prim_mst(graph);

  // Perform a preorder walk on the MST
  std::vector<int> walk = tree_preorder_walk(mst);

  // Append the first vertex to the end of the walk to form a cycle
  walk.push_back(walk[0]);

  return walk;
}

/**
 * Calculates the minimum perfect matching for a given graph.
 * 
 * @param mst The minimum spanning tree to store the matching edges.
 * @param graph The input graph represented as an adjacency matrix.
 * @param vertices The list of vertices in the graph.
 */
void minimum_perfect_matching(std::vector<std::vector<float>>& mst, const std::vector<std::vector<float>>& graph, const std::vector<int>& vertices) {
  std::vector<bool> matched(vertices.size(), false);

  for (size_t i = 0; i < vertices.size(); ++i) {
    if (!matched[i]) {
      int vertex = vertices[i];
      float minWeight = std::numeric_limits<float>::max();
      int minVertex = -1;

      for (size_t j = 0; j < vertices.size(); ++j) {
        if (i != j && !matched[j] && graph[vertex][vertices[j]] < minWeight) {
          minWeight = graph[vertex][vertices[j]];
          minVertex = vertices[j];
        }
      }

      mst[vertex][minVertex] = minWeight;
      mst[minVertex][vertex] = minWeight;
      matched[i] = true;
      matched[std::find(vertices.begin(), vertices.end(), minVertex) - vertices.begin()] = true;
    }
  }
}

std::vector<int> eulerian_tour(std::vector<std::vector<float>>& graph) {
  std::vector<int> tour;
  std::stack<int> stack;

  int vertex = 0;
  while (!stack.empty() || std::count_if(graph[vertex].begin(), graph[vertex].end(), [](float weight) { return weight > 0; }) > 0) {
    if (std::count_if(graph[vertex].begin(), graph[vertex].end(), [](float weight) { return weight > 0; }) == 0) {
      tour.push_back(vertex);
      vertex = stack.top();
      stack.pop();
    } else {
      stack.push(vertex);
      int neighbor = std::distance(graph[vertex].begin(), std::find_if(graph[vertex].begin(), graph[vertex].end(), [](float weight) { return weight > 0; }));
      graph[vertex][neighbor] = 0;
      graph[neighbor][vertex] = 0;
      vertex = neighbor;
    }
  }
  tour.push_back(vertex);

  return tour;
}

std::vector<int> christofides_tsp(std::vector<std::vector<float>>& graph) {
  // Calculate the MST
  std::vector<std::vector<float>> mst = prim_mst(graph);

  // Find vertices with odd degree in the MST
  std::vector<int> oddVertices;
  for (size_t vertex = 0; vertex < mst.size(); ++vertex) {
    if (std::count_if(mst[vertex].begin(), mst[vertex].end(), [](float weight) { return weight > 0; }) % 2 != 0) {
      oddVertices.push_back(vertex);
    }
  }

  // Add minimum perfect matching to the MST
  minimum_perfect_matching(mst, graph, oddVertices);

  // Calculate an Eulerian tour of the MST
  std::vector<int> tour = eulerian_tour(mst);

  // Append the first vertex to the end of the tour to form a cycle
  tour.push_back(tour[0]);

  return tour;
}

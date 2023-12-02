#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

class MinHeap {
private:
  std::vector<std::tuple<float, int, int>> data;

  struct comparator {
    bool operator()(const std::tuple<float, int, int>& a, const std::tuple<float, int, int>& b) {
      return std::get<0>(a) > std::get<0>(b);
    }
  };

public:
  void push(std::tuple<float, int, int> val) {
    data.push_back(val);
    std::push_heap(data.begin(), data.end(), comparator());
  }

  void pop() {
    std::pop_heap(data.begin(), data.end(), comparator());
    data.pop_back();
  }

  std::tuple<float, int, int> top() {
    return data.front();
  }

  bool empty() {
    return data.empty();
  }

  size_t size() {
    return data.size();
  }
};

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

std::vector<int> approximate_tsp(const std::vector<std::vector<float>>& graph) {
    // Calculate the MST
    std::vector<std::vector<float>> mst = prim_mst(graph);

    // Perform a preorder walk on the MST
    std::vector<int> walk = tree_preorder_walk(mst);

    // Append the first vertex to the end of the walk to form a cycle
    walk.push_back(walk[0]);

    return walk;
}

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

float euclidean_distance(std::tuple<float, float> point1, std::tuple<float, float> point2) {
  float x_diff = std::get<0>(point1) - std::get<0>(point2);
  float y_diff = std::get<1>(point1) - std::get<1>(point2);
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

std::vector<std::tuple<float, float>> read_tsp_file_input(std::string file_path) {
  std::ifstream file(file_path);
  std::string line;
  int dimension = 0;

  // Read the dimension
  while (std::getline(file, line)) {
    if (line.find("DIMENSION") != std::string::npos) {
      std::istringstream iss(line);
      std::string id;
      iss >> id >> dimension;
      break;
    }
  }

  // Initialize the vector with the dimension
  std::vector<std::tuple<float, float>> points(dimension);
  bool read_coordinates = false;
  int i = 0;

  // Read the coordinates
  while (std::getline(file, line)) {
    if (line.find("NODE_COORD_SECTION") != std::string::npos) {
      read_coordinates = true;
      continue;
    }
    if (read_coordinates && !line.empty() && line.find("EOF") == std::string::npos) {
      std::istringstream iss(line);
      std::string id;
      float x, y;
      iss >> id >> x >> y;
      points[i] = std::make_tuple(x, y);
      i++;
    }
  }
  return points;
}

void fill_matrix_with_distances(std::vector<std::vector<float>>& matrix, const std::vector<std::tuple<float, float>>& points) {
  for (size_t i = 0; i < points.size(); ++i) {
    for (size_t j = 0; j < points.size(); ++j) {
      matrix[i][j] = euclidean_distance(points[i], points[j]);
    }
  }
}

float calculate_path_weight(const std::vector<std::vector<float>>& graph, const std::vector<int>& path) {
  float totalWeight = 0.0;
  for (size_t i = 0; i < path.size() - 1; ++i) {
    totalWeight += graph[path[i]][path[i + 1]];
  }
  return totalWeight;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::tuple<float, float>> points = read_tsp_file_input("data/brd14051/brd14051.tsp");
  // Print the points
  // for (const auto& point : points) {
  //   std::cout << "Point: (" << std::get<0>(point) << ", " << std::get<1>(point) << ")" << std::endl;
  // }

  // Create a matrix
  std::vector<std::vector<float>> matrix(points.size(), std::vector<float>(points.size()));
  fill_matrix_with_distances(matrix, points);

  // Approximate TSP
  std::vector<int> walk_approx = approximate_tsp(matrix);

  // Print the walk
  std::cout << "Approximate TSP Algorithm: " << std::endl;
  std::cout << "Path: [";
  for (const auto& vertex : walk_approx) {
    std::cout << vertex << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "Weight: " << calculate_path_weight(matrix, walk_approx) << std::endl;

  // Christofides TSP
  std::vector<int> walk_christofides = christofides_tsp(matrix);

  // Print the walk
  std::cout << "Christofides TSP Algorithm: " << std::endl;
  std::cout << "Path: [";
  for (const auto& vertex : walk_christofides) {
    std::cout << vertex << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "Weight: " << calculate_path_weight(matrix, walk_christofides) << std::endl;

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
  return 0;
}
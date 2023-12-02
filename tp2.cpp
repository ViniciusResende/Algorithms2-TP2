#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "approx_algs.hpp"

#define FILE_PATH "data/berlin52/berlin52.tsp"

/**
 * Calculates the Euclidean distance between two points in a 2D space.
 *
 * @param point1 The first point, represented as a tuple of two floats (x, y).
 * @param point2 The second point, represented as a tuple of two floats (x, y).
 * @return The Euclidean distance between the two points.
 */
float euclidean_distance(std::tuple<float, float> point1, std::tuple<float, float> point2) {
  float x_diff = std::get<0>(point1) - std::get<0>(point2);
  float y_diff = std::get<1>(point1) - std::get<1>(point2);
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

/**
 * Reads a TSP file input and returns a vector of tuples representing the coordinates.
 *
 * @param file_path The path to the TSP file.
 * @return A vector of tuples representing the coordinates in the TSP file.
 */
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

/**
 * Fills a matrix with distances between points.
 * 
 * This function calculates the Euclidean distance between each pair of points
 * and stores the result in the given matrix.
 * 
 * @param matrix The matrix to be filled with distances.
 * @param points The vector of points.
 */
void fill_matrix_with_distances(std::vector<std::vector<float>>& matrix, const std::vector<std::tuple<float, float>>& points) {
  for (size_t i = 0; i < points.size(); ++i) {
    for (size_t j = 0; j < points.size(); ++j) {
      matrix[i][j] = euclidean_distance(points[i], points[j]);
    }
  }
}

/**
 * Calculates the total weight of a given path in a graph.
 * 
 * @param graph The adjacency matrix representing the graph.
 * @param path The path represented as a vector of node indices.
 * @return The total weight of the path.
 */
float calculate_path_weight(const std::vector<std::vector<float>>& graph, const std::vector<int>& path) {
  float totalWeight = 0.0;
  for (size_t i = 0; i < path.size() - 1; ++i) {
    totalWeight += graph[path[i]][path[i + 1]];
  }
  return totalWeight;
}

/**
 * @brief The main function of the program.
 * 
 * This function reads input from a TSP file, creates a distance matrix,
 * and applies two different algorithms to approximate the Traveling Salesman Problem (TSP).
 * It then prints the paths and weights of the approximations, as well as the execution time.
 * 
 * @return 0 indicating successful execution of the program.
 */
int main() {
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::tuple<float, float>> points = read_tsp_file_input(FILE_PATH);
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
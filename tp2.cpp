#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "approx_algs.hpp"
#include "tsp_utils.hpp"
#include "bnb_alg.hpp"

#define FILE_PATH "data/ulyssess22/ulyssess22.tsp"

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

void print_minutes_and_second(long long microseconds) {
  int minutes = microseconds / 60000000;
  int seconds = (microseconds % 60000000) / 1000000;
  if(seconds > 0) {
    std::cout << "Execution time: " << minutes << " minutes and " << seconds << " seconds." << std::endl;
  } else {
    std::cout << "Execution time: " << microseconds << " microseconds." << std::endl;
  }
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
  // std::vector<std::vector<float>> matrix(points.size(), std::vector<float>(points.size()));
  // fill_matrix_with_distances(matrix, points);

  std::vector<std::vector<float>> matrix = std::vector<std::vector<float>>({
      {0, 10, 15, 20},
      {10, 0, 35, 25},
      {15, 35, 0, 30},
      {20, 25, 30, 0}});

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

  // BNB TSP
  std::vector<int> walk_bnb = branchAndBound(matrix);
  // std::vector<int> walk_bnb = {0, 13, 12, 11, 6, 5, 14, 4, 10, 8, 9, 18, 19, 20, 15, 2, 1, 16, 21, 3, 17, 7, 0};

  // Print the walk
  std::cout << "Branch and Bound TSP Algorithm: " << std::endl;
  std::cout << "Path: [";
  for (const auto& vertex : walk_bnb) {
    std::cout << vertex << " ";
  }
  std::cout << "]" << std::endl;
  std::cout << "Weight: " << calculate_path_weight(matrix, walk_bnb) << std::endl;

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  print_minutes_and_second(duration.count());
  return 0;
}
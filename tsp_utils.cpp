#include <cmath>

#include "tsp_utils.hpp"

float euclidean_distance(std::tuple<float, float> point1, std::tuple<float, float> point2) {
  float x_diff = std::get<0>(point1) - std::get<0>(point2);
  float y_diff = std::get<1>(point1) - std::get<1>(point2);
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
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
#include <vector>
#include <tuple>

/**
 * Calculates the Euclidean distance between two points in a 2D space.
 *
 * @param point1 The first point, represented as a tuple of two floats (x, y).
 * @param point2 The second point, represented as a tuple of two floats (x, y).
 * @return The Euclidean distance between the two points.
 */
float euclidean_distance(std::tuple<float, float> point1, std::tuple<float, float> point2);

/**
 * Fills a matrix with distances between points.
 * 
 * This function calculates the Euclidean distance between each pair of points
 * and stores the result in the given matrix.
 * 
 * @param matrix The matrix to be filled with distances.
 * @param points The vector of points.
 */
void fill_matrix_with_distances(std::vector<std::vector<float>>& matrix, const std::vector<std::tuple<float, float>>& points);

/**
 * Calculates the total weight of a given path in a graph.
 * 
 * @param graph The adjacency matrix representing the graph.
 * @param path The path represented as a vector of node indices.
 * @return The total weight of the path.
 */
float calculate_path_weight(const std::vector<std::vector<float>>& graph, const std::vector<int>& path);
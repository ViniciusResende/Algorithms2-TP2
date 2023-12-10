#include <vector>

std::vector<int> christofides_tsp(std::vector<std::vector<float>>& graph);

/**
 * @brief Approximate the Traveling Salesman Problem (TSP) using a given graph.
 * 
 * This function takes a graph represented as a 2D vector of floats and returns an approximate solution to the TSP.
 * The graph should be a symmetric matrix, where each element represents the distance between two vertices.
 * 
 * @param graph The input graph represented as a 2D vector of floats.
 * @return std::vector<int> An approximate solution to the TSP represented as a vector of integers.
 */
std::vector<int> twice_around_the_tree(const std::vector<std::vector<float>>& graph);
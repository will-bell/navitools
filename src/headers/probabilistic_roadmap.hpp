#pragma once

#include "roadmap.hpp"
#include "search_space.hpp"

Roadmap build_prm(int n_samples, int n_batch, int k_neighbors, const SearchSpace& search_space);

Roadmap testing_polygon_space_prm(int n_samples, int n_batch, int k_neighbors, std::pair<double, double> x_range, std::pair<double, double> y_range);
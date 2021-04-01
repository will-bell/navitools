#pragma once

#include "roadmap.hpp"
#include "search_space.hpp"

Roadmap build_prm(int n_samples, int n_batch, int k_neighbors, const SearchSpace& search_space);
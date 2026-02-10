#pragma once

#include <array>
#include <vector>

using mesh = std::vector<std::array<std::array<float, 3>, 3>>;
mesh getTriangles(const std::vector<std::pair<std::array<float, 3>, unsigned short>>& cell, std::pair<unsigned short, unsigned short>& innerRange, int &cornerSet, std::vector<int>& edges);
// MarchingCubes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <array>
#include <iostream>
#include <map>
#include <vector>

using mesh = std::vector<std::array<std::array<float, 3>, 3>>;


std::vector<int> getVertices(const std::byte cellConfiguration);


/// <summary>
/// This function returns an array of triangle vertices included in mesh
/// </summary>
/// <param name="cell">8 vertices of cell</param>
/// <returns>mesh as an array of triangles</returns>
mesh getTriangles(const std::vector<std::pair<std::array<float,3>, unsigned short>> &cell, std::pair<unsigned short, unsigned short> &innerRange)
{
	mesh trianglesArray;

	int count = 0;
	std::byte cornerSet;

	for (const auto& c : cell)
	{
		//this is outter point
		if (c.second < innerRange.first || c.second > innerRange.second) 
		{
			cornerSet |= static_cast<std::byte>(1 << count);
		}

		count++;
	}



	return trianglesArray;
}


int main()
{

}

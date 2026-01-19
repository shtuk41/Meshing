// PolygonizeVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <ioData.h>
#include <MarchingCubes.h>

struct Vec3 { float x, y, z; };
struct Triangle { Vec3 normal; Vec3 v1, v2, v3; uint16_t attr = 0; };

void writeMeshToStl(const std::string& stlFileName, mesh& meshToStore);

int main()
{
	std::string volumeFile(R"(D:\Files\CTLab\SaveVolumeToFile\volumeHeader.uint16_scv)");

	VolumeData vd(volumeFile);
	vd.fillBuffer();

	std::cout << vd.getHeaderString() << std::endl;

	std::vector<unsigned char> buffer = vd.getVolumeDataTex();

	auto header = vd.getHeader();

	const int width = header->recoX;
	const int height = header->recoY;
	const int depth = header->recoZ;

	const float voxSizeX = static_cast<float>(header->voxSizeX);
	const float voxSizeY = static_cast<float>(header->voxSizeY);
	const float voxSizeZ = static_cast<float>(header->voxSizeZ);

	mesh fullMesh;

    for (int z = 0; z < depth - 1; ++z)
    {
        for (int y = 0; y < height - 1; ++y)
        {
            for (int x = 0; x < width - 1; ++x)
            {
				std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;
				cell.reserve(8);

				std::array<float, 3> aindex0 = {x * voxSizeX, y * voxSizeY, z * voxSizeZ};
				std::array<float, 3> aindex1 = {(x + 1) * voxSizeX, y * voxSizeY, z * voxSizeZ};
				std::array<float, 3> aindex2 = {x * voxSizeX, y * voxSizeY, (z + 1) * voxSizeZ};
				std::array<float, 3> aindex3 = {(x + 1) * voxSizeX, y * voxSizeY, (z + 1) * voxSizeZ};
				std::array<float, 3> aindex4 = { x * voxSizeX, (y + 1) * voxSizeY, z * voxSizeZ };
				std::array<float, 3> aindex5 = { (x + 1) * voxSizeX, (y + 1) * voxSizeY, z * voxSizeZ };
				std::array<float, 3> aindex6 = { x * voxSizeX, (y + 1) * voxSizeY, (z + 1) * voxSizeZ };
				std::array<float, 3> aindex7 = { (x + 1) * voxSizeX, (y + 1) * voxSizeY, (z + 1) * voxSizeZ };

				unsigned short usIndex0 = static_cast<unsigned short>(buffer[x + width * (y + height * z)]);
				unsigned short usIndex1 = static_cast<unsigned short>(buffer[(x + 1) + width * (y + height * z)]);
				unsigned short usIndex2 = static_cast<unsigned short>(buffer[x + width * (y + height * (z + 1))]);
				unsigned short usIndex3 = static_cast<unsigned short>(buffer[(x + 1) + width * (y + height * (z + 1))]);
				unsigned short usIndex4 = static_cast<unsigned short>(buffer[x + width * ((y + 1) + height * z)]);
				unsigned short usIndex5 = static_cast<unsigned short>(buffer[(x + 1) + width * ((y + 1) + height * z)]);
				unsigned short usIndex6 = static_cast<unsigned short>(buffer[x + width * ((y + 1) + height * (z + 1))]);
				unsigned short usIndex7 = static_cast<unsigned short>(buffer[(x + 1) + width * ((y + 1) + height * (z + 1))]);

				cell.push_back({ aindex0, usIndex0 });
				cell.push_back({ aindex1, usIndex1 });
				cell.push_back({ aindex2, usIndex2 });
				cell.push_back({ aindex3, usIndex3 });
				cell.push_back({ aindex4, usIndex4 });
				cell.push_back({ aindex5, usIndex5 });
				cell.push_back({ aindex6, usIndex6 });
				cell.push_back({ aindex7, usIndex7 });

				std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

				int cornerSet{ 0 };

				mesh localMesh = getTriangles(cell, innerRange, cornerSet);

				fullMesh.insert(fullMesh.end(), localMesh.begin(), localMesh.end());
            }
        }
    }

	return 0;
}

Vec3 computeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
	Vec3 u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
	Vec3 v = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
	Vec3 n = { u.y * v.z - u.z * v.y,
			   u.z * v.x - u.x * v.z,
			   u.x * v.y - u.y * v.x };
	float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	if (len > 0) { n.x /= len; n.y /= len; n.z /= len; }
	return n;
}

void writeMeshToStl(const std::string& stlFileName, mesh& meshToStore)
{
	std::ofstream stl(stlFileName.c_str(), std::ios::binary);
	char header[80] = {};
	stl.write(header, 80);

	size_t count = meshToStore.size();
	stl.write(reinterpret_cast<char*>(&count), sizeof(count));

	if (stl.is_open())
	{
		for (auto& t : meshToStore)
		{
			Triangle triangle;

			triangle.v1.x = t[0][0];
			triangle.v1.y = t[0][1];
			triangle.v1.z = t[0][2];

			triangle.v2.x = t[1][0];
			triangle.v2.y = t[1][1];
			triangle.v2.z = t[1][2];

			triangle.v3.x = t[2][0];
			triangle.v3.y = t[2][1];
			triangle.v3.z = t[2][2];

			triangle.normal = computeNormal(triangle.v1, triangle.v2, triangle.v3);

			stl.write(reinterpret_cast<char*>(&triangle), sizeof(Triangle));
		}
	}
	else
	{
		throw std::exception("can't open stl file");
	}

	stl.close();
}



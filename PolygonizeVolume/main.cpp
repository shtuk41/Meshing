// PolygonizeVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <ioData.h>
#include <MarchingCubes.h>

struct Vec3 
{ 
	float x, y, z; 
};

#pragma pack(push, 1) // Forces 1-byte alignment (no padding)
struct Triangle 
{ 
	Vec3 normal; 
	Vec3 v1, v2, v3; 
	uint16_t attr = 0; 
};
#pragma pack(pop)

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

	//size_t dstIdx = x + width * (y + height * z);

	//std::vector<unsigned char> buffer = {0,0,0,0,0,0,0,0,0,
	//									0,0,0,0,255,0,0,0,0,
	//									0,0,0,0,0,0,0,0,0};

	//const int width = 3;
	//const int height = 3;
	//const int depth = 3;

	//const float voxSizeX = 5; 
	//const float voxSizeY = 5; 
	//const float voxSizeZ = 5; 

	const float voxSizeX = static_cast<float>(header->voxSizeX);
	const float voxSizeY = static_cast<float>(header->voxSizeY);
	const float voxSizeZ = static_cast<float>(header->voxSizeZ);

	unsigned int bufferSize = static_cast<unsigned int>(buffer.size());

	mesh fullMesh;

    for (int z = -1; z < depth; ++z)
    {
        for (int y = -1; y < height; ++y)
        {
            for (int x = -1; x < width; ++x)
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


				unsigned int index = x + width * (y + height * z);
				unsigned short usIndex0 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = (x + 1) + width * (y + height * z);
				unsigned short usIndex1 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = x + width * (y + height * (z + 1));
				unsigned short usIndex2 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = (x + 1) + width * (y + height * (z + 1));
				unsigned short usIndex3 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = x + width * ((y + 1) + height * z);
				unsigned short usIndex4 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = (x + 1) + width * ((y + 1) + height * z);
				unsigned short usIndex5 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = x + width * ((y + 1) + height * (z + 1));
				unsigned short usIndex6 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);
				index = (x + 1) + width * ((y + 1) + height * (z + 1));
				unsigned short usIndex7 = static_cast<unsigned short>((index < 0 || index >= bufferSize) ? 0 : buffer[index]);

				cell.push_back({ aindex0, usIndex0 });
				cell.push_back({ aindex1, usIndex1 });
				cell.push_back({ aindex2, usIndex2 });
				cell.push_back({ aindex3, usIndex3 });
				cell.push_back({ aindex4, usIndex4 });
				cell.push_back({ aindex5, usIndex5 });
				cell.push_back({ aindex6, usIndex6 });
				cell.push_back({ aindex7, usIndex7 });

				std::pair<unsigned short, unsigned short> innerRange = { 100,300 };

				int cornerSet{ 0 };

				mesh localMesh = getTriangles(cell, innerRange, cornerSet);

				fullMesh.insert(fullMesh.end(), localMesh.begin(), localMesh.end());
            }
        }
    }


	std::cout << "mesh size: " << fullMesh.size() << std::endl;
	writeMeshToStl("test.stl", fullMesh);

	return 0;
}

Vec3 computeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
	// Edge vectors
	Vec3 u = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
	Vec3 v = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };

	// ORIGINAL CROSS PRODUCT: u cross v
	Vec3 n = {
		//v.y * u.z - v.z * u.y,
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	};

	float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	if (len > 0) { n.x /= len; n.y /= len; n.z /= len; }
	return n;
}

void writeMeshToStl(const std::string& stlFileName, mesh& meshToStore)
{
	std::ofstream stl(stlFileName.c_str(), std::ios::binary);
	if (!stl.is_open()) throw std::runtime_error("can't open stl file");

	char header[80] = "created by aleksander shtuk 2026";
	stl.write(header, 80);

	// FIX 1: Use uint32_t to ensure exactly 4 bytes are written
	uint32_t count = static_cast<uint32_t>(meshToStore.size());
	stl.write(reinterpret_cast<char*>(&count), 4);

	for (auto& t : meshToStore)
	{
		Triangle triangle;

		triangle.v1.x = (float)t[0][0];
		triangle.v1.y = (float)t[0][1];
		triangle.v1.z = (float)t[0][2];

		triangle.v2.x = (float)t[1][0];
		triangle.v2.y = (float)t[1][1];
		triangle.v2.z = (float)t[1][2];

		triangle.v3.x = (float)t[2][0];
		triangle.v3.y = (float)t[2][1];
		triangle.v3.z = (float)t[2][2];

		triangle.normal = computeNormal(triangle.v1, triangle.v2, triangle.v3);

		// FIX 2: Write exactly 50 bytes per triangle (Normal + 3 Vertices + 2-byte attribute)
		// This prevents struct padding from corrupting the file.
		stl.write(reinterpret_cast<char*>(&triangle.normal), 12);
		stl.write(reinterpret_cast<char*>(&triangle.v1), 12);
		stl.write(reinterpret_cast<char*>(&triangle.v3), 12);
		stl.write(reinterpret_cast<char*>(&triangle.v2), 12);

		uint16_t attributeByteCount = 0; // Required 2-byte spacer
		stl.write(reinterpret_cast<char*>(&attributeByteCount), 2);
	}

	stl.close();
}



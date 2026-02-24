// PolygonizeVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

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
uint64_t getVertexPosition(uint64_t x, uint64_t y, uint64_t z, uint64_t edge);

struct Vertex
{
	float x, y, z;
	unsigned int halfEdgeRef;
};

Vertex getVertex(uint64_t vertexPosition, uint32_t edgeReference, float voxelSizeX, float voxelSizeY, float voxelSizeZ);

struct Face
{
	unsigned int halfEdgeRef;
};

struct HalfEdge
{
	unsigned int vertexRef;
	unsigned int faceRef;
	unsigned int nextHalfEdgeRef;
	unsigned int prevHalfEdgeRef;
	int oppositeHalfEdgeRef = -1;
};

std::unordered_map<uint64_t, unsigned int> vertexMap;
std::unordered_map<uint64_t, unsigned int> halfEdgeMap;
std::vector<Vertex> vertices;
std::vector<HalfEdge> halfEdges;
std::vector<Face> faces;

unsigned int vertexCounter = 0;
unsigned int halfEdgeCounter = 0;
unsigned int faceCounter = 0;

int main()
{
	//std::string volumeFile(R"(D:\Files\CTLab\SaveVolumeToFile\volumeHeader.uint16_scv)");
	//std::string volumeFile(R"(D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv)");
	//std::string volumeFile(R"(D:\Files\Cesars\BostonSci_109134204_F 2025-8-14 11-39-3.uint16_scv)");

	//VolumeData vd(volumeFile);
 	//vd.fillBuffer();

	//std::cout << vd.getHeaderString() << std::endl;

	//std::vector<unsigned char> buffer = vd.getVolumeDataTex();

	//auto header = vd.getHeader();

	//const int width = header->recoX;
	//const int height = header->recoY;
	//const int depth = header->recoZ;

	//size_t dstIdx = x + width * (y + height * z);

	/*std::vector<unsigned char> buffer = {0,0,0,0,0,0,0,0,0,
										0,0,0,0,255,0,0,0,0,
										0,0,0,0,0,0,0,0,0};

	const int width = 3;
	const int height = 3;
	const int depth = 3;

	std::vector<unsigned char> buffer = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										  0,0,0,0,0,255,255,0, 0,0,0,0,0,0,0,0,
										  0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0, 
									      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	const int width = 4;
	const int height = 4;
	const int depth = 4;*/


	/*std::vector<unsigned char> buffer(22 * 22 * 22, 0);

	int N = 22;
	double cx = 11.0;
	double cy = 11.0;
	double cz = 11.0;
	double R = 6.0;
	double r = 3.0;

	for (int z = 0; z < N; ++z)
	{
		for (int y = 0; y < N; ++y)
		{
			for (int x = 0; x < N; ++x)
			{
				double dx = x - cx;
				double dy = y - cy;
				double dz = z - cz;

				double q = std::sqrt(dx * dx + dy * dy) - R;
				double torus = q * q + dz * dz;

				if (torus <= r * r)
					buffer[z * N * N + y * N + x] = 255;
			}
		}
	}*/

	int NX = 32;
	int NY = 22;
	int NZ = 22;

	std::vector<unsigned char> buffer(NX * NY * NZ, 0);

	double cy = 11.0;
	double cz = 11.0;

	double R = 6.0;
	double r = 3.0;

	double cx1 = 10.0;
	double cx2 = 20.0;

	for (int z = 0; z < NZ; ++z)
	{
		for (int y = 0; y < NY; ++y)
		{
			for (int x = 0; x < NX; ++x)
			{
				double dy = y - cy;
				double dz = z - cz;

				// Torus 1
				double dx1 = x - cx1;
				double q1 = std::sqrt(dx1 * dx1 + dy * dy) - R;
				double t1 = q1 * q1 + dz * dz;

				// Torus 2
				double dx2 = x - cx2;
				double q2 = std::sqrt(dx2 * dx2 + dy * dy) - R;
				double t2 = q2 * q2 + dz * dz;

				if (t1 <= r * r || t2 <= r * r)
					buffer[z * NX * NY + y * NX + x] = 255;
			}
		}
	}


	const int width = NX;
	const int height = NY;
	const int depth = 22; 

	const float voxSizeX = 5;
	const float voxSizeY = 5;
	const float voxSizeZ = 5; 


	//const float voxSizeX = static_cast<float>(header->voxSizeX);
	//const float voxSizeY = static_cast<float>(header->voxSizeY);
	//const float voxSizeZ = static_cast<float>(header->voxSizeZ);

	unsigned int bufferSize = static_cast<unsigned int>(buffer.size());

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
				//std::pair<unsigned short, unsigned short> innerRange = { 100,120 };
				int cornerSet{ 0 };

				std::vector<int> edges;

				mesh localMesh = getTriangles(cell, innerRange, cornerSet, edges);

				fullMesh.insert(fullMesh.end(), localMesh.begin(), localMesh.end());

				//a. check if a vertex exists, if not insert into vertex collection
				//b. get second vertex
				//c. check if halfedge exists, if not add it to collection, check if opposite halfedge exists, if so, assign reverses to both halfedges  
				//d. Add face into collection, reference the half edge
				//e. get third vertex. do procedure C for two halfedges.

				int eCount = 0;
				uint64_t pointPosition1;
				uint64_t pointPosition2;
				uint64_t pointPosition3;
				uint32_t vid1, vid2, vid3;
				uint32_t hid1, hid2, hid3;

				HalfEdge edge1, edge2, edge3;

				for (auto e : edges)
				{
					if ((eCount % 3) == 0)
					{
						edge1.oppositeHalfEdgeRef = -1;
						edge2.oppositeHalfEdgeRef = -1;
						edge3.oppositeHalfEdgeRef = -1;

						//A
						pointPosition1 = getVertexPosition(x, y, z, e);

						auto pm = vertexMap.find(pointPosition1);
						if (pm != vertexMap.end())
						{
							vid1 = pm->second;
						}
						else
						{
							vid1 = vertexCounter++;
							vertexMap.insert({pointPosition1, vid1});

							Vertex v = getVertex(pointPosition1, -1, voxSizeX, voxSizeY, voxSizeZ);
							vertices.push_back(v);
						}
					}
					else if ((eCount % 3) == 1)
					{
						//B
						pointPosition2 = getVertexPosition(x, y, z, e);
						bool vid2Found = false;

						auto pm = vertexMap.find(pointPosition2);
						if (pm != vertexMap.end())
						{
							vid2 = pm->second;
							vid2Found = true;
						}
						else
						{
							vid2 = vertexCounter++;
							vertexMap.insert({ pointPosition2, vid2 });

							Vertex v = getVertex(pointPosition2, -1, voxSizeX, voxSizeY, voxSizeZ);
							vertices.push_back(v);
						}

						uint64_t halfedge1Key = ((uint64_t)vid1 << 32) | vid2;

						auto hem = halfEdgeMap.find(halfedge1Key);

						if (hem != halfEdgeMap.end())
						{
							throw std::exception("halfedge exists");
							
						}
						else
						{
							hid1 = halfEdgeCounter++;

#ifdef DEBUG_PRINT
							std::cout << "Case 1: " <<  halfedge1Key << "," << hid1 << "," << vid1 << "," << vid2 << std::endl;
#endif

							halfEdgeMap.insert({ halfedge1Key , hid1});

							uint64_t halfedge1KeyOpposite = ((uint64_t)vid2 << 32) | vid1;
							auto hemOpposite = halfEdgeMap.find(halfedge1KeyOpposite);

							if (hemOpposite != halfEdgeMap.end())
							{
								uint32_t oppositeId = hemOpposite->second;
								halfEdges[oppositeId].oppositeHalfEdgeRef = hid1;

								edge1.oppositeHalfEdgeRef = oppositeId;
							}

							//halfEdges.push_back(edge1);

							if (vertices[vid2].halfEdgeRef == 0xffffffff)
							{
								vertices[vid2].halfEdgeRef = hid1;
							}

							Face f;
							f.halfEdgeRef = hid1;

							faces.push_back(f);
							faceCounter++;
						}
					}
					else
					{
						pointPosition3 = getVertexPosition(x, y, z, e);
						bool vid3Found = false;

						auto pm = vertexMap.find(pointPosition3);
						if (pm != vertexMap.end())
						{
							vid3 = pm->second;
							vid3Found = true;
						}
						else
						{
							vid3 = vertexCounter++;
							vertexMap.insert({ pointPosition3, vid3 });

							Vertex v = getVertex(pointPosition3, -1, voxSizeX, voxSizeY, voxSizeZ);
							vertices.push_back(v);
						}

						uint64_t halfedge2Key = ((uint64_t)vid2 << 32) | vid3;

						auto hem = halfEdgeMap.find(halfedge2Key);

						if (hem != halfEdgeMap.end())
						{
							throw std::exception("halfedge exists");

						}
						else
						{
							hid2 = halfEdgeCounter++;

#ifdef DEBUG_PRINT
							std::cout << "Case 2: " << halfedge2Key << "," << hid2 << "," << vid2 << "," << vid3 << std::endl;
#endif
							halfEdgeMap.insert({ halfedge2Key , hid2 });

							uint64_t halfedge2KeyOpposite = ((uint64_t)vid3 << 32) | vid2;
							auto hemOpposite = halfEdgeMap.find(halfedge2KeyOpposite);

							if (hemOpposite != halfEdgeMap.end())
							{
								uint32_t oppositeId = hemOpposite->second;
								halfEdges[oppositeId].oppositeHalfEdgeRef = hid2;

								edge2.oppositeHalfEdgeRef = oppositeId;
							}
							
							//halfEdges.push_back(edge2);

							if (vertices[vid3].halfEdgeRef == 0xffffffff)
							{
								vertices[vid3].halfEdgeRef = hid2;
							}
						}

						uint64_t halfedge3Key = ((uint64_t)vid3 << 32) | vid1;

						hem = halfEdgeMap.find(halfedge3Key);

						if (hem != halfEdgeMap.end())
						{
							throw std::exception("halfedge exists");

						}
						else
						{
							hid3 = halfEdgeCounter++;
							halfEdgeMap.insert({ halfedge3Key , hid3 });

							uint64_t halfedge3KeyOpposite = ((uint64_t)vid1 << 32) | vid3;
							auto hemOpposite = halfEdgeMap.find(halfedge3KeyOpposite);

							if (hemOpposite != halfEdgeMap.end())
							{
								uint32_t oppositeId = hemOpposite->second;
								halfEdges[oppositeId].oppositeHalfEdgeRef = hid3;

								edge3.oppositeHalfEdgeRef = oppositeId;
							}

							//halfEdges.push_back(edge3);

							if (vertices[vid1].halfEdgeRef == 0xffffffff)
							{
								vertices[vid1].halfEdgeRef = hid3;
							}
						}

						edge1.vertexRef = vid2;
						edge1.faceRef = faceCounter - 1;
						edge1.nextHalfEdgeRef = hid2;
						edge1.prevHalfEdgeRef = hid3;

						halfEdges.push_back(edge1);

						edge2.vertexRef = vid3;
						edge2.faceRef = faceCounter - 1;
						edge2.nextHalfEdgeRef = hid3;
						edge2.prevHalfEdgeRef = hid1;

						halfEdges.push_back(edge2);

						edge3.vertexRef = vid1;
						edge3.faceRef = faceCounter - 1;
						edge3.nextHalfEdgeRef = hid1;
						edge3.prevHalfEdgeRef = hid2;

						halfEdges.push_back(edge3);
					}

					eCount++;
				}
            }
        }
    }

	std::cout << "mesh size: " << fullMesh.size() << std::endl;

	int64_t eulerNumber = ((int64_t)vertices.size() - (int64_t)(halfEdges.size() / 2) + (int64_t)faces.size());
	std::cout << "Euler: " << eulerNumber << std::endl;
	writeMeshToStl("testboston.stl", fullMesh);

	return 0;
}

uint64_t getVertexPosition(uint64_t x, uint64_t y, uint64_t z, uint64_t edge)
{
	int axis;

	switch (edge)
	{
		case 10:
			axis = 0; 
			break;
		case 23:
			z += 1;
			axis = 0; 
			break;
		case 45:
			y += 1;
			axis = 0;
			break;
		case 67:
			y += 1;
			z += 1;
			axis = 0; 
			break;
		case 40:
			axis = 1;
			break;
		case 15:
			x += 1;
			axis = 1; 
			break;
		case 26:
			z += 1;
			axis = 1; 
			break;
		case 37:
			x += 1;
			z += 1;
			axis = 1; 
			break;
		case 20:
			axis = 2; 
			break;
		case 13: 
			x += 1;
			axis = 2; 
			break;
		case 46:
			y += 1;
			axis = 2; 
			break;
		case 57:
			x += 1;
			y += 1;
 			axis = 2; 
			break;
		default:
			throw std::exception("edge is undefined");
	}

	uint64_t edgeIdentifier =  (x << 30) | (y << 16) | (z << 2) | axis;

	return edgeIdentifier;
}

Vertex getVertex(uint64_t vertexPosition,
				uint32_t edgeReference,
				float voxelSizeX, 
				float voxelSizeY, 
				float voxelSizeZ)
{
	Vertex v;

	int direction = vertexPosition & 0x3;

	v.x = (static_cast<float>((vertexPosition >> 30) & 0x3fff) + (direction == 0 ? 0.5f : 0.0f)) * voxelSizeX;
	v.y = (static_cast<float>((vertexPosition >> 16) & 0x3fff) + (direction == 1 ? 0.5f : 0.0f)) * voxelSizeY;
	v.z = (static_cast<float>((vertexPosition >> 2) & 0x3fff) + (direction == 2 ? 0.5f : 0.0f)) * voxelSizeZ;

	v.halfEdgeRef = edgeReference;

	return v;
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

		triangle.v1.x = (float)t[1][0];
		triangle.v1.y = (float)t[1][1];
		triangle.v1.z = (float)t[1][2];

		triangle.v2.x = (float)t[0][0];
		triangle.v2.y = (float)t[0][1];
		triangle.v2.z = (float)t[0][2];

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



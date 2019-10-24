#pragma once
#include "Geometry.h"
	
class SquraGeometry : public cGeometry
{
public:
	SquraGeometry(int w, int h)
	{
		//for (int i = 0; i < n)
	}
	SquraGeometry()
	{
		m_vertices.clear();
		m_indices.clear();
		m_vertices.push_back(cGeometryVertex(-1, -1, 0, 0, 0));
		m_vertices.push_back(cGeometryVertex(1, -1, 0, 1, 0));
		m_vertices.push_back(cGeometryVertex(-1, 1, 0, 0, 1));
		m_vertices.push_back(cGeometryVertex(1, 1, 0, 1, 1));
		m_indices = std::vector<unsigned int>({0, 1, 2, 1, 3, 2});
	}
};


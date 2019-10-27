#pragma once
#include "Geometry.h"
	
class SquraGeometry : public cGeometry
{
public:
	SquraGeometry(int w, int h)
	{
		m_vertices.clear();
		m_indices.clear();
		for (int i = 0; i <= h; i++)
		{
			for (int j = 0; j <= w; j++)
			{

				m_vertices.push_back(cGeometryVertex(-1 + 2.0 * j / w, -1 + 2.0 * i / h, 0, 1.0 * j / w, 1.0 * i / h));

			}
		}
		for (int i = 1; i <= h; i++)
		{
			for (int j = 1; j <= w; j++)
			{
				m_indices.push_back((i - 1) * (w + 1) + j - 1);
				m_indices.push_back((i - 1) * (w + 1) + j);
				m_indices.push_back(i * (w + 1) + j - 1);
				m_indices.push_back((i - 1) * (w + 1) + j);
				m_indices.push_back(i * (w + 1) + j);
				m_indices.push_back(i * (w + 1) + j - 1);
			}
		}
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


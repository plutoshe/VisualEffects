#pragma once
#include <vector>
class cGeometryVertex
{
public:
	float m_x, m_y, m_z;
	float m_u, m_v;
	cGeometryVertex() { m_x = m_y = m_z = 0; m_u = m_v = 0; }
	cGeometryVertex(float x, float y, float z, float u, float v) { m_x = x; m_y = y; m_z = z; m_u = u; m_v = v; }
	cGeometryVertex(const cGeometryVertex& c) { this->Equal(c); }
	void operator = (const cGeometryVertex& c) { this->Equal(c); }
	void Equal(const cGeometryVertex& c) { m_x = c.m_x; m_y = c.m_y; m_z = c.m_z; m_u = c.m_u; m_v = c.m_v; }
};

class cGeometry
{
public:
	std::vector<cGeometryVertex> m_vertices;
	std::vector<unsigned int> m_indices;
	cGeometry() {}
};


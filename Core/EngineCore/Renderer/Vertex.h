#pragma once

struct Vertex
{
	float posX, posY, posZ, posW;  // Position data
	float r, g, b, a;              // Color
};

#define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f
#define UV(_u_, _v_) (_u_), (_v_)
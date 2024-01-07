#pragma once
class CCordnt
{
public:
	float minX, minY, minZ, maxX, maxY, maxZ;
	float lx, mx, nx; //新坐标系中X轴的方向余弦
	float ly, my, ny; //新坐标系中Y轴的方向余弦
	float lz, mz, nz; //新坐标系中Z轴的方向余弦
	float u, v, w; ////新坐标系坐标原点的平移量
	float mu, mv, mw; ////新坐标系坐标原点的平移量
	float Scale;//zoom scale

	CCordnt(void);
	~CCordnt(void);
};


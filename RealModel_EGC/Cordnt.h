#pragma once
class CCordnt
{
public:
	float minX, minY, minZ, maxX, maxY, maxZ;
	float lx, mx, nx; //������ϵ��X��ķ�������
	float ly, my, ny; //������ϵ��Y��ķ�������
	float lz, mz, nz; //������ϵ��Z��ķ�������
	float u, v, w; ////������ϵ����ԭ���ƽ����
	float mu, mv, mw; ////������ϵ����ԭ���ƽ����
	float Scale;//zoom scale

	CCordnt(void);
	~CCordnt(void);
};


#pragma once
#include"MeiGang.h"

class CViewPara
{
public:
	point3D SceneAngle;             //��ͼ������ת�Ƕ�
	point3D ScenePos;               //��ͼ����ƽ��λ��
	double  SceneZoom;              //��ͼ����ϵ��
	double  SceneDepth;             //��ͼ�����

	CViewPara(void);
	~CViewPara(void);
};


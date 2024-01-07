#pragma once
#include"MeiGang.h"

class CViewPara
{
public:
	point3D SceneAngle;             //视图场景旋转角度
	point3D ScenePos;               //视图场景平移位置
	double  SceneZoom;              //视图比例系数
	double  SceneDepth;             //视图区深度

	CViewPara(void);
	~CViewPara(void);
};


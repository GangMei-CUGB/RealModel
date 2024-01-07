//gang.mei@geologie.uni-freiburg.de

#define OPERATOR_VIEW_NULL      0         //定义视口操作为空
#define OPERATOR_VIEW_ZOOM      1         //定义视口放缩操作
#define OPERATOR_VIEW_ROTATE    2         //定义视口旋转操作
#define OPERATOR_VIEW_MOVE      3         //定义视口平移操作

#define SHOW_MODE_NULL          9000         //定义空显示模式
#define SHOW_MODE_SOLID         9001         //定义实体显示模式
#define SHOW_MODE_LINE          9002         //定义线框显示模式

#define DISPLAY_MODE_NULL          0  // NULL
#define DISPLAY_MODE_POINT         1  // Point
#define DISPLAY_MODE_LINE          2  // Line
#define DISPLAY_MODE_AREA          3  // Area
#define DISPLAY_MODE_SURFACE       4  // Surface
#define DISPLAY_MODE_POLYHEDRA     5  // Polyhedra  
#define DISPLAY_MODE_BLOCK         6  // Block

#define OBJECT_TYPE_SURFACE        11000  // Surface
#define OBJECT_TYPE_POLYHEDRA      11001  // Polyhedra

// For Antialising
#define DISPLAY_ANTIALISING_OFF         0  // Turn OFF
#define DISPLAY_ANTIALISING_ON          1  // Turn ON

#define PI 3.1415926

// For Selecting
#define SELECT_POINT         0  // Point
#define SELECT_LINE          1  // Line
#define SELECT_AREA          2  // Area
#define SELECT_SURFACE       3  // Surface
#define SELECT_POLYHEDRA     4  // Polyhedra  
#define SELECT_BLOCK         5  // Block

#define SEL_BY_NUM_PICK      0  // By Num/Pick
#define SEL_BY_PROPERTY      1  // By Property
#define SEL_BY_ADJACENT      2  // By Adjacent to

#define PROP_BY_ID           0  // By ID
#define PROP_BY_NAME         1  // By Name
#define PROP_BY_AGE          2  // By Age
#define PROP_BY_MATERIAL     3  // By Material

struct point2D{
	float x,y;
};

struct point3D{
	float x,y,z;
};

struct point4D{
	float x, y, z, w;
};

struct glcolor{
	float r, g, b;
};



/*
#define OPERATOR_VIEW_NULL      2001         //定义视口操作为空
#define OPERATOR_VIEW_ZOOM      2002         //定义视口放缩操作
#define OPERATOR_VIEW_ROTATE    2003         //定义视口旋转操作
#define OPERATOR_VIEW_TRANSLATE 2004         //定义视口平移操作

#define SHOW_MODE_NULL          9000         //定义空显示模式
#define SHOW_MODE_SOLID         9001         //定义实体显示模式
#define SHOW_MODE_LINE          9002         //定义线框显示模式

#define DISPLAY_MODE_POINT         10000  // Point
#define DISPLAY_MODE_LINE          10001  // Line
#define DISPLAY_MODE_AREA          10002  // Area
#define DISPLAY_MODE_SURFACE       10003  // Surface
#define DISPLAY_MODE_POLYHEDRA     10004  // Polyhedra  
#define DISPLAY_MODE_BLOCK         10005  // Block

#define OBJECT_TYPE_SURFACE        11000  // Surface
#define OBJECT_TYPE_POLYHEDRA      11001  // Polyhedra

// For Antialising
#define DISPLAY_ANTIALISING_OFF         0  // Turn OFF
#define DISPLAY_ANTIALISING_ON          1  // Turn ON

#define PI 3.1415926

// For Selecting
#define SELECT_POINT         0  // Point
#define SELECT_LINE          1  // Line
#define SELECT_AREA          2  // Area
#define SELECT_SURFACE       3  // Surface
#define SELECT_POLYHEDRA     4  // Polyhedra  
#define SELECT_BLOCK         5  // Block

#define SEL_BY_NUM_PICK      0  // By Num/Pick
#define SEL_BY_PROPERTY      1  // By Property
#define SEL_BY_ADJACENT      2  // By Adjacent to

#define PROP_BY_ID           0  // By ID
#define PROP_BY_NAME         1  // By Name
#define PROP_BY_AGE          2  // By Age
#define PROP_BY_MATERIAL     3  // By Material

struct point2D{
	float x,y;
};

struct point3D{
	float x,y,z;
};

struct point4D{
	float x, y, z, w;
};

struct glcolor{
	float r, g, b;
};
*/
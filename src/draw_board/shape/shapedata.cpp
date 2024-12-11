#include "shapedata.h"

ShapeData::ShapeData()
{
    m_ShapeType = EShapeType::Shape_Unkonwn;
}

void ShapeData::rotatePoint(QPoint &BasePoint,QPoint &point, double arg = 0.0)
{
    double sina = sin(arg*PI);
    double cosa = cos(arg*PI);
    int px = point.x();
    int py = point.y();
    point.setX( BasePoint.x()+(px-BasePoint.x())*cosa-(py-BasePoint.y())*sina + 0.5);
    point.setY( BasePoint.y()+(px-BasePoint.x())*sina+(py-BasePoint.y())*cosa + 0.5);
}

void ShapeData::rotate(QPoint &BasePoint, double arg = 0.0)
{
    qDebug()<<"rotate ShapeData";
}

ShapeData::~ShapeData()
{

}

//已知直线上的两点P1(X1,Y1) P2(X2,Y2)， P1 P2两点不重合。则直线的一般式方程AX+BY+C=0中，A B C分别等于：
//struct LineArgument{
//    double A; // A = Y2 - Y1
//    double B; // B = X1 - X2
//    double C; // C = X2*Y1 - X1*Y2
//};
// 获取直线参数  求直线的方程 y = kx +b
LineArgument getLinePara(const double& x1,const double& y1,const double& x2,const double& y2)
{
    LineArgument LP;
    LP.A = y2 - y1;
    LP.B = x1 - x2;
    LP.C = x2*y1 - x1*y2;
    return LP;
}

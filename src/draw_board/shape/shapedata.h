#ifndef SHAPEDATA_H
#define SHAPEDATA_H

#include <QPainter>
#include <QPainterPath>
#include <cmath>
#include <QDebug>
#include <QPoint>

const double PI = 3.1415926/180;

enum class EShapeType
{
    kUnkonwn,     // 未知形状
    kReckangle,   // 矩形
    kEllipse,     // 椭圆
    kLine,        // 线条
    kText,        // 文本
    kCustomLine,  //画笔画线
};

//已知直线上的两点P1(X1,Y1) P2(X2,Y2)， P1 P2两点不重合。则直线的一般式方程AX+BY+C=0中，A B C分别等于：
struct LineArgument{
    double A; // A = Y2 - Y1
    double B; // B = X1 - X2
    double C; // C = X2*Y1 - X1*Y2
};
LineArgument getLinePara(const double& x1,const double& y1,const double& x2,const double& y2);

class ShapeData
{
public:
    inline void SetShapeType(const EShapeType &shapetype){ this->m_ShapeType= shapetype ;}// 设置形状
    inline const EShapeType &GetShapeType() const {return m_ShapeType;}// 获取形状类型
    void SetStartPoint(const QPoint &StartPoint) {m_StartPoint = StartPoint;}
    void SetEndPoint(const QPoint &EndPoint) {m_EndPoint = EndPoint;}
    const QPoint &GetStartPoint() const {return m_StartPoint;}
    const QPoint &GetEndPoint() const {return m_EndPoint;}


    virtual bool hasSelected(const QPoint &){return false;} // 图形是否被选中
    virtual void drawShape(QPainter &){}// 画图函数
    virtual void moveShape(const QPoint &,const QPoint &){} // 移动图形
    virtual void paintFrame(QPainter& ) {} // 画边框

    void rotatePoint(QPoint &BasePoint,QPoint &point, double arg); // 点point进行旋转，旋转角度为arg(度)
    virtual void rotate(QPoint &BasePoint ,double arg);
    //    void zoomPoint(QPoint &BasePoint,QPoint & point, double sx, double sy);// 以BasePoint为基准点，;point进行缩放，sx为x方向的缩放比例，sy为y方向的缩放比例
    //    virtual void zoom(QPoint &BasePoint,double sx, double sy);
    //    void flipPoint(QPoint* point, bool isV);//翻转
    //    virtual void flip(bool isV);
    //    virtual void cutShape(QPoint* cutStartPos, QPoint* cutEndPos);// 裁剪

public:
    QPoint m_StartPoint;   //起始点
    QPoint m_EndPoint;    //最新点
    QPoint m_Center;// 中心点

    ShapeData();
    virtual ~ShapeData();
protected:
    EShapeType m_ShapeType;// 形状类型

};

#endif // SHAPEDATA_H

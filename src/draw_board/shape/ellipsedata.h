#ifndef ELLIPSEDATA_H
#define ELLIPSEDATA_H
#include "shapedata.h"
#include <QDebug>

class EllipseData:public ShapeData
{
public:
    EllipseData();
    EllipseData(QPoint &clickPoint,QPoint &movePoint);
    EllipseData(const double &dStartPosX,const double &dStartPosY,const double &dRadiusW,const double &dRediusH);
    ~EllipseData();
    inline void SetStartPosX(const double &dStartPosX){m_dStartPosX = dStartPosX;}
    inline void SetStartPosY(const double &dStartPosY){m_dStartPosY = dStartPosY;}
    inline void SetRadiusW(const double &dRadiusW){m_dRadiusW = dRadiusW;}
    inline void SetRediusH(const double &dRediusH){m_dRediusH = dRediusH;}

    inline const double &GetStartPosX() const { return m_dStartPosX;}
    inline const double &GetStartPosY() const { return m_dStartPosY;}
    inline const double &GetRadiusW() const { return m_dRadiusW;}
    inline const double &GetRediusH() const { return m_dRediusH;}

    bool hasSelected(const QPoint &pos) override; // 判断选中的点是否在椭圆上
    void drawShape(QPainter &painter) override;// 画椭圆
    void moveShape(const QPoint &curPoint, const QPoint &nextPoint) override;// 移动图形
    void paintFrame(QPainter& painter) override;// 画椭圆的边框

private:
    double m_dStartPosX;// 起点横坐标
    double m_dStartPosY;// 起点纵坐标
    double m_dRadiusW;// 横向半径长度
    double m_dRediusH;// 纵向半径长度
    bool m_IsSelected = false;
};

#endif // ELLIPSEDATA_H

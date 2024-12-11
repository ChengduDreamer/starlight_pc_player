#ifndef LINEDATA_H
#define LINEDATA_H

#include "shapedata.h"
#include <QPolygonF>
#include <QVector>
#include <QPainterPath>
#include <QDebug>
#include <QTransform>
#include <QPoint>

class LineData:public ShapeData
{
public:
    LineData();
    LineData(QPoint &clickPoint,QPoint &movePoint);
    ~LineData();
    LineData(const double &dStartPosX,const double &dStartPosY,const double &dEndPosX,const double dEndPosY);
    inline void SetStartPosX(const double &dPosX) {m_dStartPosX = dPosX;}
    inline void SetStartPosY(const double &dPosY) {m_dStartPosY = dPosY;}
    inline void SetEndPosX(const double &dWidth) {m_dEndPosX = dWidth;}
    inline void SetEndPosY(const double &dHeight) {m_dEndPosY = dHeight;}
    inline const double &GetStartPosX() const {return m_dStartPosX;}
    inline const double &GetStartPosY() const {return m_dStartPosY;}
    inline const double &GetEndPosX() const {return m_dEndPosX;}
    inline const double &GetEndPosY() const {return m_dEndPosY;}

    bool hasSelected(const QPoint &pos) override; // 判断选中的点是否在直线上
    void drawShape(QPainter &painter) override;// 画直线
    void moveShape(const QPoint &p1,const QPoint &p2) override;
    void paintFrame(QPainter& p) override;
    void rotate(QPoint &BasePoint,double arg) override;

private:
    double m_dStartPosX;// 起点横坐标
    double m_dStartPosY;// 起点纵坐标&
    double m_dEndPosX;// 终点横坐标
    double m_dEndPosY;// 终点纵坐标
    QVector<QPoint> vectorPath;// 路径容器
    bool m_IsSelected;// 是否选中
};

#endif // LINEDATA_H

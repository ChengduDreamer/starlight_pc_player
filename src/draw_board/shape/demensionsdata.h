#ifndef DEMENSIONSDATA_H
#define DEMENSIONSDATA_H

#include "shapedata.h"
#include <QVector>
#include <QDebug>
#include <QPoint>
#include <QPainter>
#include <math.h>
#include <QPolygonF>
#include <QPen>
#include <QPointF>

class DemensionsData:public ShapeData
{
public:
    DemensionsData(const QVector<QPoint> &);
    void drawShape(QPainter &painter) override;// 画直线

    QPoint m_PointOne_Online;
    QPoint m_PointTwo_Online;
    QPoint m_PointOne_Not_Online;
    QPoint m_PointTwo_Not_Online;

    void Calculate();
    QPoint m_PointThreeCalculate;
    inline void SetShowText(){this->m_ShowText = true;}
    void DrawLineWithArrow(QPainter& painter, QPen , QPoint start, QPoint end);
    void ShowText(QPainter &painter,const QPoint &local,const double &distance);


private:
    bool doCalculate = false;
    double m_Distance = 0.0;
    bool m_ShowText = false;
    double m_Angle = 0.0;
};

#endif // DEMENSIONSDATA_H

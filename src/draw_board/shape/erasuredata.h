#ifndef ERASUREDATA_H
#define ERASUREDATA_H

#include "shapedata.h"
#include <QVector>
#include <QPoint>
#include <QBrush>
#include <QPen>
#include <QPoint>


class ErasureData:public ShapeData
{
public:
    ErasureData();
    ErasureData(QVector<QPoint> &erasureVector);
    ~ErasureData();
    inline void SetErasureVector(const QPoint &point) {ErasureVec.push_back(point);}
    inline const QVector<QPoint> &GetErasureVector() const {return ErasureVec;}

    bool hasSelected(const QPoint &pos) override; // 判断选中的点是否在橡皮上
    void drawShape(QPainter &painter) override;// 画橡皮
    //    void moveShape(const QPoint &curPoint, const QPoint &nextPoint) override;// 移动图形
    //    void paintFrame(QPainter& painter) override;// 画边框

private:

    //    QVector<QVector<QPoint>> ErasurePoints;
    QVector<QPoint> ErasureVec;

    QBrush m_ErasureBrush;//橡皮檫颜色
    QPen m_ErasurePen;// 橡皮檫
};

#endif // ERASUREDATA_H

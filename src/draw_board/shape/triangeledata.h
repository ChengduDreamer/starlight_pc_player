#ifndef TRIANGELEDATA_H
#define TRIANGELEDATA_H

#include "shapedata.h"

class TriangeleData :public ShapeData
{
public:
    TriangeleData();
    TriangeleData(QPoint &clickPoint,QPoint &movePoint);
    TriangeleData(const double &dPosX,const double &dPosY,const double &dWidth,const double dHeight);
    ~TriangeleData();
    inline void SetStartPosX(const double &dPosX) {m_dStartPosX = dPosX;}
    inline void SetStartPosY(const double &dPosY) {m_dStartPosY = dPosY;}
    inline void SetWidth(const double &dWidth) {m_dWidth = dWidth;}
    inline void SetHeight(const double &dHeight) {m_dHeight = dHeight;}
    inline const double &GetStartPosX() const {return m_dStartPosX;}
    inline const double &GetStartPosY() const {return m_dStartPosY;}
    inline const double &GetWidth() const {return m_dWidth;}
    inline const double &GetHeight() const {return m_dHeight;}

    bool hasSelected(const QPoint &pos) override; // 判断选中的点是否在三角形上
    void drawShape(QPainter &painter) override;// 画三角形
    void moveShape(const QPoint &curPoint, const QPoint &nextPoint) override;// 移动图形
    void paintFrame(QPainter& painter) override;// 画三角形的边框

private:
    double m_dStartPosX;// 起点横坐标
    double m_dStartPosY;// 起点纵坐标
    double m_dWidth;// 宽度
    double m_dHeight;// 高度
    bool m_IsSelected;
};

#endif // TRIANGELEDATA_H

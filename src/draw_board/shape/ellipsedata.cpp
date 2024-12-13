#include "ellipsedata.h"

EllipseData::EllipseData()
{
    m_ShapeType = EShapeType::kEllipse;

    m_dStartPosX = 0.0;
    m_dStartPosY = 0.0;
    m_dRadiusW = 0.0;
    m_dRediusH = 0.0;
}

EllipseData::EllipseData(QPoint &clickPoint, QPoint &movePoint)
{
    int Xmin = clickPoint.x() < movePoint.x()?clickPoint.x():movePoint.x();
    int Ymin = clickPoint.y() < movePoint.y()?clickPoint.y():movePoint.y();
    QPoint dis = movePoint - clickPoint;

    m_ShapeType = EShapeType::kEllipse;

    m_dStartPosX = double(Xmin);
    m_dStartPosY = double(Ymin);
    m_dRadiusW = qAbs(double(dis.x()));
    m_dRediusH = qAbs(double(dis.y()));
}
/*
* 接口说明： 公共接口，有参构造函数
* 参数说明： 【in】    dStartPosX 起点横坐标
*          【in】    dStartPosY 起点横坐标
*          【in】    dRadiusW 横向半径长度
*          【in】    dRediusH 纵向半径长度
* 返回值说明： 无
*/
EllipseData::EllipseData(const double &dStartPosX, const double &dStartPosY, const double &dRadiusW, const double &dRediusH)
{
    m_ShapeType = EShapeType::kEllipse;

    m_dStartPosX = dStartPosX;
    m_dStartPosY = dStartPosY;
    m_dRadiusW = dRadiusW;
    m_dRediusH = dRediusH;
}

EllipseData::~EllipseData()
{

}

bool EllipseData::hasSelected(const QPoint &pos)
{
    this->m_IsSelected = false;
    if((pos.x() >=this->GetStartPosX()) && (pos.x() <= this->GetStartPosX()+this->m_dRadiusW) && (pos.y()>= this->GetStartPosY()) && (pos.y() <= this->GetStartPosY()+this->m_dRediusH) ){
        this->m_IsSelected = true;
    }
    return this->m_IsSelected ;
}

void EllipseData::drawShape(QPainter &painter)
{
    painter.drawEllipse(QRectF(this->GetStartPosX(),this->GetStartPosY(),this->GetRadiusW(),this->GetRediusH()));
}

void EllipseData::moveShape(const QPoint &curPoint, const QPoint &nextPoint)
{
    QPoint dis = nextPoint - curPoint;
    this->SetStartPosX(this->m_dStartPosX + dis.x());
    this->SetStartPosY(this->m_dStartPosY + dis.y());
}

void EllipseData::paintFrame(QPainter &painter)
{
    painter.save(); // 保存画笔
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    painter.setPen(framePen);
    painter.drawRect(this->m_dStartPosX-5,this->m_dStartPosY-5,this->m_dRadiusW+10,this->m_dRediusH+10);
    painter.restore(); // 恢复画笔
}

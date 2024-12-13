#include "rectangledata.h"

RectangleData::RectangleData()
{
    m_ShapeType = EShapeType::kReckangle;
    this->m_dStartPosX = 0.0;
    this->m_dStartPosY = 0.0;
    this->m_dHeight = 0.0;
    this->m_dWidth = 0.0;
}

RectangleData::RectangleData(QPoint &clickPoint, QPoint &movePoint)
{
    int Xmin = clickPoint.x() < movePoint.x()?clickPoint.x():movePoint.x();
    int Ymin = clickPoint.y() < movePoint.y()?clickPoint.y():movePoint.y();
    QPoint dis = movePoint - clickPoint;

    m_ShapeType = EShapeType::kReckangle;
    this->m_dStartPosX = double(Xmin);
    this->m_dStartPosY = double(Ymin);
    this->m_dWidth = qAbs(double(dis.x()));
    this->m_dHeight = qAbs(double(dis.y()));

}

RectangleData::RectangleData(const double &dPosX, const double &dPosY, const double &dWidth, const double dHeight)
{
    m_ShapeType = EShapeType::kReckangle;
    this->m_dStartPosX = dPosX;
    this->m_dStartPosY = dPosY;
    this->m_dHeight = dHeight;
    this->m_dWidth = dWidth;
}

RectangleData::~RectangleData()
{

}

bool RectangleData::hasSelected(const QPoint &pos)
{
    this->m_IsSelected = false;
    if(pos.x() >=this->GetStartPosX() && pos.x() <= this->GetStartPosX()+this->m_dWidth && pos.y()>= this->GetStartPosY() && pos.y() <= this->GetStartPosY()+this->m_dHeight ){
        this->m_IsSelected = true;
    }
    return this->m_IsSelected;
}

void RectangleData::drawShape(QPainter &painter)
{
    painter.drawRect(QRectF(this->GetStartPosX(),this->GetStartPosY(),this->GetWidth(),this->GetHeight()));
}

void RectangleData::moveShape(const QPoint &curPoint, const QPoint &nextPoint)
{
    QPoint dis = nextPoint - curPoint;
    this->SetStartPosX(this->m_dStartPosX + dis.x());
    this->SetStartPosY(this->m_dStartPosY + dis.y());
}

void RectangleData::paintFrame(QPainter &painter)
{
    painter.save(); // 保存画笔
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    painter.setPen(framePen);
    painter.drawRect(this->m_dStartPosX-5,this->m_dStartPosY-5,this->m_dWidth+10,this->m_dHeight+10);
    painter.restore(); // 恢复画笔
}

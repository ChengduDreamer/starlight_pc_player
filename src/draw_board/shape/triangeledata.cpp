#include "triangeledata.h"

TriangeleData::TriangeleData()
{
    m_ShapeType = EShapeType::Shape_Triangle;
    this->m_dStartPosX = 0.0;
    this->m_dStartPosY = 0.0;
    this->m_dHeight = 0.0;
    this->m_dWidth = 0.0;
}

TriangeleData::TriangeleData(QPoint &clickPoint, QPoint &movePoint)
{
    int Xmin = clickPoint.x() < movePoint.x()?clickPoint.x():movePoint.x();
    int Ymin = clickPoint.y() < movePoint.y()?clickPoint.y():movePoint.y();
    QPoint dis = movePoint - clickPoint;
    m_ShapeType = EShapeType::Shape_Triangle;
    this->m_dStartPosX = double(Xmin);
    this->m_dStartPosY = double(Ymin);
    this->m_dWidth = qAbs(double(dis.x()));
    this->m_dHeight = qAbs(double(dis.y()));
}

TriangeleData::TriangeleData(const double &dPosX, const double &dPosY, const double &dWidth, const double dHeight)
{
    m_ShapeType = EShapeType::Shape_Triangle;
    this->m_dStartPosX = dPosX;
    this->m_dStartPosY = dPosY;
    this->m_dWidth = dWidth;
    this->m_dHeight = dHeight;
}

bool TriangeleData::hasSelected(const QPoint &pos)
{
    //判断点是否在直线上
    LineArgument LP1 = getLinePara(this->GetStartPosX(),this->GetStartPosY(),this->GetStartPosX()-this->GetWidth(),this->GetStartPosY()+this->GetHeight());// 左边线
    LineArgument LP2 = getLinePara(this->GetStartPosX(),this->GetStartPosY(),this->GetStartPosX()+this->GetWidth(),this->GetStartPosY()+this->GetHeight());// 右边线
    this->m_IsSelected = false;
    if(pos.y() > this->GetStartPosY() && pos.y() < this->GetStartPosY() + this->GetHeight()){
        if((LP1.A*pos.x()+LP1.B*(pos.y() - 5)+LP1.C > 0) && (LP2.A*pos.x()+LP2.B*(pos.y() + 5)+LP2.C < 0) ){
            this->m_IsSelected = true;
        }
    }
    return this->m_IsSelected;
}

void TriangeleData::drawShape(QPainter &painter)
{
    QPoint point1(this->GetStartPosX(),this->GetStartPosY());
    QPoint point2(this->GetStartPosX()-this->GetWidth(),this->GetStartPosY()+this->GetHeight());
    QPoint point3(this->GetStartPosX()+this->GetWidth(),this->GetStartPosY()+this->GetHeight());
    painter.drawLine(point1,point2);
    painter.drawLine(point1,point3);
    painter.drawLine(point2,point3);
}

void TriangeleData::moveShape(const QPoint &curPoint, const QPoint &nextPoint)
{
    QPoint dis = nextPoint - curPoint;
    this->SetStartPosX(this->m_dStartPosX + dis.x());
    this->SetStartPosY(this->m_dStartPosY + dis.y());
}

void TriangeleData::paintFrame(QPainter &painter)
{   painter.save(); // 保存画笔
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    painter.setPen(framePen);
    QPoint point1(this->GetStartPosX(),this->GetStartPosY()-5);
    QPoint point2(this->GetStartPosX()-this->GetWidth()-7.5,this->GetStartPosY()+this->GetHeight()+5);
    QPoint point3(this->GetStartPosX()+this->GetWidth()+7.5,this->GetStartPosY()+this->GetHeight()+5);
    painter.drawLine(point1,point2);
    painter.drawLine(point1,point3);
    painter.drawLine(point2,point3);
    painter.restore(); // 恢复画笔
}

TriangeleData::~TriangeleData()
{

}

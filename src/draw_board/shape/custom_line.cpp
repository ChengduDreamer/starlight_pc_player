#include "custom_line.h"

CustomLine::CustomLine()
{
    m_ShapeType = EShapeType::Shape_Line;
    this->m_dStartPosX = 0.0;
    this->m_dStartPosY = 0.0;
    this->m_dEndPosX = 0.0;
    this->m_dEndPosY = 0.0;
}

CustomLine::CustomLine(QPoint& clickPoint, QPoint& movePoint)
{
    m_ShapeType = EShapeType::Shape_Line;
    this->m_dStartPosX = double(clickPoint.x());
    this->m_dStartPosY = double(clickPoint.y());
    this->m_dEndPosX = double(movePoint.x());
    this->m_dEndPosY = double(movePoint.y());

    this->m_StartPoint.setX(m_dStartPosX);
    this->m_StartPoint.setY(m_dStartPosY);
    this->m_EndPoint.setX(m_dEndPosX);
    this->m_EndPoint.setY(m_dEndPosY);

    this->m_Center.setX((this->m_StartPoint.x() + this->m_EndPoint.x()) / 2);
    this->m_Center.setY((this->m_StartPoint.y() + this->m_EndPoint.y()) / 2);
}

CustomLine::~CustomLine()
{

}

CustomLine::CustomLine(const double& dStartPosX, const double& dStartPosY, const double& dEndPosX, const double dEndPosY)
{
    m_ShapeType = EShapeType::Shape_Line;
    this->m_dStartPosX = dStartPosX;
    this->m_dStartPosY = dStartPosY;
    this->m_dEndPosX = dEndPosX;
    this->m_dEndPosY = dEndPosY;

    this->m_StartPoint.setX(dStartPosX);
    this->m_StartPoint.setY(dStartPosY);
    this->m_EndPoint.setX(dEndPosX);
    this->m_EndPoint.setY(dEndPosY);

    this->m_Center.setX((this->m_StartPoint.x() + this->m_EndPoint.x()) / 2);
    this->m_Center.setY((this->m_StartPoint.y() + this->m_EndPoint.y()) / 2);
}

bool CustomLine::hasSelected(const QPoint& pos)
{
    //判断点是否在直线上
    LineArgument LP = getLinePara(this->GetStartPosX(), this->GetStartPosY(), this->GetEndPosX(), this->GetEndPosY());
    this->m_IsSelected = false;
    if (pos.x() > this->GetStartPosX() && pos.x() < this->GetEndPosX()) {
        if (((LP.A * pos.x() + LP.B * (pos.y() + 5) + LP.C < 0) && (LP.A * pos.x() + LP.B * (pos.y() - 5) + LP.C > 0)) || ((LP.A * pos.x() + LP.B * (pos.y() + 5) + LP.C > 0) && (LP.A * pos.x() + LP.B * (pos.y() - 5) + LP.C < 0))) {
            this->m_IsSelected = true;
        }
    }
    return this->m_IsSelected;
}

void CustomLine::drawShape(QPainter& painter)
{
    //painter.drawLine(this->GetStartPosX(), this->GetStartPosY(), this->GetEndPosX(), this->GetEndPosY());

    painter.drawPolyline(points_data_);

}

// 更新数据
void CustomLine::moveShape(const QPoint& curPoint, const QPoint& nextPoint)
{
    QPoint dis = nextPoint - curPoint;
    this->SetStartPosX(this->GetStartPosX() + dis.x());
    this->SetStartPosY(this->GetStartPosY() + dis.y());
    this->SetEndPosX(this->GetEndPosX() + dis.x());
    this->SetEndPosY(this->GetEndPosY() + dis.y());
    this->m_StartPoint.setX(this->GetStartPosX());
    this->m_StartPoint.setY(this->GetStartPosY());
    this->m_EndPoint.setX(this->GetEndPosX());
    this->m_EndPoint.setY(this->GetEndPosY());

    this->m_Center.setX((this->m_StartPoint.x() + this->m_EndPoint.x()) / 2);
    this->m_Center.setY((this->m_StartPoint.y() + this->m_EndPoint.y()) / 2);
}


void CustomLine::paintFrame(QPainter& painter)
{
    painter.save(); // 保存画笔
    QPen framePen(Qt::blue, 1, Qt::DashDotLine, Qt::RoundCap);
    painter.setPen(framePen);
    painter.drawLine(m_StartPoint.x(), m_StartPoint.y() + 5, m_StartPoint.x(), m_StartPoint.y() - 5);
    painter.drawLine(m_StartPoint.x(), m_StartPoint.y() - 5, m_EndPoint.x(), m_EndPoint.y() - 5);
    painter.drawLine(m_EndPoint.x(), m_EndPoint.y() - 5, m_EndPoint.x(), m_EndPoint.y() + 5);
    painter.drawLine(m_EndPoint.x(), m_EndPoint.y() + 5, m_StartPoint.x(), m_StartPoint.y() + 5);

    painter.restore(); // 恢复画笔
}

//void CustomLine::rotate(QPoint& BasePoint, double arg = 90)
//{
//    this->rotatePoint(BasePoint, this->m_StartPoint, arg);
//    this->rotatePoint(BasePoint, this->m_EndPoint, arg);
//    this->m_dStartPosX = this->m_StartPoint.x();
//    this->m_dStartPosY = this->m_StartPoint.y();
//    this->m_dEndPosX = this->m_EndPoint.x();
//    this->m_dEndPosY = this->m_EndPoint.y();
//}

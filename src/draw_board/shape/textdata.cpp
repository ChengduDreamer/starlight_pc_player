#include "textdata.h"

TextData::TextData()
{
    m_ShapeType = EShapeType::Shape_Text;
    m_qStartPosX = 0.0;
    m_qStartPosY = 0.0;
}

TextData::TextData(QPoint &clickPoint, ContentEdit &contentEdit)
{
    m_ShapeType = EShapeType::Shape_Text;
    m_qStartPosX = clickPoint.x();
    m_qStartPosY = clickPoint.y();
    contentEdit.clear();
    contentEdit.show();
    contentEdit.setGeometry(clickPoint.x(),clickPoint.y(),200,30);
    contentEdit.setFocus();
}

TextData::TextData(const double &dStartPosX, const double &dStartPosY, const QString &qstrContent)
{
    m_ShapeType = EShapeType::Shape_Text;
    m_qStartPosX = dStartPosX;
    m_qStartPosY = dStartPosY;
    m_qstrContent = qstrContent;
    m_Size = qstrContent.size();
}

TextData::~TextData()
{

}
bool TextData::hasSelected(const QPoint &pos)
{
    this->m_IsSelected = false;
    if(this->GetStartPosX() < pos.x() && this->GetStartPosX() + this->m_Size*20 > pos.x()
            && this->GetStartPosY()-30 < pos.y() && this->GetStartPosY() > pos.y() ){
        this->m_IsSelected = true;
    }
    return this->m_IsSelected;
}

void TextData::drawShape(QPainter &painter)
{
    painter.drawText(QPoint(this->GetStartPosX(),this->GetStartPosY()),this->m_qstrContent);
}

void TextData::moveShape(const QPoint &curPoint, const QPoint &nextPoint)
{
    QPoint dis = nextPoint - curPoint;
    this->SetStartPosX(this->m_qStartPosX + dis.x());
    this->SetStartPosY(this->m_qStartPosY + dis.y());
}

void TextData::paintFrame(QPainter &painter)
{
    painter.save(); // 保存画笔
    QPen framePen(Qt::blue, 1 ,Qt::DashDotLine, Qt::RoundCap);
    painter.setPen(framePen);
    painter.drawRect(this->m_qStartPosX-4,this->m_qStartPosY-25,this->m_Size*20+10,30);
    painter.restore(); // 恢复画笔
}

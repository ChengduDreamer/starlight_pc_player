#include "erasuredata.h"

ErasureData::ErasureData()
{
    m_ErasureBrush.setColor(QColor(255,255,255));
    m_ErasureBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_ErasurePen = QPen(m_ErasureBrush,10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    //    m_ErasurePen = QPen(QBrush(QColor(255,0,0)),10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

}

ErasureData::ErasureData(QVector<QPoint> &erasureVector)
{
    m_ErasureBrush.setColor(QColor(255,255,255));
    m_ErasureBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_ErasurePen = QPen(m_ErasureBrush,10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    m_ShapeType = EShapeType::Erasure;
    this->ErasureVec = erasureVector;
}

ErasureData::~ErasureData()
{

}

bool ErasureData::hasSelected(const QPoint &)
{
    return false;
}

void ErasureData::drawShape(QPainter &painter)
{
    QBrush tempBrush = painter.brush();
    QPen tempPen = painter.pen();
    painter.setBrush(m_ErasureBrush);
    painter.setPen(m_ErasurePen);
    painter.drawPolyline(QPolygon(this->GetErasureVector()));

    painter.setBrush(tempBrush);
    painter.setPen(tempPen);
}

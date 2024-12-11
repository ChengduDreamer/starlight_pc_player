#include "demensionsdata.h"
#include "draw_board/MACRO.h"
#include <QtCore/qmath.h>
/**
 * @brief DemensionsData::DemensionsData
 *        函数在初始化的时候用到了vec容器，在容器< 3的时候其实可以不进行处理，直接通过修改DrawWiget里面的工厂代码就可以了
 * @param vec
 *        参数是标注的三个控制点
 */
DemensionsData::DemensionsData(const QVector<QPoint> &vec)
{
    //    if(vec.size() == 1){
    //        m_PointOne_Online = m_PointTwo_Online = m_PointOne_Not_Online =  m_PointTwo_Not_Online = m_PointThreeCalculate =  vec[0];
    //    }else if(vec.size()==2){
    //        qDebug()<<"计算了m_PointOne_Online = m_PointOne_Not_Online =  m_PointTwo_Not_Online = m_PointThreeCalculate =  vec[0];";
    //        m_PointOne_Online = m_PointOne_Not_Online =  m_PointTwo_Not_Online = m_PointThreeCalculate =  vec[0];
    //        m_PointTwo_Online = vec[1];
    //    }else if(vec.size()==3){
    this->doCalculate = true;
    m_PointOne_Online =  vec[0];
    m_PointTwo_Online = vec[1];
    m_PointThreeCalculate = vec[2];
    this->m_ShowText= true;// 如果只想计算一次，这里关闭，DrawWidget类里的打开
    //    }
}

/**
 * @brief DemensionsData::drawShape
 *        绘制标注
 * @param painter
 *        画笔
 */
void DemensionsData::drawShape(QPainter &painter)
{
    if(this->doCalculate){
        this->Calculate();
    }
    if(this->m_ShowText){
        this->m_Distance = sqrt((m_PointOne_Online.x() - m_PointTwo_Online.x() )*(m_PointOne_Online.x() - m_PointTwo_Online.x())+
                                (m_PointOne_Online.y() - m_PointTwo_Online.y() )*(m_PointOne_Online.y() - m_PointTwo_Online.y())
                                );
    }
    painter.save(); // 保存画笔
    QPen Pen = QPen(QColor(	30 ,144, 255), 2, Qt::SolidLine);
    painter.setPen(Pen);
    painter.setBrush(Pen.color());

    painter.drawLine(m_PointOne_Online,m_PointOne_Not_Online);
    painter.drawLine(m_PointTwo_Online,m_PointTwo_Not_Online);
    //    painter.drawLine(m_PointOne_Not_Online,m_PointTwo_Not_Online); // 这里用的是箭头的标注线没有用原来的线

    DrawLineWithArrow(painter,Pen,m_PointOne_Not_Online,m_PointTwo_Not_Online);
    DrawLineWithArrow(painter,Pen,m_PointTwo_Not_Online,m_PointOne_Not_Online);

    if(this->m_ShowText){
        QPoint center = QPoint(m_PointTwo_Not_Online+m_PointOne_Not_Online)/2;
        this->ShowText(painter,center,m_Distance);
    }

    painter.restore(); // 恢复画笔
}

/**
 * @brief DemensionsData::ShowText: 标注的时候显示尺寸
 * @param painter : 画笔
 * @param local ：标注的位置
 * @param distance
 */
void DemensionsData::ShowText(QPainter &painter,const QPoint &local,const double &distance){
    //    https://www.cnblogs.com/lifexy/p/9245913.html
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(local.x(),local.y());

    //    painter.rotate(this->m_Angle); // 如果直接这样旋转，标注的尺寸始终是水平显示的；
    QPoint maxPoint = this->m_PointOne_Online.x() > this->m_PointTwo_Online.x()?this->m_PointOne_Online:this->m_PointTwo_Online;
    QPoint minPoint = this->m_PointOne_Online.x() <= this->m_PointTwo_Online.x()?this->m_PointOne_Online:this->m_PointTwo_Online;
    double k = 1.0 *  double(maxPoint.y() - minPoint.y()) / double(maxPoint.x() - minPoint.x());
    this->m_Angle = std::atan(k) * 180 / M_PI ;
    painter.rotate(this->m_Angle); // 计算旋转角度，可以保持旋转后和标注线平行
    QString disStr = QString::number(distance,'f',2);
    painter.drawText(-10*disStr.size(),-25,20 * disStr.size(),20,Qt::AlignCenter,disStr);
    painter.restore();
}


void DemensionsData::Calculate()
{
    LineArgument LP = getLinePara(m_PointOne_Online.x(),m_PointOne_Online.y(),m_PointTwo_Online.x(),m_PointTwo_Online.y());
    LineArgument Cal_Lp = LP;
    Cal_Lp.C = -1.0 * m_PointThreeCalculate.y() * Cal_Lp.B - m_PointThreeCalculate.x() * Cal_Lp.A; // 过标注线的直线
    double AABB = Cal_Lp.A * Cal_Lp.A + Cal_Lp.B * Cal_Lp.B;
    m_PointOne_Not_Online.setX((Cal_Lp.B * Cal_Lp.B * m_PointOne_Online.x() - Cal_Lp.A * Cal_Lp.B * m_PointOne_Online.y() - Cal_Lp.A * Cal_Lp.C) / AABB);
    m_PointOne_Not_Online.setY((-1.0 * Cal_Lp.A * Cal_Lp.B * m_PointOne_Online.x() + Cal_Lp.A * Cal_Lp.A * m_PointOne_Online.y() - Cal_Lp.B * Cal_Lp.C ) / AABB);

    m_PointTwo_Not_Online.setX((Cal_Lp.B * Cal_Lp.B * m_PointTwo_Online.x() - Cal_Lp.A * Cal_Lp.B * m_PointTwo_Online.y() - Cal_Lp.A * Cal_Lp.C) / AABB);
    m_PointTwo_Not_Online.setY((-1.0 * Cal_Lp.A * Cal_Lp.B * m_PointTwo_Online.x() + Cal_Lp.A * Cal_Lp.A * m_PointTwo_Online.y() - Cal_Lp.B * Cal_Lp.C ) / AABB);
}

//https://blog.csdn.net/dhvdhd/article/details/121397758
/**
 * @brief DrawLineWithArrow 画直线和箭头
 * @param painter  画笔
 * @param pen   笔的属性
 * @param start 开始点
 * @param end 结束点
 */
void DemensionsData::DrawLineWithArrow(QPainter& painter, QPen , QPoint start, QPoint end)
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal arrowSize = 15;
    //    painter.setPen(pen);
    //    painter.setBrush(pen.color());

    QLineF line(end, start);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter.drawLine(line);
    painter.drawPolygon(arrowHead);
}



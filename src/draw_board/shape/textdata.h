#ifndef TEXTDATA_H
#define TEXTDATA_H

#include <QString>
#include "shapedata.h"
#include <QDebug>
#include <QFont>
#include <QObject>
#include "contentedit.h"

class TextData:public ShapeData
{
public:
    TextData();
    TextData(QPoint &clickPoint,ContentEdit &contentEdit);
    TextData(const double &dStartPosX,const double &dStartPosY,const QString &qstrContent);
    ~TextData();

    inline void SetStartPosX(const double &dPosX) {m_qStartPosX = dPosX;}
    inline void SetStartPosY(const double &dPosY) {m_qStartPosY = dPosY;}
    inline const double &GetStartPosX() const {return m_qStartPosX;}
    inline const double &GetStartPosY() const {return m_qStartPosY;}
    inline const QString &GetContent() const {return m_qstrContent;}
    inline const int &GetContentSize() const {return m_Size;}

    bool hasSelected(const QPoint &pos) override; // 判断选中的点是否在文字上
    void drawShape(QPainter &painter) override;// 画文字
    void moveShape(const QPoint &curPoint, const QPoint &nextPoint) override;// 移动图形
    void paintFrame(QPainter& painter) override;// 画文本的边框

private:
    double m_qStartPosX;
    double m_qStartPosY;
    QString m_qstrContent;
    int m_Size = 0;// 文字的字数
    bool m_IsSelected;
};

#endif // TEXTDATA_H

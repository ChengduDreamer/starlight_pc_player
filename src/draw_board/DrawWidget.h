#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <qopenglwidget.h>
#include "systemdata.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QBrush>
#include <QMouseEvent>
#include "shape/shapedata.h"
#include "shape/contentedit.h"
#include <QDebug>
#include <QVector>
#include <QPainterPath>
#include <QTimer>
#include <QSharedPointer>
#include <QPoint>
#include <QLineEdit>
#include <QKeyEvent>

//enum class ERotateType // 旋转类型
//{
//    Rotate_0 = 0,
//    Rotate_90 = 1,
//    Rotate_180 = 2,
//    Rotate_270 = 3
//};

class CDrawBoardWidget;

class DrawWidget :public QOpenGLWidget
{
    Q_OBJECT
    friend class CDrawBoardWidget;//
public:
    DrawWidget(QWidget *parent  = nullptr);
    ~DrawWidget();

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override;
    //    void keyPressEvent(QKeyEvent *event) override; // 这里写没用，去主面板写

    void  mouseOnOnePoint(QPoint &point);

    inline void SetShapeType(const EShapeType &shapeType){m_ShapeType = shapeType;}
    inline EShapeType GetShapeType(){return m_ShapeType;}

    /*int RotateLeft();
    int RotateRight();*/
    bool m_SelectBtnClicked = false;// 是否是选择状态
    bool m_RotateBtnClicked = false;// 旋转按钮是否按下
    /*void Demensions();*/


private slots:
    void fn_Recv_ContentEdit_GetContent(const QString &qstrContent);
    //void fn_Change_StatusEdit_Visual(const QString &qstrContent);

private:
    class SystemData *m_pSystemData;//数据存储的对象指针
    ShapeData *m_curShape = NULL;// 当前选中的形状

    ContentEdit m_ContentEdit;// 文本编辑窗口
    QPoint m_TextPoint;// 文本编辑位置

    bool m_bLBtnClicked;// 鼠标左键是否按下
    EShapeType m_ShapeType;// 当前绘画的类型
    QPoint m_ClickPoint;//鼠标点击的坐标
    QPoint m_ClickPoint_Two;// 鼠标点击的第二个坐标，标注的时候需要
    QPoint m_MovePoint;// 鼠标移动的坐标

    //QLineEdit m_StatusEdit; // 文本框中的文字

    QPen m_BlackPen;//画笔
    QPen m_ErasurePen;// 橡皮檫
    QFont m_TextFont;// 文字字体
    QBrush m_NullBrush;//空画刷  背景
    QBrush m_WhiteBrush;// 白色  背景
    QBrush m_BackgroundBrush;// 自己设置测试的背景色
    QBrush m_ErasureBrush;//橡皮檫颜色
    //    QVector<QVector<QPoint>> ErasurePoints;
    QVector<QPoint> ErasurePoint;// 橡皮檫的容器

    //bool m_DemensionBtnClicked = false;
    //QVector<QPoint> DemensionsPoint;// 标注的三个点

    //ERotateType m_RotateType;// 旋转角度
    //QPoint *m_StartingClickPoint;// 用于做旋转的基点指针

    //GLfloat zscale=1; // 缩放比例属性

    // 移动操作的变量

    QPoint m_MoveTempPoint;// 鼠标移动的坐标
    int m_curShapeIndex = -1;// 选中的下标

    bool doubleClicked = false;// 是否双击

    const QPoint *m_CapturePoint = nullptr;// 捕获的点
};

#endif // DRAWWIDGET_H

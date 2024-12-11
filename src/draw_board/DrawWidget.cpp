#include "DrawWidget.h"
#include <qsizepolicy.h>

DrawWidget::DrawWidget(QWidget *parent):QOpenGLWidget(parent)
{

    auto sizepolicy = this->sizePolicy();
    sizepolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    sizepolicy.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(sizepolicy);

    m_pSystemData = SystemData::GetSystemData();

    m_bLBtnClicked = false;
    m_ShapeType = EShapeType::Shape_Unkonwn;

    //m_StatusEdit.setParent(this);
    m_BlackPen.setColor(QColor(0,0,0));
    m_TextFont.setFamily(QStringLiteral("楷体"));
    m_TextFont.setPixelSize(20);
    m_WhiteBrush.setColor(QColor(255,255,255));
    m_WhiteBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_BackgroundBrush.setColor(QColor(0 ,255, 0));
    m_BackgroundBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_NullBrush.setStyle(Qt::BrushStyle::NoBrush);
    m_ErasureBrush = m_WhiteBrush;//画笔
    m_ErasurePen = QPen(m_WhiteBrush,10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    //    //    m_ErasurePen = QPen(QBrush(QColor(255,0,0)),10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    //    DemensionsPoint.resize(3);// 确定存储三个点，没有的话就自动赋值就好了

    m_ContentEdit.setParent(this);
    m_ContentEdit.hide();
    m_ContentEdit.setFont(m_TextFont);
    //m_RotateType = ERotateType::Rotate_0;// 默认0度

    this->setMouseTracking(true);// 设置为不按下鼠标键触发moveEvent，对moveevent起作用

    QObject::connect(&m_ContentEdit,SIGNAL(Signal_GetContent(QString)),this,SLOT(fn_Recv_ContentEdit_GetContent(QString)),Qt::ConnectionType::DirectConnection);

    //m_StatusEdit.setGeometry(0,this->height() + 20,this->width(),20);
    //m_StatusEdit.setStyleSheet("QLineEdit{background-color:transparent}""QLineEdit{border-width:0;border-style:outset}");
    //connect(&m_StatusEdit,SIGNAL(textChanged(QString)),this,SLOT(fn_Change_StatusEdit_Visual(QString)),Qt::ConnectionType::DirectConnection);//有文本显示边框和背景颜色
}

DrawWidget::~DrawWidget()
{

}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QOpenGLWidget::paintEvent(event);    // 先调用再渲染

    QPainter painter(this);
    painter.setBrush(m_WhiteBrush);
    painter.drawRect(rect());// 整个窗体的矩形
    painter.setRenderHint(QPainter::Antialiasing);// 平滑曲线 防止图形走样
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);// 绘制结果 = 绘制输入，目的绘制区域原来的像素被完全覆盖。https://blog.csdn.net/yejin_tianming/article/details/105113668

    // 旋转画笔
   /* switch (m_RotateType) {
    case ERotateType::Rotate_90:
    {
        painter.translate(this->width(),0.0);
        painter.rotate(90);
        break;
    }
    case ERotateType::Rotate_180:
    {
        painter.translate(this->width(),this->height());
        painter.rotate(180);
        break;
    }
    case ERotateType::Rotate_270:
    {
        painter.translate(0.0,this->height());
        painter.rotate(270);
        break;
    }
    default:
        break;
    }*/

    // 设置画笔
    painter.setPen(m_BlackPen);
    painter.setFont(m_TextFont);
    painter.setBrush(m_NullBrush);

    // 这里是对去全局进行绘画
    int iSize = m_pSystemData->m_ShapeVec.size();

    // 画完容器里所有的图
    for(int i =0;i<iSize;++i ){
        m_pSystemData->m_ShapeVec.at(i)->drawShape(painter);
        //        case EShapeType::Shape_Unkonwn:
    }


    // 这里是对当前按下的按钮选项进行绘画
    /*if(m_bLBtnClicked || (m_DemensionBtnClicked && this->DemensionsPoint.size() == 3)){*/
    if (m_bLBtnClicked) {
        //         QSharedPointer<ShapeData> pShape = m_pSystemData->CreateShapeItem(m_ShapeType,m_ClickPoint,m_MovePoint,ErasurePoint,m_ContentEdit,DemensionsPoint);
       // ShapeData *pShape = m_pSystemData->CreateShapeItem(m_ShapeType,m_ClickPoint,m_MovePoint,ErasurePoint,m_ContentEdit);
        //pShape->drawShape(painter);
        //        pShape.clear();
        //delete pShape;
        //pShape = nullptr;//一定要记得删除，可以使用智能指针比较好
        // 改进代码段1，源代码在最后面

        switch (m_ShapeType) {
        case EShapeType::Shape_Reckangle:
        {
            ShapeData* pShape = m_pSystemData->CreateShapeItem(m_ShapeType, m_ClickPoint, m_MovePoint, ErasurePoint, m_ContentEdit);
            pShape->drawShape(painter);
            delete pShape;
            pShape = nullptr;
            break;
        }
        case EShapeType::Shape_Ellipse:
        {
            ShapeData* pShape = m_pSystemData->CreateShapeItem(m_ShapeType, m_ClickPoint, m_MovePoint, ErasurePoint, m_ContentEdit);
            pShape->drawShape(painter);
            delete pShape;
            pShape = nullptr;
            break;
        }
        case EShapeType::Shape_Line:
        {
            painter.drawLine(m_ClickPoint, m_MovePoint);
            break;
        }
        case EShapeType::Shape_Text:
        {
            m_TextPoint = m_ClickPoint;
            //            TextData *qText = TextData(m_TextPoint.x(),m_TextPoint.y(),m_ContentEdit.text());
            m_ContentEdit.show();
            m_ContentEdit.setGeometry(m_TextPoint.x(), m_TextPoint.y(), 200, 30);
            painter.drawText(QPoint(m_TextPoint.x(), m_TextPoint.y()), m_ContentEdit.text());
            break;
        }
        case EShapeType::Erasure:
        {
            painter.setPen(m_ErasurePen);
            painter.drawPolyline(QPolygon(ErasurePoint));
            break;
        }
        default:
            break;
        }

    }


    // 对尺寸进行标记

    if(m_curShape){
        m_curShape->paintFrame(painter);
    }

    update();
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton){
        m_bLBtnClicked = true; // 鼠标左键按下
        if(this->m_CapturePoint){ // 如果鼠标捕获了点，就直接以捕获点为绘制的点
            m_ClickPoint = *(this->m_CapturePoint);

        }else {
            m_ClickPoint = event->pos();// 设置按下的位置
        }
        // 旋转需要执行的操作
        //if(this->m_RotateBtnClicked){
        //    QStringList rotateArgList = this->m_StatusEdit.text().split(":");
        //    //            for(auto st : rotateArgList){
        //    //                qDebug() << st;
        //    //            }
        //    //            qDebug() << rotateArgList.length();
        //    QString rotateArg = rotateArgList.at(rotateArgList.length()-1);
        //    bool isDigit = false;
        //    double arg = 90;
        //    double temp = rotateArg.toDouble(&isDigit);
        //    if(isDigit){
        //        arg = temp;
        //        //                qDebug() << QString("this arg")<< QString("%1").arg(arg);
        //    }
        //    //this->m_StartingClickPoint = &m_ClickPoint;
        //    this->m_curShape->rotate(m_ClickPoint,arg);
        //    this->m_RotateBtnClicked = false;
        //    this->m_StatusEdit.clear();
        //}

        // 这里是对鼠标标注的操作
        //if(this->m_DemensionBtnClicked){
        //    //            qDebug() << QString("this->DemensionsPoint.size() press in:") <<  this->DemensionsPoint.size();
        //    this->DemensionsPoint.push_back(m_ClickPoint);
        //    if(this->DemensionsPoint.size() == 1){
        //        this->m_StatusEdit.setText("Select the second point of the object");
        //    }
        //    else if(this->DemensionsPoint.size() == 2){
        //        this->m_StatusEdit.setText("Please select a location for demensions");
        //    }else if(this->DemensionsPoint.size() == 4){
        //        this->DemensionsPoint.pop_back();
        //        this->DemensionsPoint.resize(3);
        //        this->m_StatusEdit.clear();
        //    }
        //    //            qDebug() << QString("this->DemensionsPoint.size() press out:") <<  this->DemensionsPoint.size();
        //}
       /* if(!this->m_DemensionBtnClicked && !DemensionsPoint.empty()){
            this->m_StatusEdit.clear();
        }*/

        m_MovePoint = event->pos(); // 单击时，对移动位置进行初始化
        m_curShape  = NULL; // 每次单击，首先清空之前选中的对象
        if(m_SelectBtnClicked){ // 如果选择按钮按下，需要判断是否选择对象，如果选择进行标记,选择图像进行旋转和移动
            int iSize = m_pSystemData->m_ShapeVec.size();
            for(int i =0;i<iSize;++i ){
                if(m_curShape){ // 判断是否已经有了选中的图形
                    break;
                }
                if(this->doubleClicked){ // 如果进行了双击，第二次不要操作，否则文字选中会有边框
                    this->doubleClicked = false;
                    this->m_curShape = NULL;
                    break;
                }
                //                qDebug()<<i;
                ShapeData * pShape= m_pSystemData->m_ShapeVec.at(i);
                if(pShape->hasSelected(m_ClickPoint)){ // 如果点击了当前的图形，进行赋值
                    m_curShape = pShape;
                    this->m_curShapeIndex = i;
                }
            }
        }

       /* if(this->m_DemensionBtnClicked == true && m_ShapeType != EShapeType::Shape_Demensions ){
            this->m_DemensionBtnClicked = false;
            this->DemensionsPoint.clear();
        }*/
    }
    QOpenGLWidget::mousePressEvent(event);
}


void DrawWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton){
        if(m_curShape && m_curShape->GetShapeType()== EShapeType::Shape_Text){
            this->doubleClicked = true; // 确定当前是双击，防止单击继续操作
            TextData * pText = static_cast<TextData *>(m_curShape); // 获取当前选中的文字框的信息
            m_TextPoint = QPoint(pText->GetStartPosX(),pText->GetStartPosY());
            m_ContentEdit.show();
            m_curShape = NULL;
            update();
            //            qDebug()<<"update";
            m_ContentEdit.setGeometry(m_TextPoint.x()-3,m_TextPoint.y()-22.5,200,30);
            m_ContentEdit.setText(pText->GetContent());
        }
    }

    QOpenGLWidget::mouseDoubleClickEvent(event);
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton){
        m_bLBtnClicked = false;
        m_MovePoint = event->pos();
        if(this->m_curShapeIndex == -1){
            m_TextPoint = event->pos();
        }

        switch (m_ShapeType) {
        case EShapeType::Shape_Reckangle:
        {
            int Xmin = m_ClickPoint.x() < m_MovePoint.x()?m_ClickPoint.x():m_MovePoint.x();
            int Ymin = m_ClickPoint.y() < m_MovePoint.y()?m_ClickPoint.y():m_MovePoint.y();
            QPoint dis = m_MovePoint - m_ClickPoint;
            RectangleData * pRectangle = new RectangleData(double(Xmin),double(Ymin),qAbs(double(dis.x())),qAbs(double(dis.y())));
            m_pSystemData->m_ShapeVec.push_back(pRectangle);
            update();
            break;
        }
        case EShapeType::Shape_Ellipse:
        {
            int Xmin = m_ClickPoint.x() < m_MovePoint.x()?m_ClickPoint.x():m_MovePoint.x();
            int Ymin = m_ClickPoint.y() < m_MovePoint.y()?m_ClickPoint.y():m_MovePoint.y();
            QPoint dis = m_MovePoint - m_ClickPoint;
            EllipseData *pEllipse = new EllipseData(double(Xmin),double(Ymin),qAbs(double(dis.x())),qAbs(double(dis.y())));
            m_pSystemData->m_ShapeVec.push_back(pEllipse);
            update();
            break;
        }
        case EShapeType::Shape_Triangle:
        {
            int Xmin = m_ClickPoint.x() < m_MovePoint.x()?m_ClickPoint.x():m_MovePoint.x();
            int Ymin = m_ClickPoint.y() < m_MovePoint.y()?m_ClickPoint.y():m_MovePoint.y();
            QPoint dis = m_MovePoint - m_ClickPoint;
            // 最上面的点坐标加进去
            TriangeleData *pTriangle = new TriangeleData(double(Xmin),double(Ymin),qAbs(double(dis.x())),qAbs(double(dis.y())));
            m_pSystemData->m_ShapeVec.push_back(pTriangle);
            update();
            break;
        }
        case EShapeType::Shape_Line:
        {
            LineData *pLine = new  LineData(double(m_ClickPoint.x()),double(m_ClickPoint.y()),double(m_MovePoint.x()),double(m_MovePoint.y()));
            m_pSystemData->m_ShapeVec.push_back(pLine);
            m_pSystemData->SetShapePointMap(m_ClickPoint); // 把直线的点插入到map中去
            m_pSystemData->SetShapePointMap(m_MovePoint); // 每个点对应容器中的图像为位置
            //            qDebug()<<m_pSystemData->m_ShapePointMap.size();
            update();
            break;
        }
        case EShapeType::Shape_Text:
        {
            m_TextPoint = m_MovePoint;
            m_ContentEdit.clear();
            m_ContentEdit.show();
            m_ContentEdit.setGeometry(m_TextPoint.x(),m_TextPoint.y(),200,30);
            m_ContentEdit.setFocus();
            break;
        }
        case EShapeType::Erasure:
        {
            ErasureData *pErasure = new ErasureData(ErasurePoint);
            m_pSystemData->m_ShapeVec.push_back(pErasure);
            this->ErasurePoint.clear();
            break;
        }
        //case EShapeType::Shape_Demensions:
        //{
        //    if(DemensionsPoint.size() == 3){
        //        DemensionsData *pDemensions = new DemensionsData(this->DemensionsPoint);
        //        //                pDemensions->SetShowText();// 如果只想计算一次，这里打开，类里的关闭
        //        m_pSystemData->m_ShapeVec.push_back(pDemensions);
        //        this->DemensionsPoint.clear();
        //        this->m_DemensionBtnClicked = false;// 如果想设置连续标注，可以设置变量来控制，保存上次按下的按钮，然后通过某个键盘来触发
        //        this->SetShapeType(EShapeType::Shape_Unkonwn);// 不然回出错,为空的时候
        //    }
        //    break;
        //}
        default:
            break;
        }

        //if(this->DemensionsPoint.size() == 2){
        //    //            qDebug() << QString("this->DemensionsPoint.size() release:") <<  this->DemensionsPoint.size();
        //    this->DemensionsPoint.push_back(m_MovePoint);
        //}
    }

    QOpenGLWidget::mouseReleaseEvent(event);
}

// 本窗口鼠标移动事件
void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标进行实时捕获，判断是否是在某个点的范围内
    QPoint point = event->pos();
    this->mouseOnOnePoint(point);
    if(this->m_CapturePoint){
        //        qDebug()<<"已经在点中了";
        this->setCursor(Qt::CrossCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);  //范围之外变回原来形状  https://blog.csdn.net/technologyleader/article/details/82981718
    }
    this->m_CapturePoint = nullptr;

    // ]
    if(m_curShape && m_bLBtnClicked){
        m_curShape->moveShape(m_MovePoint,event->pos());
        update();
    }
    m_MovePoint = event->pos();

    if(m_bLBtnClicked){
        if(m_ShapeType == EShapeType::Erasure){
            ErasurePoint.push_back(event->pos());
        }
        m_MovePoint = event->pos();
        update();
    }

    //// 标注实时移动
    //if(DemensionsPoint.size() == 3){
    //    //        qDebug() << QString("this->DemensionsPoint.size() move:") <<  this->DemensionsPoint.size();
    //    DemensionsPoint[2] = m_MovePoint;// 更新移动
    //}

    QOpenGLWidget::mouseMoveEvent(event);
}

//int num =0;
void DrawWidget::mouseOnOnePoint(QPoint &point)
{
    foreach (const QPoint &p, m_pSystemData->m_ShapePointMap.keys()) {
        //        qDebug()<<num<< p << point;
        if((point.x()-p.x())*(point.x()-p.x()) + (point.y()-p.y())*(point.y()-p.y()) < 25){
            this->m_CapturePoint = &p;
        }
    }
    //    num+= 1;
}

// 文本编辑事件
void DrawWidget::fn_Recv_ContentEdit_GetContent(const QString &qstrContent)
{
    if(!qstrContent.isEmpty()){
        if(this->m_curShapeIndex != -1){
            TextData *pText = new TextData(double(m_TextPoint.x()),double(m_TextPoint.y()),qstrContent);
            m_pSystemData->m_ShapeVec[this->m_curShapeIndex] = pText;
            //            qDebug()<<"m_pSystemData->m_ShapeVec[this->m_curShapeIndex] = pText;";
        }else{
            //            qDebug()<<" m_pSystemData->m_ShapeVec.push_back(pText);";
            TextData *pText = new TextData(double(m_TextPoint.x())+3,double(m_TextPoint.y()+22.5),qstrContent);
            //            m_pSystemData->m_ShapeVec.pop_back();
            m_pSystemData->m_ShapeVec.push_back(pText);
        }
    }
    update();
    this->m_curShapeIndex = -1;
}

//void DrawWidget::fn_Change_StatusEdit_Visual(const QString &qstrContent){
//    if(qstrContent.isEmpty()){
//        this->m_StatusEdit.setStyleSheet("QLineEdit{background-color:transparent}"
//                                         "QLineEdit{border-width:0;border-style:outset}");
//    }else{
//        this->m_StatusEdit.setStyleSheet("color: blue;"
//                                         "background-color: yellow;"
//                                         "selection-color: yellow;"
//                                         "selection-background-color: blue;");
//    }
//    this->paintEvent(NULL);
//    update();
//}

// 左旋转角度
//int DrawWidget::RotateLeft()
//{
//    switch(m_RotateType){
//    case ERotateType::Rotate_180:
//    {
//        m_RotateType = ERotateType::Rotate_90;
//        break;
//    }
//    case ERotateType::Rotate_270:
//    {
//        m_RotateType = ERotateType::Rotate_180;
//        break;
//    }
//    case ERotateType::Rotate_90:
//    {
//        m_RotateType = ERotateType::Rotate_0;
//        break;
//    }
//    case ERotateType::Rotate_0:
//    {
//        m_RotateType = ERotateType::Rotate_270;
//        break;
//    }
//    default:
//        break;
//    }
//    update();
//    return NORMAL_RETURN;
//}

//// 右旋转角度
//int DrawWidget::RotateRight()
//{
//    switch(m_RotateType){
//    case ERotateType::Rotate_180:
//    {
//        m_RotateType = ERotateType::Rotate_270;
//        break;
//    }
//    case ERotateType::Rotate_270:
//    {
//        m_RotateType = ERotateType::Rotate_0;
//        break;
//    }
//    case ERotateType::Rotate_90:
//    {
//        m_RotateType = ERotateType::Rotate_180;
//        break;
//    }
//    case ERotateType::Rotate_0:
//    {
//        m_RotateType = ERotateType::Rotate_90;
//        break;
//    }
//    default:
//        break;
//    }
//    update();
//    return NORMAL_RETURN;
//}

// 进行尺寸的标记
//void DrawWidget::Demensions()
//{
//    m_DemensionBtnClicked = true;
//    if(this->DemensionsPoint.size() == 0){
//        this->m_StatusEdit.setText("Select the first point of the object");
//    }
//}



// 这段代码经过改进：改进【1】
//        switch (m_ShapeType) {
//        case EShapeType::Shape_Reckangle:
//        {
//            ShapeData *pShape = m_pSystemData->CreateShapeItem(m_ShapeType,m_ClickPoint,m_MovePoint,ErasurePoint);
//            pShape->drawShape(painter);
//            break;
//        }
//        case EShapeType::Shape_Ellipse:
//        {
//            ShapeData *pShape = m_pSystemData->CreateShapeItem(m_ShapeType,m_ClickPoint,m_MovePoint,ErasurePoint);
//            pShape->drawShape(painter);
//            break;
//        }
//        case EShapeType::Shape_Triangle:
//        {
//            int Xmin = m_ClickPoint.x() < m_MovePoint.x()?m_ClickPoint.x():m_MovePoint.x();
//            int Ymin = m_ClickPoint.y() < m_MovePoint.y()?m_ClickPoint.y():m_MovePoint.y();
//            QPoint dis = m_MovePoint - m_ClickPoint;

//            QPoint point1(Xmin,Ymin);
//            QPoint point2(Xmin-qAbs(dis.x()),Ymin+qAbs(dis.y()));
//            QPoint point3(Xmin+qAbs(dis.x()),Ymin+qAbs(dis.y()));
//            painter.drawLine(point1,point2);
//            painter.drawLine(point1,point3);
//            painter.drawLine(point2,point3);
//            break;
//        }
//        case EShapeType::Shape_Line:
//        {
//            painter.drawLine(m_ClickPoint,m_MovePoint);
//            break;
//        }
//        case EShapeType::Shape_Text:
//        {
//            m_TextPoint = m_ClickPoint;
//            //            TextData *qText = TextData(m_TextPoint.x(),m_TextPoint.y(),m_ContentEdit.text());
//            m_ContentEdit.show();
//            m_ContentEdit.setGeometry(m_TextPoint.x(),m_TextPoint.y(),200,30);
//            painter.drawText(QPoint(m_TextPoint.x(),m_TextPoint.y()),m_ContentEdit.text());
//            break;
//        }
//        case EShapeType::Erasure:
//        {
//            painter.setPen(m_ErasurePen);
//            painter.drawPolyline(QPolygon(ErasurePoint));
//            break;
//        }
//        default:
//            break;
//        }


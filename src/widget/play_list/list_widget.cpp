#include "list_widget.h"

#include <qscrollbar.h>

namespace yk {
    
ListWidget::ListWidget(QWidget* parent) : QListWidget(parent) 
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameShape(QFrame::NoFrame);
    verticalScrollBar()->setStyleSheet(
        R"(QScrollBar:vertical
            {
                width:8px;
                background:rgba(0,0,0,0%);
                margin:0px,0px,0px,0px;
                padding-top:4px;   
                padding-bottom:4px;
            }
            QScrollBar::handle:vertical
            {
                width:8px;
                background:rgba(150,150,150,50%);
                border-radius:4px;   
                min-height:20;
            }
            QScrollBar::handle:vertical:hover
            {
                width:8px;
                background:rgba(180,180,180,50%);
                border-radius:4px;
                min-height:20;
            }
            QScrollBar::add-line:vertical  
            {
                height:9px;width:8px;
                border-image:url(:/images/a/3.png);
                subcontrol-position:bottom;
            }
            QScrollBar::sub-line:vertical 
            {
                height:9px;width:8px;
                border-image:url(:/images/a/1.png);
                subcontrol-position:top;
            }
            QScrollBar::add-line:vertical:hover  
            {
                height:9px;width:8px;
                border-image:url(:/images/a/4.png);
                subcontrol-position:bottom;
            }
            QScrollBar::sub-line:vertical:hover  
            {
                height:9px;width:8px;
                border-image:url(:/images/a/2.png);
                subcontrol-position:top;
            }
            QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical   
            {
                background:rgba(0,0,0,10%);
                border-radius:4px;
            })");
    }

}
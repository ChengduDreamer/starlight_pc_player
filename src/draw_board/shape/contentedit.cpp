#include "contentedit.h"

ContentEdit::ContentEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

ContentEdit::~ContentEdit()
{

}

void ContentEdit::focusOutEvent(QFocusEvent *e){
    hide();
    //    emit Signal_GetContent(this->text());
    QLineEdit::focusOutEvent(e);
}

void ContentEdit::keyPressEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Enter
            || e->key() ==Qt::Key_Escape
            || e->key() == Qt::Key_Return){
        hide();
        emit Signal_GetContent(this->text());
        this->clear();
    }
    QLineEdit::keyPressEvent(e);
}

void ContentEdit::contextMenuEvent(QContextMenuEvent *){
    return;// 不要让菜单弹出，信号传递
}

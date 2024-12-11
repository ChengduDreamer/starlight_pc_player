#ifndef CONTENTEDITH_H
#define CONTENTEDITH_H

#include <QLineEdit>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QWidget>
#include <QMouseEvent>

class ContentEdit : public QLineEdit
{
    Q_OBJECT

public:
    ContentEdit(QWidget *parent = nullptr);
    ~ContentEdit();

    void focusOutEvent(QFocusEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    //    void focusInEvent(QFocusEvent *) override;

signals:
    void Signal_GetContent(QString qstr);


private:
};


#endif // CONTENTEDITH_H

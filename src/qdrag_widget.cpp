#include "qdrag_widget.h"

#include <qdebug.h>
#include <qlist.h>
#include <QMimeData>

namespace yk {

QDragWidget::QDragWidget(QWidget* parent) : QWidget(parent) {
    setAcceptDrops(true);
}

QDragWidget::~QDragWidget() {

}


void QDragWidget::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

void QDragWidget::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QUrl url = urls.at(0);
    QString filePath = url.toLocalFile();
    qDebug() << "file = " << filePath;

    emit SigOpenUrl(filePath);
}

}
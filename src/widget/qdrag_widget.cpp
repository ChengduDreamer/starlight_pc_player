#include "qdrag_widget.h"

#include <qdebug.h>
#include <qlist.h>
#include <QMimeData>
#include <qdebug.h>
#include "app_messages.h"
#include "context.h"

namespace yk {

QDragWidget::QDragWidget(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
    setMouseTracking(true);
    setAcceptDrops(true);
    installEventFilter(this);
}

bool QDragWidget::eventFilter(QObject* obj, QEvent* event) {



    return QWidget::eventFilter(obj, event);
}

QDragWidget::~QDragWidget() {

}

void QDragWidget::dragEnterEvent(QDragEnterEvent* event) {
    qDebug() << "dragEnterEvent";
    event->acceptProposedAction();

}

void QDragWidget::dropEvent(QDropEvent* event) {
    qDebug() << "dropEvent";
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        QWidget::dropEvent(event);
        return;
    }
    QUrl url = urls.at(0);
    QString filePath = url.toLocalFile();
    qDebug() << "file = " << filePath;
    AppOpenUrlMsg msg{.url = filePath };
    context_->SendAppMessage(msg);
    QWidget::dropEvent(event);
}

}
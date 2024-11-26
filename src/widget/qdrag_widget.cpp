#include "qdrag_widget.h"

#include <qdebug.h>
#include <qlist.h>
#include <QMimeData>
#include "app_messages.h"
#include "context.h"

namespace yk {

QDragWidget::QDragWidget(const std::shared_ptr<Context>& context, QWidget* parent) : context_(context), QWidget(parent) {
    setAcceptDrops(true);
}

QDragWidget::~QDragWidget() {

}

void QDragWidget::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

void QDragWidget::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }
    QUrl url = urls.at(0);
    QString filePath = url.toLocalFile();
    qDebug() << "file = " << filePath;
    AppOpenUrlMsg msg{.url = filePath };
    context_->SendAppMessage(msg);
}

}
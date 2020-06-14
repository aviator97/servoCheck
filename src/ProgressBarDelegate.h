#ifndef PROGRESSBARDELEGATE_H
#define PROGRESSBARDELEGATE_H

#include <QApplication>
#include <QStyledItemDelegate>
#include <QString>

class ProgressBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ProgressBarDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        int progress = index.model()->data(index).toInt() - 900;

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 1200;
        progressBarOption.progress = progress;
        progressBarOption.text = QString("%1 ms").arg(progress,5,10,QChar(' ')); //QString::number(progress) + "ms";
        progressBarOption.textVisible = true;



        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};
#endif // PROGRESSBARDELEGATE_H

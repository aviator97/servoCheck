/*
    delegate.cpp

    A delegate that allows the user to change double values from the model
    using a spin box widget.
*/

#include <iostream>
#include "DoubleSpinBoxDelegate.h"
#include <QDoubleSpinBox>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QAbstractTableModel* model, QObject *parent)
    : QStyledItemDelegate(parent), model_(model)
{
    connect(this, &DoubleSpinBoxDelegate::setRowCol, this, &DoubleSpinBoxDelegate::setRowColSlot);
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(-20);
    editor->setMaximum(20);

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{

    int row1 = index.row();
    int col1 = index.column();
    emit setRowCol(row1, col1);

    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setDecimals(1);
    spinBox->setSingleStep(0.1);
    spinBox->setValue(value);
    connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
//    connect(spinBox, &QDoubleSpinBox::valueChanged(double), this, &SpinBoxDelegate::setValue);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void DoubleSpinBoxDelegate::setValue(double value)
{
    std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " value = " << value << " row = " << row_ << " col = " << col_ << std::endl;
    QModelIndex index = model_->index(row_, col_);
    model_->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::setRowColSlot(int row, int col)
{
    std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " value = " << " row = " << row << " col = " << col << std::endl;
    this->row_ = row;
    this->col_ = col;
}

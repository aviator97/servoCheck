#include <QHeaderView>
#include <QMdiArea>
#include <iostream>
#include "PartModel.h"
#include "PartsWindow.h"
#include "serialportreader.h"
#include "DoubleSpinBoxDelegate.h"

PartsWindow::PartsWindow(QWidget *parent) : QTableView(parent)
{
    partModel = new PartModel(this);
    setModel(partModel);
    setWindowTitle("Parts");


    for(int i=3; i<PartSpec::ITEMS; i++) {
        DoubleSpinBoxDelegate* delegate = new DoubleSpinBoxDelegate(partModel);
        setItemDelegateForColumn(i, delegate);
    }
    horizontalHeader()->setStretchLastSection(true);

    partModel->readSettings();

    setMinimumSize(400, 200);
    for(int i=0; i<PartSpec::ITEMS; i++) {
        resizeColumnToContents(i);
    }

}

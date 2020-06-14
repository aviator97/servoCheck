#ifndef OSGMDICHILD_H
#define OSGMDICHILD_H
#include <iostream>
#include <QObject>
#include <OsgWidget.h>
#include <osg/MatrixTransform>
#include <QList>
#include "mdichildbase.h"

//forward declarations
class QTableView;
class SerialPortReader;
class OsgWidget;
class ArticulatedPart;
class ServoModel;
class PartModel;
class DoubleSpinBoxDelegate;
class QMdiArea;

namespace osg {
class Group;
class MatrixTransform;
class Transform;
}

class ViewerWindow : public OsgWidget, public MdiChildBase
{
    Q_OBJECT
public:
    ViewerWindow(PartModel* partModel, QWidget *parent = 0);

public slots:
    void updateDynamics(ServoModel* model);
    void partsChanged(int partNumber);

private slots:
//    void documentWasModified();
    void showWindowTitle(const QString & title);

private:
    void createVisualModel();
    osg::Group* createShapes(PartModel* partsModel);


    void createAxis(osg::Transform* previousJoint);

    PartModel *partModel_;

    QList<ArticulatedPart*> parts;

    OsgWidget* osg_widget_ = nullptr;

    osg::Group* root;

    bool showAxis = true;

};

#endif // OSGMDICHILD_H

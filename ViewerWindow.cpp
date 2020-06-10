#include <QTableView>
#include <QHeaderView>
#include <QMdiArea>
#include <iostream>
#include "OsgWidget.h"
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Math>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osg/Vec3d>
#include <osg/PositionAttitudeTransform>
#include "ArticulatedPart.h"
#include "PartSpec.h"
#include "PartModel.h"
#include "mainwindow.h"
#include "ChannelModel.h"
#include "ServoModel.h"
#include "serialportreader.h"
#include "DoubleSpinBoxDelegate.h"
#include "ProgressBarDelegate.h"

#include "ViewerWindow.h"

ViewerWindow::ViewerWindow(PartModel* partModel, QWidget *parent) : OsgWidget(parent), partModel_(partModel)
{
    setMinimumSize(400, 300);

    createVisualModel();

}

osg::Group* ViewerWindow::createShapes(PartModel* partsModel)
{
//    std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;

    osg::Group* root = new osg::Group;

    foreach(auto p, *(partsModel->getPartSpec())) {
        if(p->servo_ > 0) {
            parts.append( new ArticulatedPart(p) );
            parts.back()->addToRoot(root);
        }
    }

    std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " number of parts = " << parts.size() << std::endl;

    return root;
}

osg::TessellationHints* hints = new osg::TessellationHints;

void ViewerWindow::createAxis(osg::Transform* previousJoint)
{
    double height = 12.0;
    double radius = .5;

    osg::MatrixTransform* zmt = new osg::MatrixTransform();

    previousJoint->addChild(zmt);
    osg::ShapeDrawable *zShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
    osg::ShapeDrawable *zCone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.0f,0.0f,1.0),radius+1.0,2.0),hints);

    osg::MatrixTransform* zmtCone = new osg::MatrixTransform();
    osg::Geode *zgCone = new osg::Geode;

    zmtCone->setMatrix( osg::Matrix::translate(0.0,0.0,height));
    previousJoint->addChild(zmtCone);

    zShape->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    zCone->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
    osg::Geode *z = new osg::Geode;
    z->addDrawable(zShape);
    zgCone->addDrawable(zCone);
    zmtCone->addChild(zgCone);
    zmt->addChild(z);

    osg::MatrixTransform* mt = new osg::MatrixTransform();
    previousJoint->addChild(mt);

    osg::Matrix xMatrix = osg::Matrix::rotate(-osg::PI_2, 0.0, 1.0, 0.0);
    mt->setMatrix(xMatrix);


    osg::ShapeDrawable *xShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
    xShape->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    osg::Geode *x = new osg::Geode;
    x->addDrawable(xShape);
    mt->addChild(x);


    osg::MatrixTransform *yMt = new osg::MatrixTransform();
    previousJoint->addChild(yMt);
    osg::Matrix yMatrix = osg::Matrix::rotate(osg::PI_2, 1.0, 0.0, 0.0);
    yMt->setMatrix(yMatrix);

    osg::ShapeDrawable *yShape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,height/2),radius,height),hints);
    yShape->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    osg::Geode *y = new osg::Geode;
    y->addDrawable(yShape);
    yMt->addChild(y);
}

void ViewerWindow::createVisualModel()
{
    hints->setDetailRatio(0.5f);

    setMinimumSize(400, 300);
    setWindowTitle("Viewer");
    root = createShapes(partModel_);
    setSceneData( root );
}

void ViewerWindow::showWindowTitle(const QString & title)
{
    setWindowTitle(title);
}

void ViewerWindow::partsChanged(int partNumber)
{
    std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " partNumber = " << partNumber << std::endl;

    if(partNumber < parts.size()) {
        parts[partNumber]->update(root);
    }
}

void ViewerWindow::updateDynamics(ServoModel* model)
{
    foreach(auto p, parts) {
       int servo = p->getPartSpec()->servo_;
       double angle = model->getAngle(servo);
       p->updateDynamics(angle);
//       std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " servo = "  << servo << " angle = " << angle << std::endl;
    }
    OsgWidget::update();
}


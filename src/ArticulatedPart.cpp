#include <iostream>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Matrixd>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include "ArticulatedPart.h"
#include "PartSpec.h"

ArticulatedPart::ArticulatedPart(PartSpec *spec) : servo_(spec->servo_), spec_(spec)
{
    update();
}

void ArticulatedPart::update(osg::Group* root)
{

    if(spec_->part_ == "Box") {
        // Shapes
        osg::ref_ptr<osg::ShapeDrawable> aileronShape = new osg::ShapeDrawable(
                    new osg::Box(osg::Vec3(), spec_->dim_.x(), spec_->dim_.y(), spec_->dim_.z()) );
        // The scene graph_->
        partRef_ = osg::Matrix::translate(  spec_->joint_.x(), spec_->joint_.y(), spec_->joint_.z());// * osg::Matrix::rotate(osg::PI_2, 0.0, 1.0, 0.0);
        partTransf_ = createTransformNode(aileronShape.get(), partRef_ );
        if(root) {
            if(mainTransf_) {
                root->removeChild(mainTransf_);
                //            delete mainTransf_;
                mainTransf_ = new osg::MatrixTransform(); //createTransformNode( aileronShape.get(), osg::Matrix::identity() );
                mainTransf_->addChild(partTransf_);
                root->addChild( mainTransf_ );
            }
        }
    }
    else if(spec_->part_ == "Prop") {
        // TODO if(root) {
        std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " Propeller" << std::endl;

        osg::ref_ptr<osg::ShapeDrawable> partShape = new osg::ShapeDrawable(
                    new osg::Box(osg::Vec3(), spec_->dim_.x(), spec_->dim_.y(), spec_->dim_.z()) );
        partTransf_ = new osg::MatrixTransform;
        animation_ = new osg::AnimationPathCallback(osg::Vec3(0.0f,0.0f,0.0f),osg::Vec3(0.0f,-1.0f,0.0f), osg::inDegrees(2*360.0f));
        partTransf_->setUpdateCallback(animation_);
        osg::MatrixTransform* positioner = new osg::MatrixTransform;
        positioner->setMatrix(osg::Matrix::translate(spec_->joint_.x(), spec_->joint_.y(), spec_->joint_.z()));
        positioner->addChild(partShape);

        partTransf_->addChild(positioner);
    }

}

void ArticulatedPart::addToRoot( osg::Group* root)
{
    mainTransf_ = new osg::MatrixTransform(); //createTransformNode( aileronShape.get(), osg::Matrix::identity() );

    mainTransf_->addChild(partTransf_);

    root->addChild( mainTransf_ );

}

void ArticulatedPart::updateDynamics(double value)
{
    osg::Matrix pos;
    pos.makeTranslate(spec_->pos_);
    if(spec_->part_ == "Prop") {
        animation_->setTimeMultiplier(value * 0.01);
//        std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " TimeMultiplier = " << value * 0.02 << std::endl;
    }
    else {
        osg::Matrix xRot;
        xRot.makeRotate( value * osg::PI / 180.0, spec_->axis_.x(), spec_->axis_.y(), spec_->axis_.z()  );
        partTransf_->setMatrix(partRef_ * xRot * pos);
    }
}

osg::MatrixTransform* ArticulatedPart::createTransformNode( osg::Drawable* shape,  const osg::Matrix& matrix )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( shape );

    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
    trans->addChild( geode.get() );
    trans->setMatrix( matrix );
    return trans.release();
}

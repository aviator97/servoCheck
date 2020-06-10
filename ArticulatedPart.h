#ifndef ARTICULATEDPART_H
#define ARTICULATEDPART_H
#include <osg/Group>

namespace osg {
    class AnimationPathCallback;
}

class PartSpec;

class ArticulatedPart
{
public:
    ArticulatedPart(PartSpec* spec);
    void update(osg::Group* root = nullptr);

    osg::MatrixTransform* createTransformNode( osg::Drawable* shape,  const osg::Matrix& matrix );
    void addToRoot( osg::Group* root);
    osg::MatrixTransform* getMainTransform() { return  mainTransf_; }
    PartSpec* getPartSpec() { return spec_; }
    void updateDynamics(double angle);

private:
    int servo_;
    PartSpec* spec_;
    osg::Matrix partRef_;
    osg::MatrixTransform* partTransf_ = nullptr;
    osg::MatrixTransform* mainTransf_ = nullptr;
    osg::AnimationPathCallback* animation_ = nullptr;
    double spin_ = 0.0;
};

#endif // ARTICULATEDPART_H

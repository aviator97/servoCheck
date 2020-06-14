#ifndef PARTSPEC_H
#define PARTSPEC_H

#include <osg/Vec3>
#include <QString>

class PartSpec {
public:
    static const int ITEMS = 15;
    PartSpec();
    PartSpec( int servo, const osg::Vec3& dim, const osg::Vec3& joint, const osg::Vec3& pos, const osg::Vec3& axis, const QString& name = QString("rudder"), const QString& part = QString("Box")) :
        name_(name),
        servo_(servo),
        part_(part),
        dim_(dim),
        joint_(joint),
        pos_(pos),
        axis_(axis)
    {}

    QString name_;
    int servo_;
    QString part_;
    osg::Vec3 dim_;
    osg::Vec3 joint_;
    osg::Vec3 pos_;
    osg::Vec3 axis_;
};

#endif // PARTSPEC_H

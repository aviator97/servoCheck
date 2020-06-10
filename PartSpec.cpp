#include "PartSpec.h"

PartSpec::PartSpec() :
    name_("Rudder"),
    servo_(0),
    part_("Box"),
    dim_(osg::Vec3(3.0f, 1.0f, 0.1f)),
    joint_(osg::Vec3(0.0f, 0.5f, 0.0f)),
    pos_(osg::Vec3()),
    axis_(osg::Vec3(1.0f, 0.0f, 0.0f))
{
}

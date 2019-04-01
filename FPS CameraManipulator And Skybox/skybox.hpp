#pragma once

#include <osg/Transform>
#include <osgUtil/CullVisitor>
#include <osg/TextureCubeMap>
#include <osg/Depth>

class SkyBox : public osg::Transform
{
public:
    SkyBox();

    //Necessary construtor and macros when inherit from osg::Transform
    SkyBox( const SkyBox& copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY ) : osg::Transform(copy, copyop) {}
    META_Node( osg, SkyBox )

    void setEnvironmentMap( unsigned int unit,
                            osg::Image* posX, osg::Image* negX,
                            osg::Image* posY, osg::Image* negY,
                            osg::Image* posZ, osg::Image* negZ );

    virtual bool computeLocalToWorldMatrix( osg::Matrix& matrix,
                                            osg::NodeVisitor* nv ) const;

    virtual bool computeWorldToLocalMatrix( osg::Matrix& matrix,
                                            osg::NodeVisitor* nv ) const;
protected:
    virtual ~SkyBox() {}
};

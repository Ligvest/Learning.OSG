#include "skybox.hpp"
#include <osg/TexEnv>
#include <osg/TexGen>
#include <osg/TexMat>
#include <osg/Geode>
#include <osg/ShapeDrawable>


// Update texture matrix for cubemaps
struct TexMatCallback : public osg::NodeCallback
{
public:

    TexMatCallback(osg::TexMat& tm) :
        _texMat(tm)
    {
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (cv)
        {
        const osg::Matrix& MV = *(cv->getModelViewMatrix());
            const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
                                  osg::Matrix::rotate( osg::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

            osg::Quat q;
            MV.get(q);
            const osg::Matrix C = osg::Matrix::rotate( q.inverse() );

            _texMat.setMatrix( C*R );
        }

        traverse(node,nv);
    }

    osg::TexMat& _texMat;
};





SkyBox::SkyBox()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(), 1))); //scene->getBound().radius()
    setCullingActive(false);
    addChild( geode.get() );
}


void SkyBox::setEnvironmentMap( unsigned int unit,
                                osg::Image* posX, osg::Image* negX,
                                osg::Image* posY, osg::Image* negY,
                                osg::Image* posZ, osg::Image* negZ )
{
    osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;
    cubemap->setImage( osg::TextureCubeMap::POSITIVE_X, posX );
    cubemap->setImage( osg::TextureCubeMap::NEGATIVE_X, negX );
    cubemap->setImage( osg::TextureCubeMap::POSITIVE_Y, posY );
    cubemap->setImage( osg::TextureCubeMap::NEGATIVE_Y, negY );
    cubemap->setImage( osg::TextureCubeMap::POSITIVE_Z, posZ );
    cubemap->setImage( osg::TextureCubeMap::NEGATIVE_Z, negZ );


    cubemap->setResizeNonPowerOfTwoHint( false );
    getOrCreateStateSet()->setTextureAttributeAndModes(unit, cubemap.get() );


    setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    setCullingActive( false );
    osg::StateSet* ss = getOrCreateStateSet();
    ss->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL, 1.0f, 1.0f) );
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );


    ss->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

    //Order of rendering objects
    ss->setRenderBinDetails( 5, "RenderBin" );


    //Generate Texture coordinates
    osg::TexGen *tg = new osg::TexGen;
    tg->setMode(osg::TexGen::NORMAL_MAP);
    ss->setTextureAttributeAndModes(0, tg, osg::StateAttribute::ON);

    //Texture matrix
    osg::TexMat *tm = new osg::TexMat;
    ss->setTextureAttribute(0, tm);
    setCullCallback(new TexMatCallback(*tm));
}



//Set center os the skybox to our eye position in every frame
bool SkyBox::computeLocalToWorldMatrix( osg::Matrix& matrix, osg::NodeVisitor* nv ) const
{
    if ( nv && nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {
        osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( nv );
        matrix.preMult( osg::Matrix::translate(cv->getEyeLocal()) );
        return true;
    }
    else {
        return osg::Transform::computeLocalToWorldMatrix( matrix, nv );
    }
}


//Opposite to computeLocalToWorldMatrix
bool SkyBox::computeWorldToLocalMatrix( osg::Matrix& matrix, osg::NodeVisitor* nv ) const
{
    if ( nv && nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {
        osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( nv );
        matrix.postMult( osg::Matrix::translate(-cv->getEyeLocal()));
        return true;
    }
    else {
        return osg::Transform::computeWorldToLocalMatrix( matrix, nv );
    }
}

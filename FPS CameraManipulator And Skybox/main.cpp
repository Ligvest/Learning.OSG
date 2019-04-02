#include <osgDB/ReadFile>
#include "freecameramanipulator.hpp"
#include "skybox.hpp"

int main(int argc, char *argv[])
{
    osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile("../resourses/lz.osg");

    osg::ref_ptr<SkyBox> skybox = new SkyBox;

    skybox->setEnvironmentMap( 0,
                               osgDB::readImageFile("../resourses/ely_hills/hills_ft.tga"), //front (pos X)
                               osgDB::readImageFile("../resourses/ely_hills/hills_bk.tga"), //back (neg X)
                               osgDB::readImageFile("../resourses/ely_hills/hills_dn.tga"), //down (pos Y)
                               osgDB::readImageFile("../resourses/ely_hills/hills_up.tga"), //up (neg Y)
                               osgDB::readImageFile("../resourses/ely_hills/hills_rt.tga"), //right (pos Z)
                               osgDB::readImageFile("../resourses/ely_hills/hills_lf.tga")); //left (neg Z)


    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( scene.get() );
    root->addChild( skybox );

    osgViewer::Viewer viewer;

    //Set fps camera
    FreeCameraManipulator *fps = new FreeCameraManipulator();
    viewer.setCameraManipulator(fps);    

    viewer.setSceneData( root.get() );

    return viewer.run();
}

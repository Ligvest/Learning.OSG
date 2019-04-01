#include    <osg/AnimationPath>
#include    <osg/MatrixTransform>
#include    <osgDB/ReadFile>
#include    <osgViewer/Viewer>


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
osg::AnimationPath *createAnimationPath(double radius, double time)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode(osg::AnimationPath::LOOP);

    unsigned int numSamples = 32; //32 straight lines for moving
    double delta_yaw = 2.0 * osg::PI / (static_cast<double>(numSamples) - 1.0);
    double delta_time = time / static_cast<double>(numSamples);

    //Creating control points
    for (unsigned int i = 0; i < numSamples; ++i)
    {
        double yaw = delta_yaw * i; //Angle
        osg::Vec3d pos(radius * sin(yaw), radius * cos(yaw), 10.0); //Position
        osg::Quat rot(-yaw, osg::Z_AXIS); //Rotation

        //At "delta_time*i" time apply "pos" position and "rot" rotation
        path->insert(delta_time * i, osg::AnimationPath::ControlPoint(pos, rot));
    }

    return path.release();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("../resourses/cessna.osg.0,0,90.rot");

    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;
    root->addChild(model.get());

    osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
    apcb->setAnimationPath(createAnimationPath(50.0, 6.0));

    root->setUpdateCallback(apcb.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());

    return viewer.run();
}

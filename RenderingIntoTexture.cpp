#include    <osg/Camera>
#include    <osg/Texture2D>
#include    <osg/MatrixTransform>
#include    <osgDB/ReadFile>
#include    <osgGA/TrackballManipulator>
#include    <osgViewer/Viewer>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
osg::Geometry *createQuad(const osg::Vec3 &pos, float w, float h)
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( pos + osg::Vec3( w / 2, 0.0f, -h / 2) );
    vertices->push_back( pos + osg::Vec3( w / 2, 0.0f,  h / 2) );
    vertices->push_back( pos + osg::Vec3(-w / 2, 0.0f,  h / 2) );
    vertices->push_back( pos + osg::Vec3(-w / 2, 0.0f, -h / 2) );

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
    texcoords->push_back( osg::Vec2(1.0f, 1.0f) );
    texcoords->push_back( osg::Vec2(1.0f, 0.0f) );
    texcoords->push_back( osg::Vec2(0.0f, 0.0f) );
    texcoords->push_back( osg::Vec2(0.0f, 1.0f) );

    osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
    quad->setVertexArray(vertices.get());
    quad->setNormalArray(normals.get());
    quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
    quad->setTexCoordArray(0, texcoords.get());
    quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return quad.release();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    //Create model with transform
    osg::ref_ptr<osg::Node> sub_model = osgDB::readNodeFile("../resourses/cessna.osg");
    osg::ref_ptr<osg::MatrixTransform> transform1 = new osg::MatrixTransform;
    transform1->addChild(sub_model.get());


    //Create square and texture for it
    osg::ref_ptr<osg::Geode> model = new osg::Geode;
    model->addChild(createQuad(osg::Vec3(0.0f, 0.0f, 0.0f), 2.0f, 2.0f));
    int iTextureWidht = 1024;
    int iTextureHeight = 1024;
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setTextureSize(iTextureWidht, iTextureHeight);
    texture->setInternalFormat(GL_RGBA);
    model->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());


    //Create second (not main) Camera
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setViewport(0, 0, iTextureWidht, iTextureHeight); //Viewport is defined by square area
    camera->setClearColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //Background color
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //To clear both Color and Depth buffers
    camera->setRenderOrder(osg::Camera::PRE_RENDER); //Render this camera before main camera
    camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    camera->attach(osg::Camera::COLOR_BUFFER, texture.get());
    camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF); //Absoulute coordinate system
    camera->addChild(transform1.get()); //Add model as a child

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(model.get());
    root->addChild(camera.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);


    //Set view matrix
    float dist = 100.0f;
    float alpha = 45.0f * 3.14f / 180.0f;
    osg::Vec3 cameraPosition(0.0f, -dist * cosf(alpha), dist * sinf(alpha));
//    osg::Vec3 cameraPosition(0.0f, -100.0f , 0.0f);
    osg::Vec3 lookAt(0.0f, 0.0f, 0.0f);
    osg::Vec3 upVector(0.0f, 0.0f, -1.0f);
    camera->setViewMatrixAsLookAt(cameraPosition, lookAt, upVector);

    //Set projection matrix
    float fFoV = 30.0;
    float fScreenRatio = static_cast<double>(iTextureWidht) / (static_cast<double>(iTextureHeight));
    float fMinClippingLimit = 0.1f;
    float fMaxClippingLimit = 1000.0f;
    camera->setProjectionMatrixAsPerspective(fFoV, fScreenRatio, fMinClippingLimit, fMaxClippingLimit);


    //Simple rotating
    float phi = 0.0f;
    float delta = 0.01f;
    while (!viewer.done())
    {
        transform1->setMatrix(osg::Matrix::rotate(static_cast<double>(phi), osg::Vec3(0.0f, 0.0f, 1.0f)));
        viewer.frame();
        phi += delta;
    }

    return 0;
}

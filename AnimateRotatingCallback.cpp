#include    <osg/Geometry>
#include    <osg/Geode>
#include    <osgViewer/Viewer>


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
osg::Geometry *createQuad()
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back( osg::Vec3(0.0f, -1.0f, 0.0f) );

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
    colors->push_back( osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) );
    colors->push_back( osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
    colors->push_back( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );

    osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
    quad->setVertexArray(vertices.get());
    quad->setNormalArray(normals.get());
    quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
    quad->setColorArray(colors.get());
    quad->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return quad.release();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class DynamicQuadCallback : public osg::Drawable::UpdateCallback
{
public:

    virtual void update(osg::NodeVisitor *, osg::Drawable *drawable) override;
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void DynamicQuadCallback::update(osg::NodeVisitor *, osg::Drawable *drawable)
{
    osg::Geometry *quad = static_cast<osg::Geometry *>(drawable);

    if (!quad)
        return;

    osg::Vec3Array *vertices = static_cast<osg::Vec3Array *>(quad->getVertexArray());

    if (!vertices)
        return;

    float fAngle = 3.0f;
    float fRads = osg::PI/180 * fAngle;
//    osg::Quat quat(osg::PI * 0.01, osg::X_AXIS);
    osg::Matrix *rotateMatrix = new osg::Matrix(1,     0,      0, 0,
                                                0, cos(fRads), -sin(fRads), 0,
                                                0, sin(fRads),  cos(fRads), 0,
                                                0,     0,      0, 1);

    vertices->back() = rotateMatrix->preMult(vertices->back());

//    vertices->back() = quat * vertices->back();

    quad->dirtyDisplayList();
    quad->dirtyBound();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    osg::Geometry *quad = createQuad();
    quad->setDataVariance(osg::Object::DYNAMIC);
    quad->setUpdateCallback(new DynamicQuadCallback);

    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    root->addDrawable(quad);

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());

    return viewer.run();
}

    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;
    root->addChild(model.get());

    //Rotate to initial position
    osg::Quat rot(osg::PI/180 * 90, osg::Z_AXIS);
    root->setMatrix(osg::Matrix::rotate(rot));
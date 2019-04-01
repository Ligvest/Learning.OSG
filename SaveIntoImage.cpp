osg::ref_ptr<osg::Image> image = new osg::Image;
image->allocateImage( width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE );
camera->attach( osg::Camera::COLOR_BUFFER, image.get() );
...
osgDB::writeImageFile( *image, "saved_image.bmp" );
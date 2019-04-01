#include "FreeCameraManipulator.hpp"
#include <iostream>


FreeCameraManipulator::FreeCameraManipulator() : 
    _position(osg::Vec3(0.0, 0.0, 100.0)),
	_rotation(osg::Quat()),
	_x(0.0),
	_y(osg::PI_2),
    _speed(0.7),
    _sensitivity(1.0),
    _move(osg::Vec3d(0.0,0.0,0.0)),
   _isWPressed(false),
   _isAPressed(false),
   _isSPressed(false),
   _isDPressed(false)
{
}


FreeCameraManipulator::FreeCameraManipulator(const FreeCameraManipulator& m, const osg::CopyOp& copyOp) : osgGA::StandardManipulator(m, copyOp)
{
}


void FreeCameraManipulator::setTransformation(const osg::Vec3d& eye, const osg::Quat& rotation)
{
}


void FreeCameraManipulator::setTransformation(const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up)
{
}


void FreeCameraManipulator::getTransformation(osg::Vec3d& eye, osg::Quat& rotation) const
{
}


void FreeCameraManipulator::getTransformation(osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up) const
{
}


void FreeCameraManipulator::home(double h)
{
}


void FreeCameraManipulator::home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    std::vector<osgViewer::GraphicsWindow*> windows;
    dynamic_cast<osgViewer::Viewer*>(&us)->getWindows(windows);
    for(osgViewer::GraphicsWindow* window : windows){
        window->useCursor(false);
    }
}


osg::Matrixd FreeCameraManipulator::getMatrix() const
{
    osg::Matrixd matrix;
    matrix.postMultTranslate(_position);
	matrix.postMultRotate(_rotation);
    return matrix;
}


osg::Matrixd FreeCameraManipulator::getInverseMatrix() const
{
    osg::Matrixd matrix;
    matrix.postMultTranslate(-_position);
	matrix.postMultRotate(-_rotation);
    return matrix;
}


void FreeCameraManipulator::setByMatrix(const osg::Matrixd& matrix)
{
    _position = matrix.getTrans();
   _rotation = matrix.getRotate();
}


void FreeCameraManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
	setByMatrix(osg::Matrixd::inverse(matrix));
}


bool FreeCameraManipulator::handleMouseMove(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    centerMousePointer(ea, us);

    std::cout<<"x"<<ea.getXnormalized()<<std::endl;
    std::cout<<"y"<<ea.getYnormalized()<<std::endl;
    if(ea.getXnormalized() > 0.001 || ea.getXnormalized() < -0.001){
        _x += _sensitivity * ea.getXnormalized();
    }

    if(ea.getYnormalized() > 0.001 || ea.getYnormalized() < -0.001){
        _y += _sensitivity * ea.getYnormalized();
    }

	if(_y < 0.0)
	{
		_y = 0.0;
	}
	else if(_y > osg::PI)
	{
		_y = osg::PI;
	}

    _rotation.makeRotate(_x, osg::Vec3d(0.0, 0.0, 1.0));
    _rotation *= osg::Quat(-_y, getSideVector(getCoordinateFrame(_position))/*getSideVector(getCoordinateFrame(_position))*/);

	return true;
}


bool FreeCameraManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    switch(ea.getKey())
    {
    case 'w':
        _isWPressed = true;
        break;

    case 'a':
        _isAPressed = true;
        break;

    case 's':
        _isSPressed = true;
        break;

    case 'd':
        _isDPressed = true;
        break;

    default:
        break;
    }

	return false;
}



bool FreeCameraManipulator::handleKeyUp(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    switch(ea.getKey())
    {
    case 'w':
        _isWPressed = false;
        break;

    case 'a':
        _isAPressed = false;
        break;

    case 's':
        _isSPressed = false;
        break;

    case 'd':
        _isDPressed = false;
        break;

    default:
        break;
    }

    return false;
}

bool FreeCameraManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa){
    osgGA::StandardManipulator::handle(ea, aa);
    if(_isWPressed){
        _move = _rotation.inverse() * osg::Vec3d(0.0, 0.0, -_speed);
        _position += _move;
    }
    if(_isAPressed){
        _move = _rotation.inverse() * osg::Vec3d(-_speed, 0.0, 0.0);
        _position += _move;
    }
    if(_isSPressed){
        _move = _rotation.inverse() * osg::Vec3d(0.0, 0.0, _speed);
        _position += _move;
    }
    if(_isDPressed){
        _move = _rotation.inverse() * osg::Vec3d(_speed, 0.0, 0.0);
        _position += _move;
    }

    return false;
}

void FreeCameraManipulator::setPosition(const osg::Vec3& position)
{
	_position = position;
}


void FreeCameraManipulator::setSpeed(double speed)
{
	_speed = speed;
}


void FreeCameraManipulator::setMouseSensitivity(double sens)
{
	_sensitivity = sens;
}

package net.gxstudios.shipwreck.gui;

import javax.media.opengl.GL2;
import javax.media.opengl.glu.GLU;

import net.gxstudios.shipwreck.math.MathUtil;
import net.gxstudios.shipwreck.math.vectors.Vector3d;

/**
This class represents a camera for 3D views. Cameras are defined with
a position and three mutually-orthogonal axes, namely n (points in the
direction faced by the camera), u (points to the left of the camera)
and v (points to the top of the camera).
*/
public class Camera
{
	//################## PRIVATE VARIABLES ##################//
	private static double s_angularSpeed = 1000/20.0;	// speed of angular camera movement in degrees/s
	private static double s_linearSpeed = 1000/7.5;		// speed of linear camera movement in units/s

	private Vector3d m_position;
	private Vector3d m_nVector;
	private Vector3d m_uVector;
	private Vector3d m_vVector;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a new camera.

	@param position	The position of the camera
	@param look		The direction the camera is looking
	@param up		The "up" direction for the camera
	*/
	public Camera(Vector3d position, Vector3d look, Vector3d up)
	{
		m_position = position;

		m_nVector = look;
		m_nVector.normalize();

		m_vVector = up;
		m_vVector.normalize();

		m_uVector = m_vVector.cross(m_nVector).normalize();
	}

	//################## PUBLIC METHODS ##################//
	/**
	Returns the current angular camera speed.

	@return	...think about it...
	*/
	final public static double get_angular_speed()
	{
		return s_angularSpeed;
	}

	/**
	Returns the current linear camera speed.

	@return	...think about it...
	*/
	final public static double get_linear_speed()
	{
		return s_linearSpeed;
	}

	/**
	Returns the n vector of the camera.

	@return	...think about it...
	*/
	public Vector3d get_n()
	{
		return m_nVector;
	}

	/**
	Returns the position of the camera.

	@return	...think about it...
	*/
	public Vector3d get_position()
	{
		return m_position;
	}

	/**
	Returns the u vector of the camera.

	@return	...think about it...
	*/
	public Vector3d get_u()
	{
		return m_uVector;
	}

	/**
	Returns the v vector of the camera.

	@return	...think about it...
	*/
	public Vector3d get_v()
	{
		return m_vVector;
	}

	/**
	Moves the camera by the specified displacement in the n direction.

	@param delta	The displacement by which to move
	*/
	public void move_n(double delta)
	{
		m_position = Vector3d.scale_add(delta, m_nVector, m_position);
	}

	/**
	Moves the camera by the specified displacement in the u direction.

	@param delta	The displacement by which to move
	*/
	public void move_u(double delta)
	{
		m_position = Vector3d.scale_add(delta, m_uVector, m_position);
	}

	/**
	Moves the camera by the specified displacement in the v direction.

	@param delta	The displacement by which to move
	*/
	public void move_v(double delta)
	{
		m_position = Vector3d.scale_add(delta, m_vVector, m_position);
	}

	/**
	Rotates the camera anticlockwise by the specified angle about the specified axis.

	@param axis		The axis about which to rotate
	@param angle	The angle by which to rotate (in radians)
	*/
	public void rotate(Vector3d axis, double angle)
	{
		// Note: We try and optimise things a little by observing that there's no point rotating
		// an axis about itself and that generally when we rotate about an axis, we'll be passing
		// it in as the parameter axis, e.g. camera.rotate(camera.get_n(), Math.PI/2).
		if(axis != m_nVector) m_nVector = MathUtil.rotate_about_axis(m_nVector, angle, axis);
		if(axis != m_uVector) m_uVector = MathUtil.rotate_about_axis(m_uVector, angle, axis);
		if(axis != m_vVector) m_vVector = MathUtil.rotate_about_axis(m_vVector, angle, axis);
	}

	/**
	Sets the angular speed of the camera.

	@param newAngularSpeed	The new angular camera speed
	*/
	final public static void set_angular_speed(double newAngularSpeed)
	{
		s_angularSpeed = newAngularSpeed;
	}

	/**
	Sets the linear speed of the camera.

	@param newLinearSpeed	The new linear camera speed
	*/
	final public static void set_linear_speed(double newLinearSpeed)
	{
		s_linearSpeed = newLinearSpeed;
	}

	/**
	Changes the view to be that of the camera.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The matrix-mode of the GL context passed in must be MODELVIEW
	</dl>

	@param gl	The OpenGL object associated with the context in which to set the view
	@param glu	The GLU object associated with the context in which to set the view
	*/
	public void set_view(GL2 gl, GLU glu)
	{
		gl.glLoadIdentity();
		glu.gluLookAt(	m_position.x, m_position.y, m_position.z,
						m_position.x + m_nVector.x, m_position.y + m_nVector.y, m_position.z + m_nVector.z,
						m_vVector.x, m_vVector.y, m_vVector.z);
	}
}
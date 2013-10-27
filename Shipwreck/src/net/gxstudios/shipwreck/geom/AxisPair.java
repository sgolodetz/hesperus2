package net.gxstudios.shipwreck.geom;

import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;

/**
This immutable class represents a pair of axes. Its intended use is in the implementation of
{@link net.gxstudios.shipwreck.gui.DesignCanvas}. It contains a number of utility methods
which are useful for working with 2D and 3D points.
*/
public class AxisPair
{
	public enum Axis
	{
		X, Y, Z
	}

	/** Horizontal axis */
	final private Axis m_hAxis;

	/** Vertical axis */
	final private Axis m_vAxis;

	/** Missing axis (the unused one of {X,Y,Z})*/
	final private Axis m_mAxis;

	/**
	Constructs an AxisPair from the specified axes.

	<p><b>Notes:</b>
	<ul>
	<li>The arguments must be Axis.X, Axis.Y or Axis.Z
	<li>The arguments cannot be the same
	</ul>

	@param hAxis			The horizontal axis
	@param vAxis			The vertical axis
	@throws java.lang.Error	If the arguments are the same
	*/
	public AxisPair(final Axis hAxis, final Axis vAxis)
	{
		if(hAxis == vAxis) throw new java.lang.Error();

		m_hAxis = hAxis;
		m_vAxis = vAxis;

		if		((m_hAxis == Axis.X && m_vAxis == Axis.Y) || (m_hAxis == Axis.Y && m_vAxis == Axis.X)) m_mAxis = Axis.Z;
		else if	((m_hAxis == Axis.X && m_vAxis == Axis.Z) || (m_hAxis == Axis.Z && m_vAxis == Axis.X)) m_mAxis = Axis.Y;
		else if	((m_hAxis == Axis.Y && m_vAxis == Axis.Z) || (m_hAxis == Axis.Z && m_vAxis == Axis.Y)) m_mAxis = Axis.X;
		else throw new java.lang.Error();
	}

	/**
	Given a 2D offset (in level coordinates), figure out which of the components of the
	3D point p its components correspond to, and add the offset accordingly.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Y), p = (1,2,3), offset = (4,5) -> result = (5,7,3).
	</dl>

	@param p		The 3D point to which to add the offset
	@param offset	The 2D offset to add to it
	*/
	public void add_offset(Vector3d p, final Vector2d offset)
	{
		switch(m_hAxis)
		{
			case X:
				p.x += offset.x;
				break;
			case Y:
				p.y += offset.x;
				break;
			case Z:
				p.z += offset.x;
				break;
		}

		switch(m_vAxis)
		{
			case X:
				p.x += offset.y;
				break;
			case Y:
				p.y += offset.y;
				break;
			case Z:
				p.z += offset.y;
				break;
		}
	}

	/**
	Returns a copy of this AxisPair with the axes reversed.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Y) -> result = the axis-pair (Y,X)
	</dl>
	*/
	public AxisPair flip()
	{
		return new AxisPair(m_vAxis, m_hAxis);
	}

	/**
	Converts a 2D point in the plane specified by the axes to a 3D point, by mapping the
	2D components to the relevant components in 3D, and then filling in the missing
	component using the other parameter we pass in.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Z), p = (2,4), missingComponent = 5 -> result = (2,5,4)
	</dl>

	@param p				The 2D point in the plane
	@param missingComponent	The value to set for the extra component resulting from our move from 2D to 3D
	@return					The 3D point generated
	*/
	public Vector3d generate_3D_point(final Vector2d p, double missingComponent)
	{
		Vector3d ret = new Vector3d();

		switch(m_hAxis)
		{
			case X:
				ret.x = p.x;
				break;
			case Y:
				ret.y = p.x;
				break;
			case Z:
				ret.z = p.x;
				break;
		}

		switch(m_vAxis)
		{
			case X:
				ret.x = p.y;
				break;
			case Y:
				ret.y = p.y;
				break;
			case Z:
				ret.z = p.y;
				break;
		}

		set_missing_component(ret, missingComponent);

		return ret;
	}

	/**
	Gets the horizontal axis.

	@return	...think about it...
	*/
	public Axis get_horizontal_axis()
	{
		return m_hAxis;
	}

	/**
	Gets the component of the 3D point which is on the missing axis.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Z), p = (3,4,5) -> result = 4
	</dl>

	@param p	The 3D point
	@return		...think about it...
	*/
	public double get_missing_component(final Vector3d p)
	{
		switch(m_mAxis)
		{
			case X:
				return p.x;
			case Y:
				return p.y;
			case Z:
				return p.z;
		}
		throw new java.lang.Error();
	}

	/**
	Gets the vertical axis.

	@return	...think about it...
	*/
	public Axis get_vertical_axis()
	{
		return m_vAxis;
	}

	/**
	Returns whether the axes are a natural (ordered) pair. By this, I mean that given an AxisPair (h,v),
	the cross-product of unit vectors along h and v gives us a unit vector along the missing axis, rather
	than the negation of the same. In other words, (x,y), (y,z) and (z,x) are natural pairs, whereas the
	others, namely (y,x), (z,y) and (x,z) are not.

	@return		...think about it...
	*/
	public boolean natural_pair()
	{
		switch(m_hAxis)
		{
			case X: return m_vAxis == Axis.Y;
			case Y: return m_vAxis == Axis.Z;
			case Z: return m_vAxis == Axis.X;
			default:	// this will never happen
				throw new java.lang.Error();
		}
	}

	/**
	Scales the components in the 3D point which correspond to those of the 2D point.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Z), dest = (2,4,2), src = (3,4) -> result = (6,4,8)
	</dl>

	@param dest	The 3D point whose components we wish to scale
	@param src	The 2D point containing the scaling factors
	*/
	public void scale_relevant_components(Vector3d dest, final Vector2d src)
	{
		switch(m_hAxis)
		{
			case X:
				dest.x *= src.x;
				break;
			case Y:
				dest.y *= src.x;
				break;
			case Z:
				dest.z *= src.x;
				break;
		}

		switch(m_vAxis)
		{
			case X:
				dest.x *= src.y;
				break;
			case Y:
				dest.y *= src.y;
				break;
			case Z:
				dest.z *= src.y;
				break;
		}
	}

	/**
	Converts a 3D point to a 2D point in the plane specified by the axes, by selecting the
	relevant components of the 3D point. In this case, what I mean by the X-Y plane (for
	example), is the plane z = 0 (or, for that matter, z = anything). This is essentially
	doing an orthographic projection.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (Z,Y), p = (3,4,5) -> result = (5,4)
	</dl>

	@param p	The 3D point to convert to 2D
	@return		The 2D point obtained by projecting the 3D point onto the 2D axes
	*/
	public Vector2d select_components(final Vector3d p)
	{
		Vector2d ret = new Vector2d();

		switch(m_hAxis)
		{
			case X:
				ret.x = p.x;
				break;
			case Y:
				ret.x = p.y;
				break;
			case Z:
				ret.x = p.z;
				break;
		}

		switch(m_vAxis)
		{
			case X:
				ret.y = p.x;
				break;
			case Y:
				ret.y = p.y;
				break;
			case Z:
				ret.y = p.z;
				break;
		}

		return ret;
	}

	/**
	Sets the component of the 3D point which is on the missing axis.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Z), p = (3,?,5), missingComponent = 4 -> result = (3,4,5)
	<dd><i>In this, the ? means that the number isn't relevant, since it will be overwritten.</i>
	</dl>

	@param p				The 3D point
	@param missingComponent	The value to which to set the component on the missing axis
	*/
	public void set_missing_component(Vector3d p, double missingComponent)
	{
		switch(m_mAxis)
		{
			case X:
				p.x = missingComponent;
				break;
			case Y:
				p.y = missingComponent;
				break;
			case Z:
				p.z = missingComponent;
				break;
		}
	}

	/**
	Sets the components in the 3D point which correspond to those of the 2D point.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Z), dest = (?,4,?), src = (3,6) -> result = (3,4,6)
	<dd><i>In this, the ? means that the number isn't relevant, since it will be overwritten.</i>
	</dl>

	@param dest	The 3D point whose components we wish to set
	@param src	The 2D point
	*/
	public void set_relevant_components(Vector3d dest, final Vector2d src)
	{
		switch(m_hAxis)
		{
			case X:
				dest.x = src.x;
				break;
			case Y:
				dest.y = src.x;
				break;
			case Z:
				dest.z = src.x;
				break;
		}

		switch(m_vAxis)
		{
			case X:
				dest.x = src.y;
				break;
			case Y:
				dest.y = src.y;
				break;
			case Z:
				dest.z = src.y;
				break;
		}
	}

	/**
	Given a 2D offset (in level coordinates), figure out which of the components of the
	3D point p its components correspond to, and subtract the offset accordingly.
	<p><b>Example:</b>
	<dl>
	<dd>The axis-pair (X,Y), p = (1,2,3), offset = (4,5) -> result = (-3,-3,3).
	</dl>

	@param p		The 3D point from which to subtract the offset
	@param offset	The 2D offset to subtract from it
	*/
	public void subtract_offset(Vector3d p, final Vector2d offset)
	{
		add_offset(p, new Vector2d(-offset.x, -offset.y));
	}
}
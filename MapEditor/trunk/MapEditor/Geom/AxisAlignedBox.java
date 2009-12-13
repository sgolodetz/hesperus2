package MapEditor.Geom;

import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Constants;

public class AxisAlignedBox implements Cloneable, Constants
{
	//################## PUBLIC NESTED CLASSES ##################//
	public class Projection
	{
		// Datatype Invariant: m_corners[0].c <= m_corners[1].c for all c <- {x,y}
		public Vector2d[] m_corners = new Vector2d[2];
	}

	//################## PRIVATE VARIABLES ##################//
	// Datatype Invariant: m_corners[0].c <= m_corners[1].c for all c <- {x,y,z}
	private Vector3d[] m_corners = new Vector3d[2];

	//################## CONSTRUCTORS ##################//
	public AxisAlignedBox(Vector3d corner0, Vector3d corner1)
	{
		m_corners[0] = corner0;
		m_corners[1] = corner1;
		reestablish_invariant();
	}

	//################## PUBLIC METHODS ##################//
	/**
	Calculates the centre of the box.

	@return	The centre of the box, as a Point3d
	*/
	public Vector3d centre()
	{
		Vector3d c = m_corners[0].clone();
		c.add(m_corners[1]);
		c.scale(0.5);
		return c;
	}

	/**
	Returns a (deep) copy of the box whose bounds are distinct in memory from those of the original.

	@return	...think about it...
	*/
	public AxisAlignedBox clone()
	{
		return new AxisAlignedBox(m_corners[0].clone(), m_corners[1].clone());
	}

	/**
	Determines whether or not this box completely contains another specified box. Note that
	we're not checking for strict containment, i.e. the faces of the boxes can touch.

	@param other	The box whose containment by this one we want to check
	@return			true, if this box does completely enclose the other, or false otherwise
	*/
	public boolean completely_contains(AxisAlignedBox other)
	{
		if(m_corners[0].x - EPSILON <= other.m_corners[0].x && other.m_corners[1].x <= m_corners[1].x + EPSILON &&
		   m_corners[0].y - EPSILON <= other.m_corners[0].y && other.m_corners[1].y <= m_corners[1].y + EPSILON &&
		   m_corners[0].z - EPSILON <= other.m_corners[0].z && other.m_corners[1].z <= m_corners[1].z + EPSILON)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	public Vector3d[] get_bounds()
	{
		return m_corners;
	}

	public Projection project_to_2D_using(AxisPair ap)
	{
		Projection p = new Projection();
		for(int i=0; i<2; ++i) p.m_corners[i] = ap.select_components(m_corners[i]);
		return p;
	}

	public void resize(Vector2d corner0, Vector2d corner1, AxisPair ap)
	{
		ap.set_relevant_components(m_corners[0], corner0);
		ap.set_relevant_components(m_corners[1], corner1);
		reestablish_invariant();
	}

	public void resize(Vector3d corner0, Vector3d corner1)
	{
		m_corners[0] = corner0;
		m_corners[1] = corner1;
		reestablish_invariant();
	}

	/**
	Translates the box by the specified 3D vector.

	@param trans	The 3D vector by which to translate
	*/
	public void translate(Vector3d trans)
	{
		for(Vector3d c: m_corners) c.add(trans);
	}

	//################## PRIVATE METHODS ##################//
	private void reestablish_invariant()
	{
		if(m_corners[0].x > m_corners[1].x)
		{
			double t = m_corners[0].x;
			m_corners[0].x = m_corners[1].x;
			m_corners[1].x = t;
		}
		if(m_corners[0].y > m_corners[1].y)
		{
			double t = m_corners[0].y;
			m_corners[0].y = m_corners[1].y;
			m_corners[1].y = t;
		}
		if(m_corners[0].z > m_corners[1].z)
		{
			double t = m_corners[0].z;
			m_corners[0].z = m_corners[1].z;
			m_corners[1].z = t;
		}
	}
}
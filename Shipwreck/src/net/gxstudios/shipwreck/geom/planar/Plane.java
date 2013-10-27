package net.gxstudios.shipwreck.geom.planar;

import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.misc.Constants;

/**
This class represents planes in three-dimensional space.
*/
public class Plane implements Constants
{
	// Plane Equation: m_normal . (x,y,z) - m_d = 0
	// Datatype Invariant: |m_normal| = 1
	private Vector3d m_normal;
	private double m_d;

	/**
	Constructs a plane from the specified normal and distance value.

	@param normal			The plane normal
	@param d				The plane distance value (the value of n . x where
							n is the plane normal and x is any point in the plane)
	@throws java.lang.Error	If the length of the normal is 0
	*/
	public Plane(Vector3d normal, double d)
	{
		m_normal = normal;
		m_d = d;

		// Establish the datatype invariant.
		double length = normal.length();
		if(length == 0) throw new java.lang.Error();
		m_normal.scale(1/length);
		m_d /= length;
	}

	/**
	Returns the (perpendicular) displacement from the plane to the specified point.
	The result is signed, being positive when the point is in front of the plane
	and negative when it is behind the plane. (Obviously when it's on the plane,
	it's zero).

	@param p	The point whose displacement from the plane we want to determine
	@return		The (perpendicular) displacement from the plane to the point
	*/
	public double displacement_to_point(final Vector3d p)
	{
		// Note: We have a unit normal, so we don't need to normalise this.
		return m_normal.dot(p) - m_d;
	}

	/**
	Returns the plane's distance value (the d in the plane equation).

	@return		...think about it...
	*/
	public double get_distance_value()
	{
		return m_d;
	}

	/**
	Returns the plane's normal.

	@return		...think about it...
	*/
	public Vector3d get_normal()
	{
		return m_normal;
	}

	/**
	Returns the negation of the plane (the same plane, but with its normal negated).

	@return		...think about it...
	*/
	public Plane negate()
	{
		Vector3d negatedNormal = m_normal.clone();
		negatedNormal.negate();
		return new Plane(negatedNormal, -m_d);
	}

	/**
	Reflects the specified point across the plane.

	@param p	The point to reflect
	*/
	public void reflect_point(Vector3d p)
	{
		double disp = displacement_to_point(p);

		Vector3d scaledNormal = m_normal.clone();
		scaledNormal.scale(-2*disp);

		// Now |scaledNormal| = 2*disp, since |m_normal| = 1. It points towards the plane, so
		// adding it to our original point will reflect the latter across the plane.
		p.add(scaledNormal);
	}

	/**
	Returns a plane representing the plane moved along its normal by the specified displacement.
	If displacement > 0, this moves it in the direction of the normal, if displacement < 0, it
	moves it in the opposite direction. (Clearly if displacement == 0, it doesn't move it at all.)

	@param displacement	The amount by which to displace the plane
	@return				A displaced plane as indicated
	*/
	public Plane translate_along_normal(double displacement)
	{
		return new Plane(m_normal, m_d + displacement);
	}

	public String toString()
	{
		return "(" +	String.valueOf(m_normal.x) + ", " +
						String.valueOf(m_normal.y) + ", " +
						String.valueOf(m_normal.z) + ", " +
						String.valueOf(m_d) + ")";
	}
}
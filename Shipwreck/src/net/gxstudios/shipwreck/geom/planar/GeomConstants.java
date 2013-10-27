package net.gxstudios.shipwreck.geom.planar;

/**
This interface defines global geometric constants. Classes which want to import the
constants, i.e. use unqualified names like CP_BACK instead of GeomConstants.CP_BACK,
simply have to declare themselves as implementing the GeomConstants interface.
Whether it's a good idea to use the unqualified names is a design decision, of course.
*/
public interface GeomConstants
{
	/** Classification relative to a plane: The point or polygon is behind the plane */
	final public static int	CP_BACK = 0;
	/** Classification relative to a plane: The point or polygon is on the plane */
	final public static int CP_COPLANAR = 1;
	/** Classification relative to a plane: The point or polygon is in front of the plane */
	final public static int CP_FRONT = 2;
	/** Classification relative to a plane: The polygon straddles the plane (part of it lies on each side) */
	final public static int CP_STRADDLE = 3;

	/** Point membership classification: The point is (strictly) inside the entity in question */
	final public static int PMC_INSIDE = 0;
	/** Point membership classification: The point is (strictly) outside the entity in question */
	final public static int PMC_OUTSIDE = 1;
	/** Point membership classification: The point is on the surface (/edge) of the entity in question */
	final public static int PMC_SURFACE = 2;
}
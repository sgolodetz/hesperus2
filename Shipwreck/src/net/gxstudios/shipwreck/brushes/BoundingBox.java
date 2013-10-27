package net.gxstudios.shipwreck.brushes;

import net.gxstudios.shipwreck.geom.AxisAlignedBox;
import net.gxstudios.shipwreck.geom.AxisPair;
import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;

class BoundingBox extends AxisAlignedBox implements BrushConstants
{
	//################## CONSTRUCTORS ##################//
	/**
	Constructs a BoundingBox from two (extreme) corners specifying it.

	@param corner0	One extreme corner
	@param corner1	The other extreme corner
	*/
	public BoundingBox(Vector3d corner0, Vector3d corner1)
	{
		super(corner0, corner1);
	}

	/**
	Constructs a BoundingBox from an AxisAlignedBox.

	@param rhs	The AxisAlignedBox from which to construct our bounding box
	*/
	public BoundingBox(AxisAlignedBox rhs)
	{
		this(rhs.get_bounds()[0].clone(), rhs.get_bounds()[1].clone());
	}

	//################## PUBLIC METHODS ##################//
	public BoundingBox clone()
	{
		return new BoundingBox(get_bounds()[0].clone(), get_bounds()[1].clone());
	}

	/**
	Finds the corner handle with the smallest "Manhattan" distance from the specified point when
	the bounding box is projected onto the specified pair of axes.

	@param ap	The axes onto which to project the brush's bounding box
	@param p	The point for which to find the nearest corner handle
	@return		The nearest corner handle
	*/
	public Vector2d find_manhattan_nearest_corner_handle(AxisPair ap, Vector2d p)
	{
		Vector2d ret = null;
		double smallestDistance = Double.MAX_VALUE;
		for(int i=0; i<4; ++i)
		{
			Vector2d cornerHandle = get_corner_handle(ap, i);
			double distance = Math.abs(p.x - cornerHandle.x) + Math.abs(p.y - cornerHandle.y);
			if(distance < smallestDistance)
			{
				smallestDistance = distance;
				ret = cornerHandle;
			}
		}
		return ret;
	}

	/**
	Gets the location of the i'th corner handle when the bounding box is projected onto the
	specified pair of axes.

	@param ap	The axes onto which to project the brush's bounding box
	@param i	The corner handle index, in the range [0..4)
	@return		...think about it...
	@throws java.lang.Error	If the corner handle index is out-of-range
	*/
	final public Vector2d get_corner_handle(AxisPair ap, int i)
	{
		AxisAlignedBox.Projection proj = project_to_2D_using(ap);

		switch(i)
		{
			case 0:		return proj.m_corners[0];
			case 1:		return new Vector2d(proj.m_corners[1].x, proj.m_corners[0].y);
			case 2:		return proj.m_corners[1];
			case 3:		return new Vector2d(proj.m_corners[0].x, proj.m_corners[1].y);
			default:	throw new java.lang.Error();
		}
	}

	/**
	Gets the location of the i'th edge handle when the bounding box is projected onto the
	specified pair of axes.

	@param ap	The axes onto which to project the brush's bounding box
	@param i	The edge handle index, in the range [0..4)
	@return		...think about it...
	@throws java.lang.Error	If the edge handle index is out-of-range
	*/
	final public Vector2d get_edge_handle(AxisPair ap, int i)
	{
		AxisAlignedBox.Projection proj = project_to_2D_using(ap);

		double midX = (proj.m_corners[0].x + proj.m_corners[1].x) / 2;
		double midY = (proj.m_corners[0].y + proj.m_corners[1].y) / 2;

		switch(i)
		{
			case 0:		return new Vector2d(midX, proj.m_corners[0].y);
			case 1:		return new Vector2d(proj.m_corners[1].x, midY);
			case 2:		return new Vector2d(midX, proj.m_corners[1].y);
			case 3:		return new Vector2d(proj.m_corners[0].x, midY);
			default:	throw new java.lang.Error();
		}
	}

	/**
	Gets the location of the corner handle opposite the i'th when the bounding box is projected onto
	the specified pair of axes. (See get_corner_handle for parameter details.)
	*/
	final public Vector2d get_opposite_corner_handle(AxisPair ap, int i)
	{
		return get_corner_handle(ap, (i+2)%4);
	}

	/**
	Gets the location of the edge handle opposite the i'th when the bounding box is projected onto
	the specified pair of axes. (See get_edge_handle for parameter details.)
	*/
	final public Vector2d get_opposite_edge_handle(AxisPair ap, int i)
	{
		return get_edge_handle(ap, (i+2)%4);
	}

	/**
	Returns whether a point is within the projection of this brush's bounding box
	onto a specified pair of axes, to a default level of tolerance (HANDLE_SIZE/4).
	An example of the "overloaded-method for default parameter" idiom.

	@param p		The point in question
	@param ap		The axes onto which to project the brush's bounding box
	@return			true, if the point is within the projection, or false otherwise
	*/
	public boolean within_2D_bounds(Vector2d p, AxisPair ap)
	{
		return within_2D_bounds(p, ap, HANDLE_SIZE/4);
	}

	//################## PRIVATE METHODS ##################//
	/**
	Returns whether a point is within the projection of this brush's bounding box
	onto a specified pair of axes, to a specified level of tolerance.

	@param p			The point in question
	@param ap			The axes onto which to project the brush's bounding box
	@param tolerance	The maximum distance outside the box we can be clicking without deselecting the brush
	@return				true, if the point is within the projection, or false otherwise
	*/
	private boolean within_2D_bounds(Vector2d p, AxisPair ap, double tolerance)
	{
		AxisAlignedBox.Projection proj = project_to_2D_using(ap);

		// Note that the invariant of AxisAlignedBox.Projection makes this correct.
		if(proj.m_corners[0].x-tolerance <= p.x && p.x <= proj.m_corners[1].x+tolerance &&
		   proj.m_corners[0].y-tolerance <= p.y && p.y <= proj.m_corners[1].y+tolerance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
package MapEditor.BSP;

import MapEditor.Geom.Planar.*;
import MapEditor.Math.Vectors.VectorUtil;
import MapEditor.Misc.Pair;

public abstract class PolygonClipFunctor implements IClipFunctor<Polygon>
{
	//################## PRIVATE VARIABLES ##################//
	private static PolygonClipFunctor s_noCoplanars = null;

	//################## PUBLIC METHODS ##################//
	public int classify_against_plane(final Polygon poly, final Plane plane)
	{
		return GeomUtil.classify_polygon_against_plane(poly, plane);
	}

	/**
	Returns a complex PolygonClipFunctor with the specified behaviour.

	<p><b>Possible Behaviours:</b>
	<ol>
	<li>Intersection #1: Same direction - Pass Back, Opposite Direction - Pass Front
	<li>Intersection #2: Same direction - Pass Front, Opposite Direction - Pass Front
	<li>Union #1: Same direction - Pass Back, Opposite Direction - Pass Back
	<li>Union #2: Same direction - Pass Front, Opposite Direction - Pass Back
	</ol>

	@param bSamePassBack		Pass polygons which are facing the same direction as the plane down the back of the tree?
	@param bOppositePassBack	Pass polygons which are facing the opposite direction to the plane down the back of the tree?
	@return						A PolygonClipFunctor as specified
	*/
	public static PolygonClipFunctor complex(final boolean bSamePassBack, final boolean bOppositePassBack)
	{
		return new PolygonClipFunctor()
		{
			public boolean pass_coplanar_back(final Polygon poly, final Plane plane)
			{
				if(opposite_facing(poly, plane)) return bOppositePassBack;
				else return bSamePassBack;
			}
		};
	}

	/**
	Returns a simple PolygonClipFunctor which isn't expected to deal with coplanar polygons.

	@return	A PolygonClipFunctor as specified
	*/
	public static PolygonClipFunctor no_coplanars()
	{
		if(s_noCoplanars == null)
		{
			s_noCoplanars = new PolygonClipFunctor()
			{
				public boolean pass_coplanar_back(final Polygon poly, final Plane plane)
				{
					// This clip functor isn't expected to deal with coplanar polygons - if it gets asked to, throw an error.
					throw new java.lang.Error();
				}
			};
		}
		return s_noCoplanars;
	}

	public Pair<Polygon,Polygon> split_with_plane(final Polygon poly, final Plane plane)
	{
		return GeomUtil.split_polygon(poly, plane);
	}

	//################## PROTECTED METHODS ##################//
	protected boolean opposite_facing(final Polygon poly, final Plane plane)
	{
		return VectorUtil.angle_between(poly.get_normal(), plane.get_normal()) > Math.PI/2;
	}
}
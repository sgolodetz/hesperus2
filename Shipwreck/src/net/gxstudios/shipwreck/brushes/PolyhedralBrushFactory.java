package net.gxstudios.shipwreck.brushes;

import net.gxstudios.shipwreck.geom.AxisAlignedBox;
import net.gxstudios.shipwreck.geom.AxisPair;
import net.gxstudios.shipwreck.geom.planar.Polygon;
import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;

/**
This class contains factory methods for creating polyhedral brushes (as its name suggests).
*/
public class PolyhedralBrushFactory
{
	//################## PUBLIC METHODS ##################//
	/**
	Returns a PolyhedralBrush representing a cuboid block (which is initially axis-aligned) whose
	axis-aligned bounding box is the specified bounds.

	@param bounds	The new block's bounding box
	@return			A PolyhedralBrush representing the block
	*/
	public static PolyhedralBrush create_axis_aligned_block(AxisAlignedBox bounds)
	{
		Vector3d[] v = new Vector3d[] {	new Vector3d(0,0,0),	// v[0]
										new Vector3d(0,0,1),	// v[1]
										new Vector3d(0,1,0),	// v[2]
										new Vector3d(0,1,1),	// v[3]
										new Vector3d(1,0,0),	// v[4]
										new Vector3d(1,0,1),	// v[5]
										new Vector3d(1,1,0),	// v[6]
										new Vector3d(1,1,1),	// v[7]
									};

		Polygon[] polys = new Polygon[6];

		// Note: The winding order is anti-clockwise. We're working in a right-handed coordinate system
		// with z pointing upwards (well, we think of it in those terms, anyway; in theory, "upwards" is
		// just a name we give it).
		// (Imagine x pointing right, y pointing into the screen and z pointing up).
		polys[0] = make_polygon(new Vector3d[] {v[5],v[1],v[0],v[4]});	// front
		polys[1] = make_polygon(new Vector3d[] {v[7],v[5],v[4],v[6]});	// right
		polys[2] = make_polygon(new Vector3d[] {v[3],v[7],v[6],v[2]});	// back
		polys[3] = make_polygon(new Vector3d[] {v[1],v[3],v[2],v[0]});	// left
		polys[4] = make_polygon(new Vector3d[] {v[1],v[5],v[7],v[3]});	// top
		polys[5] = make_polygon(new Vector3d[] {v[4],v[0],v[2],v[6]});	// bottom

		return PolyhedralBrush.create_from_unit_template(polys, bounds);
	}

	/**
	Returns a PolyhedralBrush representing a cone whose axis-aligned bounding box is the
	specified bounds. The cone will have the specified number of sides, and its main axis
	(i.e. the line through its apex and the centre of its bottom face) will be perpendicular
	to the plane specified by ap.

	@param bounds		The new cone's bounding box
	@param sideCount	The number of sides the cone is to have
	@param ap			The axis-pair of the canvas on which we're dragging to create the cone
	@return				A PolyhedralBrush representing the cone
	*/
	public static PolyhedralBrush create_cone(AxisAlignedBox bounds, int sideCount, AxisPair ap)
	{
		Vector2d[] v2D = new Vector2d[sideCount];
		Vector3d[] v = new Vector3d[sideCount];
		for(int i=0; i<sideCount; ++i)
		{
			double angle = (2*Math.PI*i)/sideCount;
			// -1 <= cos x or sin x <= 1 -> 0 <= (cos x or sin x)/2 + 0.5 <= 1
			v2D[i] = new Vector2d(Math.cos(angle)/2 + 0.5, Math.sin(angle)/2 + 0.5);

			v[i] = ap.generate_3D_point(v2D[i], 0);
		}

		Vector3d top = ap.generate_3D_point(new Vector2d(0.5, 0.5), 1);

		Polygon[] polys = new Polygon[sideCount+1];	// the side faces, plus the bottom

		// Side faces
		for(int i=0; i<sideCount; ++i)
		{
			int j = (i+1)%sideCount;
			polys[i] = make_polygon(new Vector3d[] {v[j],top,v[i]});
		}

		// Bottom
		Vector3d[] bottom = new Vector3d[sideCount];
		for(int i=0; i<sideCount; ++i) bottom[i] = v[sideCount-1-i].clone();
		polys[sideCount] = make_polygon(bottom);

		// Flip the polygon winding orders if our AxisPair was not a natural one.
		if(!ap.natural_pair())
		{
			for(Polygon p: polys) p.flip_winding();
		}

		return PolyhedralBrush.create_from_unit_template(polys, bounds);
	}

	/**
	Returns a PolyhedralBrush representing a cylinder whose axis-aligned bounding box is the
	specified bounds. The cylinder will have the specified number of sides, and its main axis
	(i.e. the line through the centre of its top and bottom faces) will be perpendicular to
	the plane specified by ap. (In other words, when viewed on a canvas whose axis-pair is
	ap, it will appear circular, since we're viewing it end-on.)

	@param bounds		The new cylinder's bounding box
	@param sideCount	The number of sides the cylinder is to have
	@param ap			The axis-pair of the canvas on which we're dragging to create the cylinder
	@return				A PolyhedralBrush representing the cylinder
	*/
	public static PolyhedralBrush create_cylinder(AxisAlignedBox bounds, int sideCount, AxisPair ap)
	{
		Vector2d[] v2D = new Vector2d[sideCount];
		Vector3d[] v = new Vector3d[sideCount*2];

		for(int i=0; i<sideCount; ++i)
		{
			double angle = (2*Math.PI*i)/sideCount;
			// -1 <= cos x or sin x <= 1 -> 0 <= (cos x or sin x)/2 + 0.5 <= 1
			v2D[i] = new Vector2d(Math.cos(angle)/2 + 0.5, Math.sin(angle)/2 + 0.5);

			v[i] = ap.generate_3D_point(v2D[i], 0);
			v[i+sideCount] = ap.generate_3D_point(v2D[i], 1);
		}

		Polygon[] polys = new Polygon[sideCount+2];		// the side faces, plus the top and bottom

		// Side faces
		for(int i=0; i<sideCount; ++i)
		{
			int j = (i+1)%sideCount;
			polys[i] = make_polygon(new Vector3d[] {v[j+sideCount],v[i+sideCount],v[i],v[j]});
		}

		// Top and bottom
		polys[sideCount] = make_polygon(v, sideCount, sideCount);	// top
		Vector3d[] bottom = new Vector3d[sideCount];
		for(int i=0; i<sideCount; ++i) bottom[i] = v[sideCount-1-i].clone();
		polys[sideCount+1] = make_polygon(bottom);			// bottom

		// Flip the polygon winding orders if our AxisPair was not a natural one.
		if(!ap.natural_pair())
		{
			for(Polygon p: polys) p.flip_winding();
		}

		return PolyhedralBrush.create_from_unit_template(polys, bounds);
	}

	/**
	Returns a PolyhedralBrush representing a UV sphere whose axis-aligned bounding box is the
	specified bounds.

	@param bounds		The new sphere's bounding box
	@param latitudes	The number of lines of latitude on this sphere (note: excludes the poles)
						This is one of two variables we alter to vary how spherical our polygonal sphere is
						Note: This should be an odd number, because the sphere is symmetrical, but we have
						a line of latitude at the equator as well
	@param longitudes	The number of lines of longitude on this sphere
						This is the other variable we alter
	@param ap			The axis-pair of the canvas on which we're dragging to create the sphere
	@return				A PolyhedralBrush representing the sphere
	*/
	public static PolyhedralBrush create_uv_sphere(AxisAlignedBox bounds, int latitudes, int longitudes, AxisPair ap)
	{
		// latitudes must be an odd number, as noted above
		if(latitudes % 2 == 0) throw new java.lang.Error();

		// The 2D array stores the vertices for the upper hemisphere (excluding the north pole),
		// since the lower hemisphere is identical to it except for being upside-down.
		// Note that we don't translate the upper hemisphere into its correct position yet,
		// as that would make doing the lower hemisphere harder later on.
		// To see where the (latitudes+1)/2 comes from, consider a sphere with latitudes == 3.
		// Then the upper hemisphere has (3+1)/2 = 2 latitudes : the one above the equator and the
		// equator itself.
		int hemiLatitudes = (latitudes+1)/2;
		Vector3d[][] v = new Vector3d[hemiLatitudes][longitudes];

		for(int i=0; i<hemiLatitudes; ++i)
		{
			// 0 <= vAngle <= PI/2
			double vAngle = (Math.PI/2)*((double)i/hemiLatitudes);

			// 0 <= sin(vAngle) <= 1 -> 0 <= height <= 0.5
			double height = Math.sin(vAngle)/2;

			// 0 <= radius <= 1
			double radius = Math.cos(vAngle);

			for(int j=0; j<longitudes; ++j)
			{
				double angle = (2*Math.PI*j)/longitudes;

				Vector2d[] v2D = new Vector2d[longitudes];

				v2D[j] = new Vector2d(radius*Math.cos(angle)/2 + 0.5,
						     radius*Math.sin(angle)/2 + 0.5);

				v[i][j] = ap.generate_3D_point(v2D[j], height);
			}
		}

		Polygon[] polys = new Polygon[(latitudes+1)*longitudes];		// (latitudes + 1) layers, each with longitudes faces

		// Upper hemisphere (all except the top layer, we have to do that separately)
		for(int i=0; i<hemiLatitudes-1; ++i)
		{
			for(int j=0; j<longitudes; ++j)
			{
				int k = (j+1)%longitudes;
				polys[i*longitudes+j] =
					make_polygon(new Vector3d[] {v[i+1][k], v[i+1][j], v[i][j], v[i][k]});
			}
		}

		// The top layer of the upper hemisphere
		// Note: The north pole will get translated into the right place later on, when
		// everything else does.
		Vector3d northPole = ap.generate_3D_point(new Vector2d(0.5, 0.5), 0.5);
		for(int j=0; j<longitudes; ++j)
		{
			int i = hemiLatitudes-1;
			int k = (j+1)%longitudes;
			polys[i*longitudes+j] = make_polygon(new Vector3d[] {northPole, v[i][j], v[i][k]});
		}

		// Lower hemisphere (all except the bottom layer, we have to do that separately)
		for(int i=0; i<hemiLatitudes-1; ++i)
		{
			for(int j=0; j<longitudes; ++j)
			{
				int k = (j+1)%longitudes;
				Vector3d[] verts = new Vector3d[4];
				verts[0] = v[i+1][j].clone();
				verts[1] = v[i+1][k].clone();
				verts[2] = v[i][k].clone();
				verts[3] = v[i][j].clone();
				for(Vector3d vert: verts)
					ap.set_missing_component(vert, ap.get_missing_component(vert) * -1);
				polys[(i+hemiLatitudes)*longitudes+j] = new Polygon(verts);
			}
		}

		// The bottom layer of the lower hemisphere
		for(int j=0; j<longitudes; ++j)
		{
			int i = hemiLatitudes-1;
			int k = (j+1)%longitudes;
			Vector3d[] verts = new Vector3d[3];
			verts[0] = northPole.clone();
			verts[1] = v[i][k].clone();
			verts[2] = v[i][j].clone();
			for(Vector3d vert: verts)
				ap.set_missing_component(vert, ap.get_missing_component(vert) * -1);
			polys[(i+hemiLatitudes)*longitudes+j] = new Polygon(verts);
		}

		// Translate the sphere up by 0.5, so that it's in the correct position.
		for(int i=0; i<(latitudes+1)*longitudes; ++i)
		{
			for(Vector3d vert: polys[i].get_vertices())
				ap.set_missing_component(vert, ap.get_missing_component(vert) + 0.5);
		}

		// Flip the polygon winding orders if our AxisPair was not a natural one.
		if(!ap.natural_pair())
		{
			for(Polygon p: polys) p.flip_winding();
		}

		return PolyhedralBrush.create_from_unit_template(polys, bounds);
	}

	//################## PRIVATE METHODS ##################//
	/**
	Make a polygon whose vertices are a copy of the parameter vertices.

	@param vertices	The vertices from which to make the polygon
	@return			A polygon whose vertices are as specified
	*/
	private static Polygon make_polygon(Vector3d[] vertices)
	{
		return make_polygon(vertices, 0, vertices.length);
	}

	/**
	Make a polygon whose vertices are a copy of vertices[offset..offset+len). (In other words,
	the vertices of the newly-created polygon are distinct in memory from the elements of the
	parameter vertices.)

	@param vertices		A vertex array containing the vertices from which to make the polygon
	@param offset		The first vertex in the array which will be part of the polygon
	@param len			The number of vertices in the array which will be part of the polygon
	@return				A polygon whose vertices are as specified above
	*/
	private static Polygon make_polygon(Vector3d[] vertices, int offset, int len)
	{
		// We need to ensure that the vertices aren't shared, so we make copies of all of them.

		Vector3d[] newVertices = new Vector3d[len];
		for(int i=0; i<len; ++i) newVertices[i] = vertices[i+offset].clone();
		return new Polygon(newVertices);
	}
}
package net.gxstudios.shipwreck.brushes;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.LinkedList;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.glu.GLU;

import net.gxstudios.shipwreck.bsp.PolygonClipFunctor;
import net.gxstudios.shipwreck.bsp.Tree;
import net.gxstudios.shipwreck.bsp.TreeUtil;
import net.gxstudios.shipwreck.commands.Command;
import net.gxstudios.shipwreck.commands.CommandManager;
import net.gxstudios.shipwreck.geom.AxisAlignedBox;
import net.gxstudios.shipwreck.geom.AxisPair;
import net.gxstudios.shipwreck.geom.planar.GeomConstants;
import net.gxstudios.shipwreck.geom.planar.GeomUtil;
import net.gxstudios.shipwreck.geom.planar.Plane;
import net.gxstudios.shipwreck.geom.planar.Polygon;
import net.gxstudios.shipwreck.graphics.ColourCycle;
import net.gxstudios.shipwreck.graphics.IRenderer;
import net.gxstudios.shipwreck.math.MathUtil;
import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.math.vectors.VectorUtil;
import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.misc.MiscUtil;
import net.gxstudios.shipwreck.misc.Options;
import net.gxstudios.shipwreck.misc.Pair;
import net.gxstudios.shipwreck.textures.Texture;
import net.gxstudios.shipwreck.textures.TextureCoord;
import net.gxstudios.shipwreck.textures.TextureManager;
import net.gxstudios.shipwreck.textures.TexturePlane;

/**
This class represents convex polyhedral brushes, in other words brushes consisting of convex faces which
completely enclose a convex volume.
*/
public class PolyhedralBrush extends ArchitectureBrush implements Constants, GeomConstants, Iterable<Polygon>
{
	//################## PRIVATE VARIABLES ##################//
	private static ColourCycle COLOUR_CYCLE = new ColourCycle();

	private boolean m_ghost;					// are we in the process of creating this brush in the design canvas?
	private Color m_colour;						// the colour with which we will render the brush on a design canvas
	private Polygon[] m_cachedPolys;			// used to cache the polygons at the start of a transformation
	private Polygon[] m_polys;
	private Properties m_properties;

	//################## CONSTRUCTORS ##################//
	// Private to prevent direct instantiation.
	private PolyhedralBrush(boolean ghost, boolean isNew)
	{
		super(isNew);
		m_ghost = ghost;
		m_colour = COLOUR_CYCLE.next_colour();
		m_properties = new Properties();
	}

	// Private to prevent direct instantiation.
	private PolyhedralBrush(Iterable<Polygon> polys, int polyCount, boolean ghost, boolean isNew, Properties properties)
	{
		this(ghost, isNew);

		m_polys = new Polygon[polyCount];
		int i = 0;
		for(Polygon poly: polys)
		{
			m_polys[i] = poly.clone();
			++i;
		}

		m_boundingBox = new BoundingBox(new Vector3d(0,0,0), new Vector3d(1,1,1));
		recalculate_bounding_box();

		m_properties = properties;
	}

	public PolyhedralBrush(LinkedList<Polygon> polys)
	{
		this(polys, polys.size(), false, false, new Properties(BrushFunction.NORMAL));
	}

	//################## PUBLIC METHODS ##################//
	public int classify_against_plane(final Plane plane)
	{
		boolean backFlag = false, frontFlag = false;

		for(Polygon poly: m_polys)
		{
			switch(GeomUtil.classify_polygon_against_plane(poly, plane))
			{
				case CP_BACK:
				{
					if(frontFlag) return CP_STRADDLE;
					else backFlag = true;
					break;
				}
				case CP_COPLANAR:
				{
					// No effect.
					break;
				}
				case CP_FRONT:
				{
					if(backFlag) return CP_STRADDLE;
					else frontFlag = true;
					break;
				}
				case CP_STRADDLE:
				{
					return CP_STRADDLE;
				}
			}
		}

		if(backFlag) return CP_BACK;
		else if(frontFlag) return CP_FRONT;
		else return CP_COPLANAR;
	}

	public LinkedList<ArchitectureBrush> component_brushes()
	{
		return MiscUtil.make_singleton_list((ArchitectureBrush)this);
	}

	public PolyhedralBrush copy()
	{
		final int polyCount = m_polys.length;

		PolyhedralBrush ret = new PolyhedralBrush(false, false);
		ret.m_polys = new Polygon[polyCount];
		for(int i=0; i<polyCount; ++i) ret.m_polys[i] = m_polys[i].clone();
		ret.m_boundingBox = m_boundingBox.clone();
		ret.m_properties = m_properties.clone();
		return ret;
	}

	public void deghost(final IBrushContainer container)
	{
		if(m_ghost)
		{
			// This Command must be here rather than in the BrushCommands class because
			// we're referencing PolyhedralBrush's private variable m_ghost, which isn't
			// accessible externally.
			CommandManager.instance().execute_compressible_command(new Command("Deghost")
			{
				public void execute()
				{
					// We've finished creating this brush.
					m_ghost = false;
					container.add_brush(PolyhedralBrush.this);
				}

				public void undo()
				{
					m_ghost = true;

					// Note that the selected brush is always PolyhedralBrush.this, since
					// at the point at which we call undo(), we're in the state resulting
					// from calling execute().
					container.ghost_selection();
				}
			},
			Pair.make_pair("Initial Brush Creation", "Initial Brush Creation"));
		}
	}

	public BrushDetails details()
	{
		return new BrushDetails(m_polys.length);
	}

	public LinkedList<Polygon> faces()
	{
		// TODO: We could make this more efficient by creating a LinkedList of the polygons directly.
		return MiscUtil.list_from_iterable(this);
	}

	public boolean is_copyable()
	{
		return !is_ghost();
	}

	public boolean is_ghost()
	{
		return m_ghost;
	}

	public Iterator<Polygon> iterator()
	{
		return new Iterator<Polygon>()
		{
			private int m_index = 0;

			public boolean hasNext()
			{
				return m_index < m_polys.length;
			}

			public Polygon next()
			{
				return m_polys[m_index++];
			}

			public void remove()
			{
				throw new UnsupportedOperationException();
			}
		};
	}

	public PickResults pick(final Vector3d start, final Vector3d direction)
	{
		// Ensure that hint brushes can't be picked if they're currently invisible.
		if(m_properties.get_function() == BrushFunction.HINT && !Options.is_set("Render Hint Brushes")) return null;

		Polygon nearestFace = null;
		Vector3d nearestPoint = null;
		double nearestDistanceSquared = Double.MAX_VALUE;
		for(Polygon p: m_polys)
		{
			Plane plane = GeomUtil.make_plane(p);
			if(plane.get_normal().dot(direction) == 0) continue;							// ignore a polygon if its plane is parallel to the direction vector
			if(GeomUtil.classify_point_against_plane(start, plane) != CP_FRONT) continue;	// ignore polygons if we're behind them - the user clearly didn't
																							// intend to click a polygon they couldn't see due to back-face culling!
			Pair<Vector3d,Double> intersection = GeomUtil.determine_line_intersection_with_plane(start, direction, plane);
			if(intersection.second > 1)		// the GeomUtil function deals with a whole ray, we're only interested in the forward half-ray
											// note that this means the half-ray starting at the image plane, i.e. t > 1
			{
				double distanceSquared = start.distance_squared(intersection.first);
				if(distanceSquared < nearestDistanceSquared && GeomUtil.point_in_convex_polygon(intersection.first, p) != PMC_OUTSIDE)
				{
					nearestFace = p;
					nearestPoint = intersection.first;
					nearestDistanceSquared = distanceSquared;
				}
			}
		}
		return nearestFace != null ? new PickResults(nearestFace, nearestPoint) : null;
	}

	public void reflect(Plane plane)
	{
		for(Polygon p: m_polys)
		{
			for(Vector3d v: p.get_vertices())
			{
				plane.reflect_point(v);
			}

			p.calculate_normal();
			p.flip_winding();	// reflecting changes the winding order of all the polygons
		}
		
		// We know the bounding box was reflected across the reflection plane and can hence determine
		// where it was mapped to by the reflection.
		Vector3d[] oldBounds = m_boundingBox.get_bounds();
		Vector3d[] newBounds = new Vector3d[2];
		for(int i=0; i<2; ++i)
		{
			newBounds[i] = oldBounds[i].clone();
			plane.reflect_point(newBounds[i]);
		}
		m_boundingBox.resize(newBounds[0], newBounds[1]);
	}

	public void render(IRenderer renderer, Color overrideColour)
	{
		switch(m_properties.get_function())
		{
			case HINT:
				if(Options.is_set("Render Hint Brushes")) render_NORMAL(renderer, overrideColour);
				break;
			default:
				render_NORMAL(renderer, overrideColour);
				break;
		}
	}

	public void render3D(GL2 gl, GLU glu)
	{
		switch(m_properties.get_function())
		{
			case COLLISION:
				render_wireframe_polygons(gl, new float[] { 1.0f, 1.0f, 1.0f });
				break;
			case DETAIL:
				render3D_NORMAL(gl, glu);
				if(Options.is_set("Render Distinguishable Details")) render_wireframe_polygons(gl, new float[] { 1.0f, 0.0f, 1.0f });
				break;
			case HINT:
				if(Options.is_set("Render Hint Brushes"))
				{
					render3D_NORMAL(gl, glu);
					render_wireframe_polygons(gl, new float[] { 0.0f, 1.0f, 1.0f });
				}
				break;
			default:
				render3D_NORMAL(gl, glu);
				break;
		}

		if(Options.is_set("Render Polygon Normals")) render_normals(gl);
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		switch(m_properties.get_function())
		{
			case HINT:
				if(Options.is_set("Render Hint Brushes")) render_selected_NORMAL(renderer, overrideColour);
				break;
			default:
				render_selected_NORMAL(renderer, overrideColour);
				break;
		}
	}

	public void render3D_selected(GL2 gl, GLU glu)
	{
		switch(m_properties.get_function())
		{
			case COLLISION:
				render_wireframe_polygons(gl, new float[] {1.0f, 1.0f, 0.0f});
				break;
			case DETAIL:
				render3D_selected_NORMAL(gl, glu);
				if(Options.is_set("Render Distinguishable Details")) render_wireframe_polygons(gl, new float[] { 1.0f, 0.0f, 1.0f });
				break;
			case HINT:
				if(Options.is_set("Render Hint Brushes"))
				{
					render3D_selected_NORMAL(gl, glu);
					render_wireframe_polygons(gl, new float[] { 0.0f, 1.0f, 1.0f });
				}
				break;
			default:
				render3D_selected_NORMAL(gl, glu);
				break;
		}

		if(Options.is_set("Render Polygon Normals")) render_normals(gl);
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// The metric is simple: return the square of the nearest distance to
		// the centre or to an edge.

		AxisPair ap = renderer.get_axis_pair();
		Vector2d centre = ap.select_components(m_boundingBox.centre());

		double bestMetric = renderer.distance_squared(p, centre);
		for(Polygon poly: m_polys)
		{
			Vector3d[] verts = poly.get_vertices();
			for(int i=0, len=verts.length; i<len; ++i)
			{
				int j = (i+1)%len;
				Vector2d e1 = ap.select_components(verts[i]), e2 = ap.select_components(verts[j]);
				if(!e1.equals(e2))	// if the points are distinct on this canvas, i.e. there's an edge to check against
				{
					double metric = renderer.distance_squared_from_linesegment(p, e1, e2);
					if(metric < bestMetric) bestMetric = metric;
				}
			}
		}

		return bestMetric;
	}

	public Pair<ArchitectureBrush,ArchitectureBrush> split_with_plane(final Plane plane)
	{
		// Divide the brush polygons into two lists, namely those in front of the plane and those behind it.
		// If polygons straddle the plane, split them. If they're on the plane, discard them.
		LinkedList<Polygon> frontPolys = new LinkedList<Polygon>();
		LinkedList<Polygon> backPolys = new LinkedList<Polygon>();
		for(Polygon poly: m_polys)
		{
			switch(GeomUtil.classify_polygon_against_plane(poly, plane))
			{
				case CP_BACK:
				{
					backPolys.add(poly);
					break;
				}
				case CP_COPLANAR:
				{
					// Ignore all polygons on the splitting plane.
					break;
				}
				case CP_FRONT:
				{
					frontPolys.add(poly);
					break;
				}
				case CP_STRADDLE:
				{
					Pair<Polygon,Polygon> p = GeomUtil.split_polygon(poly, plane);
					frontPolys.add(p.first);
					backPolys.add(p.second);
					break;
				}
			}
		}

		// If the brush didn't straddle the plane, throw an error.
		if(frontPolys.size() == 0 || backPolys.size() == 0) throw new java.lang.Error();

		// The polygons on one side of the plane or the other won't form a complete brush, it will
		// have gaps in it on the plane. To cover these gaps, we make a universe polygon on the plane
		// and clip it to the BSP tree built from the polygons. We then add the polygon fragments which
		// survived the clipping process to the final brush.

		// Front brush
		Tree tree = new Tree(frontPolys);
		Polygon universePoly = GeomUtil.make_universe_polygon(plane.negate());
		Iterable<Polygon> fragments = TreeUtil.clip_to_tree(universePoly, PolygonClipFunctor.no_coplanars(), tree, TreeUtil.CPTT_SOLID_FRAGMENTS).first;
		for(Polygon fragment: fragments) frontPolys.add(fragment);
		ArchitectureBrush frontBrush = new PolyhedralBrush(frontPolys, frontPolys.size(), false, false, m_properties.clone());

		// Back brush
		tree = new Tree(backPolys);
		universePoly = GeomUtil.make_universe_polygon(plane);
		fragments = TreeUtil.clip_to_tree(universePoly, PolygonClipFunctor.no_coplanars(), tree, TreeUtil.CPTT_SOLID_FRAGMENTS).first;
		for(Polygon fragment: fragments) backPolys.add(fragment);
		ArchitectureBrush backBrush = new PolyhedralBrush(backPolys, backPolys.size(), false, false, m_properties.clone());

		return Pair.make_pair(frontBrush, backBrush);
	}

	public LinkedList<Iterable<Polygon>> tree_polygons()
	{
		return MiscUtil.make_singleton_list((Iterable<Polygon>)this);
	}

	// Loading and Saving
	public static IBrush load_MEF0(BufferedReader br) throws IOException
	{
		String line;
		String[] tokens;

		// Read the opening brace.
		br.readLine();

		// Read the bounds.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("Bounds")) throw new IOException("Bounds not found");
		Vector3d[] corners = new Vector3d[] {	new Vector3d(	Double.parseDouble(tokens[2]),
															Double.parseDouble(tokens[3]),
															Double.parseDouble(tokens[4])	),
											new Vector3d(	Double.parseDouble(tokens[7]),
															Double.parseDouble(tokens[8]),
															Double.parseDouble(tokens[9])	) };
		AxisAlignedBox bounds = new AxisAlignedBox(corners[0], corners[1]);

		// Read the template (note: originally "valid") bounds.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("ValidBounds")) throw new IOException("ValidBounds not found");
		corners = new Vector3d[] {	new Vector3d(Double.parseDouble(tokens[2]),
												Double.parseDouble(tokens[3]),
												Double.parseDouble(tokens[4])	),
									new Vector3d(Double.parseDouble(tokens[7]),
												Double.parseDouble(tokens[8]),
												Double.parseDouble(tokens[9])	) };
		AxisAlignedBox templateBounds = new AxisAlignedBox(corners[0], corners[1]);

		// Read the template polygons.
		int polyCount = Integer.parseInt(br.readLine().substring("ValidPolyCount".length()+1));	// read the polygon count
		if(polyCount <= 0) throw new IOException("Invalid polygon count");
		Polygon[] templatePolys = new Polygon[polyCount];
		
		for(int i=0; i<polyCount; ++i)
		{
			line = br.readLine();
			tokens = line.split(" ", 0);
			if(tokens.length < 1 || !tokens[0].equals("ValidPolygon"))
			{
				throw new IOException("ValidPolygon not found");
			}
			int vertCount = Integer.parseInt(tokens[1]);
			if(vertCount <= 0) throw new IOException("Invalid vertex count");
			Vector3d[] vertices = new Vector3d[vertCount];
			for(int j=0; j<vertCount; ++j)
			{
				int offset = 3+5*j;
				vertices[j] = new Vector3d(	Double.parseDouble(tokens[offset]),
											Double.parseDouble(tokens[offset+1]),
											Double.parseDouble(tokens[offset+2])	);
			}
			templatePolys[i] = new Polygon(vertices);
		}

		// Read the closing brace.
		br.readLine();		

		PolyhedralBrush b = new PolyhedralBrush(false, false);
		b.initialise_from_template(templatePolys, templateBounds, bounds);
		return b;
	}

	public static IBrush load_MEF1(BufferedReader br) throws IOException
	{
		String line;
		String[] tokens;

		// Read the opening brace.
		br.readLine();

		// Read the bounds.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("Bounds")) throw new IOException("Bounds not found");
		Vector3d[] corners = new Vector3d[] {	new Vector3d(	Double.parseDouble(tokens[2]),
															Double.parseDouble(tokens[3]),
															Double.parseDouble(tokens[4])	),
											new Vector3d(	Double.parseDouble(tokens[7]),
															Double.parseDouble(tokens[8]),
															Double.parseDouble(tokens[9])	) };
		AxisAlignedBox bounds = new AxisAlignedBox(corners[0], corners[1]);

		// Read the template (note: originally "valid") bounds.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("ValidBounds")) throw new IOException("ValidBounds not found");
		corners = new Vector3d[] {	new Vector3d(Double.parseDouble(tokens[2]),
												Double.parseDouble(tokens[3]),
												Double.parseDouble(tokens[4])	),
									new Vector3d(Double.parseDouble(tokens[7]),
												Double.parseDouble(tokens[8]),
												Double.parseDouble(tokens[9])	) };
		AxisAlignedBox templateBounds = new AxisAlignedBox(corners[0], corners[1]);

		// Read the template polygons.
		int polyCount = Integer.parseInt(br.readLine().substring("ValidPolyCount".length()+1));	// read the polygon count
		if(polyCount <= 0) throw new IOException("Invalid polygon count");
		Polygon[] templatePolys = new Polygon[polyCount];
		
		for(int i=0; i<polyCount; ++i)
		{
			line = br.readLine();
			tokens = line.split(" ", 0);
			if(tokens.length < 1 || !tokens[0].equals("ValidPolygon"))
			{
				throw new IOException("ValidPolygon not found");
			}
			int vertCount = Integer.parseInt(tokens[1]);
			if(vertCount <= 0) throw new IOException("Invalid vertex count");
			Vector3d[] vertices = new Vector3d[vertCount];
			for(int j=0; j<vertCount; ++j)
			{
				int offset = 3+5*j;
				vertices[j] = new Vector3d(	Double.parseDouble(tokens[offset]),
											Double.parseDouble(tokens[offset+1]),
											Double.parseDouble(tokens[offset+2])	);
			}

			// Load the texture details.
			String texture = tokens[2 + 5*vertCount];
			int offset = 4 + 5*vertCount;
			double offsetU = Double.parseDouble(tokens[offset]);
			double offsetV = Double.parseDouble(tokens[offset+1]);
			double scaleU = Double.parseDouble(tokens[offset+2]);
			double scaleV = Double.parseDouble(tokens[offset+3]);
			double angle = Double.parseDouble(tokens[offset+4]);
			Vector3d normal = MathUtil.calculate_normal(vertices[0], vertices[1], vertices[2]);
			TexturePlane texturePlane = new TexturePlane(normal, scaleU, scaleV, offsetU, offsetV, angle);

			templatePolys[i] = new Polygon(vertices, texture, texturePlane);
		}

		// Read the closing brace.
		br.readLine();

		PolyhedralBrush b = new PolyhedralBrush(false, false);
		b.initialise_from_template(templatePolys, templateBounds, bounds);
		return b;
	}

	public static IBrush load_MEF2(BufferedReader br) throws IOException
	{
		String line;
		String[] tokens;

		// Read the opening brace.
		br.readLine();

		PolyhedralBrush b = new PolyhedralBrush(false, false);

		// Read the bounds.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("Bounds")) throw new IOException("Bounds not found");
		Vector3d[] corners = new Vector3d[] {	new Vector3d(	Double.parseDouble(tokens[2]),
															Double.parseDouble(tokens[3]),
															Double.parseDouble(tokens[4])	),
											new Vector3d(	Double.parseDouble(tokens[7]),
															Double.parseDouble(tokens[8]),
															Double.parseDouble(tokens[9])	) };
		b.m_boundingBox = new BoundingBox(corners[0], corners[1]);

		// Read the polygons.
		int polyCount = Integer.parseInt(br.readLine().substring("PolyCount".length()+1));	// read the polygon count
		if(polyCount <= 0) throw new IOException("Invalid polygon count");
		b.m_polys = new Polygon[polyCount];
		
		for(int i=0; i<polyCount; ++i)
		{
			line = br.readLine();
			tokens = line.split(" ", 0);
			if(tokens.length < 1 || !tokens[0].equals("Polygon"))
			{
				throw new IOException("Polygon not found");
			}
			int vertCount = Integer.parseInt(tokens[1]);
			if(vertCount <= 0) throw new IOException("Invalid vertex count");
			Vector3d[] vertices = new Vector3d[vertCount];
			for(int j=0; j<vertCount; ++j)
			{
				int offset = 3+5*j;
				vertices[j] = new Vector3d(	Double.parseDouble(tokens[offset]),
											Double.parseDouble(tokens[offset+1]),
											Double.parseDouble(tokens[offset+2])	);
			}

			// Load the texture details.
			String texture = tokens[2 + 5*vertCount];
			int offset = 4 + 5*vertCount;
			double offsetU = Double.parseDouble(tokens[offset]);
			double offsetV = Double.parseDouble(tokens[offset+1]);
			double scaleU = Double.parseDouble(tokens[offset+2]);
			double scaleV = Double.parseDouble(tokens[offset+3]);
			double angle = Double.parseDouble(tokens[offset+4]);
			Vector3d normal = MathUtil.calculate_normal(vertices[0], vertices[1], vertices[2]);
			TexturePlane texturePlane = new TexturePlane(normal, scaleU, scaleV, offsetU, offsetV, angle);

			b.m_polys[i] = new Polygon(vertices, texture, texturePlane);
		}

		// Read the closing brace.
		br.readLine();

		return b;
	}

	public static IBrush load_MEF3(BufferedReader br) throws IOException
	{
		String line;
		String[] tokens;

		// Read the opening brace.
		br.readLine();

		PolyhedralBrush b = new PolyhedralBrush(false, false);

		// Read the brush function (if present).
		BrushFunction function;
		boolean functionPresent = true;
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length >= 1 && tokens[0].equals("Function"))
		{
			try
			{
				function = BrushFunction.valueOf(tokens[1]);
			}
			catch(Exception e)
			{
				System.err.println("Warning: Bad brush function " + tokens[1] + ", defaulting to NORMAL");
				function = BrushFunction.NORMAL;
			}
		}
		else
		{
			System.err.println("Warning: Missing brush function, defaulting to NORMAL");
			function = BrushFunction.NORMAL;
			functionPresent = false;
		}
		b.m_properties = new Properties(function);

		// Read the bounds.
		if(functionPresent) line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length < 1 || !tokens[0].equals("Bounds")) throw new IOException("Bounds not found");
		Vector3d[] corners = new Vector3d[] {	new Vector3d(	Double.parseDouble(tokens[2]),
															Double.parseDouble(tokens[3]),
															Double.parseDouble(tokens[4])	),
											new Vector3d(	Double.parseDouble(tokens[7]),
															Double.parseDouble(tokens[8]),
															Double.parseDouble(tokens[9])	) };
		b.m_boundingBox = new BoundingBox(corners[0], corners[1]);

		// Read the polygons.
		int polyCount = Integer.parseInt(br.readLine().substring("PolyCount".length() + 1));	// read the polygon count
		if (polyCount <= 0) throw new IOException("Invalid polygon count");
		b.m_polys = new Polygon[polyCount];

		for (int i = 0; i < polyCount; ++i)
		{
			line = br.readLine();
			tokens = line.split(" ", 0);
			if (tokens.length < 1 || !tokens[0].equals("Polygon"))
			{
				throw new IOException("Polygon not found");
			}
			int vertCount = Integer.parseInt(tokens[1]);
			if (vertCount <= 0) throw new IOException("Invalid vertex count");
			Vector3d[] vertices = new Vector3d[vertCount];
			for (int j = 0; j < vertCount; ++j)
			{
				int offset = 3 + 5 * j;
				vertices[j] = new Vector3d(Double.parseDouble(tokens[offset]),
											Double.parseDouble(tokens[offset + 1]),
											Double.parseDouble(tokens[offset + 2]));
			}

			// Load the texture details.
			String texture = tokens[2 + 5 * vertCount];
			int offset = 4 + 5 * vertCount;
			double offsetU = Double.parseDouble(tokens[offset]);
			double offsetV = Double.parseDouble(tokens[offset + 1]);
			double scaleU = Double.parseDouble(tokens[offset + 2]);
			double scaleV = Double.parseDouble(tokens[offset + 3]);
			double angle = Double.parseDouble(tokens[offset + 4]);
			Vector3d normal = MathUtil.calculate_normal(vertices[0], vertices[1], vertices[2]);
			TexturePlane texturePlane = new TexturePlane(normal, scaleU, scaleV, offsetU, offsetV, angle);

			b.m_polys[i] = new Polygon(vertices, texture, texturePlane);
		}

		// Read the closing brace.
		br.readLine();

		return b;
	}

	public void save_MEF2(PrintWriter pw)
	{
		pw.println();
		pw.println("PolyhedralBrush");
		pw.println("{");

		Vector3d[] bounds = m_boundingBox.get_bounds();
		pw.print("Bounds");
		for(int i=0; i<2; ++i) pw.print(" ( " + bounds[i].x + " " + bounds[i].y + " " + bounds[i].z + " )");
		pw.println();

		int polyCount = m_polys.length;
		pw.println("PolyCount " + polyCount);

		for(int i=0; i<polyCount; ++i)
		{
			Polygon p = m_polys[i];
			pw.print("Polygon " + p.get_vertices().length);
			for(Vector3d v: p.get_vertices())
			{
				pw.print(" ( " + v.x + " " + v.y + " " + v.z + " )");
			}

			// Output the texture details.
			Pair<String,TexturePlane> textureDetails = p.get_texture_details();
			pw.print(" " + textureDetails.first);
			pw.println(" " + textureDetails.second);
		}

		pw.print("}");
	}

	public void save_MEF3(PrintWriter pw)
	{
		pw.println();
		pw.println("PolyhedralBrush");
		pw.println("{");

		pw.print("Function ");
		pw.println(m_properties.get_function().toString());

		Vector3d[] bounds = m_boundingBox.get_bounds();
		pw.print("Bounds");
		for(int i=0; i<2; ++i) pw.print(" ( " + bounds[i].x + " " + bounds[i].y + " " + bounds[i].z + " )");
		pw.println();

		int polyCount = m_polys.length;
		pw.println("PolyCount " + polyCount);

		for(int i=0; i<polyCount; ++i)
		{
			Polygon p = m_polys[i];
			pw.print("Polygon " + p.get_vertices().length);
			for(Vector3d v: p.get_vertices())
			{
				pw.print(" ( " + v.x + " " + v.y + " " + v.z + " )");
			}

			// Output the texture details.
			Pair<String,TexturePlane> textureDetails = p.get_texture_details();
			pw.print(" " + textureDetails.first);
			pw.println(" " + textureDetails.second);
		}

		pw.print("}");
	}

	//################## PACKAGE METHODS ##################//
	/**
	Creates a brush which fills the specified bounds from a template version defined to fill the
	unit cube from (0,0,0) to (1,1,1).

	@param templatePolys	The polygons of the template brush
	@param bounds			The bounds of the brush to be created
	@return					A brush as specified
	*/
	static PolyhedralBrush create_from_unit_template(Polygon[] templatePolys, AxisAlignedBox bounds)
	{
		PolyhedralBrush b = new PolyhedralBrush(true, true);
		b.initialise_from_template(templatePolys, new BoundingBox(new Vector3d(0,0,0), new Vector3d(1,1,1)), bounds);
		return b;
	}

	//################## PROTECTED METHODS ##################//
	protected void begin_transform()
	{
		// Cache the existing brush for use during the transformation.

		m_cachedBoundingBox = m_boundingBox.clone();

		int len = m_polys.length;
		m_cachedPolys = new Polygon[len];
		for(int i=0; i<len; ++i) m_cachedPolys[i] = m_polys[i].clone();
	}

	protected void end_transform()
	{
		recalculate_bounding_box();
	}

	protected Properties get_properties()
	{
		return m_properties;
	}

	protected void resize(Vector2d corner0, Vector2d corner1, AxisPair ap)
	{
		// Generate our resized brush from the brush we had at the start of the transformation
		// (we cached it in begin_transform).

		// Ensure that corner0 and corner1 contain the 0th and 2nd corners
		// of our bounding box, respectively.
		corner0 = corner0.clone();
		corner1 = corner1.clone();
		if(corner0.x > corner1.x)
		{
			double t = corner0.x;
			corner0.x = corner1.x;
			corner1.x = t;
		}
		if(corner0.y > corner1.y)
		{
			double t = corner0.y;
			corner0.y = corner1.y;
			corner1.y = t;
		}

		Vector2d[] cachedCorners = m_cachedBoundingBox.project_to_2D_using(ap).m_corners;
		Vector2d scale = new Vector2d((corner1.x-corner0.x)/(cachedCorners[1].x-cachedCorners[0].x),
									(corner1.y-corner0.y)/(cachedCorners[1].y-cachedCorners[0].y));

		for(int i=0, polyCount=m_cachedPolys.length; i<polyCount; ++i)
		{
			Vector3d[] cachedVerts = m_cachedPolys[i].get_vertices();
			Vector3d[] verts = m_polys[i].get_vertices();
			for(int j=0, vertCount=cachedVerts.length; j<vertCount; ++j)
			{
				ap.set_relevant_components(verts[j], ap.select_components(cachedVerts[j]));
				ap.subtract_offset(verts[j], cachedCorners[0]);
				ap.scale_relevant_components(verts[j], scale);
				ap.add_offset(verts[j], corner0);
			}

			m_polys[i].calculate_normal();
			m_polys[i].set_texture_plane(generate_reasonable_texture_plane(m_cachedPolys[i].get_texture_details().second, cachedVerts,
																		   verts, m_polys[i].get_normal()));
		}

		m_boundingBox.resize(corner0, corner1, ap);
	}

	protected void rotate(final Vector2d centre, final double angle, AxisPair ap)
	{
		// Rotating affects polygon normals, so we have to recalculate them.
		generic_transform(ap, true, new TransformFunctor()
		{
			public Vector2d transform2D(Vector2d v)
			{
				v.subtract(centre);
				Vector2d newV = new Vector2d(v.x*Math.cos(angle) - v.y*Math.sin(angle),
										   v.x*Math.sin(angle) + v.y*Math.cos(angle));
				newV.add(centre);
				return newV;
			}
		});
	}

	protected void set_properties(Properties properties)
	{
		m_properties = properties;
	}

	protected void shear_horizontal(final Vector2d anchor, final double factor, AxisPair ap)
	{
		// Shearing affects polygon normals, so we have to recalculate them. We have to do
		// something special with texture planes, since just blindly shearing them would
		// leave us with non-orthogonal texture axes (a.k.a. not a good thing).
		generic_transform(ap, true, new TransformFunctor()
		{
			public Vector2d transform2D(Vector2d v)
			{
				v.subtract(anchor);
				Vector2d newV = new Vector2d(v.x + factor*v.y, v.y);
				newV.add(anchor);
				return newV;
			}
		});
	}

	protected void translate(final Vector3d trans)
	{
		// Translating doesn't affect polygon normals, so there's no need to recalculate them.
		generic_transform(null, false, new TransformFunctor()
		{
			public Vector3d transform3D(Vector3d v, AxisPair ap)
			{
				Vector3d ret = v.clone();
				ret.add(trans);
				return ret;
			}
		});

		// It's easy to recalculate bounding boxes while we're translating, so we might as well
		// in order to make things easier for users.
		m_boundingBox = m_cachedBoundingBox.clone();
		m_boundingBox.translate(trans);
	}

	//################## PRIVATE METHODS ##################//
	/**
	Takes the old texture plane for a polygon, the old vertices, the new vertices and the new normal,
	and uses them to generate a new texture plane for the polygon. By default, we try and preserve the
	old texture coordinates, but if this isn't possible then we preserve the texture parameters instead.

	@param oldTexturePlane		The old texture plane
	@param oldVerts				The old vertices
	@param newVerts				The new vertices
	@param newNormal			The new normal
	@return						The new texture plane
	*/
	private static TexturePlane generate_reasonable_texture_plane(final TexturePlane oldTexturePlane, final Vector3d[] oldVerts,
																  final Vector3d[] newVerts, final Vector3d newNormal)
	{
		if(Options.is_set("Preserve Texture Coordinates"))
		{
			// Try and preserve the old texture coordinates if we can.
			TexturePlane ret = oldTexturePlane.generate_equivalent_plane(oldVerts, newVerts);
			if(ret != null) return ret;
		}

		// It wasn't possible to generate an equivalent texture plane, so we take the "reasonable" option of preserving the texture parameters
		// rather than the texture coordinates.
		return new TexturePlane(newNormal,
								oldTexturePlane.scale_u(),
								oldTexturePlane.scale_v(),
								oldTexturePlane.offset().u,
								oldTexturePlane.offset().v,
								oldTexturePlane.rotation());
	}

	private void generic_transform(AxisPair ap, boolean bRecalculateNormals, TransformFunctor transformFunctor)
	{
		// Generate our transformed brush from the brush we cached in begin_transform.
		for(int i=0, polyCount=m_cachedPolys.length; i<polyCount; ++i)
		{
			Vector3d[] cachedVerts = m_cachedPolys[i].get_vertices();
			Vector3d[] verts = m_polys[i].get_vertices();
			for(int j=0, vertCount=cachedVerts.length; j<vertCount; ++j) verts[j] = transformFunctor.transform3D(cachedVerts[j], ap);

			if(bRecalculateNormals) m_polys[i].calculate_normal();

			m_polys[i].set_texture_plane(generate_reasonable_texture_plane(m_cachedPolys[i].get_texture_details().second, cachedVerts,
																		   verts, m_polys[i].get_normal()));
		}
	}

	/**
	Initialises a brush from a template version of it by resizing a set of polygons which fill a specified
	bounding box to fit a different bounding box.

	@param templatePolys	The template polygons which fill the specified template bounding box
	@param templateBounds	The bounding box for the template polygons
	@param bounds			The bounding box we want for our initialised brush
	*/
	private void initialise_from_template(Polygon[] templatePolys, AxisAlignedBox templateBounds, AxisAlignedBox bounds)
	{
		m_boundingBox = new BoundingBox(bounds);

		Vector3d[] corners = bounds.get_bounds(), templateCorners = templateBounds.get_bounds();

		Vector3d size = VectorUtil.subtract(corners[1], corners[0]);
		Vector3d templateSize = VectorUtil.subtract(templateCorners[1], templateCorners[0]);

		Vector3d scale = new Vector3d(size.x/templateSize.x, size.y/templateSize.y, size.z/templateSize.z);

		int polyCount = templatePolys.length;
		m_polys = new Polygon[polyCount];
		for(int i=0; i<polyCount; ++i)
		{
			m_polys[i] = templatePolys[i].clone();
			Vector3d[] verts = m_polys[i].get_vertices();
			int vertCount = verts.length;
			for(int j=0; j<vertCount; ++j)
			{
				verts[j].subtract(templateCorners[0]);
				verts[j].x*=scale.x; verts[j].y*=scale.y; verts[j].z*=scale.z;
				verts[j].add(corners[0]);
			}
		}

		// Calculate normals and texture planes.
		for(int i=0; i<polyCount; ++i)
		{
			m_polys[i].calculate_normal();
			TexturePlane texturePlane = templatePolys[i].get_texture_details().second;
			m_polys[i].set_texture_plane(texturePlane.generate_equivalent_plane(templatePolys[i].get_vertices(), m_polys[i].get_vertices()));
		}
	}

	/**
	Recalculates the brush's bounding box so that it exactly encloses its polygons.
	*/
	private void recalculate_bounding_box()
	{
		double max = Double.MAX_VALUE, min = Double.MIN_VALUE;
		Vector3d[] corners = new Vector3d[] { new Vector3d(max,max,max), new Vector3d(min,min,min) };
		for(Polygon p: m_polys)
		{
			for(Vector3d v: p.get_vertices())
			{
				if(v.x < corners[0].x) corners[0].x = v.x;
				if(v.y < corners[0].y) corners[0].y = v.y;
				if(v.z < corners[0].z) corners[0].z = v.z;
				if(v.x > corners[1].x) corners[1].x = v.x;
				if(v.y > corners[1].y) corners[1].y = v.y;
				if(v.z > corners[1].z) corners[1].z = v.z;
			}
		}
		m_boundingBox.resize(corners[0], corners[1]);
	}

	/**
	Renders the polygons of the brush onto a 3D view, using the specified colour.

	@param gl		The OpenGL object to use for rendering
	@param colour	The colour with which to colour the polygons
	*/
	private void render_flatshaded_polygons(GL2 gl, float[] colour)
	{
		gl.glColor3fv(colour, 0);
		for(Polygon p: m_polys)
		{
			Vector3d[] verts = p.get_vertices();
			gl.glBegin(GL2.GL_POLYGON);
				for(int i=0, len=verts.length; i<len; ++i)
				{
					gl.glVertex3d(verts[i].x, verts[i].y, verts[i].z);
				}
			gl.glEnd();
		}
	}

	/**
	Renders the polygons of the brush onto a 3D view, using the specified colours.

	@param gl		The OpenGL object to use for rendering
	@param colours	The colours with which to colour the polygons - vertex i is coloured with
					the colour specified by colours[i%3] (note that each colours[j] should be
					of length 3 - colours[j][0] contains the red component of the colour,
					colours[j][1] the green component and colours[j][2] the blue component)
	*/
	private void render_multicoloured_polygons(GL2 gl, float[][] colours)
	{
		for(Polygon p: m_polys)
		{
			Vector3d[] verts = p.get_vertices();
			gl.glBegin(GL2.GL_POLYGON);
				for(int i=0, len=verts.length; i<len; ++i)
				{
					int j = i%3;
					gl.glColor3f(colours[j][0], colours[j][1], colours[j][2]);
					gl.glVertex3d(verts[i].x, verts[i].y, verts[i].z);
				}
			gl.glEnd();
		}
	}

	private void render_NORMAL(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else if(m_ghost) renderer.set_colour(Color.white);
		else renderer.set_colour(m_colour);

		for(Polygon p: m_polys)
		{
			for(int i=0, len=p.get_vertices().length; i<len; ++i)
			{
				int j = (i+1)%len;
				renderer.draw_line(p.get_vertices()[i], p.get_vertices()[j]);
			}
		}

		render_centre_cross(renderer);

		if(Options.is_set("Render Polygon Normals"))
		{
			// Draw the polygon normals.
			for(Polygon p: m_polys)
			{
				Vector3d centre = new Vector3d();
				for(Vector3d v: p.get_vertices())
				{
					centre.add(v);
				}
				centre.scale(1.0/p.get_vertices().length);

				Vector3d scaledNormal = p.get_normal().clone();
				scaledNormal.scale(15);
				Vector3d centrePLUSnormal = centre.clone();
				centrePLUSnormal.add(scaledNormal);

				renderer.set_colour(Color.magenta);
				renderer.draw_line(centre, centrePLUSnormal);

				scaledNormal.scale(0.5);
				Vector3d centrePLUShalfNormal = centre.clone();
				centrePLUShalfNormal.add(scaledNormal);
				renderer.set_colour(Color.cyan);
				renderer.draw_line(centrePLUShalfNormal, centrePLUSnormal);
			}
		}
	}

	/**
	Render the face normals onto a 3D view using the specified OpenGL object.

	@param gl	The OpenGL object to use for rendering
	*/
	private void render_normals(GL2 gl)
	{
		gl.glBegin(GL.GL_LINES);
			for(Polygon p: m_polys)
			{
				Vector3d centre = p.centre();

				Vector3d scaledNormal = p.get_normal().clone();
				scaledNormal.scale(7.5);
				Vector3d centrePLUSnormal = centre.clone();
				centrePLUSnormal.add(scaledNormal);

				gl.glColor3f(1.0f, 0.0f, 1.0f);		// magenta
				gl.glVertex3d(centre.x, centre.y, centre.z);
				gl.glVertex3d(centrePLUSnormal.x, centrePLUSnormal.y, centrePLUSnormal.z);

				scaledNormal.scale(0.5);
				Vector3d centrePLUShalfNormal = centre.clone();
				centrePLUShalfNormal.add(scaledNormal);

				gl.glColor3f(0.0f, 1.0f, 1.0f);		// cyan
				gl.glVertex3d(centrePLUShalfNormal.x, centrePLUShalfNormal.y, centrePLUShalfNormal.z);
				gl.glVertex3d(centrePLUSnormal.x, centrePLUSnormal.y, centrePLUSnormal.z);
			}
		gl.glEnd();
	}

	private void render_selected_NORMAL(IRenderer renderer, Color overrideColour)
	{
		renderer.set_colour(Color.red);
		render_bounds(renderer);

		render(renderer, overrideColour);

		render_transformation_effects(renderer);
	}

	private void render_wireframe_polygons(GL2 gl, float[] colour)
	{
		gl.glPushAttrib(GL2.GL_ENABLE_BIT | GL2.GL_POLYGON_BIT);
		gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL2.GL_LINE);
		gl.glDisable(GL.GL_CULL_FACE);

		render_flatshaded_polygons(gl, colour);

		gl.glPopAttrib();
	}

	private void render3D_NORMAL(GL2 gl, GLU glu)
	{
		if(Options.is_set("Render Textures"))
		{
			for(Polygon p: m_polys)
			{
				Pair<String,TexturePlane> textureDetails = p.get_texture_details();
				Texture texture = TextureManager.instance().get_texture(textureDetails.first);
				if(texture != null)		// if there's a texture to use, render a textured polygon
				{
					gl.glEnable(GL.GL_TEXTURE_2D);
					texture.bind(gl);

					gl.glBegin(GL2.GL_POLYGON);
						gl.glColor3f(1.0f, 1.0f, 1.0f);
						for(Vector3d v: p.get_vertices())
						{
							TextureCoord texCoords = textureDetails.second.calculate_coordinates(v);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(v.x, v.y, v.z);
						}
					gl.glEnd();

					gl.glDisable(GL.GL_TEXTURE_2D);
				}
				else					// otherwise, render a blank white polygon
				{
					gl.glBegin(GL2.GL_POLYGON);
						gl.glColor3f(1.0f, 1.0f, 1.0f);
						for(Vector3d v: p.get_vertices()) gl.glVertex3d(v.x, v.y, v.z);
					gl.glEnd();
				}
			}
		}
		else if(Options.is_set("Flat-Shading"))
		{
			// Use flat-shaded rendering without textures.
			float[] colour = null;
			colour = m_colour.getRGBComponents(colour);

			render_flatshaded_polygons(gl, colour);
		}
		else
		{
			// Use multi-coloured rendering without textures.
			render_multicoloured_polygons(gl, new float[][]
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f}
			});
		}
	}

	private void render3D_selected_NORMAL(GL2 gl, GLU glu)
	{
		if(Options.is_set("Render Textures"))
		{
			for(Polygon p: m_polys)
			{
				Pair<String,TexturePlane> textureDetails = p.get_texture_details();
				Texture texture = TextureManager.instance().get_texture(textureDetails.first);
				if(texture != null)		// if there's a texture to use, render a textured polygon
				{
					gl.glEnable(GL.GL_TEXTURE_2D);
					texture.bind(gl);

					// Set up the stencil buffer to write 1 when the depth test passes, and 0 otherwise.
					gl.glStencilFunc(GL.GL_ALWAYS, 1, 1);
					gl.glStencilOp(GL.GL_ZERO, GL.GL_ZERO, GL.GL_REPLACE);
					gl.glEnable(GL.GL_STENCIL_TEST);

					// Render the textured polygon.
					gl.glBegin(GL2.GL_POLYGON);
						gl.glColor3f(1.0f, 1.0f, 1.0f);
						for(Vector3d v: p.get_vertices())
						{
							TextureCoord texCoords = textureDetails.second.calculate_coordinates(v);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(v.x, v.y, v.z);
						}
					gl.glEnd();

					gl.glDisable(GL.GL_TEXTURE_2D);

					// Set up the stencil buffer to pass when it contains a 1, and disable depth testing.
					gl.glStencilFunc(GL.GL_EQUAL, 1, 1);
					gl.glStencilOp(GL.GL_KEEP, GL.GL_KEEP, GL.GL_KEEP);
					gl.glDisable(GL.GL_DEPTH_TEST);

					// Render the selection highlight polygon by blending it with the texture underneath.
					gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
					gl.glEnable(GL.GL_BLEND);
					gl.glBegin(GL2.GL_POLYGON);
						gl.glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
						for(Vector3d v: p.get_vertices())
						{
							gl.glVertex3d(v.x, v.y, v.z);
						}
					gl.glEnd();
					gl.glDisable(GL.GL_BLEND);

					gl.glEnable(GL.GL_DEPTH_TEST);
					gl.glDisable(GL.GL_STENCIL_TEST);
				}
				else				// otherwise, render a blank white polygon
				{
					gl.glBegin(GL2.GL_POLYGON);
						gl.glColor3f(1.0f, 1.0f, 1.0f);
						for(Vector3d v: p.get_vertices()) gl.glVertex3d(v.x, v.y, v.z);
					gl.glEnd();
				}
			}
		}
		else if(Options.is_set("Flat-Shading"))
		{
			// Use flat-shaded rendering without textures.
			render_flatshaded_polygons(gl, new float[] {1.0f, 0.0f, 0.0f});
		}
		else
		{
			// Use multi-coloured rendering without textures.
			render_multicoloured_polygons(gl, new float[][]
			{
				{1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 1.0f}
			});
		}
	}

	//################## PRIVATE NESTED CLASSES ##################//
	private abstract class TransformFunctor
	{
		/**
		A transformation function which takes a 2D vertex and returns another one. What the
		function does is intended to be defined via inheritance, but if we override transform3D
		then we may not need to override transform2D as well, hence the default implementation
		here.

		@param v	A 2D vertex
		@return		The result of doing some transformation on that vertex
		*/
		public Vector2d transform2D(final Vector2d v)
		{
			throw new UnsupportedOperationException();
		}

		/**
		A transformation function which takes a 3D vertex and an AxisPair and returns a 3D vertex.
		The default is to perform the (overriden) 2D transformation on the projected part of the
		3D vertex, but it makes sense to override this in certain circumstances, e.g. for translation.

		<p><b>Notes:</b>
		<ul>
		<li>Whether ap can be null depends on how we're implementing this function - it clearly can't be here,
		but there's no particular reason why it can't be if we override transform3D.
		</ul>

		@param v	A 3D vertex
		@param ap	The AxisPair defining the projection of our 3D vertex into 2D
		*/
		public Vector3d transform3D(final Vector3d v, final AxisPair ap)
		{
			Vector3d ret = v.clone();
			Vector2d newV = transform2D(ap.select_components(ret));
			ap.set_relevant_components(ret, newV);
			return ret;
		}
	}
}

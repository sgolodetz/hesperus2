package MapEditor.Brushes;

import MapEditor.BSP.*;
import MapEditor.Geom.*;
import MapEditor.Geom.Planar.*;
import MapEditor.Graphics.ColourCycle;
import MapEditor.Graphics.IRenderer;
import MapEditor.Maps.*;
import MapEditor.Math.MathUtil;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import java.awt.Color;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.LinkedList;
import javax.media.opengl.*;
import javax.media.opengl.glu.GLU;

public class ArchitectureBrushComposite extends ArchitectureBrush implements GeomConstants
{
	//################## PRIVATE NESTED CLASSES ##################//
	private class BrushData
	{
		public Vector3d[] offsets = new Vector3d[2];	// the offsets of the top-left and bottom-right
														// corners of a brush's bounding box from the
														// top-left of our cached bounding box
	}

	//################## PRIVATE VARIABLES ##################//
	private static ColourCycle COLOUR_CYCLE = new ColourCycle();

	private boolean m_transient = true;					// is the composite a temporary/transient one which will
														// revert back to its component brushes when deselected?
	private Color m_colour;								// the colour with which we will render the brush on a design canvas
	private LinkedList<Pair<ArchitectureBrush,BrushData>> m_brushes = new LinkedList<Pair<ArchitectureBrush,BrushData>>();
	private static int s_colourIndex = 0;

	//################## CONSTRUCTORS ##################//
	public ArchitectureBrushComposite()
	{
		super(false);	// composite brushes don't get dragged out by the user
		m_colour = COLOUR_CYCLE.next_colour();
	}

	//################## PUBLIC METHODS ##################//
	public void add_brush(ArchitectureBrush b)
	{
		m_brushes.add(new Pair<ArchitectureBrush,BrushData>(b, new BrushData()));
		extend_bounds_with(b);
	}

	/**
	Returns the number of brushes in the composite.

	@return	...think about it...
	*/
	public int brush_count()
	{
		return m_brushes.size();
	}

	public int classify_against_plane(final Plane plane)
	{
// TODO: Check that this is right.
		boolean backFlag = false, frontFlag = false;

		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			switch(p.first.classify_against_plane(plane))
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
		LinkedList<ArchitectureBrush> ret = new LinkedList<ArchitectureBrush>();
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) ret.addAll(p.first.component_brushes());
		return ret;
	}

	public ArchitectureBrushComposite copy()
	{
		ArchitectureBrushComposite ret = new ArchitectureBrushComposite();
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) ret.add_brush(p.first.copy());
		ret.m_transient = m_transient;
		return ret;
	}

	public void delete_brush(ArchitectureBrush b)
	{
		Iterator<Pair<ArchitectureBrush,BrushData>> it = m_brushes.iterator();
		while(it.hasNext())
		{
			Pair<ArchitectureBrush,BrushData> p = it.next();
			if(p.first == b)
			{
				it.remove();
				recalculate_bounds();
				return;
			}
		}

		// If we get here, the brush wasn't in the composite.
		System.err.println("Warning: Trying to delete a brush that isn't in the composite");
		System.err.println("---");
		System.err.println(new Throwable().getStackTrace()[1]);
		System.err.println("---");
	}

	public IBrush deselect(IBrushContainer container)
	{
		super.deselect(container);

		if(m_transient)
		{
			for(ArchitectureBrush ab: get_brushes()) container.add_brush(ab);
			container.delete_brush(this);
		}
		return null;
	}

	public BrushDetails details()
	{
		int faceCount = 0;
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			BrushDetails bd = p.first.details();
			faceCount += bd.m_faceCount;
		}
		return new BrushDetails(faceCount);
	}

	public LinkedList<Polygon> faces()
	{
		LinkedList<Polygon> ret = new LinkedList<Polygon>();
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			LinkedList<Polygon> result = p.first.faces();
			ret.addAll(result);
		}
		return ret;
	}

	/**
	Groups the brushes in the composite into a brush group. In practice, all this involves
	is setting the composite's transient flag to false, because brush groups are merely
	permanent composites.
	*/
	public void group()
	{
		m_transient = false;
	}

	public boolean is_copyable()
	{
		// Note that component brushes of ArchitectureBrushComposites are in the map and are
		// copyable ArchitectureBrushes, so we don't need to check that they're all copyable
		// themselves.
		return true;
	}

	public boolean is_ghost()
	{
		return false;
	}

	/**
	Returns a boolean indicating whether or not the brush is transient, or in other words
	whether or not the brushes within the composite are grouped together.

	@return		true, if the brushes within the composite are only temporarily so, or false otherwise
	*/
	public boolean is_transient()
	{
		return m_transient;
	}

	/**
	Returns the optimal brush for the composite. If the composite contains exactly one brush,
	the optimal brush is that brush. If the composite contains more than one brush, the optimal
	brush is the composite itself. If there are no brushes in the composite, there isn't an
	optimal brush and we return null.

	@return		The optimal brush for the composite as described above
	*/
	public ArchitectureBrush optimal_brush()
	{
		int brushCount = m_brushes.size();
		if(brushCount > 1) return this;
		else if(brushCount == 1) return m_brushes.getFirst().first;
		else return null;
	}

	public PickResults pick(final Vector3d start, final Vector3d direction)
	{
		Polygon nearestFace = null;
		Vector3d nearestPoint = null;
		IBrush nearestNestedBrush = null;
		double nearestDistanceSquared = Double.MAX_VALUE;
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			PickResults result = p.first.pick(start, direction);
			if(result != null)
			{
				double distanceSquared = start.distance_squared(result.m_pickedPoint);
				if(distanceSquared < nearestDistanceSquared)
				{
					nearestFace = result.m_pickedFace;
					nearestPoint = result.m_pickedPoint;
					nearestNestedBrush = p.first;
					nearestDistanceSquared = distanceSquared;
				}
			}
		}
		return nearestFace != null ? new PickResults(nearestFace, nearestPoint, nearestNestedBrush) : null;
	}

	public void reflect(Plane plane)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.reflect(plane);

		// Recalculate the composite bounding box, since we are not necessarily reflecting
		// across a plane through the centre of the brush (even though we are at the moment,
		// the parameters to reflect() are more general than that and we may choose to exploit
		// this in the future, so to prevent later bugs it's important to get it right now).
		recalculate_bounds();
	}

	public void render(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour == null && !m_transient) overrideColour = m_colour;

		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			p.first.render(renderer, overrideColour);
		}
	}

	public void render3D(GL2 gl, GLU glu)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			p.first.render3D(gl, glu);
		}
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour == null && !m_transient) overrideColour = m_colour;

		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			p.first.render(renderer, overrideColour);
		}

		renderer.set_colour(Color.red);
		render_bounds(renderer);

		renderer.set_colour(Color.yellow);
		render_centre_cross(renderer);

		render_transformation_effects(renderer);
	}

	public void render3D_selected(GL2 gl, GLU glu)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			p.first.render3D_selected(gl, glu);
		}
	}

	public void select(IBrushContainer container)
	{
		super.select(container);

		if(m_transient)
		{
			for(ArchitectureBrush ab: get_brushes()) container.delete_brush(ab);
			container.add_brush(this);
		}
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		if(m_transient)
		{
			// There's no chance of selecting a transient ArchitectureBrushComposite, since they only exist
			// when they're the selected brush.
			return Double.MAX_VALUE;
		}
		else
		{
			// The selection metric for the composite is the minimum of the metrics for its component brushes
			// since we want to select the composite if we're near enough to the nearest component brush.
			double bestMetric = Double.MAX_VALUE;
			for(ArchitectureBrush ab: get_brushes())
			{
				double metric = ab.selection_metric(p, renderer);
				if(metric < bestMetric) bestMetric = metric;
			}
			return bestMetric;
		}
	}

	public Pair<ArchitectureBrush,ArchitectureBrush> split_with_plane(final Plane plane)
	{
		ArchitectureBrushComposite backGroup = new ArchitectureBrushComposite();
		ArchitectureBrushComposite frontGroup = new ArchitectureBrushComposite();

		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			switch(p.first.classify_against_plane(plane))
			{
				case CP_BACK:
				{
					backGroup.add_brush(p.first);
					break;
				}
				case CP_COPLANAR:
				{
					// This should never happen.
					throw new java.lang.Error();
				}
				case CP_FRONT:
				{
					frontGroup.add_brush(p.first);
					break;
				}
				case CP_STRADDLE:
				{
					Pair<ArchitectureBrush,ArchitectureBrush> result = p.first.split_with_plane(plane);
					frontGroup.add_brush(result.first);
					backGroup.add_brush(result.second);
					break;
				}
			}
		}

		backGroup.group();
		frontGroup.group();

		return Pair.make_pair(frontGroup.optimal_brush(), backGroup.optimal_brush());
	}

	public LinkedList<Iterable<Polygon>> tree_polygons()
	{
		LinkedList<Iterable<Polygon>> ret = new LinkedList<Iterable<Polygon>>();
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			LinkedList<Iterable<Polygon>> result = p.first.tree_polygons();
			if(result != null) ret.addAll(result);
		}

		if(ret.isEmpty()) return null;
		else return ret;
	}

	/**
	Splits a brush group back into its component brushes. In practice, all this involves is setting
	the composite's transient flag to true, since brush groups are merely permanent composites.
	*/
	public void ungroup()
	{
		m_transient = true;
	}

	// Loading and Saving
	/**
	This method exists so that we don't have to rewrite the loading code every time we have a new version of
	the MEF file format. It's effectively the same each time, except that the name of the load methods to call
	to load component brushes changes. We therefore pass the correct method name in as a parameter.
	*/
	private static IBrush load_MEF(BufferedReader br, String methodName) throws Exception
	{
		ArchitectureBrushComposite ret = new ArchitectureBrushComposite();

		// Read the opening brace.
		br.readLine();

		// Recursively load component brushes and add them to the composite.
		String line;
		while((line = br.readLine()) != null)
		{
			if(line.equals("}")) break;

			Class<?> componentClass = Class.forName("MapEditor.Brushes." + line);
			java.lang.reflect.Method loadMethod = componentClass.getMethod(methodName, new Class[] {BufferedReader.class});
			ret.add_brush((ArchitectureBrush)loadMethod.invoke(null, new Object[] {br}));
		}

		// If we got here because we reached the end of the file, we're missing a closing brace.
		if(line == null) throw new IOException("Closing brace not found");

		ret.group();
		return ret;
	}

	public static IBrush load_MEF0(BufferedReader br) throws Exception
	{
		return load_MEF(br, "load_MEF0");
	}

	public static IBrush load_MEF1(BufferedReader br) throws Exception
	{
		return load_MEF(br, "load_MEF1");
	}

	public static IBrush load_MEF2(BufferedReader br) throws Exception
	{
		return load_MEF(br, "load_MEF2");
	}

	public static IBrush load_MEF3(BufferedReader br) throws Exception
	{
		return load_MEF(br, "load_MEF3");
	}

	/**
	This method exists so that we don't have to rewrite the saving code every time we have a new version of
	the MEF file format. It's effectively the same each time, except that the name of the save methods to call
	to save component brushes changes. We therefore pass the correct method name in as a parameter.
	*/
	public void save_MEF(PrintWriter pw, String methodName)
	{
		try
		{
			java.lang.reflect.Method saveMethod = ArchitectureBrush.class.getMethod(methodName, new Class[] { PrintWriter.class });

			if(m_transient)
			{
				for(Pair<ArchitectureBrush,BrushData> p: m_brushes) saveMethod.invoke(p.first, new Object[] { pw });
			}
			else
			{
				pw.println();
				pw.println("ArchitectureBrushComposite");
				pw.print("{");

				for(Pair<ArchitectureBrush,BrushData> p: m_brushes) saveMethod.invoke(p.first, new Object[] { pw });

				pw.println();
				pw.print("}");
			}
		}
		catch(Exception e)
		{
			throw new Error("An unexpected error occurred whilst trying to save: " + e.getMessage());
		}
	}

	public void save_MEF2(PrintWriter pw)
	{
		save_MEF(pw, "save_MEF2");
	}

	public void save_MEF3(PrintWriter pw)
	{
		save_MEF(pw, "save_MEF3");
	}

	//################## PROTECTED METHODS ##################//
	protected void begin_transform()
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.begin_transform();

		// Cache the existing brush for use during the transformation.

		m_cachedBoundingBox = m_boundingBox.clone();

		// Store the offsets of each brush's bounding box corners from the top-left of our cached bounding box.
		final Vector3d topLeft = m_cachedBoundingBox.get_bounds()[0];
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			for(int i=0; i<2; ++i)
			{
				p.second.offsets[i] = VectorUtil.subtract(p.first.m_boundingBox.get_bounds()[i], topLeft);
			}
		}
	}

	protected void end_transform()
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.end_transform();

		// Recalculate the composite bounding box.
		recalculate_bounds();
	}

	protected Properties get_properties()
	{
		// Note:	This function will never actually be called, since get_properties() is only
		//			ever invoked on component brushes from within ArchitectureBrush. The only
		//			reason it's implemented here at all is to satisfy an interface requirement.
		throw new UnsupportedOperationException();
	}

	protected void render_bounds(IRenderer renderer)
	{
		if(m_boundingBox != null)
		{
			renderer.set_colour(m_transient ? Color.orange : Color.red);
			super.render_bounds(renderer);
		}
	}

	protected void resize(Vector2d corner0, Vector2d corner1, AxisPair ap)
	{
		// Generate our resized brush from the brush we cached in begin_transform.

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

		// Determine where the corners of the cached brush would be on this canvas.
		Vector2d cachedCorners[] = new Vector2d[2];
		for(int i=0; i<2; ++i) cachedCorners[i] = ap.select_components(m_cachedBoundingBox.get_bounds()[i]);

		// Calculate the scaling factors for the corner offsets.
		Vector2d scale = new Vector2d((corner1.x-corner0.x)/(cachedCorners[1].x-cachedCorners[0].x),
									(corner1.y-corner0.y)/(cachedCorners[1].y-cachedCorners[0].y));

		for(Pair<ArchitectureBrush,BrushData> p: m_brushes)
		{
			// Calculate the new corners for this brush.
			Vector2d[] newCorners = new Vector2d[2];
			for(int i=0; i<2; ++i)
			{
				// We scale the cached offsets to get the offsets from the top-left of our new
				// brush, then add the top-left of the new brush to get the actual corners.
				newCorners[i] = ap.select_components(p.second.offsets[i]);
				newCorners[i].x*=scale.x;
				newCorners[i].y*=scale.y;
				newCorners[i].add(corner0);
			}
			p.first.resize(newCorners[0], newCorners[1], ap);
		}

		m_boundingBox.resize(corner0, corner1, ap);
	}

	protected void rotate(Vector2d centre, double angle, AxisPair ap)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.rotate(centre, angle, ap);
	}

	protected void set_properties(Properties properties)
	{
		// Note:	This function will never actually be called, since get_properties() is only
		//			ever invoked on component brushes from within ArchitectureBrush. The only
		//			reason it's implemented here at all is to satisfy an interface requirement.
		throw new UnsupportedOperationException();
	}

	protected void shear_horizontal(Vector2d anchor, double factor, AxisPair ap)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.shear_horizontal(anchor, factor, ap);
	}

	protected void translate(Vector3d trans)
	{
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) p.first.translate(trans);
		m_boundingBox = m_cachedBoundingBox.clone();
		m_boundingBox.translate(trans);
	}

	//################## PRIVATE METHODS ##################//
	private void extend_bounds_with(ArchitectureBrush b)
	{
		if(m_boundingBox == null)
		{
			m_boundingBox = b.m_boundingBox.clone();
		}
		else
		{
			Vector3d[] extraCorners = b.m_boundingBox.get_bounds();
			Vector3d[] existingCorners = m_boundingBox.get_bounds();

			Vector3d[] newCorners = new Vector3d[2];
			newCorners[0] = min_corner(extraCorners[0],existingCorners[0]);
			newCorners[1] = max_corner(extraCorners[1],existingCorners[1]);

			m_boundingBox.resize(newCorners[0],newCorners[1]);
		}
	}

	/**
	Returns an iterable of the brushes in the composite.

	@return	An iterable of the brushes as specified
	*/
	private Iterable<ArchitectureBrush> get_brushes()
	{
		return new Iterable<ArchitectureBrush>()
		{
			public Iterator<ArchitectureBrush> iterator()
			{
				return new Iterator<ArchitectureBrush>()
				{
					private Iterator<Pair<ArchitectureBrush,BrushData>> m_it = m_brushes.iterator();

					public boolean hasNext()
					{
						return m_it.hasNext();
					}

					public ArchitectureBrush next()
					{
						return m_it.next().first;
					}

					public void remove()
					{
						throw new UnsupportedOperationException();
					}
				};
			}
		};
	}

	private static Vector3d max_corner(Vector3d c1, Vector3d c2)
	{
		Vector3d max = new Vector3d();
		if(c1.x > c2.x) max.x = c1.x;
		else max.x = c2.x;
		if(c1.y > c2.y) max.y = c1.y;
		else max.y = c2.y;
		if(c1.z > c2.z) max.z = c1.z;
		else max.z = c2.z;
		return max;
	}

	private static Vector3d min_corner(Vector3d c1, Vector3d c2)
	{
		Vector3d min = new Vector3d();
		if(c1.x < c2.x) min.x = c1.x;
		else min.x = c2.x;
		if(c1.y < c2.y) min.y = c1.y;
		else min.y = c2.y;
		if(c1.z < c2.z) min.z = c1.z;
		else min.z = c2.z;
		return min;
	}

	/**
	Recalculate the composite bounding box.
	*/
	private void recalculate_bounds()
	{
		m_boundingBox = null;
		for(Pair<ArchitectureBrush,BrushData> p: m_brushes) extend_bounds_with(p.first);
	}
}
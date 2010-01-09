package MapEditor.GUI;

import MapEditor.Brushes.*;
import MapEditor.Commands.*;
import MapEditor.Event.*;
import MapEditor.Geom.*;
import MapEditor.Geom.Planar.*;
import MapEditor.Graphics.*;
import MapEditor.Maps.*;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import java.awt.*;
import java.awt.event.*;

public class DesignCanvas extends Canvas implements BrushConstants, IRepaintListener, IScrollable, IStatusSource<String>
{
	// Note: There are three coordinate systems active in a design canvas:
	// Coords:	The position of a point, in level coordinates
	// Pixels:	The position of a point, in pixels, on the canvas (measured from the top-left of the canvas)
	// Scrolls: Similar to Coords, but may have one or both axes inverted (examine m_hAxisSign and m_vAxisSign to tell)
	// We're going to use the suffices _Coords, _Pixels and _Scrolls on the names of coordinate variables to make things
	// as clear as possible.
	// Note that in the rest of the program, everything is in Coords (unless explicitly stated otherwise).

	//################## CONSTANTS ##################//
	final private static int AXIS_H = 0;
	final private static int AXIS_V = 1;
	final private static double MAX_ZOOM_LEVEL = 64;		// the maximum zoom level
	final private static double MIN_GRID_SIZE_PIXELS = 4;	// the minimum grid size in pixels
	final private static double MIN_ZOOM_LEVEL = 0.125;		// the minimum zoom level
	final private static double ZOOM_FACTOR = 1.25;			// the zoom in factor (the zoom out factor is the inverse of this)

	//################## PRIVATE VARIABLES ##################//
	// General stuff (which is primarily to do with how we do our rendering)
	private boolean m_bHasFocus = false;					// whether or not the canvas has the focus
	private Dimension m_dimension = null;					// the dimensions of the viewable area (in Pixels)
	private double m_gridSize_Coords = 16;					// the (fine) grid size in Coords
	private double m_zoomLevel = 1;							// the ratio of Pixels : Coords
	private int m_visibleHeight;							// the height of the viewable area (in Coords)
	private int m_visibleWidth;								// the width of the viewable area (in Coords)
	private Map m_map;										// the map we're editing using this canvas
	private Vector2d m_topLeft_Coords = new Vector2d();		// the top left of the canvas (in Coords)

	// AxisPair stuff
	private AxisPair m_axisPair;							// the (r,d) axis pair for this canvas
	private int m_vAxisSign;								// 1 if the d-axis increases downwards, -1 if not
	private int m_hAxisSign;								// 1 if the r-axis increases to the right, -1 if not

	// Double-buffering stuff
	private Dimension m_backDimension;
	private Graphics m_backGraphics;
	private Image m_backImage;

	// Initialisation - has this canvas already been initialised?
	// Note that when we say initialisation, we are not referring to the work done in the constructor
	// in this instance, we're talking about the initialisation done after things like the canvas's
	// size have been properly determined, namely the code in initialise().
	private boolean m_bInitialised = false;

	// Mouse stuff
	private Vector2d m_mouseAnchor_Pixels;
	private Vector2d m_snappedMouseAnchor_Pixels;
	private Vector2d m_mouseAnchor_Coords;
	private Vector2d m_snappedMouseAnchor_Coords;

	// Scrollbar stuff
	private Scrollbar m_horiz = null;
	private Scrollbar m_vert = null;

	// Status stuff
	private StatusListenerComposite<String> m_statusListener = new StatusListenerComposite<String>();
	private String m_status = "";

	// Renderer stuff
	private IRenderer m_renderer = new IRenderer()
	{
		public Vector2d add_pixel_offset(final Vector2d p_Coords, double dx_Pixels, double dy_Pixels)
		{
			Vector2d p_Pixels = coords_to_pixels(p_Coords);
			p_Pixels.x += dx_Pixels;
			p_Pixels.y += dy_Pixels;
			return pixels_to_coords(p_Pixels);
		}

		public int axis_h()
		{
			return AXIS_H;
		}

		public int axis_v()
		{
			return AXIS_V;
		}

		public double determine_nearest_grid_size_multiple_in_coords(final double size)
		{
			return Math.round(size/m_gridSize_Coords)*m_gridSize_Coords;
		}

		/**
		Returns the square of the distance (in Pixels) between the mappings of the two
		specified points onto the canvas. Or, to give an operational specification, this
		method converts both points in Coords into points in Pixels using the relevant
		method from the canvas and then calculates the square of the distance between
		the points in Pixels.

		@param p1_Coords	The first point
		@param p2_Coords	The second point
		@return				The square of the distance between their conversions into Pixels as a double
		*/
		public double distance_squared(final Vector2d p1_Coords, final Vector2d p2_Coords)
		{
			Vector2d p1_Pixels = coords_to_pixels(p1_Coords);
			Vector2d p2_Pixels = coords_to_pixels(p2_Coords);
			double x = p1_Pixels.x - p2_Pixels.x;
			double y = p1_Pixels.y - p2_Pixels.y;
			return x*x + y*y;
		}

		public double distance_squared_from_linesegment(final Vector2d p_Coords, final Vector2d e1_Coords, final Vector2d e2_Coords)
		{
			double distanceSquared_Coords = GeomUtil.distance_squared_from_linesegment(p_Coords, e1_Coords, e2_Coords);
			return distanceSquared_Coords*m_zoomLevel*m_zoomLevel;
		}

		public void draw_line(final Vector2d p1_Coords, final Vector2d p2_Coords)
		{
			Vector2d p1_Pixels = coords_to_pixels(p1_Coords);
			Vector2d p2_Pixels = coords_to_pixels(p2_Coords);

			GraphicsUtil.draw_line(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public void draw_line(final Vector3d p1_3D, final Vector3d p2_3D)
		{
			Vector2d p1_Pixels = determine_canvas_location_in_pixels(p1_3D);
			Vector2d p2_Pixels = determine_canvas_location_in_pixels(p2_3D);

			GraphicsUtil.draw_line(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public void draw_oval(final Vector2d p1_Coords, final Vector2d p2_Coords)
		{
			Vector2d p1_Pixels = coords_to_pixels(p1_Coords);
			Vector2d p2_Pixels = coords_to_pixels(p2_Coords);

			GraphicsUtil.draw_oval(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public void draw_oval(final Vector3d p1_3D, final Vector3d p2_3D)
		{
			Vector2d p1_Pixels = determine_canvas_location_in_pixels(p1_3D);
			Vector2d p2_Pixels = determine_canvas_location_in_pixels(p2_3D);

			GraphicsUtil.draw_oval(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public void draw_polyline(final Vector2d[] ps_Coords)
		{
			int len = ps_Coords.length;
			int[] xPoints = new int[len];
			int[] yPoints = new int[len];
			for(int i=0; i<len; ++i)
			{
				Vector2d p_Pixels = coords_to_pixels(ps_Coords[i]);
				xPoints[i] = (int)Math.round(p_Pixels.x);
				yPoints[i] = (int)Math.round(p_Pixels.y);
			}
			m_backGraphics.drawPolyline(xPoints, yPoints, len);
		}

		public void draw_rectangle(final Vector2d p1_Coords, final Vector2d p2_Coords)
		{
			Vector2d p1_Pixels = coords_to_pixels(p1_Coords);
			Vector2d p2_Pixels = coords_to_pixels(p2_Coords);

			GraphicsUtil.draw_rectangle(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public void fill_oval(final Vector2d p1_Coords, final Vector2d p2_Coords)
		{
			Vector2d p1_Pixels = coords_to_pixels(p1_Coords);
			Vector2d p2_Pixels = coords_to_pixels(p2_Coords);

			GraphicsUtil.fill_oval(m_backGraphics, p1_Pixels.x, p1_Pixels.y, p2_Pixels.x, p2_Pixels.y);
		}

		public Vector2d find_nearest_grid_intersect_in_coords(final Vector2d p_Coords)
		{
			return DesignCanvas.this.find_nearest_grid_intersect_in_coords(p_Coords);
		}

		public AxisPair get_axis_pair()
		{
			return m_axisPair;
		}

		public Stroke get_stroke()
		{
			Graphics2D g2D = (Graphics2D)m_backGraphics;
			return g2D.getStroke();
		}

		public boolean is_inverted_axis(int axis)
		{
			return DesignCanvas.this.is_inverted_axis(axis);
		}

		public double pixel_distance(final double x, final double y)
		{
			double distance_Coords = Math.abs(y-x);
			return distance_Coords * m_zoomLevel;
		}

		public void set_colour(final Color c)
		{
			m_backGraphics.setColor(c);
		}

		public void set_cursor(final Cursor c)
		{
			DesignCanvas.this.setCursor(c);
		}

		public void set_stroke(final Stroke stroke)
		{
			Graphics2D g2D = (Graphics2D)m_backGraphics;
			g2D.setStroke(stroke);
		}
	};

	//################## PRIVATE METHODS ##################//
	/**
	Converts a size (e.g. the grid size) in Coords to a size in Pixels.

	@param coordSize	A size in Coords
	@return				The size in Pixels
	*/
	private double coord_size_to_pixel_size(double coordSize)
	{
		// Convert a size (e.g. the grid size) in Coords to a size in Pixels.
		return coordSize*m_zoomLevel;
	}

	/**
	Converts a point in Coords to a point in Pixels.

	@param p_Coords	The point in Coords
	@return	A Point containing the conversion of the specified point in Coords into Pixels
	*/
	private Vector2d coords_to_pixels(final Vector2d p_Coords)
	{
		double xOffset_Coords = m_hAxisSign*(p_Coords.x-m_topLeft_Coords.x);
		double yOffset_Coords = m_vAxisSign*(p_Coords.y-m_topLeft_Coords.y);
		return new Vector2d(coord_size_to_pixel_size(xOffset_Coords), coord_size_to_pixel_size(yOffset_Coords));
	}

	/**
	Converts a point in Coords to a point in Scrolls.

	@param p_Coords	The point in Coords
	@return	A Vector2d containing the conversion of the specified point in Coords into Scrolls
	*/
	private Vector2d coords_to_scrolls(final Vector2d p_Coords)
	{
		Vector2d dimensions_Coords = m_axisPair.select_components(m_map.dimensions());
		return new Vector2d(	m_hAxisSign == 1 ? p_Coords.x : dimensions_Coords.x - p_Coords.x,
							m_vAxisSign == 1 ? p_Coords.y : dimensions_Coords.y - p_Coords.y	);
	}

	/**
	Determines the 2D point (in Pixels) on this canvas to which the specified 3D point
	in Coords maps.

	@param p_3D		The 3D point in Coords
	@return			A Point containing the conversion of the specified 3D point in Coords into
					a 2D one in Pixels
	*/
	private Vector2d determine_canvas_location_in_pixels(final Vector3d p_3D)
	{
		Vector2d p_Coords = m_axisPair.select_components(p_3D);
		return coords_to_pixels(p_Coords);
	}

	/**
	Draws a grid of the specified colour with the specified size onto the canvas.

	@param c		The colour of the grid
	@param gridSize_Pixels	The size of the grid in Pixels
	*/
	private void draw_grid(final Color c, double gridSize_Pixels)
	{
		Graphics g = m_backGraphics;	// g is shorter to type!

		Vector2d gridOffset = find_nearest_grid_intersect_in_pixels(new Vector2d(gridSize_Pixels,gridSize_Pixels), gridSize_Pixels);

		gridOffset.x -= gridSize_Pixels;
		gridOffset.y -= gridSize_Pixels;

		g.setColor(c);
		for(double y=gridOffset.y; y<m_dimension.height; y += gridSize_Pixels)
		{
			int iY = (int)Math.round(y);
			g.drawLine(0, iY, m_dimension.width-1, iY);
		}
		for(double x=gridOffset.x; x<m_dimension.width; x += gridSize_Pixels)
		{
			int iX = (int)Math.round(x);
			g.drawLine(iX, 0, iX, m_dimension.height-1);
		}
	}

	/**
	Draws two grids onto the canvas - a fine grid and a larger grid. The larger grid
	has squares which are four times larger than those of the fine grid. We need
	both: the larger grid is there to make it easier to get the proportions of things
	right, whereas the fine grid is there so that we can edit things precisely. Of
	course, we can always adjust the grid size interactively, but having both grids
	is still helpful for editing purposes.
	*/
	private void draw_grids()
	{
		double gridSize_Pixels = coord_size_to_pixel_size(m_gridSize_Coords);

		// Draw the fine grid
		draw_grid(m_bHasFocus ? new Color(0,128,0) : new Color(0,0,128), gridSize_Pixels);

		// Draw the larger grid
		draw_grid(new Color(128,128,128), gridSize_Pixels*4);
	}

	private Vector2d find_nearest_grid_intersect_in_coords(final Vector2d p_Coords)
	{
		return find_nearest_grid_intersect_in_coords(p_Coords, m_gridSize_Coords);
	}

	private static Vector2d find_nearest_grid_intersect_in_coords(final Vector2d p_Coords, double gridSize_Coords)
	{
		// Note: Coords space is imagined to be such that x always increases to the right and y always
		// increases downwards. This explains the variable naming-scheme.

		// Note: Flooring the result of the division makes this work.
		double gridLineToLeft = ((int)(p_Coords.x/gridSize_Coords))*gridSize_Coords;
		double gridLineAbove = ((int)(p_Coords.y/gridSize_Coords))*gridSize_Coords;
		double vertGridLine = p_Coords.x-gridLineToLeft < gridSize_Coords/2 ? gridLineToLeft : gridLineToLeft+gridSize_Coords;
		double horizGridLine = p_Coords.y-gridLineAbove < gridSize_Coords/2 ? gridLineAbove : gridLineAbove+gridSize_Coords;
		return new Vector2d(vertGridLine, horizGridLine);
	}

	private Vector2d find_nearest_grid_intersect_in_pixels(final Vector2d p_Pixels)
	{
		Vector2d gi_Coords = find_nearest_grid_intersect_in_coords(pixels_to_coords(p_Pixels), m_gridSize_Coords);
		return coords_to_pixels(gi_Coords);
	}

	private Vector2d find_nearest_grid_intersect_in_pixels(final Vector2d p_Pixels, double gridSize_Pixels)
	{
		Vector2d gi_Coords = find_nearest_grid_intersect_in_coords(pixels_to_coords(p_Pixels), pixel_size_to_coord_size(gridSize_Pixels));
		return coords_to_pixels(gi_Coords);
	}

	/**
	Finds the nearest brush to the specified point, assuming there is one near enough. In this
	context, "nearest" means the one with the lowest selection metric, as returned by each brush's
	selection_metric method.

	@param p_Coords			The point mentioned above (generally speaking, where the user clicked on the canvas)
	@return					The nearest brush, if there was one, otherwise null
	@throws java.lang.Error	If p_Coords is null
	*/
	private IBrush find_nearest_nearby_brush_in_coords(final Vector2d p_Coords)
	{
		if(p_Coords == null) throw new java.lang.Error();

		// Brushes with metrics above this will definitely not be selected. As things
		// stand, the metric threshold is being calculated by working out how many
		// pixels from the centre we want the user to be able to click whilst still
		// selecting the brush, and squaring it. This relies on knowledge of how the
		// metric is calculated.
		final double METRIC_THRESHOLD = 36.0;

		double lowestMetric = Double.MAX_VALUE;
		IBrush brushWithLowestMetric = null;

		for(IBrush b: m_map.get_brushes())
		{
			double metric = b.selection_metric(p_Coords, m_renderer);
			if(metric <= METRIC_THRESHOLD)
			{
				if(metric < lowestMetric)
				{
					lowestMetric = metric;
					brushWithLowestMetric = b;
				}
			}
		}

		return brushWithLowestMetric;
	}

	/**
	Generate the bounding box for brushes created at the map's brush creation anchor.
	The second corner is offset from the first by a small amount on the current canvas,
	and its missing component is offset by one grid square to make manipulating the brush
	in the other canvases easier.

	@return	The bounding box described
	*/
	private AxisAlignedBox generate_brush_creation_bounds()
	{
		Vector3d tl_3D = m_map.get_brush_creation_anchor().clone();
		Vector3d br_3D = m_map.get_brush_creation_anchor().clone();

		// Add a small offset to the bottom-right corner of the bounding box, so that the box's dimensions
		// are all non-zero. Note that the offset being added is actually 2D, something which isn't necessarily
		// obvious at a first glance.
		m_axisPair.add_offset(br_3D, m_axisPair.select_components(new Vector3d(MINIMUM_BRUSH_DIMENSION, MINIMUM_BRUSH_DIMENSION, MINIMUM_BRUSH_DIMENSION)));

		m_axisPair.set_missing_component(br_3D, m_axisPair.get_missing_component(tl_3D)+m_gridSize_Coords);
		return new AxisAlignedBox(tl_3D, br_3D);
	}

	private void initialise()
	{
		m_dimension = getSize();
		update_scrollbars();

		if(m_vAxisSign == -1)
		{
			m_vert.setValue((int)Math.round(m_axisPair.select_components(m_map.dimensions()).y));
			set_vertical_scrollbar_value(m_vert.getValue());
		}
		if(m_hAxisSign == -1)
		{
			m_horiz.setValue((int)Math.round(m_axisPair.select_components(m_map.dimensions()).x));
			set_horizontal_scrollbar_value(m_horiz.getValue());
		}

		m_bInitialised = true;
	}

	/**
	Returns a boolean indicating whether or not the specified axis is inverted. In this instance, we say
	that the horizontal axis is inverted if values on it increase to the left and that the vertical axis
	is inverted if values on it increase upwards.

	@param axis				The axis we want to check for inversion (must be DesignCanvas.AXIS_H or
							DesignCanvas.AXIS_V)
	@return					A boolean indicating whether the specified axis is inverted
	@throws java.lang.Error	If the axis parameter is not one of the two values specified
	*/
	private boolean is_inverted_axis(int axis)
	{
		switch(axis)
		{
			case AXIS_H:
				return m_hAxisSign == -1;
			case AXIS_V:
				return m_vAxisSign == -1;
			default:
				throw new java.lang.Error();
		}
	}

	/**
	Converts a size (e.g. the grid size) in Pixels to a size in Coords.

	@param pixelSize	A size in Pixels
	@return				The size in Coords
	*/
	private double pixel_size_to_coord_size(double pixelSize)
	{
		return pixelSize/m_zoomLevel;
	}

	/**
	Converts a point in Pixels to a point in Coords.

	@param p_Pixels		The point in Pixels
	@return				A Vector2d containing the conversion of the specified point in Pixels into Coords
	*/
	private Vector2d pixels_to_coords(final Vector2d p_Pixels)
	{
		double xOffset_Coords = pixel_size_to_coord_size(p_Pixels.x);
		double yOffset_Coords = pixel_size_to_coord_size(p_Pixels.y);
		return new Vector2d(	xOffset_Coords/m_hAxisSign+m_topLeft_Coords.x,
							yOffset_Coords/m_vAxisSign+m_topLeft_Coords.y	);
	}

	/**
	Renders the brush creation anchor.
	*/
	private void render_brush_creation_anchor()
	{
		Graphics g = m_backGraphics;	// g is shorter to type!

		Vector2d p_Coords = m_axisPair.select_components(m_map.get_brush_creation_anchor());
		Vector2d p_Pixels = coords_to_pixels(p_Coords);

		g.setColor(Color.yellow);
		GraphicsUtil.draw_rectangle(g, p_Pixels.x-5, p_Pixels.y-5, p_Pixels.x+5, p_Pixels.y+5);
	}

	/**
	Renders the map we're editing using this canvas.
	*/
	private void render_map()
	{
		Color overrideColour = Options.is_set("Render In Unicolour") ? Color.red : null;

		IBrush selectedBrush = m_map.get_selected_brush();

		// Render all the brushes except for the currently selected brush (if there is one, obviously).
		for(IBrush b: m_map.get_brushes())
		{
			if(b != selectedBrush) b.render(m_renderer, overrideColour);
		}

		// Now render the selected brush. The point of rendering this after the other brushes is that
		// we want the selected brush to be unobscured, since the user is trying to edit it. Note also
		// that rendering the selected brush separately is essential because in the case of a SelectionBrush
		// it may not be part of the map.
		if(selectedBrush != null) selectedBrush.render_selected(m_renderer, overrideColour);

		if(Options.is_set("Render Brush Creation Anchor")) render_brush_creation_anchor();
	}

	/**
	Updates the brush creation anchor, using the centre of this canvas as the 2D point with which to update.
	*/
	private void update_brush_creation_anchor()
	{
		Vector2d p_Coords = new Vector2d(m_topLeft_Coords.x + m_hAxisSign * m_visibleWidth/2,
									   m_topLeft_Coords.y + m_vAxisSign * m_visibleHeight/2);
		if(Options.is_set("Snap To Grid")) p_Coords = find_nearest_grid_intersect_in_coords(p_Coords);
		update_brush_creation_anchor(p_Coords);
		m_map.repaint_full();
	}

	/**
	Updates the brush creation anchor using the specified 2D point. This involves setting the relevant components
	of the 3D point to those of the 2D one, whilst leaving the remaining component (the missing component from
	this canvas's axis pair) unchanged.

	@param p_Coords	The 2D point with which to update the 3D position of the anchor
	*/
	private void update_brush_creation_anchor(Vector2d p_Coords)
	{
		Vector3d brushCreationAnchor = m_map.get_brush_creation_anchor();
		m_axisPair.set_relevant_components(brushCreationAnchor, p_Coords);
		m_map.set_brush_creation_anchor(brushCreationAnchor);
	}

	private void update_scrollbars()
	{
		// Note that the values on the scrollbars are in Coords. Depending on the orientation of the 
		// axes, the values on the scrollbars may be the coordinates of the top-left of the canvas, or
		// a given coordinate component may be derived by subtracting the value on the scrollbar from
		// the appropriate map dimension. See set_horizontal_scrollbar_value and set_vertical_scrollbar_value for the
		// code involved.

		m_visibleHeight = (int)Math.round(pixel_size_to_coord_size(m_dimension.height));
		m_visibleWidth = (int)Math.round(pixel_size_to_coord_size(m_dimension.width));

		// If these are 0, we've called this method accidentally before the canvas has been properly
		// initialised.
		if(m_visibleHeight == 0) throw new java.lang.Error();
		if(m_visibleWidth == 0) throw new java.lang.Error();

		Vector2d dimensions_Coords = m_axisPair.select_components(m_map.dimensions());
		int maxHeight = (int)Math.round(dimensions_Coords.y);
		int maxWidth = (int)Math.round(dimensions_Coords.x);
		m_vert.setValues(m_vert.getValue(),m_visibleHeight,0,maxHeight);
		m_horiz.setValues(m_horiz.getValue(),m_visibleWidth,0,maxWidth);
	}

	private void update_status()
	{
		m_status = "[ Zoom: " + new Double(m_zoomLevel).floatValue() + " Grid Size: " + m_gridSize_Coords + " ]";
		m_statusListener.status_changed(m_status);
	}

	/**
	Performs a zoom centred on the specified point. If factor > 1, this
	means zooming in on the point in question. If 0 < factor < 1, this means
	zooming out from the point in question. (If factor == 1, this is a
	no-op.)
	
	<p><b>Preconditions:</b>
	<dl>
	<dd>factor > 0
	</dl>

	@param p_Scrolls		The point on which to centre the zoom, in scrollbar coordinates
	@param factor			The zoom factor, as described above
	@throws java.lang.Error	If factor <= 0
	*/
	private void zoom(Vector2d p_Scrolls, double factor)
	{
		/*
		Notes on the algorithm:

		Suppose we are seeing the area from (a,b) to (c,d). This is of size m_visibleWidth x m_visibleHeight,
		where m_visibleWidth = c-a and m_visibleHeight = d-b. After zooming, we want to see an area that is
		of size (m_visibleWidth / factor) x (m_visibleHeight / factor) and centred on p_Scrolls.

		The algorithm thus proceeds as follows:

		delta_Scrolls = <the offset from the top left of the new canvas to the centre of it, i.e. half the area of the new canvas>
		newValues_Scrolls = p_Scrolls - delta_Scrolls (i.e. the top left of the new canvas)
		Update the zoom level and set the new top left of the canvas from newValues_Scrolls
		*/

		if(factor <= 0) throw new java.lang.Error();

		// Don't allow the zoom level to get too small.
		if(m_zoomLevel*factor < MIN_ZOOM_LEVEL) return;

		// Don't allow the zoom level to get too large.
		if(m_zoomLevel*factor > MAX_ZOOM_LEVEL) return;

		// Don't allow the grid size to get too small (termination of the loop in draw_grid depends on
		// the grid size in pixels being strictly greater than 0).
		double potentialGridSize_Pixels = m_gridSize_Coords*m_zoomLevel*factor;
		if(potentialGridSize_Pixels < MIN_GRID_SIZE_PIXELS) return;

		// Don't allow the grid size to get too large (having a grid which can't be seen in the window
		// is pointless).
		if(potentialGridSize_Pixels > coord_size_to_pixel_size(m_visibleWidth/2) || potentialGridSize_Pixels > coord_size_to_pixel_size(m_visibleHeight/2)) return;

		// Main Algorithm
		Vector2d delta_Scrolls = new Vector2d(0.5 * m_visibleWidth / factor, 0.5 * m_visibleHeight / factor);
		Vector2d newValues_Scrolls = VectorUtil.subtract(p_Scrolls, delta_Scrolls);

		m_zoomLevel *= factor;
		update_scrollbars();

		m_horiz.setValue((int)Math.round(newValues_Scrolls.x));
		m_vert.setValue((int)Math.round(newValues_Scrolls.y));

		set_horizontal_scrollbar_value(m_horiz.getValue());
		set_vertical_scrollbar_value(m_vert.getValue());
	}

	//################## CONSTRUCTORS ##################//
	public DesignCanvas(Map map, AxisPair axisPair, int hAxisSign, int vAxisSign)
	{
		if(hAxisSign != 1 && hAxisSign != -1) throw new java.lang.Error();
		if(vAxisSign != 1 && vAxisSign != -1) throw new java.lang.Error();

		m_map = map;
		m_axisPair = axisPair;
		m_hAxisSign = hAxisSign;
		m_vAxisSign = vAxisSign;

		setBackground(Color.black);

		addFocusListener(new FocusAdapter()
		{
			public void focusGained(FocusEvent e)
			{
				m_bHasFocus = true;
				repaint();
			}

			public void focusLost(FocusEvent e)
			{
				m_bHasFocus = false;
				repaint();
			}
		});

		addKeyListener(new KeyAdapter()
		{
			public void keyPressed(KeyEvent e)
			{
				switch(e.getKeyCode())
				{
					case KeyEvent.VK_DOWN:
					{
						m_vert.setValue((int)Math.round(m_vert.getValue()+m_gridSize_Coords));
						set_vertical_scrollbar_value(m_vert.getValue());
						repaint();
						break;
					}
					case KeyEvent.VK_LEFT:
					{
						m_horiz.setValue((int)Math.round(m_horiz.getValue()-m_gridSize_Coords));
						set_horizontal_scrollbar_value(m_horiz.getValue());
						repaint();
						break;
					}
					case KeyEvent.VK_RIGHT:
					{
						m_horiz.setValue((int)Math.round(m_horiz.getValue()+m_gridSize_Coords));
						set_horizontal_scrollbar_value(m_horiz.getValue());
						repaint();
						break;
					}
					case KeyEvent.VK_UP:
					{
						m_vert.setValue((int)Math.round(m_vert.getValue()-m_gridSize_Coords));
						set_vertical_scrollbar_value(m_vert.getValue());
						repaint();
						break;
					}
					case KeyEvent.VK_ENTER:
					{
						m_map.deghost_selection();
						m_map.repaint_full();
						setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
						break;
					}
					case KeyEvent.VK_DELETE:
					{
						IBrush b = m_map.get_selected_brush();
						if(b != null)
						{
							CommandManager.instance().execute_command(BrushCommands.delete_brush(b, m_map));
							setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
						}
						break;
					}
				}
			}

			public void keyTyped(KeyEvent e)
			{
				switch(e.getKeyChar())
				{
					case '[':
					{
						// Don't allow the grid size to get too small (termination of the loop in draw_grid depends on
						// the grid size in pixels being strictly greater than 0).
						double potentialGridSize_Pixels = coord_size_to_pixel_size(m_gridSize_Coords/2);
						if(potentialGridSize_Pixels < MIN_GRID_SIZE_PIXELS) break;

						// Decrease the grid size, but don't let it become smaller than 1 Coord.
						if(m_gridSize_Coords >= 2) m_gridSize_Coords/=2;
						repaint();
						break;
					}
					case ']':
					{
						// Don't allow the grid size to get too large (having a grid which can't be seen in the window
						// is pointless).
						double potentialGridSize_Pixels = coord_size_to_pixel_size(m_gridSize_Coords*2);
						if(potentialGridSize_Pixels > coord_size_to_pixel_size(m_visibleWidth/2) ||
						   potentialGridSize_Pixels > coord_size_to_pixel_size(m_visibleHeight/2))
						{
							break;
						}
						
						// Increase the grid size.
						m_gridSize_Coords*=2;
						repaint();
						break;
					}
					case '-':
					{
						// We want to zoom out from the centre of the view. Because we've
						// centralised all the zooming code, all this involves is working
						// out where the centre of the canvas is in Scrolls and passing
						// it to the zoom method.
						Vector2d centre_Scrolls = new Vector2d(m_horiz.getValue() + m_visibleWidth/2.0, m_vert.getValue() + m_visibleHeight/2.0);
						zoom(centre_Scrolls, 1/ZOOM_FACTOR);

						repaint();
						break;
					}
					case '+':
					{
						// We want to zoom in on the centre of the view. Because we've
						// centralised all the zooming code, all this involves is working
						// out where the centre of the canvas is in Scrolls and passing
						// it to the zoom method.
						Vector2d centre_Scrolls = new Vector2d(m_horiz.getValue() + 0.5*m_visibleWidth, m_vert.getValue() + 0.5*m_visibleHeight);
						zoom(centre_Scrolls, ZOOM_FACTOR);

						repaint();
						break;
					}
				}
			}
		});

		addMouseListener(new MouseAdapter()
		{
			public void mousePressed(MouseEvent e)
			{
				m_mouseAnchor_Pixels = new Vector2d(e.getX(), e.getY());
				m_mouseAnchor_Coords = pixels_to_coords(m_mouseAnchor_Pixels);

				if(Options.is_set("Snap To Grid"))
				{
					m_snappedMouseAnchor_Pixels = find_nearest_grid_intersect_in_pixels(m_mouseAnchor_Pixels);
					m_snappedMouseAnchor_Coords = pixels_to_coords(m_snappedMouseAnchor_Pixels);
				}
				else
				{
					m_snappedMouseAnchor_Pixels = m_mouseAnchor_Pixels;
					m_snappedMouseAnchor_Coords = m_mouseAnchor_Coords;
				}

				switch(m_map.get_state())
				{
					case Map.STATE_CREATE:
					{
						// Update the brush creation anchor.
						update_brush_creation_anchor(m_snappedMouseAnchor_Coords);

						// Create a new brush of the current type using the bounding box
						// for brushes created at the map's brush creation anchor.

						IBrush b = null;
						switch(m_map.get_brush_creation_type())
						{
							case BLOCK:
							{
								b = PolyhedralBrushFactory.create_axis_aligned_block(generate_brush_creation_bounds());
								break;
							}
							case CYLINDER:
							{
								b = PolyhedralBrushFactory.create_cylinder(generate_brush_creation_bounds(), 32, m_axisPair);
								break;
							}
							case CONE:
							{
								b = PolyhedralBrushFactory.create_cone(generate_brush_creation_bounds(), 32, m_axisPair);
								break;
							}
							case UV_SPHERE:
							{
								int precision = 16;
								b = PolyhedralBrushFactory.create_uv_sphere(generate_brush_creation_bounds(), precision/2 - 1, precision, m_axisPair);
								break;
							}
							case LANDSCAPE:
							{
								b = new LandscapeBrush(generate_brush_creation_bounds());
								break;
							}
							case LIGHT:
							{
								b = new LightBrush(m_map.get_brush_creation_anchor());
								break;
							}
						}
						CommandManager.instance().execute_command(BrushCommands.select("Initial Brush Creation", b, m_map.get_selected_brush(), m_map));
						if(Options.is_set("Automatic Brush Deghosting")) m_map.deghost_selection();
						b.mouse_pressed(m_renderer, m_mouseAnchor_Coords, e.getButton(), false);

						// Note that the default edit state for brushes under
						// creation should be resize (anchored at the point of
						// creation).
						break;
					}
					case Map.STATE_EDITING:
					{
						IBrush b = m_map.get_selected_brush();

						// Forward it on to the currently selected brush to deal
						// with. If the brush still wants to be selected when
						// b.mouse_pressed returns (signified by a return value of
						// true), then we break; if not, the brush wants to deselect
						// itself as a result of the mouse click and we hence want
						// to fall through into the STATE_NORMAL case.
						if(b.mouse_pressed(m_renderer, m_mouseAnchor_Coords, e.getButton(), false)) break;
					}
					case Map.STATE_NORMAL:
					{
						// Update the brush creation anchor.
						update_brush_creation_anchor(m_snappedMouseAnchor_Coords);

						IBrush nearest = find_nearest_nearby_brush_in_coords(m_mouseAnchor_Coords);
						if(nearest != null)	// there was a nearby brush
						{
							CommandManager.instance().execute_command(BrushCommands.select("Selection", nearest, m_map.get_selected_brush(), m_map));
							nearest.mouse_pressed(m_renderer, m_mouseAnchor_Coords, e.getButton(), true);
						}
						else
						{
							IBrush b = new SelectionBrush(generate_brush_creation_bounds(), m_map.get_brushes());
							CommandManager.instance().execute_command(BrushCommands.select("Initial New Selection", b, m_map.get_selected_brush(), m_map));
							b.mouse_pressed(m_renderer, m_mouseAnchor_Coords, e.getButton(), false);
						}
						break;
					}
				}

				m_map.repaint_full();
			}

			public void mouseReleased(MouseEvent e)
			{
				switch(m_map.get_state())
				{
					case Map.STATE_EDITING:
					{
						// Forward it on to the currently selected brush to deal with.
						m_map.get_selected_brush().mouse_released();
						break;
					}
				}

				m_map.repaint_full();
			}
		});

		addMouseMotionListener(new MouseMotionAdapter()
		{
			public void mouseDragged(MouseEvent e)
			{
				switch(m_map.get_state())
				{
					case Map.STATE_EDITING:
					{
						// Forward it on to the currently selected brush to deal with.
						Vector2d p_Coords = pixels_to_coords(new Vector2d(e.getX(), e.getY()));
						m_map.get_selected_brush().mouse_dragged(m_renderer, p_Coords);
						break;
					}
				}

				m_map.repaint();
			}

			public void mouseMoved(MouseEvent e)
			{
				requestFocusInWindow();

				if(m_map.get_state() == Map.STATE_EDITING)
				{
					// Forward it on to the currently selected brush to deal with.
					Vector2d p_Coords = pixels_to_coords(new Vector2d(e.getX(), e.getY()));
					m_map.get_selected_brush().mouse_moved(m_renderer, p_Coords);
				}
			}
		});

		addMouseWheelListener(new MouseWheelListener()
		{
			public void mouseWheelMoved(MouseWheelEvent e)
			{
				/*
				// Scrolling around with the mouse
				m_vert.setValue((int)Math.round(m_vert.getValue()+e.getWheelRotation()*m_gridSize_Coords));
				set_vertical_scrollbar_value(m_vert.getValue());
				*/

				/*
				Zooming in and out with the mouse

				Things get moderately interesting at this point. By trying out the user interface and
				finding that it's not easy enough to zoom accurately, we creep up on the idea that
				instead of zooming on the point in question by making it central to the screen, what
				we actually want to do is zoom on it so that the mouse is still over it afterwards in
				case we want to continue zooming on it. This is somewhat more complicated, but makes
				the interface more user-friendly.

				The algorithm can be derived from the following (I've written it out more clearly on paper):

				We clicked at point p_Scrolls
				This is (dx,dy) from the top left of the canvas
				After zooming, we want it to be (dx/factor, dy/factor) from the top left of the zoomed canvas
				The size of the zoomed canvas is (m_visibleWidth/factor) x (m_visibleHeight/factor)
				The centre of the zoomed canvas is thus an offset of half that from the top left of the canvas

				We calculate that p_Scrolls - (dx/factor, dy/factor) = centre_Scrolls - 1/2(m_visibleWidth/factor, m_visibleHeight/factor)
				and hence that centre_Scrolls = p_Scrolls - (dx/factor, dy/factor) + 1/2(m_visibleWidth/factor, m_visibleHeight/factor)
				*/

				final double factor = e.getWheelRotation() < 0 ? ZOOM_FACTOR : 1/ZOOM_FACTOR;

				Vector2d p_Scrolls = coords_to_scrolls(pixels_to_coords(new Vector2d(e.getX(), e.getY())));
				Vector2d topLeft_Scrolls = coords_to_scrolls(m_topLeft_Coords);
				Vector2d delta_Scrolls = new Vector2d((p_Scrolls.x - topLeft_Scrolls.x)/factor, (p_Scrolls.y - topLeft_Scrolls.y)/factor);
				Vector2d centre_Scrolls = p_Scrolls.clone();
				centre_Scrolls.subtract(delta_Scrolls);		// determines the top left of the zoomed canvas
				centre_Scrolls.add(new Vector2d(0.5*m_visibleWidth/factor, 0.5*m_visibleHeight/factor));

				for(int i=0, count=Math.abs(e.getWheelRotation()); i<count; ++i) zoom(centre_Scrolls, factor);

				repaint();
			}
		});
	}

	//################## PUBLIC METHODS ##################//
	// Methods which override ones in the AWT
	public void paint(Graphics g)
	{
		update(g);
	}

	public void update(Graphics frontGraphics)
	{
		if(!m_bInitialised) initialise();

		update_status();

		m_dimension = getSize();
		update_scrollbars();

		if((m_backGraphics == null) ||
		   (m_dimension.width != m_backDimension.width) ||
		   (m_dimension.height != m_backDimension.height))
		{
			m_backDimension = m_dimension;
			m_backImage = createImage(m_dimension.width, m_dimension.height);
			m_backGraphics = m_backImage.getGraphics();
		}

		Graphics g = m_backGraphics;	// g is shorter to type!

		g.clearRect(0, 0, m_dimension.width, m_dimension.height);

		// Draw the design grids
		draw_grids();

		render_map();

		g.setColor(Color.yellow);
		g.drawString("(" + (m_hAxisSign == -1 ? "-" : "") + m_axisPair.get_horizontal_axis() + ","
						 + (m_vAxisSign == -1 ? "-" : "") + m_axisPair.get_vertical_axis() + ")", 10, 20);

		frontGraphics.drawImage(m_backImage, 0, 0, this);
	}

	// Repaint stuff
	public void repaint_full()
	{
		repaint();
	}

	// Scrollbar stuff
	public void set_horizontal_scrollbar_value(int i)
	{
		if(m_hAxisSign == 1) m_topLeft_Coords.x = i;
		else m_topLeft_Coords.x = m_axisPair.select_components(m_map.dimensions()).x - i;

		update_brush_creation_anchor();
	}

	public void set_scrollbars(Scrollbar vert, Scrollbar horiz)
	{
		m_vert = vert;
		m_horiz = horiz;
	}

	public void set_vertical_scrollbar_value(int i)
	{
		if(m_vAxisSign == 1) m_topLeft_Coords.y = i;
		else m_topLeft_Coords.y = m_axisPair.select_components(m_map.dimensions()).y - i;

		update_brush_creation_anchor();
	}

	// Status stuff
	public void add_status_listener(IStatusListener<String> listener)
	{
		m_statusListener.add(listener);
	}
}

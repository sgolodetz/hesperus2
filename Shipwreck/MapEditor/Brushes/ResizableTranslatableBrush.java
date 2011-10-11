package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.MathUtil;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Stroke;

/**
This is the base class for brushes which can be resized and translated.
*/
public abstract class ResizableTranslatableBrush extends TranslatableBrush implements BrushConstants, Constants
{
	//################## PROTECTED ENUMERATIONS ##################//
	protected enum Axis
	{
		NONE, X, Y
	}

	protected enum HandleState
	{
		RESIZE, ROTATE, SHEAR
	}

	//################## PROTECTED VARIABLES ##################//
	protected HandleState m_handleState = HandleState.RESIZE;	// the current handle type (we start off with resize handles)

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a ResizableTranslatableBrush whose initial state depends on whether or not it's
	a new brush. By "new", I mean one the user is creating, as opposed to one which we're
	loading in, one which is being constructed as a result of a split operation, etc.

	@param isNew	Is the brush a new one that the user is creating?
	*/
	public ResizableTranslatableBrush(boolean isNew)
	{
		super(isNew);
	}

	//################## PROTECTED ABSTRACT METHODS ##################//
	/**
	Resizes the brush on the axes given by the AxisPair, so that the 2D projection onto those axes of
	its axis-aligned bounding box has the specified corners. (This sounds complicated, but intuitively
	it does exactly what we would expect, namely resizing two of the dimensions of the brush whilst
	leaving the other dimension unchanged. The two dimensions which change are those of the canvas on
	which we're doing the resizing.) Exactly how we do this depends on the brush type.

	@param corner0	A corner of the 2D projection of the new axis-aligned bounding box
	@param corner1	The opposite corner to corner0
	@param ap	The axis-pair of the canvas on which we're resizing
	*/
	protected abstract void resize(Vector2d corner0, Vector2d corner1, AxisPair ap);

	//################## PUBLIC METHODS ##################//
	public void mouse_moved(IRenderer renderer, Vector2d p)
	{
		set_correct_cursor(renderer, p);
	}

	/**
	This is the event handler which deals with mouse button presses over the brush.
	The immediate flag tells us whether or not the brush is being transformed as
	it's being selected - in that case, we always want to translate rather than do
	anything else.

	@param renderer		The renderer associated with the canvas on which the mouse was pressed
	@param p			The location of the mouse press in level coordinates
	@param mouseButton	Which mouse button was pressed
	@param immediate	Is this the start of an immediate transformation? (see above description)
	@return				A boolean indicating whether or not the brush thinks it should still be selected after returning
	*/
	public boolean mouse_pressed(IRenderer renderer, Vector2d p, int mouseButton, boolean immediate)
	{
		switch(mouseButton)
		{
			case LEFT_BUTTON:
			{
				m_renderer = renderer;

				switch(m_state)
				{
					case INITIAL:
					{
						// Initially, users drag out the brush to the size they want. They use the 2nd corner (i.e.
						// the one opposite the 0th corner), because the 0th corner is located at the exact point of
						// creation and serves as the correct anchor for the resize.
						m_transformation = new HandleResizeTransformation(2);
						return true;
					}
					case IDLE:
					{
						if(immediate)
						{
							m_transformation = new TranslationTransformation(p);
							return true;
						}
						else
						{
							int nearbyHandle, nearbyEdge;
							if((nearbyHandle = find_nearby_handle(m_renderer, p)) != -1)
							{
								m_transformation = generate_handle_transformation(nearbyHandle);
								return true;
							}
							else if((nearbyEdge = find_nearby_edge(m_renderer, p)) != -1)
							{
								m_transformation = new EdgeResizeTransformation(nearbyEdge);
								return true;
							}
							else if(m_boundingBox.within_2D_bounds(p, m_renderer.get_axis_pair()))
							{
								m_transformation = new TranslationTransformation(p);
								return true;
							}
							else return false;
						}
					}
					default:
					{
						System.err.println("Brush pressed whilst in invalid state: " + m_state);
						return true;
					}
				}
			}
			case RIGHT_BUTTON:
			{
				right_mouse_pressed();
				return true;
			}
			default:
			{
				m_transformation = new NullTransformation();	// transformations can only be performed with the left mouse button
				return true;
			}
		}
	}

	//################## PROTECTED METHODS ##################//
	/**
	Clears the brush's state.
	*/
	protected void clear_state()
	{
		m_handleState = HandleState.RESIZE;
		super.clear_state();
	}

	/**
	Generates a transformation based on the current handle state and the
	specified handle index.

	@param handleIndex	The index of the handle being manipulated by the user
	*/
	protected Transformation generate_handle_transformation(int handleIndex)
	{
		return new HandleResizeTransformation(handleIndex);
	}

	/**
	Renders the projection of the brush's bounding box using the specified renderer. In the case of brushes
	which have handles (i.e. anything other than a purely translatable brush), this entails rendering the
	handles as well, since we only render a brush's bounds when it's selected.

	@param renderer	The renderer with which we want to render the brush's bounds
	*/
	protected void render_bounds(IRenderer renderer)
	{
		super.render_bounds(renderer);
		render_handles(renderer);
	}

	/**
	Renders the projection of the brush's bounding box using the specified renderer and stroke. In the case of
	brushes which have handles (i.e. anything other than a purely translatable brush), this entails rendering the
	handles as well, since we only render a brush's bounds when it's selected. (Note that the handles always get
	rendered using the default stroke, and that the stroke is always set to the default when the method returns.)

	@param renderer	The renderer with which we want to render the brush's bounds
	@param stroke	The stroke with which we want to render the brush's bounds
	*/
	protected void render_bounds(IRenderer renderer, Stroke stroke)
	{
		super.render_bounds(renderer, stroke);
		render_handles(renderer);
	}

	/**
	Renders the effects associated with the current transformation (if any).

	@param renderer	The renderer with which to render the effects
	*/
	final protected void render_transformation_effects(IRenderer renderer)
	{
		if(m_transformation != null) m_transformation.render_effects(renderer);
	}

	//################## PRIVATE METHODS ##################//
	/**
	Looks for an edge near to the specified point.

	@param renderer	The renderer associated with the canvas on which to check the proximity between the point and edges
	@param p		A point in level coordinates
	@return			The index of the nearby edge, if any, or -1 otherwise
	*/
	private int find_nearby_edge(IRenderer renderer, Vector2d p)
	{
		Vector2d[] cornerHandles = new Vector2d[4];
		for(int i=0; i<4; ++i) cornerHandles[i] = m_boundingBox.get_corner_handle(renderer.get_axis_pair(), i);

		for(int i=0; i<4; ++i)
		{
			int j = (i+1)%4;
			if(i%2 == 0)
			{
				double minX, maxX;
				if(cornerHandles[i].x < cornerHandles[j].x)	{ minX = cornerHandles[i].x; maxX = cornerHandles[j].x; }
				else										{ minX = cornerHandles[j].x; maxX = cornerHandles[i].x; }
				if(BrushUtil.near_edge_y(renderer, p, cornerHandles[i].y, minX, maxX)) return i;
			}
			else
			{
				double minY, maxY;
				if(cornerHandles[i].y < cornerHandles[j].y)	{ minY = cornerHandles[i].y; maxY = cornerHandles[j].y; }
				else										{ minY = cornerHandles[j].y; maxY = cornerHandles[i].y; }
				if(BrushUtil.near_edge_x(renderer, p, cornerHandles[i].x, minY, maxY)) return i;
			}
		}
		return -1;
	}

	/**
	Looks for a handle near to the specified point.

	@param renderer		The renderer associated with the canvas on which to check the proximity between the point and handles
	@param p			A point in level coordinates
	@return				The index of the nearby handle, if any, or -1 otherwise
	*/
	private int find_nearby_handle(IRenderer renderer, Vector2d p)
	{
		AxisPair ap = renderer.get_axis_pair();

		switch(m_handleState)
		{
			case RESIZE:
			case ROTATE:
			{
				for(int i=0; i<4; ++i)
				{
					Vector2d cornerHandle = m_boundingBox.get_corner_handle(ap, i);
					if(BrushUtil.near_handle(renderer, p, cornerHandle.x, cornerHandle.y)) return i;
				}
				break;
			}
			case SHEAR:
			{
				for(int i=0; i<4; ++i)
				{
					Vector2d edgeHandle = m_boundingBox.get_edge_handle(ap, i);
					if(BrushUtil.near_handle(renderer, p, edgeHandle.x, edgeHandle.y)) return i;
				}
				break;
			}
		}
		return -1;
	}

	/**
	Renders the handles which the user can grab to resize, rotate or shear the brush. Which
	handles are rendered depends on the handle state we're currently in.

	@param renderer	The renderer associated with the canvas onto which the handles are to be rendered
	*/
	private void render_handles(IRenderer renderer)
	{
		Color[] handleColours = new Color[] {Color.magenta,Color.cyan,Color.pink,Color.orange};

		AxisPair ap = renderer.get_axis_pair();

		switch(m_handleState)
		{
			case RESIZE:
			{
				for(int i=0; i<4; ++i)
				{
					Vector2d p = m_boundingBox.get_corner_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_square_handle(renderer, p);
				}
				break;
			}
			case ROTATE:
			{
				for(int i=0; i<4; ++i)
				{
					Vector2d p = m_boundingBox.get_corner_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_circular_handle(renderer, p);
				}
				break;
			}
			case SHEAR:
			{
				for(int i=0; i<4; ++i)
				{
					Vector2d p = m_boundingBox.get_edge_handle(ap, i);
					renderer.set_colour(handleColours[i]);
					BrushUtil.render_diamond_handle(renderer, p);
				}
				break;
			}
		}
	}

	/**
	The currently selected brush has the responsibility to decide what the cursor on the
	active canvas looks like. If the cursor is near one of the brush's handles, for
	instance, we want the cursor to be a resize cursor. This method decides which cursor
	should be set and sets it accordingly.

	@param renderer	The renderer associated with the canvas on which we want to set the cursor
	@param p_Coords	The location of the mouse in level coordinates on the canvas
	*/
	private void set_correct_cursor(IRenderer renderer, Vector2d p_Coords)
	{
		// FIXME: This function is a mess.

		renderer.set_cursor(new Cursor(Cursor.DEFAULT_CURSOR));

		AxisAlignedBox.Projection proj = m_boundingBox.project_to_2D_using(renderer.get_axis_pair());
		boolean hAxisInverted = renderer.is_inverted_axis(renderer.axis_h());
		boolean vAxisInverted = renderer.is_inverted_axis(renderer.axis_v());
		double left = hAxisInverted ? proj.m_corners[1].x : proj.m_corners[0].x;
		double right = hAxisInverted ? proj.m_corners[0].x : proj.m_corners[1].x;
		double top = vAxisInverted ? proj.m_corners[1].y : proj.m_corners[0].y;
		double bottom = vAxisInverted ? proj.m_corners[0].y : proj.m_corners[1].y;

		if(BrushUtil.near_edge_x(renderer, p_Coords, left, proj.m_corners[0].y, proj.m_corners[1].y))
			renderer.set_cursor(new Cursor(Cursor.W_RESIZE_CURSOR));
		if(BrushUtil.near_edge_x(renderer, p_Coords, right, proj.m_corners[0].y, proj.m_corners[1].y))
			renderer.set_cursor(new Cursor(Cursor.E_RESIZE_CURSOR));
		if(BrushUtil.near_edge_y(renderer, p_Coords, top, proj.m_corners[0].x, proj.m_corners[1].x))
			renderer.set_cursor(new Cursor(Cursor.N_RESIZE_CURSOR));
		if(BrushUtil.near_edge_y(renderer, p_Coords, bottom, proj.m_corners[0].x, proj.m_corners[1].x))
			renderer.set_cursor(new Cursor(Cursor.S_RESIZE_CURSOR));

		switch(m_handleState)
		{
			case RESIZE:
			{
				if(BrushUtil.near_handle(renderer, p_Coords, left, top))
					renderer.set_cursor(new Cursor(Cursor.NW_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, left, bottom))
					renderer.set_cursor(new Cursor(Cursor.SW_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, right, top))
					renderer.set_cursor(new Cursor(Cursor.NE_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, right, bottom))
					renderer.set_cursor(new Cursor(Cursor.SE_RESIZE_CURSOR));
				break;
			}
			case ROTATE:
			{
				if(BrushUtil.near_handle(renderer, p_Coords, left, top) ||
				   BrushUtil.near_handle(renderer, p_Coords, left, bottom) ||
				   BrushUtil.near_handle(renderer, p_Coords, right, top) ||
				   BrushUtil.near_handle(renderer, p_Coords, right, bottom))
				{
					renderer.set_cursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
				}
				break;
			}
			case SHEAR:
			{
				double midX = (left + right) / 2;
				double midY = (top + bottom) / 2;
				if(BrushUtil.near_handle(renderer, p_Coords, left, midY) || BrushUtil.near_handle(renderer, p_Coords, right, midY))
					renderer.set_cursor(new Cursor(Cursor.N_RESIZE_CURSOR));
				if(BrushUtil.near_handle(renderer, p_Coords, midX, top) || BrushUtil.near_handle(renderer, p_Coords, midX, bottom))
					renderer.set_cursor(new Cursor(Cursor.E_RESIZE_CURSOR));
				break;
			}
		}
	}

	//################## NESTED CLASSES ##################//
	private abstract class ResizeTransformation extends Transformation
	{
		protected Axis m_fixedAxis;				// if we're dragging the left edge, for instance, then Y is the fixed axis
		protected double m_fixedCoord;			// for edge-resizing: the coordinate on the fixed axis gets stored in this variable
		protected int m_handleIndex;			// the index of the handle we're using to resize (note that if we're doing edge-resizing,
												// there will still be such a handle, it's just that we only allow changes on one of the
												// axes)
		protected Vector2d m_fixedCorner;		// the location of the fixed corner (the one opposite to the one we're dragging around)
		protected Vector2d m_variableCorner;		// the current location of the variable corner (the one we're dragging around)
		protected Vector2d m_variableCornerOld;	// the original location of the variable corner at the start of the resize

		public ResizeTransformation()
		{
			begin_transform();
		}

		public void execute_command()
		{
			final AxisPair ap = m_renderer.get_axis_pair();
			final Vector2d fixedCorner = m_fixedCorner.clone();
			final Vector2d variableCorner = m_variableCorner.clone();
			final Vector2d variableCornerOld = m_variableCornerOld.clone();
			CommandManager.instance().execute_compressible_command(new Command("Resize")
				{
					public void execute()
					{
						begin_transform();
						resize(fixedCorner, variableCorner, ap);
						end_transform();
					}

					public void undo()
					{
						begin_transform();
						resize(fixedCorner, variableCornerOld, ap);
						end_transform();
					}
				},
				Pair.make_pair("Initial Brush Creation", "Brush Creation"),
				Pair.make_pair("Initial New Selection", "New Selection"));
		}

		public void transform(Vector2d p)
		{
			if(Options.is_set("Snap To Grid"))
			{
				p = m_renderer.find_nearest_grid_intersect_in_coords(p);
			}

			m_variableCorner = p.clone();

			// Handle edge-resizing.
			switch(m_fixedAxis)
			{
				case NONE:
				{
					break;
				}
				case X:
				{
					m_variableCorner.x = m_fixedCoord;
					break;
				}
				case Y:
				{
					m_variableCorner.y = m_fixedCoord;
					break;
				}
			}

			// Prevent the dimensions from ever becoming smaller than a given amount.
			Vector2d dimensions = VectorUtil.subtract(m_variableCorner, m_fixedCorner);
			if(0 <= dimensions.x && dimensions.x < MINIMUM_BRUSH_DIMENSION) m_variableCorner.x = m_fixedCorner.x + MINIMUM_BRUSH_DIMENSION;
			else if(-MINIMUM_BRUSH_DIMENSION < dimensions.x && dimensions.x < 0) m_variableCorner.x = m_fixedCorner.x - MINIMUM_BRUSH_DIMENSION;
			if(0 <= dimensions.y && dimensions.y < MINIMUM_BRUSH_DIMENSION) m_variableCorner.y = m_fixedCorner.y + MINIMUM_BRUSH_DIMENSION;
			else if(-MINIMUM_BRUSH_DIMENSION < dimensions.y && dimensions.y < 0) m_variableCorner.y = m_fixedCorner.y - MINIMUM_BRUSH_DIMENSION;

			resize(m_fixedCorner, m_variableCorner, m_renderer.get_axis_pair());
			set_correct_cursor(m_renderer, p);
		}
	}

	private class EdgeResizeTransformation extends ResizeTransformation
	{
		/**
		Constructs an edge resize transformation for the brush.

		@param edgeIndex	The index of the edge with which we're going to resize
		*/
		public EdgeResizeTransformation(int edgeIndex)
		{
			AxisPair ap = m_renderer.get_axis_pair();

			if(edgeIndex % 2 == 0)
			{
				m_fixedAxis = Axis.X;
				m_fixedCoord = m_boundingBox.get_corner_handle(ap, edgeIndex).x;
			}
			else
			{
				m_fixedAxis = Axis.Y;
				m_fixedCoord = m_boundingBox.get_corner_handle(ap, edgeIndex).y;
			}
			m_handleIndex = edgeIndex;
			m_fixedCorner = m_boundingBox.get_opposite_corner_handle(ap, edgeIndex);
			m_variableCorner = m_boundingBox.get_corner_handle(ap, edgeIndex);
			m_variableCornerOld = m_boundingBox.get_corner_handle(ap, edgeIndex);
		}
	}

	final protected class HandleResizeTransformation extends ResizeTransformation
	{
		/**
		Constructs a handle resize transformation for the brush.

		@param handleIndex	The index of the handle with which we're going to resize
		*/
		public HandleResizeTransformation(int handleIndex)
		{
			AxisPair ap = m_renderer.get_axis_pair();

			m_fixedAxis = Axis.NONE;
			m_handleIndex = handleIndex;
			m_fixedCorner = m_boundingBox.get_opposite_corner_handle(ap, handleIndex);
			m_variableCorner = m_boundingBox.get_corner_handle(ap, handleIndex);
			m_variableCornerOld = m_boundingBox.get_corner_handle(ap, handleIndex);
		}
	}
}
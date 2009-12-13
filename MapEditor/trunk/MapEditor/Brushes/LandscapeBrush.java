package MapEditor.Brushes;

import MapEditor.Commands.*;
import MapEditor.Geom.AxisAlignedBox;
import MapEditor.Geom.AxisPair;
import MapEditor.Geom.Splines.*;
import MapEditor.Graphics.GraphicsUtil;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import MapEditor.Textures.*;
import java.awt.Color;
import java.awt.Cursor;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import net.java.games.jogl.*;

/**
This class represents landscapes made of cubic spline surfaces.
*/
public class LandscapeBrush extends ResizableTranslatableBrush
{
	//################## CONSTANTS ##################//
	final private static double MANIPULATOR_THRESHOLD = 36.0;	// you're over a manipulator if the cursor is within sqrt(36.0) = 6 pixels of it
	final private static int SUBDIVISIONS = 4;

	//################## PRIVATE ENUMERATIONS ##################//
	private enum LandscapeState
	{
		MODIFYING,		// we can modify the landscape heights
		NORMAL			// we can resize or translate the landscape as normal
	}

	//################## PRIVATE VARIABLES ##################//
	private boolean m_ghost;								// are we in the process of creating this brush in the design canvas?
	private boolean m_meshNeedsUpdating = true;				// does the mesh need updating?
	private double[][] m_heights;							// the heights of the grid of points we are interpolating
															// (between 0 and 1, where 0 means the bottom of the brush on the z axis and 1 means the top)
	private double m_oldManipulatorHeight;					// the height of the current manipulator before the user changes it
	private Pair<Integer,Integer> m_manipulator = null;		// the manipulator we're currently using to change the landscape heights, if any
	private Vector3d[][] m_mesh;								// the 3D mesh to render (generated using spline interpolation)
	private LandscapeState m_landscapeState = LandscapeState.NORMAL;

// TODO: Do landscape texturing properly.
	/* The name of the landscape's texture */
	private String m_texture = "LANDSCAPE";
	/* The landscape's texture plane */
	private TexturePlane m_texturePlane = new TexturePlane(new Vector3d(0,0,1), 1, 1, 0, 0, 0);

	//################## CONSTRUCTORS ##################//
	private LandscapeBrush()	// for internal use
	{
		super(false);
	}

	private LandscapeBrush(AxisAlignedBox bounds, boolean ghost, boolean isNew)
	{
		super(isNew);

		m_boundingBox = new BoundingBox(bounds);
		m_ghost = ghost;

		m_heights = new double[][]
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		};
	}

	public LandscapeBrush(AxisAlignedBox bounds)
	{
		this(bounds, true, true);
	}

	//################## PUBLIC METHODS ##################//
	public LandscapeBrush copy()
	{
		LandscapeBrush ret = new LandscapeBrush();

		final int cols = columns(), rows = rows();

		ret.m_boundingBox = m_boundingBox.clone();
		ret.m_ghost = false;
		ret.m_meshNeedsUpdating = true;
		ret.m_heights = new double[rows][cols];
		for(int r=0; r<rows; ++r)
		{
			for(int c=0; c<cols; ++c)
			{
				ret.m_heights[r][c] = m_heights[r][c];
			}
		}

		return ret;
	}

	/**
	Decreases the size of the grid and thus the number of heights the user can manipulate.
	It's sometimes useful to decrease the grid resolution to save space, to speed things up,
	or just because the extra precision isn't adding anything much, but it's a destructive
	operation in that landscape detail may be lost. The grid is decreased by removing every
	second row and column from the grid, starting one in from the edge. This implies that
	the landscape must be of size (2m+1)x(2n+1), for some integral m and n, if we are to
	decrease the resolution. Note that the decrease changes the size to (m+1)x(n+1), which
	may or may not be further reducible, depending on whether m and n are even or not.
	*/
	public void decrease_resolution()
	{
		final int cols = columns(), rows = rows();

		// Check that the grid can be reduced.
		if(rows % 2 == 0 || cols % 2 == 0) return;

		int newCols = cols/2 + 1, newRows = rows/2 + 1;
		final double[][] newHeights = new double[newRows][newCols];

		// One might note, were one doing things slightly more formally, that (newRows-1)*2 == newRows*2 - 2
		// == (rows div 2 + 1)*2 - 2 == ((rows-1)/2 + 1)*2 - 2 == rows-1 + 2 - 2 == rows-1, and similarly
		// for the columns, so the last row and column of the new grid are those of the old grid. This isn't
		// surprising, but is worth checking.
		for(int i=0; i<newRows; ++i)
		{
			for(int j=0; j<newCols; ++j)
			{
				newHeights[i][j] = m_heights[i*2][j*2];
			}
		}

		final double[][] oldHeights = m_heights;
		CommandManager.instance().execute_command(new Command("Decrease Resolution")
		{
			public void execute()
			{
				m_heights = newHeights;

				m_manipulator = null;
				m_meshNeedsUpdating = true;
			}

			public void undo()
			{
				m_heights = oldHeights;

				m_manipulator = null;
				m_meshNeedsUpdating = true;
			}
		});
	}

	public void deghost(final IBrushContainer container)
	{
		if(m_ghost)
		{
			// This Command must be here rather than in the Commands.Command class because
			// we're referencing LandscapeBrush's private variable m_ghost, which isn't
			// accessible externally.
			CommandManager.instance().execute_compressible_command(new Command("Deghost")
			{
				public void execute()
				{
					// We've finished creating this brush.
					m_ghost = false;
					container.add_brush(LandscapeBrush.this);
				}

				public void undo()
				{
					m_ghost = true;

					// Note that the selected brush is always LandscapeBrush.this, since
					// at the point at which we call undo(), we're in the state resulting
					// from calling execute().
					container.ghost_selection();
				}
			},
			Pair.make_pair("Initial Brush Creation", "Initial Brush Creation"));
		}
	}

	public IBrush deselect(IBrushContainer container)
	{
		clear_state();
		m_landscapeState = LandscapeState.NORMAL;
		return null;
	}

	public void flip_x()
	{
		for(int i=0, rows=rows(); i<rows; ++i) MiscUtil.reverse_array(m_heights[i]);
		m_meshNeedsUpdating = true;
	}

	public void flip_y()
	{
		final int cols = columns(), rows = rows();

		// Note: In principle, this is no different from flip_x(), but in practice, the
		// two-dimensional array structure means it's more efficient to rewrite the
		// array-reversing code verbatim than to extract the relevant array, reverse it
		// and then update the heightmap.

		for(int j=0; j<cols; ++j)
		{
			for(int i=0; i<rows/2; ++i)
			{
				int k = rows - 1 - i;

				double temp = m_heights[i][j];
				m_heights[i][j] = m_heights[k][j];
				m_heights[k][j] = temp;
			}
		}

		m_meshNeedsUpdating = true;
	}

	public void flip_z()
	{
		final int cols = columns(), rows = rows();

		for(int i=0; i<rows; ++i)
		{
			for(int j=0; j<cols; ++j)
			{
				m_heights[i][j] = 1 - m_heights[i][j];
			}
		}

		m_meshNeedsUpdating = true;
	}

	/**
	Increases the size of the grid and thus the number of heights the user can manipulate.
	It's often useful to increase the grid resolution to add extra detail to a landscape.
	The grid is increased by adding an extra row half-way between each existing row, and
	similarly for columns. The new heights in the grid are calculated from the existing
	mesh. Thus increasing the resolution is a non-destructive operation(*) which preserves
	the existing mesh but subsequently allows the user finer control over the landscape.
	This sounds as if it would mitigate for increasing the resolution in general, but
	the obvious concerns about space/time efficiency apply. If the original landscape
	is of size m x n, the new landscape will be of size (2m-1)x(2n-1).

	(*) It is, however, worth noting that because heights are clamped to the range [0,1],
	adding a new control point in between two extrema will produce "unexpected" results
	in that the new control point will be clamped, rather than appearing where you'd
	expect (i.e. out of range). This is probably a FIXME at some point, but it's not
	a priority one.
	*/
	public void increase_resolution()
	{
		final int cols = columns(), rows = rows();

		// Don't let the user increase the resolution too much, it takes too long to
		// increase the resolution beyond a certain point (there are cubic algorithms
		// involved).
		if(rows >= 49 || cols >= 49) return;

		// Set up the constraints we're going to use.
		CubicBSpline.IConstraint xConstraint1 = new CubicBSpline.GradientConstraint(0)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
		};
		CubicBSpline.IConstraint xConstraint2 = new CubicBSpline.GradientConstraint(cols-1)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
		};
		CubicBSpline.IConstraint yConstraint1 = new CubicBSpline.GradientConstraint(0)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(0,1,0); }
		};
		CubicBSpline.IConstraint yConstraint2 = new CubicBSpline.GradientConstraint(rows-1)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(0,1,0); }
		};

		int newRows = 2*rows-1, newCols = 2*cols-1;
		Vector3d[][] interpolatedGrid = SplineUtil.construct_interpolating_mesh_ex(calculate_grid(), newRows, newCols,
																				  xConstraint1, xConstraint2,
																				  yConstraint1, yConstraint2);

		// If something went wrong with the interpolation, just bail out. Increasing the resolution
		// is nice, but if it's not doable, we'd rather not crash because of it.
		if(interpolatedGrid.length != newRows) return;

		double loZ = m_boundingBox.get_bounds()[0].z, hiZ = m_boundingBox.get_bounds()[1].z;
		final double[][] newHeights = new double[newRows][newCols];
		for(int i=0; i<newRows; ++i)
		{
			for(int j=0; j<newCols; ++j)
			{
				if(i % 2 == 0 && j % 2 == 0) newHeights[i][j] = m_heights[i/2][j/2];
				else
				{
					newHeights[i][j] = (interpolatedGrid[i][j].z-loZ)/(hiZ-loZ);

					// Clamp the new height to the required range if necessary.
					if(newHeights[i][j] < 0.0) newHeights[i][j] = 0.0;
					else if(newHeights[i][j] > 1.0) newHeights[i][j] = 1.0;
				}
			}
		}

		final double[][] oldHeights = m_heights;
		CommandManager.instance().execute_command(new Command("Increase Resolution")
		{
			public void execute()
			{
				m_heights = newHeights;

				m_manipulator = null;
				m_meshNeedsUpdating = true;
			}

			public void undo()
			{
				m_heights = oldHeights;

				m_manipulator = null;
				m_meshNeedsUpdating = true;
			}
		});
	}

	public boolean is_copyable()
	{
		return !is_ghost();
	}

	public boolean is_flippable()
	{
		return true;
	}

	public boolean is_ghost()
	{
		return m_ghost;
	}

	public void mouse_dragged(IRenderer renderer, Vector2d p)
	{
		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				if(m_manipulator != null)
				{
					if(Options.is_set("Snap To Grid"))
					{
						p = renderer.find_nearest_grid_intersect_in_coords(p);
					}

					AxisPair ap = renderer.get_axis_pair();

					Double z = null;
					if(ap.get_horizontal_axis() == AxisPair.Axis.Z) z = p.x;
					else if(ap.get_vertical_axis() == AxisPair.Axis.Z) z = p.y;

					double loZ = m_boundingBox.get_bounds()[0].z, hiZ = m_boundingBox.get_bounds()[1].z;
					if(z != null && z >= loZ && z <= hiZ)
					{
						m_heights[m_manipulator.first][m_manipulator.second] = (z-loZ)/(hiZ-loZ);
						m_meshNeedsUpdating = true;
					}
				}
				break;
			}
			case NORMAL:
			{
				super.mouse_dragged(renderer, p);
				break;
			}
		}
	}

	public void mouse_moved(IRenderer renderer, Vector2d p)
	{
		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				renderer.set_cursor(new Cursor(Cursor.DEFAULT_CURSOR));

				// Display a "+" cursor when we're over a manipulator.
				Vector2d[][] manipulators = calculate2D_grid(renderer.get_axis_pair());
				for(int i=0, rows=rows(); i<rows; ++i)
				{
					for(int j=0, cols=columns(); j<cols; ++j)
					{
						if(renderer.distance_squared(p, manipulators[i][j]) < MANIPULATOR_THRESHOLD)
						{
							renderer.set_cursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
						}
					}
				}
				break;
			}
			case NORMAL:
			{
				super.mouse_moved(renderer, p);
				break;
			}
		}
	}

	public boolean mouse_pressed(IRenderer renderer, Vector2d p, int mouseButton, boolean immediate)
	{
		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				switch(mouseButton)
				{
					case LEFT_BUTTON:
					{
						if(m_manipulator != null)
						{
							// If there's a manipulator already selected, we don't change to another one if we're still clicking near it.
							// This is to make it possible to accurately select manipulators which appear in the same place on a given canvas.
							// Consider that if we want to select a given manipulator, we can do it easily in the top-down view, and we don't
							// want it to change when we try and manipulate it in another view (as we must, since we can't modify landscape
							// heights in the top-down view).
							Vector2d curManipulator = calculate2D_grid_coordinates(m_manipulator.first, m_manipulator.second, renderer.get_axis_pair());

							if(renderer.distance_squared(p, curManipulator) >= MANIPULATOR_THRESHOLD)
							{
								m_manipulator = find_nearby_manipulator(renderer, p);
								if(m_manipulator != null) m_oldManipulatorHeight = m_heights[m_manipulator.first][m_manipulator.second];
							}
						}
						else
						{
							m_manipulator = find_nearby_manipulator(renderer, p);
							if(m_manipulator != null) m_oldManipulatorHeight = m_heights[m_manipulator.first][m_manipulator.second];
						}

						return true;
					}
					case RIGHT_BUTTON:
					{
						CommandManager.instance().execute_command(new Command("Leave Landscape Modification Mode")
						{
							public void execute()
							{
								m_landscapeState = LandscapeState.NORMAL;
							}

							public void undo()
							{
								m_landscapeState = LandscapeState.MODIFYING;
							}
						});
						return true;
					}
					default:
					{
						return true;
					}
				}
			}
			case NORMAL:
			{
				switch(mouseButton)
				{
					case RIGHT_BUTTON:
					{
						CommandManager.instance().execute_command(new Command("Enter Landscape Modification Mode")
						{
							public void execute()
							{
								m_landscapeState = LandscapeState.MODIFYING;
							}

							public void undo()
							{
								m_landscapeState = LandscapeState.NORMAL;
							}
						});
						return true;
					}
					default:
					{
						return super.mouse_pressed(renderer, p, mouseButton, immediate);
					}
				}
			}
			default:
			{
				// This will never happen, it's just here to keep the compiler happy.
				throw new java.lang.Error();
			}
		}
	}

	public void mouse_released()
	{
		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				if(m_manipulator != null)
				{
					final int i = m_manipulator.first, j = m_manipulator.second;
					final double oldManipulatorHeight = m_oldManipulatorHeight, newManipulatorHeight = m_heights[i][j];
					if(newManipulatorHeight != oldManipulatorHeight)	// don't bother with no-op height changes
					{
						CommandManager.instance().execute_command(new Command("Change Landscape Height")
						{
							public void execute()
							{
								m_heights[i][j] = newManipulatorHeight;
								m_meshNeedsUpdating = true;
							}

							public void undo()
							{
								m_heights[i][j] = oldManipulatorHeight;
								m_meshNeedsUpdating = true;
							}
						});
					}
				}
				break;
			}
			case NORMAL:
			{
				super.mouse_released();
				break;
			}
		}
	}

	public void render(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else if(m_ghost) renderer.set_colour(Color.white);
		else renderer.set_colour(Color.green);

		render_grid(renderer, calculate_grid());
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		if(overrideColour != null) renderer.set_colour(overrideColour);
		else if(m_ghost) renderer.set_colour(Color.white);
		else renderer.set_colour(Color.green);

		Vector3d[][] grid = calculate_grid();
		render_grid(renderer, grid);

		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				// TODO: Render a limited bounding box so that users can see the extents of the brush.
				render_manipulators(renderer, grid);
				break;
			}
			case NORMAL:
			{
				render_bounds(renderer);
				break;
			}
		}
	}

	public void render3D(GL gl, GLU glu)
	{
		render3D_mesh(gl, glu);
	}

	public void render3D_selected(GL gl, GLU glu)
	{
		// TODO: Do something to distinguish this from the unselected case, like changing the colour.
		render3D_mesh(gl, glu);

		switch(m_landscapeState)
		{
			case MODIFYING:
			{
				render3D_manipulators(gl, calculate_grid());
				break;
			}
			case NORMAL:
			{
				break;
			}
		}
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// The metric is simple: return the square of the nearest distance to a grid point or edge.

		Vector2d[][] grid2D = calculate2D_grid(renderer.get_axis_pair());

		double bestMetric = Double.MAX_VALUE;
		for(int i=0, rows=rows(); i<rows; ++i)
		{
			for(int j=0, cols=columns(); j<cols; ++j)
			{
				double metric = renderer.distance_squared(p, grid2D[i][j]);
				if(metric < bestMetric) bestMetric = metric;

				if(i+1 < rows)
				{
					if(!grid2D[i][j].equals(grid2D[i+1][j]))	// if the points are distinct on this canvas, i.e. there's an edge to check against
					{
						metric = renderer.distance_squared_from_linesegment(p, grid2D[i][j], grid2D[i+1][j]);
						if(metric < bestMetric) bestMetric = metric;
					}
				}
				if(j+1 < cols)
				{
					if(!grid2D[i][j].equals(grid2D[i][j+1]))	// if the points are distinct on this canvas, i.e. there's an edge to check against
					{
						metric = renderer.distance_squared_from_linesegment(p, grid2D[i][j], grid2D[i][j+1]);
						if(metric < bestMetric) bestMetric = metric;
					}
				}
			}
		}

		return bestMetric;
	}

	// Loading and Saving
	public static IBrush load_MEF1(BufferedReader br) throws IOException
	{
		String line;
		String[] tokens;

		LandscapeBrush b = new LandscapeBrush();

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
		b.m_boundingBox = new BoundingBox(corners[0], corners[1]);

		// Read the dimensions.
		line = br.readLine();
		tokens = line.split(" ", 0);
		if(tokens.length != 3 || !tokens[0].equals("Dimensions")) throw new IOException("Dimensions not found or corrupted");
		int rows = Integer.parseInt(tokens[1]), cols = Integer.parseInt(tokens[2]);
		b.m_heights = new double[rows][cols];

		// Read the heights.
		for(int i=0; i<rows; ++i)
		{
			line = br.readLine();
			tokens = line.split(" ", 0);

			if(tokens.length != cols) throw new IOException("Height data corrupted");

			for(int j=0; j<cols; ++j) b.m_heights[i][j] = Double.parseDouble(tokens[j]);
		}

		// Read the closing brace.
		br.readLine();

		return b;
	}

	public static IBrush load_MEF2(BufferedReader br) throws IOException
	{
		return load_MEF1(br);
	}

	public static IBrush load_MEF3(BufferedReader br) throws IOException
	{
		return load_MEF1(br);
	}

	public void save_MEF2(PrintWriter pw)
	{
		pw.println();
		pw.println("LandscapeBrush");
		pw.println("{");

		Vector3d[] bounds = m_boundingBox.get_bounds();
		pw.print("Bounds");
		for(int i=0; i<2; ++i) pw.print(" ( " + bounds[i].x + " " + bounds[i].y + " " + bounds[i].z + " )");
		pw.println();

		final int cols = columns(), rows = rows();

		pw.println("Dimensions " + rows + " " + cols);
		for(int i=0; i<rows; ++i)
		{
			for(int j=0; j<cols; ++j)
			{
				pw.print(m_heights[i][j]);
				if(j < cols-1) pw.print(" ");
			}
			pw.println();
		}

		pw.print("}");
	}

	//################## PROTECTED METHODS ##################//
	protected void resize(Vector2d corner0, Vector2d corner1, AxisPair ap)
	{
		m_boundingBox.resize(corner0, corner1, ap);

		m_meshNeedsUpdating = true;
	}

	protected void translate(Vector3d trans)
	{
		m_boundingBox = m_cachedBoundingBox.clone();
		m_boundingBox.translate(trans);

		m_meshNeedsUpdating = true;
	}

	//################## PRIVATE METHODS ##################//
	/**
	Calculates all the 3D grid points.

	@return	...think about it...
	*/
	private Vector3d[][] calculate_grid()
	{
		int rows = rows();
		int columns = columns();

		Vector3d[][] grid = new Vector3d[rows][columns];
		for(int i=0; i<rows; ++i)
			for(int j=0; j<columns; ++j)
				grid[i][j] = calculate_grid_coordinates(i,j);

		return grid;
	}

	/**
	Calculates the 3D coordinates of the (i,j) grid point.

	@param i	The row of the grid point
	@param j	The column of the grid point
	@return		The 3D coordinates of the (i,j) grid point as a Vector3d
	*/
	private Vector3d calculate_grid_coordinates(int i, int j)
	{
		double x = j*size_x()/(columns()-1) + m_boundingBox.get_bounds()[0].x;
		double y = i*size_y()/(rows()-1) + m_boundingBox.get_bounds()[0].y;
		double z = m_heights[i][j]*size_z() + m_boundingBox.get_bounds()[0].z;
		return new Vector3d(x, y, z);
	}

	/**
	Calculates all the 2D grid points.

	@return	...think about it...
	*/
	private Vector2d[][] calculate2D_grid(AxisPair ap)
	{
		final int cols = columns(), rows = rows();

		Vector3d[][] grid = calculate_grid();

		Vector2d[][] grid2D = new Vector2d[rows][cols];
		for(int i=0; i<rows; ++i)
		{
			for(int j=0; j<cols; ++j)
			{
				grid2D[i][j] = ap.select_components(grid[i][j]);
			}
		}

		return grid2D;
	}

	/**
	Calculates the 2D coordinates of the (i,j) grid point.

	@param i	The row of the grid point
	@param j	The column of the grid point
	@return		The 2D coordinates of the (i,j) grid point as a Vector2d
	*/
	private Vector2d calculate2D_grid_coordinates(int i, int j, AxisPair ap)
	{
		return ap.select_components(calculate_grid_coordinates(i, j));
	}

	/**
	Returns the number of columns in the mesh. Note that "columns" are always in the x direction.
	*/
	private int columns()
	{
		return m_heights[0].length;
	}

	/**
	Looks for a manipulator near to the specified point.

	@param renderer		The renderer associated with the canvas on which to check the proximity between the point and manipulators
	@param p			A point in level coordinates
	@return				The (row, column) index pair of the nearby manipulator, if any, or null otherwise
	*/
	private Pair<Integer,Integer> find_nearby_manipulator(IRenderer renderer, Vector2d p)
	{
		Vector2d[][] grid2D = calculate2D_grid(renderer.get_axis_pair());

		for(int i=0, rows=rows(); i<rows; ++i)
		{
			for(int j=0, cols=columns(); j<cols; ++j)
			{	
				if(renderer.distance_squared(p, grid2D[i][j]) < MANIPULATOR_THRESHOLD) return Pair.make_pair(i, j);
			}
		}

		return null;
	}

	/**
	Recalculates the mesh based on the new heights.
	*/
	private void recalculate_mesh()
	{
		final int cols = columns(), rows = rows();

		// Set up the constraints we're going to use.
		CubicBSpline.IConstraint xConstraint1 = new CubicBSpline.GradientConstraint(0)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
		};
		CubicBSpline.IConstraint xConstraint2 = new CubicBSpline.GradientConstraint(cols-1)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(1,0,0); }
		};
		CubicBSpline.IConstraint yConstraint1 = new CubicBSpline.GradientConstraint(0)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(0,1,0); }
		};
		CubicBSpline.IConstraint yConstraint2 = new CubicBSpline.GradientConstraint(rows-1)
		{
			public Vector3d determine_value(Vector3d[] data) { return new Vector3d(0,1,0); }
		};

		int meshHeight = (rows-1)*SUBDIVISIONS+1;
		int meshWidth = (cols-1)*SUBDIVISIONS+1;
		m_mesh = SplineUtil.construct_interpolating_mesh(calculate_grid(), meshHeight, meshWidth,
														 xConstraint1, xConstraint2,
														 yConstraint1, yConstraint2);
	}

	/**
	Renders a grid in 2D using the specified renderer.

	<p><b>Preconditions:</b>
	<dl>
	<dd>grid must be a rectangular two-dimensional array
	</dl>

	@param renderer	The renderer with which to render the grid
	@param grid		The grid to render
	*/
	private void render_grid(IRenderer renderer, Vector3d[][] grid)
	{
		for(int i=0, height=grid.length; i<height; ++i)
		{
			// Note: It's safe to say grid[0].length without checking, since we only get here if grid.length > 0.
			for(int j=0, width=grid[0].length; j<width; ++j)
			{
				if(i+1 < height) renderer.draw_line(grid[i][j], grid[i+1][j]);
				if(j+1 < width) renderer.draw_line(grid[i][j], grid[i][j+1]);
			}
		}
	}

	/**
	Renders the manipulators used to modify the landscape heights in 2D.

	<p><b>Preconditions:</b>
	<dl>
	<dd>grid must be a rectangular two-dimensional array
	</dl>

	@param renderer	The renderer with which to render the grid
	@param grid		The grid in which the manipulators lie
	*/
	private void render_manipulators(IRenderer renderer, Vector3d[][] grid)
	{
		AxisPair ap = renderer.get_axis_pair();

		renderer.set_colour(Color.cyan);

		for(int i=0, height=grid.length; i<height; ++i)
		{
			for(int j=0, width=grid[0].length; j<width; ++j)
			{
				// Don't render the currently selected manipulator, if any, we'll render it afterwards in another style to differentiate it.
				if(m_manipulator != null && m_manipulator.first == i && m_manipulator.second == j) continue;

				Vector2d p = ap.select_components(grid[i][j]);
				BrushUtil.draw_dot(renderer, p);
			}
		}

		if(m_manipulator != null)
		{
			renderer.set_colour(Color.red);
			Vector2d p = ap.select_components(grid[m_manipulator.first][m_manipulator.second]);
			BrushUtil.draw_dot(renderer, p);
		}
	}

	/**
	Renders the underlying grid mesh in 3D using the specified OpenGL context.

	@param gl	The OpenGL context with which to render the mesh
	*/
	private void render3D_grid(GL gl)
	{
		render3D_mesh(gl, calculate_grid());
	}

	/**
	Renders the manipulators used to modify the landscape heights in 3D.

	<p><b>Preconditions:</b>
	<dl>
	<dd>grid must be a rectangular two-dimensional array
	</dl>

	@param gl	The OpenGL context with which to render the manipulators
	@param grid	The grid in which the manipulators lie
	*/
	private void render3D_manipulators(GL gl, Vector3d[][] grid)
	{
		final double SIZE = 4;
		Vector3d delta = new Vector3d(SIZE/2, SIZE/2, SIZE/2), p1 = new Vector3d(), p2 = new Vector3d();

		gl.glColor3f(1.0f, 1.0f, 1.0f);

		for(int i=0, height=grid.length; i<height; ++i)
		{
			for(int j=0, width=grid[0].length; j<width; ++j)
			{
				// Don't render the currently selected manipulator, if any, we'll render it afterwards in another style to differentiate it.
				if(m_manipulator != null && m_manipulator.first == i && m_manipulator.second == j) continue;

				p1 = VectorUtil.subtract(grid[i][j], delta);
				p2 = VectorUtil.add(grid[i][j], delta);
				GraphicsUtil.draw_cuboid(gl, p1, p2);
			}
		}

		if(m_manipulator != null)
		{
			gl.glColor3f(1.0f, 0.0f, 0.0f);
			p1 = VectorUtil.subtract(grid[m_manipulator.first][m_manipulator.second], delta);
			p2 = VectorUtil.add(grid[m_manipulator.first][m_manipulator.second], delta);
			GraphicsUtil.draw_cuboid(gl, p1, p2);
		}
	}

	/**
	Renders the interpolated mesh in 3D using the specified OpenGL context.

	@param gl				The OpenGL context with which to render the mesh
	@param glu				The corresponding GLU context
	*/
	private void render3D_mesh(GL gl, GLU glu)
	{
		if(m_meshNeedsUpdating)
		{
			recalculate_mesh();
			m_meshNeedsUpdating = false;
		}

		if(Options.is_set("Render Textures")) render3D_textured_mesh(gl, glu, m_mesh);
		else render3D_mesh(gl, m_mesh);
	}

	/**
	Renders a mesh in 3D using the specified OpenGL context.

	@param gl	The OpenGL context with which to render the mesh
	@param mesh	The mesh points
	*/
	private void render3D_mesh(GL gl, Vector3d[][] mesh)
	{
		gl.glDisable(GL.GL_CULL_FACE);

		for(int i=0, rows=mesh.length; i<rows; ++i)
		{
			for(int j=0, columns=mesh[0].length; j<columns; ++j)
			{
				if(i+1 < rows && j+1 < columns)
				{
					gl.glBegin(GL.GL_TRIANGLE_STRIP);
						gl.glColor3f(1.0f, 0.0f, 0.0f);
						gl.glVertex3d(mesh[i][j].x, mesh[i][j].y, mesh[i][j].z);
						gl.glColor3f(0.0f, 1.0f, 0.0f);
						gl.glVertex3d(mesh[i+1][j].x, mesh[i+1][j].y, mesh[i+1][j].z);
						gl.glColor3f(0.0f, 0.0f, 1.0f);
						gl.glVertex3d(mesh[i][j+1].x, mesh[i][j+1].y, mesh[i][j+1].z);
						gl.glColor3f(1.0f, 0.0f, 0.0f);
						gl.glVertex3d(mesh[i+1][j+1].x, mesh[i+1][j+1].y, mesh[i+1][j+1].z);
					gl.glEnd();
				}
			}
		}

		gl.glEnable(GL.GL_CULL_FACE);
	}

	private void render3D_textured_mesh(GL gl, GLU glu, Vector3d[][] mesh)
	{
		gl.glDisable(GL.GL_CULL_FACE);

		Texture texture = TextureManager.instance().get_texture(m_texture);
		if(texture != null)		// if there's a texture to use, render a textured landscape
		{
			gl.glEnable(GL.GL_TEXTURE_2D);
			texture.bind(gl, glu);

			// Render the textured mesh itself.
			gl.glColor3f(1.0f, 1.0f, 1.0f);
			for(int i=0, rows=mesh.length; i<rows; ++i)
			{
				for(int j=0, columns=mesh[0].length; j<columns; ++j)
				{
					if(i+1 < rows && j+1 < columns)
					{
						gl.glBegin(GL.GL_TRIANGLE_STRIP);
							TextureCoord texCoords = m_texturePlane.calculate_coordinates(mesh[i][j]);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(mesh[i][j].x, mesh[i][j].y, mesh[i][j].z);

							texCoords = m_texturePlane.calculate_coordinates(mesh[i+1][j]);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(mesh[i+1][j].x, mesh[i+1][j].y, mesh[i+1][j].z);

							texCoords = m_texturePlane.calculate_coordinates(mesh[i][j+1]);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(mesh[i][j+1].x, mesh[i][j+1].y, mesh[i][j+1].z);

							texCoords = m_texturePlane.calculate_coordinates(mesh[i+1][j+1]);
							gl.glTexCoord2d(texCoords.u, texCoords.v);
							gl.glVertex3d(mesh[i+1][j+1].x, mesh[i+1][j+1].y, mesh[i+1][j+1].z);
						gl.glEnd();
					}
				}
			}

			gl.glDisable(GL.GL_TEXTURE_2D);
		}
		else					// otherwise, render a height-shaded green landscape
		{
// FIXME: This is a little messy.
			double loZ = m_boundingBox.get_bounds()[0].z, hiZ = m_boundingBox.get_bounds()[1].z;
			double height = hiZ - loZ;
			double offset = 0.25;

			for(int i=0, rows=mesh.length; i<rows; ++i)
			{
				for(int j=0, columns=mesh[0].length; j<columns; ++j)
				{
					if(i+1 < rows && j+1 < columns)
					{
						gl.glBegin(GL.GL_TRIANGLE_STRIP);
							gl.glColor3d(0.0, (mesh[i][j].z - loZ)/height + offset, 0.0);
							gl.glVertex3d(mesh[i][j].x, mesh[i][j].y, mesh[i][j].z);

							gl.glColor3d(0.0, (mesh[i+1][j].z - loZ)/height + offset, 0.0);
							gl.glVertex3d(mesh[i+1][j].x, mesh[i+1][j].y, mesh[i+1][j].z);

							gl.glColor3d(0.0, (mesh[i][j+1].z - loZ)/height + offset, 0.0);
							gl.glVertex3d(mesh[i][j+1].x, mesh[i][j+1].y, mesh[i][j+1].z);

							gl.glColor3d(0.0, (mesh[i+1][j+1].z - loZ)/height + offset, 0.0);
							gl.glVertex3d(mesh[i+1][j+1].x, mesh[i+1][j+1].y, mesh[i+1][j+1].z);
						gl.glEnd();
					}
				}
			}
		}

		gl.glEnable(GL.GL_CULL_FACE);
	}

	/**
	Returns the number of rows in the mesh. Note that "rows" are always in the y direction.

	@return	...think about it...
	*/
	private int rows()
	{
		return m_heights.length;
	}

	/**
	Returns the size of the brush on the x axis.

	@return	...think about it...
	*/
	private double size_x()
	{
		return m_boundingBox.get_bounds()[1].x - m_boundingBox.get_bounds()[0].x;
	}

	/**
	Returns the size of the brush on the y axis.

	@return	...think about it...
	*/
	private double size_y()
	{
		return m_boundingBox.get_bounds()[1].y - m_boundingBox.get_bounds()[0].y;
	}

	/**
	Returns the size of the brush on the z axis.

	@return	...think about it...
	*/
	private double size_z()
	{
		return m_boundingBox.get_bounds()[1].z - m_boundingBox.get_bounds()[0].z;
	}
}
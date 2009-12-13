package MapEditor.GUI;

import MapEditor.Brushes.*;
import MapEditor.Brushes.IBrush.PickResults;
import MapEditor.Commands.*;
import MapEditor.Event.IRepaintListener;
import MapEditor.Geom.Planar.Polygon;
import MapEditor.Maps.Map;
import MapEditor.Math.MathUtil;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.*;
import java.util.Timer;
import java.util.TimerTask;
import java.util.TreeMap;
import net.java.games.jogl.*;

/**
This class implements a 3D view of the map.
*/
public class View3D implements Constants, GLEventListener, IRepaintListener
{
	//################## CONSTANTS ##################//
	final private static double FAR = 4096.0;		// far clipping plane distance
	final private static double NEAR = 1.0;			// near clipping plane distance

	final private static int COMMAND_COUNT = 13;	// number of commands

	// Commands
	final private static int	MOVE_FORWARD = 0,
								MOVE_BACK = 1,
								STRAFE_LEFT = 2,
								STRAFE_RIGHT = 3,
								MOVE_UP = 4,
								MOVE_DOWN = 5,
								TWIST_LEFT = 6,
								TWIST_RIGHT = 7,
								TURN_LEFT = 8,
								TURN_RIGHT = 9,
								LOOK_DOWN = 10,
								LOOK_UP = 11,
								MULTISELECT = 12;

	// Map from keys to commands
	final private static java.util.Map<Integer,Integer> s_keyLookup = new TreeMap<Integer,Integer>();
	static
	{
		s_keyLookup.put(KeyEvent.VK_W,			MOVE_FORWARD);
		s_keyLookup.put(KeyEvent.VK_S,			MOVE_BACK);
		s_keyLookup.put(KeyEvent.VK_A,			STRAFE_LEFT);
		s_keyLookup.put(KeyEvent.VK_D,			STRAFE_RIGHT);
		s_keyLookup.put(KeyEvent.VK_Q,			MOVE_UP);
		s_keyLookup.put(KeyEvent.VK_E,			MOVE_DOWN);
		s_keyLookup.put(KeyEvent.VK_Z,			TWIST_LEFT);
		s_keyLookup.put(KeyEvent.VK_X,			TWIST_RIGHT);
		s_keyLookup.put(KeyEvent.VK_LEFT,		TURN_LEFT);
		s_keyLookup.put(KeyEvent.VK_RIGHT,		TURN_RIGHT);
		s_keyLookup.put(KeyEvent.VK_UP,			LOOK_DOWN);
		s_keyLookup.put(KeyEvent.VK_DOWN,		LOOK_UP);
		s_keyLookup.put(KeyEvent.VK_CONTROL,	MULTISELECT);
	}

	//################## PRIVATE VARIABLES ##################//
	private boolean[] m_keysDown = new boolean[COMMAND_COUNT];
	{
		for(int i=0; i<COMMAND_COUNT; ++i) m_keysDown[i] = false;
	}

	private Camera m_camera;
	private double m_fovY = Math.PI/3;				// y-axis field-of-view (angle in radians)
	private double m_maxX, m_maxY, m_minX, m_minY;	// coordinates of right, top, left and bottom clip planes
	private double m_width, m_height;				// the dimensions of the view
	private GLCanvas m_canvas;
	private Map m_map;
	private Point m_mouseLast = null;
	private Timer m_inputTimer = new Timer(true);

	//################## PRIVATE METHODS ##################//
	/**
	Calculates the real location in world space of the point clicked.

	@param p	The point clicked (in pixel coordinates)
	@return		The real location of the clicked point in world space as a Vector3d
	*/
	private Vector3d calculate_image_plane_point(final Point p)
	{
		double n = NEAR;
		double u = -((m_maxX - m_minX)*p.x/m_width + m_minX);
		double v = m_maxY - (m_maxY - m_minY)*p.y/m_height;
		Vector3d ret = m_camera.get_position().clone();
		ret = Vector3d.scale_add(n, m_camera.get_n(), ret);
		ret = Vector3d.scale_add(u, m_camera.get_u(), ret);
		ret = Vector3d.scale_add(v, m_camera.get_v(), ret);
		return ret;
	}

	/**
	Determine if any brush is intersected by the half-ray (defined by the camera position and
	a point on the image plane corresponding to where the user clicked) and return the nearest
	one and its associated picking results if any of them were intersected.

	@param imagePlanePoint	The point on the image plane referred to in the above specification
	@return					If a brush was intersected by the half-ray, then the return value is
							a pair, the first component of which is the nearest such brush and
							the second component of which is its associated picking results.
							If not, the return value is null.
	*/
	private Pair<IBrush,PickResults> pick_operation(final Vector3d imagePlanePoint)
	{
		Vector3d cameraPosition = m_camera.get_position();

		// v = imagePlanePoint - cameraPosition
		Vector3d v = VectorUtil.subtract(imagePlanePoint, cameraPosition);

		IBrush nearestBrush = null;
		PickResults nearestResults = null;
		double nearestDistanceSquared = Double.MAX_VALUE;
		for(IBrush b: m_map.get_brushes())
		{
			PickResults result = b.pick(cameraPosition, v);
			if(result != null)
			{
				double distanceSquared = cameraPosition.distance_squared(result.m_pickedPoint);
				if(distanceSquared < nearestDistanceSquared)
				{
					nearestBrush = b;
					nearestResults = result;
					nearestDistanceSquared = distanceSquared;
				}
			}
		}

		if(nearestBrush != null) return Pair.make_pair(nearestBrush, nearestResults);
		else return null;
	}

	/**
	Renders the map we're viewing.

	@param gl	The OpenGL object with which to render the map
	@param glu	The corresponding GLU object
	*/
	private void render_map(GL gl, GLU glu)
	{
		if(Options.is_set("Texture-Editing Mode"))
		{
			// Render all the brushes as unselected.
			for(IBrush b: m_map.get_brushes()) b.render3D(gl, glu);

			if(Options.is_set("Render Texture Mask"))
			{
				// Render the selected faces. Blend the selection highlight polygons with the texture underneath.
				gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
				gl.glEnable(GL.GL_BLEND);
				gl.glColor4f(0.0f, 1.0f, 0.0f, 0.25f);
				for(Polygon p: m_map.get_selected_faces())
				{
					Vector3d[] verts = p.get_vertices();
					gl.glBegin(GL.GL_POLYGON);
						for(int i=0, len=verts.length; i<len; ++i)
						{
							gl.glVertex3d(verts[i].x, verts[i].y, verts[i].z);
						}
					gl.glEnd();
					
				}
				gl.glDisable(GL.GL_BLEND);
			}

			// Render texture axes for the selected faces.
			for(Polygon p: m_map.get_selected_faces())
			{
				Pair<Vector3d,Vector3d> axes = p.get_texture_details().second.axes();

				Vector3d centre = VectorUtil.add(p.centre(), p.get_normal());

				Vector3d centrePLUSu = Vector3d.scale_add(7.5, axes.first, centre);
				Vector3d centrePLUSv = Vector3d.scale_add(7.5, axes.second, centre);

				gl.glBegin(GL.GL_LINES);
					gl.glColor3f(1.0f, 1.0f, 0.0f);	
					gl.glVertex3d(centre.x, centre.y, centre.z);
					gl.glVertex3d(centrePLUSu.x, centrePLUSu.y, centrePLUSu.z);

					gl.glColor3f(0.0f, 1.0f, 0.0f);	
					gl.glVertex3d(centre.x, centre.y, centre.z);
					gl.glVertex3d(centrePLUSv.x, centrePLUSv.y, centrePLUSv.z);
				gl.glEnd();
			}
		}
		else
		{
			IBrush selectedBrush = m_map.get_selected_brush();

			// Render all the brushes except for the currently selected brush (if there is one, obviously).
			for(IBrush b: m_map.get_brushes())
			{
				if(b != selectedBrush) b.render3D(gl, glu);
			}

			// Now render the selected brush. Note that rendering the selected brush separately is
			// essential because in the case of a SelectionBrush it may not be part of the map.
			if(selectedBrush != null) selectedBrush.render3D_selected(gl, glu);
		}
	}

	//################## CONSTRUCTORS ##################//
	public View3D(Map map)
	{
		m_map = map;

		reset_camera();

		GLCapabilities capabilities = new GLCapabilities();
		capabilities.setStencilBits(1);
		m_canvas = GLDrawableFactory.getFactory().createGLCanvas(capabilities);

		// Note:	This fixes the canvas flickering problem, at the cost of using a proprietary API
		//			which may be removed at a later date.
		sun.awt.SunToolkit sunToolkit = (sun.awt.SunToolkit)Toolkit.getDefaultToolkit();
		sunToolkit.disableBackgroundErase(m_canvas);

		m_canvas.addGLEventListener(this);

		m_canvas.addMouseListener(new MouseAdapter()
		{
			public void mousePressed(MouseEvent e)
			{
				Vector3d imagePlanePoint = calculate_image_plane_point(new Point(e.getX(), e.getY()));
				Pair<IBrush,PickResults> p = pick_operation(imagePlanePoint);
				if(Options.is_set("Texture-Editing Mode"))
				{
					Polygon pickedFace = (p != null) ? p.second.m_pickedFace : null;
					if(m_keysDown[MULTISELECT]) m_map.toggle_selected_face(pickedFace);
					else m_map.select_face(pickedFace);
					m_map.repaint_full();
				}
				else
				{
					final IBrush pickedBrush = (p != null) ? p.first : null;
					final IBrush selectedBrush = m_map.get_selected_brush();

// FIXME: Multiselection and deselection are using a lot of instanceof tests and casts at the moment. This may be fixable in the future, but at the moment
// I don't know enough about what I might be adding later to sort it out.
					if(pickedBrush != selectedBrush)
					{
						if(m_keysDown[MULTISELECT] && (pickedBrush instanceof ArchitectureBrush) && (selectedBrush instanceof ArchitectureBrush))
						{
							// If the selectedBrush is about to be added to a composite, we assume the intention is that it's in the map.
							selectedBrush.deghost(m_map);

							if(selectedBrush instanceof ArchitectureBrushComposite && ((ArchitectureBrushComposite)selectedBrush).is_transient())
							{
								final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)selectedBrush;
								CommandManager.instance().execute_command(new Command("3D Multiselection")
								{
									public void execute()
									{
										m_map.delete_brush(pickedBrush);
										bc.add_brush((ArchitectureBrush)pickedBrush);
									}

									public void undo()
									{
										bc.delete_brush((ArchitectureBrush)pickedBrush);
										m_map.add_brush(pickedBrush);
									}
								});
							}
							else
							{
								ArchitectureBrushComposite bc = new ArchitectureBrushComposite();
								bc.add_brush((ArchitectureBrush)m_map.get_selected_brush());
								bc.add_brush((ArchitectureBrush)pickedBrush);
								CommandManager.instance().execute_command(BrushCommands.select("3D Multiselection", bc, m_map.get_selected_brush(), m_map));
							}
						}
						else CommandManager.instance().execute_command(BrushCommands.select("3D Selection", pickedBrush, m_map.get_selected_brush(), m_map));
					}
					else if(m_keysDown[MULTISELECT] && selectedBrush instanceof ArchitectureBrushComposite && ((ArchitectureBrushComposite)selectedBrush).is_transient())
					{
						// Multideselection: Remove a brush from a transient composite.
						if(p != null)
						{
							final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)selectedBrush;
							final IBrush pickedNestedBrush = p.second.m_pickedNestedBrush;
							switch(bc.brush_count())
							{
								case 0:
								case 1:
								{
									// This should never happen.
									throw new java.lang.Error("Warning: Composite brush contains fewer than two (" + bc.brush_count() + ") component brushes");
								}
								case 2:
								{
									// After removing a brush, our composite would only have one brush in it, so we replace it with its
									// "optimal brush", namely the singleton brush now inside it.
									CommandManager.instance().execute_command(new Command("3D Multideselection")
									{
										public void execute()
										{
											bc.delete_brush((ArchitectureBrush)pickedNestedBrush);
											ArchitectureBrush ob = bc.optimal_brush();
											m_map.add_brush(pickedNestedBrush);
											m_map.add_brush(ob);
											m_map.delete_brush(bc);
											m_map.change_selection(ob);
										}

										public void undo()
										{
											m_map.delete_brush(pickedNestedBrush);
											m_map.delete_brush(bc.optimal_brush());
											bc.add_brush((ArchitectureBrush)pickedNestedBrush);
											m_map.add_brush(bc);
											m_map.change_selection(bc);
										}
									});
									break;
								}
								default:
								{
									// Even after removing a brush, we will still have a composite brush with more than two component brushes,
									// so all we need to do is remove the brush from the composite and re-add it to the map (and the opposite
									// to undo the operation).
									CommandManager.instance().execute_command(new Command("3D Multideselection")
									{
										public void execute()
										{
											bc.delete_brush((ArchitectureBrush)pickedNestedBrush);
											m_map.add_brush(pickedNestedBrush);
										}

										public void undo()
										{
											m_map.delete_brush(pickedNestedBrush);
											bc.add_brush((ArchitectureBrush)pickedNestedBrush);
										}
									});
								}
							}
						}
					}
				}
			}

			public void mouseReleased(MouseEvent e)
			{
				m_mouseLast = null;
			}
		});

		m_canvas.addMouseMotionListener(new MouseMotionAdapter()
		{
			public void mouseDragged(MouseEvent e)
			{
// FIXME: We only want to use mouse look if we're dragging with the right mouse button.
				if(m_mouseLast == null)
				{
					m_mouseLast = new Point(e.getX(), e.getY());
					return;
				}

				Point delta = new Point(e.getX() - m_mouseLast.x, e.getY() - m_mouseLast.y);

				final double MOUSE_SPEED_MODIFIER = 0.25;
				double horizAngle = delta.x*MOUSE_SPEED_MODIFIER, vertAngle = delta.y*MOUSE_SPEED_MODIFIER;

				// Turn horizontally by rotating about the standard up vector (0,0,1).
				m_camera.rotate(new Vector3d(0,0,1), -horizAngle);

				// Then look up or down by rotating about u. Note that which way we rotate
				// depends entirely on whether the user wanted the y axis of the mouse
				// inverted or not.
				m_camera.rotate(m_camera.get_u(), vertAngle * (Options.is_set("Invert Mouse Y Axis") ? -1 : 1));

// TODO: This is untidy, let's sort it out.
				boolean anyDown = false;
				for(boolean b: m_keysDown)
				{
					if(b)
					{
						anyDown = true;
						break;
					}
				}
				if(!anyDown) m_canvas.display();

				m_mouseLast = new Point(e.getX(), e.getY());
			}

			public void mouseMoved(MouseEvent e)
			{
				m_canvas.requestFocusInWindow();
			}
		});

		m_canvas.addFocusListener(new FocusAdapter()
		{
			public void focusLost(FocusEvent e)
			{
				for(int i=0; i<m_keysDown.length; ++i) m_keysDown[i] = false;
			}
		});

		m_canvas.addKeyListener(new KeyAdapter()
		{
			public void keyPressed(KeyEvent e)
			{
				int key = e.getKeyCode();
				if(s_keyLookup.containsKey(key))
				{
					m_keysDown[s_keyLookup.get(key)] = true;
				}
				else
				{
					switch(key)
					{
						case KeyEvent.VK_DELETE:
						{
							IBrush b = m_map.get_selected_brush();
							if(b != null) CommandManager.instance().execute_command(BrushCommands.delete_brush(b, m_map));
							break;
						}
					}
				}
			}

			public void keyReleased(KeyEvent e)
			{
				int key = e.getKeyCode();
				if(s_keyLookup.containsKey(key)) m_keysDown[s_keyLookup.get(key)] = false;
			}
		});

		final int framePeriod = 50;		// in milliseconds, thus 20 frames per second
		m_inputTimer.schedule(new TimerTask()
		{
			public void run()
			{
				final double deltaOffset = framePeriod * Camera.get_linear_speed() / 1000;	// note: / 1000 because framePeriod is in milliseconds
				final double deltaAngle = framePeriod * Camera.get_angular_speed() / 1000;

				boolean anyDown = false;
				if(m_keysDown[MOVE_FORWARD])
				{
					m_camera.move_n(deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[MOVE_BACK])
				{
					m_camera.move_n(-deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[STRAFE_LEFT])
				{
					m_camera.move_u(deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[STRAFE_RIGHT])
				{
					m_camera.move_u(-deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[MOVE_UP])
				{
					m_camera.move_v(deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[MOVE_DOWN])
				{
					m_camera.move_v(-deltaOffset);
					anyDown = true;
				}
				if(m_keysDown[TWIST_LEFT])
				{
					m_camera.rotate(m_camera.get_n(), deltaAngle);
					anyDown = true;
				}
				if(m_keysDown[TWIST_RIGHT])
				{
					m_camera.rotate(m_camera.get_n(), -deltaAngle);
					anyDown = true;
				}
				if(m_keysDown[TURN_LEFT])
				{
					m_camera.rotate(new Vector3d(0,0,1), deltaAngle);
					anyDown = true;
				}
				if(m_keysDown[TURN_RIGHT])
				{
					m_camera.rotate(new Vector3d(0,0,1), -deltaAngle);
					anyDown = true;
				}
				if(m_keysDown[LOOK_DOWN])
				{
					m_camera.rotate(m_camera.get_u(), deltaAngle);
					anyDown = true;
				}
				if(m_keysDown[LOOK_UP])
				{
					m_camera.rotate(m_camera.get_u(), -deltaAngle);
					anyDown = true;
				}

				if(anyDown) m_canvas.display();
			}
		}, 0, framePeriod);
	}

	//################## PUBLIC METHODS ##################//
	// GLEventListener methods
	public void display(GLDrawable drawable)
	{
		GL gl = drawable.getGL();
		GLU glu = drawable.getGLU();

		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);

		// Modify the view to see what the camera sees.
		m_camera.set_view(gl, glu);

		// Draw axes.
		gl.glBegin(GL.GL_LINES);
			gl.glColor3f(1.0f, 0.0f, 0.0f);
			gl.glVertex3f(0.0f, 0.0f, 0.0f);
			gl.glVertex3f(10.0f, 0.0f, 0.0f);
			gl.glColor3f(0.0f, 1.0f, 0.0f);
			gl.glVertex3f(0.0f, 0.0f, 0.0f);
			gl.glVertex3f(0.0f, 10.0f, 0.0f);
			gl.glColor3f(0.0f, 0.0f, 1.0f);
			gl.glVertex3f(0.0f, 0.0f, 0.0f);
			gl.glVertex3f(0.0f, 0.0f, 10.0f);
		gl.glEnd();

		// Enable/disable wireframe mode, depending on the user's preference.
		if(Options.is_set("Render In Wireframe"))
		{
			gl.glPolygonMode(GL.GL_FRONT, GL.GL_LINE);
			gl.glPolygonMode(GL.GL_BACK, GL.GL_LINE);
			gl.glDisable(GL.GL_CULL_FACE);
		}
		else
		{
			gl.glPolygonMode(GL.GL_FRONT, GL.GL_FILL);
			gl.glPolygonMode(GL.GL_BACK, GL.GL_FILL);
			gl.glEnable(GL.GL_CULL_FACE);
		}

		render_map(gl, glu);
	}

	public void displayChanged(GLDrawable drawable, boolean modeChanged, boolean deviceChanged) {}

	final public GLCanvas get_canvas()
	{
		return m_canvas;
	}

	public void init(GLDrawable drawable) {}

	/**
	Resets (or sets, the first time it's called) the camera to its initial position and orientation.
	*/
	public void reset_camera()
	{
		m_camera = new Camera(new Vector3d(-20, -20, 10), new Vector3d(1, 1, 0), new Vector3d(0, 0, 1));
	}

	public void reshape(GLDrawable drawable, int x, int y, int width, int height)
	{
		GL gl = drawable.getGL();
		GLU glu = drawable.getGLU();

		// Cache the new width and height values.
		m_width = width;
		m_height = height;

		// Set up the viewport.
		gl.glViewport(0, 0, width, height);

		// Set the background clear colour.
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Set up the projection matrix.
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glLoadIdentity();

		double aspect = (double)width/(double)height;
		m_maxY = NEAR * Math.tan(m_fovY/2);
		m_minY = -m_maxY;
		m_minX = m_minY * aspect;
		m_maxX = m_maxY * aspect;
		gl.glFrustum(m_minX, m_maxX, m_minY, m_maxY, NEAR, FAR);

		// Select and reset the modelview matrix.
		gl.glMatrixMode(GL.GL_MODELVIEW);
		gl.glLoadIdentity();

		// Set up and enable a z-buffer.
		gl.glClearDepth(1.0);
		gl.glDepthFunc(GL.GL_LEQUAL);
		gl.glEnable(GL.GL_DEPTH_TEST);

		// Set up and enable back-face culling.
		gl.glFrontFace(GL.GL_CCW);
		gl.glEnable(GL.GL_CULL_FACE);
	}

	// IRepaintListener methods
	public void repaint()
	{
		// FIXME:
		// It would be nice to find a way to make things fast enough to allow
		// us to repaint as the user is dragging brushes around. This may not
		// be possible, though.
		/*new Thread()
		{
			public void run()
			{
				m_canvas.display();
			}
		}.start();*/
	}

	public void repaint_full()
	{
		// Start the repaint in a separate thread so that the user can continue editing
		// while we're repainting.
		new Thread()
		{
			public void run()
			{
				m_canvas.display();
			}
		}.start();
	}
}
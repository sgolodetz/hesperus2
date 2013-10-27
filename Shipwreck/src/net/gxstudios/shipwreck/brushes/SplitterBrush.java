package net.gxstudios.shipwreck.brushes;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Stroke;
import java.io.PrintWriter;

import javax.media.opengl.GL2;
import javax.media.opengl.glu.GLU;

import net.gxstudios.shipwreck.commands.Command;
import net.gxstudios.shipwreck.commands.CommandManager;
import net.gxstudios.shipwreck.geom.AxisPair;
import net.gxstudios.shipwreck.geom.planar.GeomConstants;
import net.gxstudios.shipwreck.geom.planar.Plane;
import net.gxstudios.shipwreck.graphics.IRenderer;
import net.gxstudios.shipwreck.math.vectors.Vector2d;
import net.gxstudios.shipwreck.math.vectors.Vector3d;
import net.gxstudios.shipwreck.math.vectors.VectorUtil;
import net.gxstudios.shipwreck.misc.Constants;
import net.gxstudios.shipwreck.misc.Options;
import net.gxstudios.shipwreck.misc.Pair;

/**
This class handles the splitting of an {@link ArchitectureBrush} across a plane.
*/
public class SplitterBrush extends BrushAdapter implements Constants, GeomConstants
{
	//################## PRIVATE VARIABLES ##################//
	private ArchitectureBrush m_brush;
	private IRenderer m_renderer;
	private Plane m_plane = null;	// the splitting plane
	private Vector2d m_mouseAnchor_Coords = null;
	private Vector2d m_mouseLast_Coords = null;

	//################## CONSTRUCTORS ##################//
	public SplitterBrush(final ArchitectureBrush brush)
	{
		m_brush = brush;
	}

	//################## PUBLIC METHODS ##################//
	public SplitterBrush copy()
	{
// TODO: Re-examine whether this is what we want. If not, we'll have to change is_copyable() to return true, of course.
		throw new UnsupportedOperationException();
	}

	public void deghost(final IBrushContainer container)
	{
		if(m_plane == null) return;		// we can't split the brush without a splitting plane

		switch(m_brush.classify_against_plane(m_plane))
		{
			case CP_STRADDLE:
			{
				Pair<ArchitectureBrush,ArchitectureBrush> result = m_brush.split_with_plane(m_plane);
				ArchitectureBrushComposite bc = new ArchitectureBrushComposite();
				bc.add_brush(result.first);
				bc.add_brush(result.second);

				final ArchitectureBrush ob = bc.optimal_brush();
				CommandManager.instance().execute_command(new Command("Brush Split")
				{
					public void execute()
					{
						container.delete_brush(SplitterBrush.this);
						container.add_brush(ob);
						container.change_selection(ob);
					}

					public void undo()
					{
						container.delete_brush(ob);
						container.add_brush(SplitterBrush.this);
						container.change_selection(SplitterBrush.this);
					}
				});
				break;
			}
			default:	// if the brush doesn't straddle the plane, there's no splitting to be done
			{
				break;
			}
		}
	}

	public IBrush deselect(IBrushContainer container)
	{
		container.delete_brush(this);
		container.add_brush(m_brush);
		return m_brush;
	}

	public BrushDetails details()
	{
		return m_brush.details();
	}

	public boolean is_copyable()
	{
		return false;
	}

	public boolean is_ghost()
	{
		return false;
	}

	public void mouse_dragged(IRenderer renderer, Vector2d p)
	{
		if(m_renderer == null)
		{
			// If we somehow generated a mouse_dragged without calling mouse_pressed first
			// (perhaps by dragging onto this canvas from another component...users really
			// can be that annoying...), then we need to call mouse_pressed to set things up.
			mouse_pressed(renderer, p, LEFT_BUTTON, false);
		}

		if(Options.is_set("Snap To Grid"))
		{
			p = m_renderer.find_nearest_grid_intersect_in_coords(p);
		}

		m_mouseLast_Coords = p;
		calculate_splitting_plane(renderer);
	}

	public boolean mouse_pressed(IRenderer renderer, Vector2d p, int button, boolean immediate)
	{
		m_renderer = renderer;

		if(Options.is_set("Snap To Grid"))
		{
			p = m_renderer.find_nearest_grid_intersect_in_coords(p);
		}

		final double THRESHOLD = 36.0;

		// If the click's near either of the control handles, just allow the user to drag the handle,
		// otherwise start a new split line.
		if(m_mouseAnchor_Coords != null && renderer.distance_squared(p, m_mouseAnchor_Coords) < THRESHOLD)
		{
			m_mouseAnchor_Coords = m_mouseLast_Coords;
			m_mouseLast_Coords = p;
		}
		else if(m_mouseLast_Coords != null && renderer.distance_squared(p, m_mouseLast_Coords) < THRESHOLD)
		{
			m_mouseLast_Coords = p;
		}
		else
		{
			m_mouseAnchor_Coords = p;
			m_mouseLast_Coords = p;
		}

		return true;
	}

	public PickResults pick(final Vector3d start, final Vector3d direction)
	{
		return m_brush.pick(start, direction);
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		float[] dash = {5.0f,10.0f};
		Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);
		renderer.set_stroke(stroke);

		m_brush.render(renderer, overrideColour);

		if(m_mouseAnchor_Coords != null && renderer == m_renderer)
		{
			renderer.set_colour(Color.white);
			renderer.set_stroke(new BasicStroke(2.0f));
			BrushUtil.draw_infinite_line(renderer, m_mouseAnchor_Coords, m_mouseLast_Coords);
			renderer.set_colour(Color.cyan);
			BrushUtil.draw_dot(renderer, m_mouseAnchor_Coords);
			BrushUtil.draw_dot(renderer, m_mouseLast_Coords);
		}

		renderer.set_stroke(new BasicStroke());
	}

	public void render3D(GL2 gl, GLU glu)
	{
		// This isn't an unnecessary function, even though splitter brushes only exist when
		// they're selected. There are occasions when it makes sense to render brushes as
		// unselected even if they're actually selected, for instance when in texture-editing
		// mode.
		m_brush.render3D(gl, glu);
	}

	public void render3D_selected(GL2 gl, GLU glu)
	{
		m_brush.render3D_selected(gl, glu);

// FIXME: Render the splitting plane, if there is one. This sort of works, but it looks rubbish at the moment.
// Recalculating it every time is also ludicrously inefficient.
		/*if(m_plane != null)
		{
			Polygon p = GeomUtil.make_universe_polygon(m_plane);
			Vector3d[] verts = p.get_vertices();

			gl.glDisable(GL.GL_CULL_FACE);

			// Render the splitting plane as transparent, so users can see what they're doing.
			gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
			gl.glEnable(GL.GL_BLEND);

			gl.glBegin(GL.GL_POLYGON);
				Color[] colours = new Color[] { Color.red, Color.green, Color.blue, Color.white };
				for(int i=0, len=verts.length; i<len; ++i)
				{
					float[] components = null;
					components = colours[i].getRGBComponents(components);
					gl.glColor4f(components[0], components[1], components[2], 0.5f);
					gl.glVertex3d(verts[i].x, verts[i].y, verts[i].z);
				}
			gl.glEnd();

			gl.glDisable(GL.GL_BLEND);

			gl.glEnable(GL.GL_CULL_FACE);
		}*/
	}

	public void select(IBrushContainer container)
	{
		container.add_brush(this);
		container.delete_brush(m_brush);
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// There's no chance of selecting a SplitterBrush, since they only exist when they're
		// the selected brush. They're only created to facilitate splitting an ArchitectureBrush.
		return Double.MAX_VALUE;
	}

	// Saving
	public void save_MEF2(PrintWriter pw)
	{
		m_brush.save_MEF2(pw);
	}

	//################## PRIVATE METHODS ##################//
	private void calculate_splitting_plane(final IRenderer renderer)
	{
		Vector2d delta = VectorUtil.subtract(m_mouseLast_Coords, m_mouseAnchor_Coords);
		if(delta.x != 0 || delta.y != 0)
		{
			AxisPair ap = renderer.get_axis_pair();

			// Note: It doesn't matter which way the plane normal points as far as splitting is concerned.
			Vector2d n2D = new Vector2d(delta.y, -delta.x);
			Vector3d n = ap.generate_3D_point(n2D, 0);

			// n . x - d = 0 -> d = n . x, for any x on the plane
			double d = n2D.dot(m_mouseAnchor_Coords);

			m_plane = new Plane(n, d);
		}
		else
		{
			m_plane = null;
		}
	}
}
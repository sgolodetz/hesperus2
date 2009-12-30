package MapEditor.Brushes;

import MapEditor.BSP.Tree;
import MapEditor.Commands.*;
import MapEditor.Geom.*;
import MapEditor.Graphics.*;
import MapEditor.Maps.Map;
import MapEditor.Math.Vectors.*;
import MapEditor.Misc.Pair;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Stroke;
import java.util.LinkedList;
import javax.media.opengl.*;
import javax.media.opengl.glu.GLU;

/**
This class represents a selection. You drag it out on the screen, and you can resize it or translate it as you
see fit.
*/
public class SelectionBrush extends ResizableTranslatableBrush
{
	//################## PRIVATE VARIABLES ##################//
	private Iterable<IBrush> m_brushes;

	//################## CONSTRUCTORS ##################//
	/**
	Constructs a SelectionBrush with the specified bounds.

	@param bounds	The bounding box of the new selection brush
	@param brushes	The other brushes in the map (used in deghost())
	*/
	public SelectionBrush(AxisAlignedBox bounds, Iterable<IBrush> brushes)
	{
		super(true);	// selection brushes get dragged out by the user

		m_boundingBox = new BoundingBox(bounds);
		m_brushes = brushes;
	}

	//################## PUBLIC METHODS ##################//
	public SelectionBrush copy()
	{
		throw new UnsupportedOperationException();
	}

	public void deghost(final IBrushContainer container)
	{
		// FIXME: Eventually, we'll need to do brush composites properly for all brush types.
		final ArchitectureBrushComposite bc = new ArchitectureBrushComposite();

		for(IBrush b: m_brushes)
		{
			// FIXME: Eventually, we'll need to do brush bounds properly for all brush types.
			if(!(b instanceof ArchitectureBrush)) continue;

			ArchitectureBrush ab = (ArchitectureBrush)b;
			if(m_boundingBox.completely_contains(ab.m_boundingBox))
			{
				bc.add_brush(ab);
			}
		}

		final ArchitectureBrush ob = bc.optimal_brush();
		if(ob != null)
		{
			CommandManager.instance().execute_command(BrushCommands.select("Group Selection", ob, this, container));
		}
	}

	public boolean is_copyable()
	{
		return false;
	}

	public boolean is_ghost()
	{
		return true;
	}

	public void render_selected(IRenderer renderer, Color overrideColour)
	{
		renderer.set_colour(Color.white);

		float[] dash = {5.0f};
		Stroke stroke = new BasicStroke(1.0f, BasicStroke.CAP_SQUARE, BasicStroke.JOIN_MITER, 10.0f, dash, 0.0f);
		render_bounds(renderer, stroke);

		render_transformation_effects(renderer);
	}

	public void render3D_selected(GL2 gl, GLU glu)
	{
		Vector3d[] bounds = m_boundingBox.get_bounds();
		gl.glColor3f(1.0f, 1.0f, 1.0f);

		gl.glLineStipple(1, (short)0x00ff);
		gl.glEnable(GL2.GL_LINE_STIPPLE);

		GraphicsUtil.draw_cuboid(gl, bounds[0], bounds[1]);

		gl.glDisable(GL2.GL_LINE_STIPPLE);
	}

	public double selection_metric(Vector2d p, IRenderer renderer)
	{
		// There's no chance of selecting a SelectionBrush, since they only exist when they're
		// the selected brush.
		return Double.MAX_VALUE;
	}

	//################## PROTECTED METHODS ##################//
	protected void resize(Vector2d corner0, Vector2d corner1, AxisPair ap)
	{
		m_boundingBox.resize(corner0, corner1, ap);
	}

	protected void translate(Vector3d trans)
	{
		m_boundingBox = m_cachedBoundingBox.clone();
		m_boundingBox.translate(trans);
	}
}
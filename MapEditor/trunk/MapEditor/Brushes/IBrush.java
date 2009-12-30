package MapEditor.Brushes;

import MapEditor.Geom.Planar.Polygon;
import MapEditor.Graphics.IRenderer;
import MapEditor.Math.Vectors.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.io.PrintWriter;
import javax.media.opengl.GL2;
import javax.media.opengl.glu.GLU;

public interface IBrush
{
	/**
	Returns a (deep) copy of the brush such that no modification to the copy
	can change the original brush, or vice-versa.

	<p><b>Preconditions:</b>
	<dl>
	<dd>is_copyable() must return true
	</dl>

	@return	A copy of the brush, as specified
	@throws java.lang.UnsupportedOperationException	If is_copyable() is false
	*/
	IBrush copy();	

	/**
	Deghosts the brush. What this involves depends on the brush type.

	@param container	The container provides various useful operations such as
						adding and deleting brushes from the map - of course, the
						container is really the map itself in practice, but we don't
						want brushes to know about maps directly because it introduces
						an unnecessary cyclic dependency
	*/
	void deghost(IBrushContainer container);
	
	/**
	Informs the brush that it's being deselected. The brush is allowed to return
	a brush which should be selected afterwards. This is useful for splitter brushes,
	for instance, which when deselected can ask for the brush on which they're being
	used to be reselected.

	@param container	The container provides various useful operations such as
						adding and deleting brushes from the map

	@return	A brush which should be selected as described above
	*/
	IBrush deselect(IBrushContainer container);

	/**
	Returns a BrushDetails object containing information about the brush, for instance
	the number of faces it contains (if any).
	*/
	BrushDetails details();

	/**
	Flips the brush across a plane through its centre of the form x = k, for some constant k.

	<p><b>Preconditions:</b>
	<dl>
	<dd>is_flippable() must return true
	</dl>
	*/
	void flip_x();

	/**
	Flips the brush across a plane through its centre of the form y = k, for some constant k.

	<p><b>Preconditions:</b>
	<dl>
	<dd>is_flippable() must return true
	</dl>
	*/
	void flip_y();

	/**
	Flips the brush across a plane through its centre of the form z = k, for some constant k.

	<p><b>Preconditions:</b>
	<dl>
	<dd>is_flippable() must return true
	</dl>
	*/
	void flip_z();

	/**
	Returns a boolean indicating whether or not the brush can be copied using its
	copy method.

	@return true if it's copyable, or false otherwise
	*/
	boolean is_copyable();

	/**
	Returns a boolean indicating whether or not the brush can be flipped
	(across axis-aligned planes through its centre).

	@return	true if it's flippable, or false otherwise
	*/
	boolean is_flippable();

	/**
	Returns a boolean indicating whether or not the brush is a ghost brush (not yet
	added to the map).

	@return true if it's a ghost brush, or false otherwise
	*/
	boolean is_ghost();

	void mouse_dragged(IRenderer renderer, Vector2d p);
	void mouse_moved(IRenderer renderer, Vector2d p);
	boolean mouse_pressed(IRenderer renderer, Vector2d p, int button, boolean immediate);
	void mouse_released();

	/**
	Returns the result of intersecting the half-ray (specified by the start point and direction vector) with
	the brush, part of a process known as "picking".

	@param start		The start of the half-ray (generally the camera position)
	@param direction	The direction vector of the half-ray (generally a vector from the camera position to the
						point on the image plane corresponding to where the user clicked the mouse on the screen)
	@return				The results of the picking operation as a PickResults object, or null if either the
						brush wasn't pickable or the half-ray didn't intersect it
	*/
	PickResults pick(final Vector3d start, final Vector3d direction);

	/**
	Returns a dialog which allows the user to edit the associated properties of the specified brush.

	@return		A dialog as described, if the brush has associated properties, or null otherwise
	*/
	Dialog properties_dialog(Frame owner);

	/**
	Renders the brush onto the rendering surface associated with the specified renderer
	in such a way that it looks unselected.

	<p><b>Preconditions:</b>
	<dl>
	<dd>renderer != null
	</dl>

	@param renderer			The renderer associated with the rendering surface onto which to render the brush
	@param overrideColour	The colour (if not null) with which to override the normal rendering colour for the brush
	*/
	void render(IRenderer renderer, Color overrideColour);

	/**
	Renders the brush onto a 3D view using the OpenGL object provided in such a way that
	it looks unselected.

	@param gl				The OpenGL object to use for rendering
	@param glu				The corresponding GLU context
	*/
	void render3D(GL2 gl, GLU glu);

	/**
	Renders the brush onto the rendering surface associated with the specified renderer
	in such a way that it looks selected.

	<p><b>Preconditions:</b>
	<dl>
	<dd>renderer != null
	</dl>

	@param renderer			The renderer associated with the rendering surface onto which to render the brush
	@param overrideColour	The colour (if not null) with which to override the normal rendering colour for the brush
	*/
	void render_selected(IRenderer renderer, Color overrideColour);

	/**
	Renders the brush onto a 3D view using the OpenGL object provided in such a way that
	it looks selected.

	@param gl				The OpenGL object to use for rendering
	@param glu				The corresponding GLU context
	*/
	void render3D_selected(GL2 gl, GLU glu);

	/**
	Saves the brush in MEF 2 format. In practice, this involves writing the MEF representation of the brush to the current
	position in a file using the specified PrintWriter.

	@param pw	The PrintWriter to use for outputting the brush representation
	*/
	void save_MEF2(PrintWriter pw);

	/**
	Saves the brush in MEF 3 format. In practice, this involves writing the MEF representation of the brush to the current
	position in a file using the specified PrintWriter. Note that MEF 3 format is essentially the same as MEF 2, except that
	some new brush types (lights, entities, etc.) have been added in MEF 3. Versions of the editor which only load MEF 2
	files will be unable to load files saved as MEF 3.
	*/
	void save_MEF3(PrintWriter pw);

	/**
	The selection handler for the brush: code to execute when the brush is informed that it has
	been selected in the specified brush container (which in practice means "in the specified map").

	@param container	The brush container in which the brush has been selected
	*/
	void select(IBrushContainer container);

	/**
	Returns a metric indicating how good a candidate this brush is for selection in response
	to a mouse click at the specified point. A lower metric means the brush is a better candidate.
	Factors which might affect the metric are the point's distance from the brush's centre cross
	and edges. For ties, other factors might need to come into play as well.

	@param p		The 2D point (on one of the canvases) which the user clicked (in Coords)
	@param renderer	The renderer for the canvas in question
	@return			The metric as a positive double (>= 0)
	*/
	double selection_metric(Vector2d p, IRenderer renderer);

	/**
	This interface is used to break a cyclic dependency between the Brushes package
	and the Maps package. By isolating the information from Map that brushes need
	to know about using this interface, we can decouple the two packages.
	*/
	public interface IBrushContainer
	{
		void add_brush(IBrush b);
		void change_selection(IBrush selection);
		boolean contains(IBrush b);
		void delete_brush(IBrush b);
		void deselect_selection();
		Iterable<IBrush> get_brushes();
		IBrush get_selected_brush();
		void ghost_selection();
		void select_brush(IBrush selection);
	}

	/**
	Instances of this class are used to return the results of picking operations.
	*/
	public class PickResults
	{
		public PickResults(final Polygon pickedFace, final Vector3d pickedPoint)
		{
			this(pickedFace, pickedPoint, null);
		}

		public PickResults(final Polygon pickedFace, final Vector3d pickedPoint, final IBrush pickedNestedBrush)
		{
			m_pickedFace = pickedFace;
			m_pickedPoint = pickedPoint;
			m_pickedNestedBrush = pickedNestedBrush;
		}

		final public IBrush m_pickedNestedBrush;	// the brush picked inside a composite - used for multideselection
		final public Polygon m_pickedFace;
		final public Vector3d m_pickedPoint;
	}
}
package MapEditor.Maps;

import MapEditor.Brushes.BrushDetails;
import MapEditor.Brushes.IBrush;
import MapEditor.Brushes.LandscapeBrush;
import MapEditor.Commands.*;
import MapEditor.Event.*;
import MapEditor.Geom.Planar.Polygon;
import MapEditor.Math.Vectors.Vector3d;
import java.util.*;

public class Map implements IBrush.IBrushContainer, IRepaintListener
{
	//################## INVARIANTS ##################//
	/*
	m_state == STATE_NORMAL -> m_selectedBrush == null
	m_state == STATE_EDITING -> m_selectedBrush != null
	*/

	//################## ENUMERATIONS ##################//
	public enum BrushType
	{
		BLOCK,
		CYLINDER,
		CONE,
		UV_SPHERE,
		LANDSCAPE,
		LIGHT
	};

	//################## CONSTANTS ##################//
	final public static int STATE_NORMAL = 0;
	final public static int STATE_CREATE = 1;
	final public static int STATE_EDITING = 2;

	//################## PRIVATE VARIABLES ##################//
	private IBrush m_selectedBrush = null;	// the currently selected brush
	private BrushType m_brushCreationType;
	private int m_state = STATE_NORMAL;
	private ISelectionListener m_selectionListener = ISelectionListener.NULL;
	private LinkedList<IBrush> m_brushes = new LinkedList<IBrush>();
	private LinkedList<Polygon> m_selectedFaces = new LinkedList<Polygon>();		// the faces currently selected for texturing
	private Vector3d m_brushCreationAnchor = new Vector3d();	// helps determine where to create a new brush
															// two of the components come from where we're
															// clicking in a design canvas, the missing one
															// comes from this variable
	private RepaintListenerComposite m_repaintListener = new RepaintListenerComposite();

	//################## PRIVATE METHODS ##################//
	/**
	Sets the currently selected brush and, if desired, informs it that it's been selected.
	<p><b>Notes:</b>
	<ul>
	<li>If we pass in null, we want all the brushes to be deselected. This means that we are no longer editing
	any brush, so we have to change the map state back to STATE_NORMAL.
	<li>If we pass in a brush, we're going to be editing it, so we have to change the map state to STATE_EDITING.
	<li>If we pass in the currently selected brush, select_brush doesn't do anything.
	</ul>

	@param selection	The brush to set as the selected brush
	@param informBrush	A boolean indicating whether or not we want to inform the brush that it's been selected
	*/
	private void select_brush(IBrush selection, boolean informBrush)
	{
		if(selection == m_selectedBrush) return;	// the brush we passed in is already selected, do nothing

		m_selectedBrush = selection;

		if(m_selectedBrush == null)
		{
			m_state = STATE_NORMAL;
		}
		else
		{
			m_state = STATE_EDITING;
			if(informBrush) m_selectedBrush.select(this);	// inform the brush that it's been selected
		}

		update_menu_commands();
	}

	/**
	Updates certain menu commands to reflect the current state of the map.
	*/
	private void update_menu_commands()
	{
		MenuManager.instance().get_item("Edit_Cut").setEnabled(m_selectedBrush != null && m_selectedBrush.is_copyable());
		MenuManager.instance().get_item("Edit_Copy").setEnabled(m_selectedBrush != null && m_selectedBrush.is_copyable());
		MenuManager.instance().get_item("Edit_Delete").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_CSG_Hollow Brush...").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_CSG_Split Brush").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_CSG_Carve With Brush").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_CSG_Intersect Brushes").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_CSG_Union Brushes").setEnabled(m_selectedBrush != null);
		MenuManager.instance().get_item("Brush_Landscape_Decrease Resolution").setEnabled(m_selectedBrush instanceof LandscapeBrush);
		MenuManager.instance().get_item("Brush_Landscape_Increase Resolution").setEnabled(m_selectedBrush instanceof LandscapeBrush);
	}

	//################## PUBLIC METHODS ##################//
	public void add_brush(IBrush b)
	{
		m_brushes.add(b);
	}

	public void add_repaint_listener(final IRepaintListener listener)
	{
		m_repaintListener.add(listener);
	}

	/**
	Sets the currently selected brush without informing the brush itself that it's been selected.

	@param selection	The brush to set as the selected brush
	*/
	public void change_selection(IBrush selection)
	{
		select_brush(selection, false);
	}

	/**
	Returns whether or not the specified brush is in the map.

	@param b	The brush to check
	@return		true if the brush is in the map, otherwise false
	*/
	public boolean contains(IBrush b)
	{
		return m_brushes.contains(b);
	}

	public void deghost_selection()
	{
		if(m_selectedBrush != null)
		{
			m_selectedBrush.deghost(this);
			update_menu_commands();
		}
	}

	public void delete_brush(IBrush b)
	{
		if(b.is_ghost())
		{
			// Ghost brushes only exist when they're selected, so if we're
			// deleting one then we can safely assume it's the selected brush.
			change_selection(null);
		}
		else if(m_brushes.contains(b))
		{
			m_brushes.remove(b);
			if(b == get_selected_brush())
			{
				change_selection(null);
			}
		}
		else
		{
			System.err.println("Warning: Trying to delete a brush that isn't in the map");
			System.err.println("---");
			System.err.println(new Throwable().getStackTrace()[1]);
			System.err.println("---");
		}
	}

	public void deselect_selection()
	{
		// It might be noted that this is very similar to the code:
		// if(m_selectedBrush != null) select_brush(m_selectedBrush.deselect(this));
		// It's different, however, in that here we're not informing the "new" brush
		// that it's been selected. This turns out to be important - the primary use
		// of the deselect method is to allow brushes to arrange for brushes they've
		// been working on to be reselected. These contained brushes have actually
		// been "selected", in a sense, the entire time, since they're the brushes
		// being worked on, so we don't reinform them that they're selected as bad
		// things will happen (brushes' select methods are generally not idempotent -
		// we can't call them multiple times in a row and expect it to work properly).

		if(m_selectedBrush != null) change_selection(m_selectedBrush.deselect(this));
	}

	/**
	Returns a MapDetails object containing information about the map, for instance the
	number of brushes it contains.

	@return	A MapDetails object as described
	*/
	public MapDetails details()
	{
		int faceCount = 0;
		for(IBrush b: m_brushes)
		{
			BrushDetails bd = b.details();
			faceCount += bd.m_faceCount;
		}
		return new MapDetails(m_brushes.size(), faceCount);
	}

	public Vector3d dimensions()
	{
		return new Vector3d(32768,32768,32768);
	}

	public Iterable<IBrush> get_brushes()
	{
		return m_brushes;
	}

	/**
	Gets the brush creation anchor.

	@return	...think about it...
	*/
	public Vector3d get_brush_creation_anchor()
	{
		return m_brushCreationAnchor;
	}

	public BrushType get_brush_creation_type()
	{
		return m_brushCreationType;
	}

	/**
	Returns the selected brush.

	@return	...think about it...
	*/
	public IBrush get_selected_brush()
	{
		return m_selectedBrush;
	}

	/**
	Returns the selected faces.

	@return	The selected faces as a linked list
	*/
	public LinkedList<Polygon> get_selected_faces()
	{
		return m_selectedFaces;
	}

	/**
	Returns the editing state of the map.

	@return	...think about it...
	*/
	public int get_state()
	{
		return m_state;
	}

	/**
	Ghosting the selection means removing it from the map. Note the subtle difference between
	this and delete_brush(m_selectedBrush): the latter clears the selection - it really means
	delete. This, on the other hand, means "take it out of the map, but continue working with it".
	*/
	public void ghost_selection()
	{
		m_brushes.remove(m_selectedBrush);
	}

	/**
	Repaint the map on all canvases which are showing it.
	*/
	public void repaint()
	{
		m_repaintListener.repaint();
	}

	public void repaint_full()
	{
		m_repaintListener.repaint_full();
	}

	public void replace_with(Map otherMap)
	{
		change_selection(null);
		m_brushCreationType = otherMap.m_brushCreationType;
		m_state = otherMap.m_state;
		m_brushes = otherMap.m_brushes;
// TODO: Set the brush creation anchor possibly?
		repaint_full();
	}

	/**
	Sets the currently selected brush and informs it that it's been selected. Note that this is
	subtly different from change_selection, which does the same thing except that it doesn't inform
	the brush that it's been selected. There are occasions where both are needed, we can't make do
	with just one of them.

	@param selection	The brush to set as the selected brush
	*/
	public void select_brush(IBrush selection)
	{
		select_brush(selection, true);
	}

	/**
	Sets the selected face for texturing.

	@param selection	The face to select
	*/
	public void select_face(Polygon selection)
	{
		m_selectedFaces.clear();
		if(selection != null) m_selectedFaces.add(selection);

		m_selectionListener.face_selection_changed();
	}

	/**
	Sets the selected faces for texturing.

	@param selectedFaces	A list of faces to select
	*/
	public void select_faces(LinkedList<Polygon> selectedFaces)
	{
		m_selectedFaces = selectedFaces;

		m_selectionListener.face_selection_changed();
	}

	/**
	Sets the position of the brush creation anchor.
	<p><b>Notes:</b>
	<ul>
	<li>The point will either be the centre of the canvas (if we scrolled around the canvas, whether using the
	scrollbars or the keyboard), or the point we just clicked using the mouse.
	</ul>

	@param p	The new position of the anchor
	*/
	public void set_brush_creation_anchor(Vector3d p)
	{
		m_brushCreationAnchor = p;
	}

	public void set_brush_creation_type(BrushType brushCreationType)
	{
		m_brushCreationType = brushCreationType;
	}

	public void set_selection_listener(final ISelectionListener listener)
	{
		if(listener != null)
		{
			m_selectionListener = listener;
		}
		else
		{
			System.err.println("There's a perfectly good ISelectionListener.NULL object, which is preferable to null");
			m_selectionListener = ISelectionListener.NULL;
		}
	}

	/**
	Sets the current map state.

	@param state			The state to set
	@throws java.lang.Error	If we pass in an integer which does not specify a valid map state
	*/
	public void set_state(int state)
	{
		if(state < 0 || state > 3) throw new java.lang.Error();
		m_state = state;
	}

	/**
	Toggles the status of a face - if it was previously unselected, this adds it
	to the list of selected faces; if it was previously selected, this deselects
	it again.

	@param selection	The face whose status we want to toggle
	*/
	public void toggle_selected_face(Polygon selection)
	{
		if(selection != null)
		{
			if(m_selectedFaces.contains(selection)) m_selectedFaces.remove(selection);
			else m_selectedFaces.add(selection);

			m_selectionListener.face_selection_changed();
		}
	}
}
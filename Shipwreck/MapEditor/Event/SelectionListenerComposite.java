package MapEditor.Event;

import java.util.LinkedList;

/**
This class (which uses the Composite pattern) acts as an {@link ISelectionListener} and forwards
its selection updates onto any selection listeners which it contains.
*/
public class SelectionListenerComposite implements ISelectionListener
{
	/** The selection listeners stored in the composite */
	private LinkedList<ISelectionListener> m_listeners = new LinkedList<ISelectionListener>();

	/**
	Add a selection listener to the composite.

	@param listener	The selection listener to add
	*/
	public void add(ISelectionListener listener)
	{
		m_listeners.add(listener);
	}

	/**
	Inform any listeners in the composite that a different face has been selected.
	*/
	public void face_selection_changed()
	{
		for(ISelectionListener l: m_listeners) l.face_selection_changed();
	}
}
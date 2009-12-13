package MapEditor.Event;

import java.util.LinkedList;

/**
This class (which uses the Composite pattern) acts as an {@link IRepaintListener} and forwards
its repaints onto any repaint listeners which it contains.
*/
public class RepaintListenerComposite implements IRepaintListener
{
	/** The repaint listeners stored in the composite */
	private LinkedList<IRepaintListener> m_listeners = new LinkedList<IRepaintListener>();

	/**
	Add a repaint listener to the composite.

	@param listener	The repaint listener to add
	*/
	public void add(IRepaintListener listener)
	{
		m_listeners.add(listener);
	}

	/**
	Repaint any listeners in the composite.
	*/
	public void repaint()
	{
		for(IRepaintListener l: m_listeners) l.repaint();
	}

	/**
	Repaint any listeners in the composite. Note that this isn't the same as repaint, which
	is called much more frequently. In particular, full repaints update 3D views, which
	normal repaints don't.
	*/
	public void repaint_full()
	{
		for(IRepaintListener l: m_listeners) l.repaint_full();
	}
}
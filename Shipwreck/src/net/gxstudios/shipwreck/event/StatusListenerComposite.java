package net.gxstudios.shipwreck.event;

import java.util.LinkedList;

/**
This class (which uses the Composite pattern) acts as an {@link IStatusListener} and forwards
its status updates onto any status listeners which it contains.
*/
public class StatusListenerComposite<T> implements IStatusListener<T>
{
	/** The status listeners stored in the composite */
	private LinkedList<IStatusListener<T>> m_listeners = new LinkedList<IStatusListener<T>>();

	/**
	Add a status listener to the composite.

	@param listener	The status listener to add
	*/
	public void add(IStatusListener<T> listener)
	{
		m_listeners.add(listener);
	}

	/**
	Update the status of any listeners in the composite.
	*/
	public void status_changed(T newStatus)
	{
		for(IStatusListener<T> l: m_listeners) l.status_changed(newStatus);
	}
}
package MapEditor.Event;

/**
This specifies an interface for anything that can be repainted. Good examples of
things which can be repainted include a {@link MapEditor.GUI.DesignCanvas}.
*/
public interface IRepaintListener
{
	/** Repaint the listener */
	void repaint();

	/**
	Invoked on a full repaint. Not everything needs to be repainted every time.
	It's better to only repaint 3D views when commands are executed, for instance,
	because repainting as we're dragging gets quite slow.
	*/
	void repaint_full();
}
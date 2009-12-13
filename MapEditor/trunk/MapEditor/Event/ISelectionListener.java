package MapEditor.Event;

public interface ISelectionListener
{
	void face_selection_changed();

	public ISelectionListener NULL = new ISelectionListener()
	{
		public void face_selection_changed() {}
	};
}
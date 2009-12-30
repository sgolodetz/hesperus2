package MapEditor.Event;

public interface IStatusListener<T>
{
	void status_changed(T newStatus);
}
package MapEditor.Event;

public interface IStatusSource<T>
{
	void add_status_listener(IStatusListener<T> listener);
}
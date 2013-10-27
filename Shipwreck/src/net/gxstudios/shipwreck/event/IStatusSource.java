package net.gxstudios.shipwreck.event;

public interface IStatusSource<T>
{
	void add_status_listener(IStatusListener<T> listener);
}
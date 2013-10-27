package net.gxstudios.shipwreck.event;

public interface IStatusListener<T>
{
	void status_changed(T newStatus);
}
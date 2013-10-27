package net.gxstudios.shipwreck.gui;

import java.awt.Color;
import java.awt.Label;
import java.util.Vector;

import net.gxstudios.shipwreck.event.IStatusListener;
import net.gxstudios.shipwreck.event.IStatusSource;

@SuppressWarnings("serial")
public class StatusBar extends Label
{
	private Vector<String> m_sources = new Vector<String>();

	public StatusBar()
	{
		setBackground(Color.gray);
	}

	public void add_status_source(IStatusSource<String> source)
	{
		final int sourceID = m_sources.size();
		m_sources.add("");
		source.add_status_listener(new IStatusListener<String>()
		{
			public void status_changed(String newStatus)
			{
				m_sources.set(sourceID, newStatus);
				StringBuffer sb = new StringBuffer("Status: ");
				for(String s: m_sources) sb.append(s + " ");
				setText(sb.toString());
			}
		});
	}
}
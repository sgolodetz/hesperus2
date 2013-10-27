package net.gxstudios.shipwreck.gui;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Panel;
import java.awt.Scrollbar;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

/**
This class (which uses the Decorator pattern) can add scrollbars to any component which implements the
{@link IScrollable} interface.
*/
public class AddScrollbars extends Panel
{
	/** The component, casted to IScrollable for ease of use */
	final private IScrollable m_scrollableComp;
	/** The vertical scrollbar */
	final private Scrollbar m_vert = new Scrollbar(Scrollbar.VERTICAL);
	/** The horizontal scrollbar */
	final private Scrollbar m_horiz = new Scrollbar(Scrollbar.HORIZONTAL);

	/**
	Construct an instance of AddScrollbars to add scrollbars to the specified component.

	@param comp		The component to which we want to add scrollbars
	@throws java.lang.Error	If the component does not implement {@link IScrollable}
	*/
	public AddScrollbars(final Component comp)
	{
		if(!(comp instanceof IScrollable)) throw new java.lang.Error();

		setLayout(new BorderLayout());
		add("Center", comp);
		add("East", m_vert);
		add("South", m_horiz);

		m_scrollableComp = (IScrollable)comp;
		m_scrollableComp.set_scrollbars(m_vert, m_horiz);

		m_vert.addAdjustmentListener(new AdjustmentListener()
		{
			public void adjustmentValueChanged(AdjustmentEvent e)
			{
				m_scrollableComp.set_vertical_scrollbar_value(e.getValue());
				comp.repaint();
			}
		});

		m_horiz.addAdjustmentListener(new AdjustmentListener()
		{
			public void adjustmentValueChanged(AdjustmentEvent e)
			{
				m_scrollableComp.set_horizontal_scrollbar_value(e.getValue());
				comp.repaint();
			}
		});
	}
}
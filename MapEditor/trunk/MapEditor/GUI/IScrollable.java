package MapEditor.GUI;

import java.awt.*;

public interface IScrollable
{
	void set_horizontal_scrollbar_value(int i);
	void set_scrollbars(final Scrollbar vert, final Scrollbar horiz);
	void set_vertical_scrollbar_value(int i);
}
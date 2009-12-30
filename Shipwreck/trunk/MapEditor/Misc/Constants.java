package MapEditor.Misc;

import java.awt.event.MouseEvent;

/**
This interface defines global numeric constants. Classes which want to import the
constants, i.e. use unqualified names like EPSILON instead of Constants.EPSILON,
simply have to declare themselves as implementing the Constants interface. Whether
it's a good idea to use the unqualified names is a design decision, of course.
*/
public interface Constants
{
	final public static double EPSILON = 0.0001;
	final public static int LEFT_BUTTON = MouseEvent.BUTTON1;
	final public static int RIGHT_BUTTON = MouseEvent.BUTTON3;
	final public static double SMALL_EPSILON = 0.0000000001;
}
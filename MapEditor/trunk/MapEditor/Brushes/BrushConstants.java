package MapEditor.Brushes;

/**
This interface defines global brush constants. Classes which want to import the constants,
i.e. use unqualified names like HANDLE_SIZE instead of BrushConstants.HANDLE_SIZE, simply have
to declare themselves as implementing the BrushConstants interface. Whether it's a good idea
to use the unqualified names is a design decision, of course.
*/
public interface BrushConstants
{
	int HANDLE_SIZE = 8;					// the size of the handles on a selected brush (in pixels)
	double MINIMUM_BRUSH_DIMENSION = 0.01;	// the smallest one of the dimensions of a brush can be
}
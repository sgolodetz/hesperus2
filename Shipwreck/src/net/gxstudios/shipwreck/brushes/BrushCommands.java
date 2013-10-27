package net.gxstudios.shipwreck.brushes;

import net.gxstudios.shipwreck.commands.Command;

public class BrushCommands
{
	public static Command delete_brush(final IBrush b, final IBrush.IBrushContainer container)
	{
		if(b.is_ghost())
		{
			return new Command("Delete Ghost")
			{
				public void execute()
				{
					container.change_selection(null);
				}

				public void undo()
				{
					container.change_selection(b);
				}
			};
		}
		else if(container.contains(b))
		{
			return new Command("Delete Brush")
			{
				public void execute()
				{
					container.delete_brush(b);
				}

				public void undo()
				{
					container.add_brush(b);

					// To delete the brush in the first place, it must have been selected beforehand.
					container.change_selection(b);
				}
			};
		}
		else
		{
			throw new Error();		// this should never happen
		}
	}

	public static Command flatten(final ArchitectureBrushComposite bc, final IBrush.IBrushContainer container)
	{
		final ArchitectureBrushComposite flattenedComposite = new ArchitectureBrushComposite();
		for(ArchitectureBrush ab: bc.component_brushes()) flattenedComposite.add_brush(ab);

		return new Command("Flatten Composite")
		{
			public void execute()
			{
				container.delete_brush(bc);
				container.add_brush(flattenedComposite);
				container.change_selection(flattenedComposite);
			}

			public void undo()
			{
				container.delete_brush(flattenedComposite);
				container.add_brush(bc);
				container.change_selection(bc);
			}
		};
	}

	public static Command flip_x(final IBrush b)
	{
		return new Command("Flip X")
		{
			public void execute()	{ b.flip_x();	}
			public void undo()		{ b.flip_x();	}
		};
	}

	public static Command flip_y(final IBrush b)
	{
		return new Command("Flip Y")
		{
			public void execute()	{ b.flip_y();	}
			public void undo()		{ b.flip_y();	}
		};
	}

	public static Command flip_z(final IBrush b)
	{
		return new Command("Flip Z")
		{
			public void execute()	{ b.flip_z();	}
			public void undo()		{ b.flip_z();	}
		};
	}

	public static Command group(final ArchitectureBrushComposite bc)
	{
		return new Command("Group")
		{
			public void execute()	{ bc.group();	}
			public void undo()		{ bc.ungroup();	}
		};
	}

	public static Command paste(final IBrush b, final IBrush.IBrushContainer container)
	{
		final IBrush copy = b.copy();
		final IBrush oldSelection = container.get_selected_brush();
		return new Command("Paste")
		{
			public void execute()
			{
				container.add_brush(copy);
				container.deselect_selection();
				container.change_selection(copy);
			}

			public void undo()
			{
				container.select_brush(oldSelection);
				container.delete_brush(copy);
			}
		};
	}

	public static Command select(String description, final IBrush newSelection, final IBrush oldSelection, final IBrush.IBrushContainer container)
	{
		return new Command(description)
		{
			public void execute()
			{
				container.deselect_selection();
				container.select_brush(newSelection);
			}

			public void undo()
			{
				container.deselect_selection();
				container.select_brush(oldSelection);
			}
		};
	}

	public static Command ungroup(final ArchitectureBrushComposite bc)
	{
		return new Command("Ungroup")
		{
			public void execute()	{ bc.ungroup();	}
			public void undo()		{ bc.group();	}
		};
	}
}
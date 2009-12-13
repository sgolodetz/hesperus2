Things which may need looking at:

ArchitectureBrushComposite (design rethink perhaps? implementation issues also)
BrushComposite (design rethink perhaps?)

(THE ABOVE THINGS ARE NO LONGER RELEVANT, THE DESIGN HAS MOVED ON)

DONE

* The zoom level is the wrong way round. (FIXED)
* Points in Coords should have component types of double, not int. (FIXED)
* The way adding brushes to the map using the mouse works should be controlled by the type of brush; the code
  shouldn't be in DesignCanvas. However, exactly what happens still depends on which DesignCanvas has the
  focus. (DONE)
* We're going to have a composite brush type called BrushComposite. (DONE)
* Figure out what's wrong with putting the axes in the other direction. (FIGURED OUT AND FIXED)
* The brush should have the responsibility of deselecting itself if necessary. This makes the whole resize handle
  issue much easier to deal with (DONE).
* Write a separate routine to draw rectangles, whichever way round we want to specify their coordinates, and then
  use it instead of Graphics.drawRect wherever the latter is inappropriate.
  (DONE - see GraphicsUtil.draw_rectangle)
* Globally change the formatting for adding listeners (DONE).
* Use a WindowListener instead of processEvent in MainWindow (DONE).
* Put edge-resizing in (DONE).
* We need some way of devising the missing components in 3D of objects drawn in 2D.
  (and we've got it - when we move around the canvases, we change the relevant components of a Point3d in the map,
   and then we've got our third component when we need it)
  (DONE)
* Change the name of GraphicsUtil to Util and write a generic swap method in it. (DONE)
  (THIS WAS DUMB, YOU CAN'T WRITE SWAP METHODS, GENERIC OR OTHERWISE, IN JAVA! I'VE TAKEN IT OUT)
* Implement rotation (DONE).
* Implement dashed selection brushes, etc. (DONE)
* Fix the problem with resizing. (DONE)
* Check that Polygon normals are being computed in the right direction (FIXED - THEY WEREN'T).
  (CARE NEEDS TO BE TAKEN ABOUT THEM IF WE WANT TO CHANGE THE WAY RESIZING WORKS TO ALLOW REFLECTION)
* Write an option-storer for Map. (DONE)
* Fix and finish ArchitectureBrushComposite - it needs a lot of work.
  (DONE, EXCEPT FOR SHEARING, WHICH ISN'T EVEN IMPLEMENTED IN THE INDIVIDUAL BRUSHES YET).
* Implement reflection (DONE).
* Figure out why Polygon normals are pointing the wrong way when we create brushes on the (X,-Z) canvas, but
  only for cylinders, cones and spheres (THE RIGHT PLACE TO LOOK IS PROBABLY AT THEIR STATIC CREATION METHODS
  IN PolygonalBrush).
  (I'VE FIGURED OUT WHAT THE PROBLEM IS, I THINK, IT'S TO DO WITH WHICH SIDE WE GENERATE THE TOP ON).
  (FIXED - IT WAS TO DO WITH "NATURAL" AXIS PAIRS)
* Figure out why we're ending up outside the editable area when we zoom out (GRRRR, I THOUGHT THIS WORKED!).
  (FIXED - IT WAS TO DO WITH THE STAGE AT WHICH WE UPDATE THE SCROLLBAR RANGES)
* Change which corner is fixed during translation depending on where we click so that different ones can be
  snapped to the grid (SHOULDN'T BE TOO TRICKY, BUT DEFINITELY WORTH DOING).
  (DONE)
* Decouple ArchitectureBrush from DesignCanvas.
  (USE IRenderer INSTEAD, ADDING METHODS TO IT WHERE NECESSARY)
  (DONE)
* Fix everything marked URGENT in the source code!
  (DONE)
* Decouple the Brushes package from the Maps package.
  (Maps SHOULD USE Brushes, BUT NOT VICE-VERSA)
  (DONE, ALTHOUGH THE METHOD USED, NAMELY USING THE IBrushDeghoster INTERFACE, SEEMS QUESTIONABLE)
* Fix saving and loading functions in PolygonalBrush.
  (SEE THE FIXME COMMENTS IN THE FUNCTIONS - I SHOULD BE SAVING THE VALID BRUSH NOT THE ACTUAL ONE)
  (DONE)
* Change the layout of the DesignCanvases so that it's easier to see what's going on. Rich came up with this
  one and he's right.
  (EVENTUALLY I WANT TO MAKE THE LAYOUT OF THE CANVASES CUSTOMISABLE BY THE USER)
  (LAYING THEM OUT DIFFERENTLY REQUIRES DRAWING A BOX ROUND THE EDGE OF EACH OF THEM SO THAT IT'S CLEAR
   WHERE THE BOUNDARIES ARE)
  (SORT OF DONE, MINUS PUTTING THE BOX ROUND AS DESCRIBED)
* Implement shearing (MAJOR TASK).
  (DONE!)
* Find a better place for the IRenderer class than Geom and do something about the Misc.GraphicsUtil class.
  (THE FORMER NEEDS TO BE SOMEWHERE OTHER THAN THE GUI PACKAGE FOR DEPENDENCY REASONS, BUT PUTTING IT IN Geom ISN'T RIGHT)
  (THE LATTER CONTAINS A COUPLE OF UNRELATED FUNCTIONS WHICH PROBABLY SHOULDN'T BE IN THE SAME CLASS)
  (SORTED, SEE THE NEW Graphics PACKAGE)
* Check that the BSP implementation works.
  (SORT OF DONE, THOUGH MORE CHECKING WOULD BE A GOOD IDEA)
* Test the implementation of GeomUtil.split_polygon.
  (DONE)
* Get the status bar working.
  (ESSENTIALLY DONE, THOUGH GETTING IT TO DISPLAY MORE USEFUL INFORMATION WOULD BE GOOD)
* Implement GeomUtil.make_universe_polygon.
  (DONE, THOUGH IT'S DIFFICULT TO CHECK THAT IT'S RIGHT BECAUSE IT'S NECESSARILY UNDERSPECIFIED)
* Check the implementation of PolygonalBrush.split_with_plane.
  (DONE, BUT MORE CHECKING WOULD COME IN HANDY)
* Implement the second PolygonalBrush constructor.
  (DONE, BUT NOT RIGOROUSLY TESTED AS YET - TRY SAVING BRUSHES GENERATED THIS WAY)
* Change AxisAlignedBox.completely_contains so that the test has a bit more leeway in it (i.e. use EPSILON).
  (CHECK THE BRUSHES THAT ARE BEING GENERATED BY SPLITTING ACROSS PLANES TO MAKE SURE THAT THIS SHOULD BE DONE
   AND THAT IT'S NOT JUST A PROBLEM WITH THE ACTUAL SPLITTING CODE)
  (IT REALLY SHOULD BE DONE - THE SPLITTING CODE HAS SOME VERY MINOR PRECISION ISSUES WHICH ARE UNAVOIDABLE AND
   OF A NEGLIGIBLE ORDER OF MAGNITUDE, SO WE HAVE TO MAKE A SLIGHT ACCOMMODATION IN THE completely_contains
   METHOD)
  (DONE)
* Comment IRenderer.
  (DONE)
* Implement ArchitectureBrushComposite.split_with_plane.
  (DONE)
* Fix problem: When a SplitterBrush gets deselected (by whatever means), the brush inside it should replace it
  in the map.
  (FIXED)
* Fix problem: When a SplitterBrush gets deselected because the user clicked on "Select", the user should be able
  to select the brush inside it again immediately.
  (FIXED - THE BRUSH INSIDE IT GETS AUTOMATICALLY RESELECTED)
* Finish implementing Brushes.SplitterBrush.
  (DONE)
* Fix problem: See FIXME in SplitterBrush.deghost.
  (The composite should also contain all the unsplit component brushes of original composites.)
  (FIXED)
* Fix problem: Returning a pair of linked lists from split_with_plane is extremely messy and error-prone.
  (MORE THOUGHT NEEDED)
  (FIXED - we're now returning a SplitResults structure)
* Find a way to render composites inside SplitterBrushes correctly.
  (DONE)
* Render an infinite splitting line in SplitterBrush.
  (DONE)
* Make it possible to drag the handles on the infinite splitting line in SplitterBrush.
  (DONE)
* Add functions to GraphicsUtil which render ovals etc. correctly regardless of which way round we specify
  their bounds.
  (DONE, THOUGH THERE'S A CERTAIN AMOUNT OF CODE DUPLICATION - PROBABLY NOT WORTH DOING ANYTHING ABOUT THOUGH!)
* Add a parameter to the PolygonalBrush constructor allowing us to specify which edit mode it starts in.
  (NOT VITAL, BUT IT WILL MAKE THE CODE A LITTLE TIDIER)
  (DONE)
* Change the way menus are added in MainWindow.
  (IT'S RATHER HAPHAZARD AT THE MOMENT, MAYBE WE SHOULD BE GENERATING MENUS FROM A FILE?)
  (DONE, NOW LOADING MENUS FROM Resources/Menus.txt)
* Rather too much repainting may be going on as a result of my not being particularly consistent about where
  I'm calling m_map.repaint(). Matters can perhaps be improved by putting repaint calls in CommandManager's
  execute_command method, etc.
  (DONE)
* Should zooming be centred on the brush creation anchor? It might make more sense than the centre of the
  canvas, because if we're drawing at the side of a canvas and then zoom in, we want to see what we just drew, not
  have to scroll around for it. Even better, why not zoom in on the centre of the selected brush if there is one
  and if the centre of it's on the screen? While we're dealing with zooming, changing the function of the mouse
  wheel so that it zooms instead of scrolls might not be a bad idea either.
  (DONE - I'VE DECIDED TO KEEP THE + AND - KEYS ZOOMING ON THE CENTRE OF THE CANVAS, BUT I'VE REWRITTEN THE MOUSE
   WHEEL FUNCTION TO CUNNINGLY ZOOM IN ON THE POINT UNDER THE MOUSE WHILST KEEPING THE MOUSE OVER IT - THIS MAKES
   FOR ACCURATE ZOOMING)
* Constrain how far we can zoom in and out and changes in grid size - invalid combinations can crash the
  program!
  (AND I THOUGHT THERE WEREN'T MANY CRITICAL BUGS...)
  (I'VE FIXED THIS - IT DOES REMAIN TO BE SEEN WHETHER WE SHOULD ALLOW GRID SIZES LESS THAN 1 Coord, AND SOME OF
   THE EXACT CONSTRAINTS I'VE SET MAY NOT BE IDEAL - I MAY CHANGE THEM AFTER FURTHER EXPERIMENTATION)
* There appears to be a cyclic dependency between the Brushes and Commands packages. For some reason, SA4J
  isn't picking up on it, but that doesn't mean it's not there. I need to find a good way to centralise
  command stuff whilst removing the cyclic dependency. I could get rid of the dependency by moving command
  creation to the point of execution, but then I'd have to rewrite the code for (say) selection multiple
  times, which is dodgy.
  (IT TURNS OUT THAT THE REASON SA4J DIDN'T PICK UP ON IT WAS THAT I NEEDED TO DELETE ALL THE CLASS FILES
   AND REBUILD PROPERLY)
  (I'VE CENTRALISED THE COMMAND STUFF TO SOME EXTENT IN Brushes.BrushCommands. SOME COMMANDS DO SEEM TO
   BE INHERENTLY LOCAL, LIKE ROTATION etc., I'M LEAVING THEM WHERE THEY ARE FOR THE MOMENT)
  (THERE'S NO LONGER A CYCLIC DEPENDENCY, WHETHER I MIGHT WANT TO TRY AND CENTRALISE THE COMMANDS A BIT MORE
   AT SOME STAGE REMAINS TO BE SEEN)
* Make further changes to the way menus are added in MainWindow.
  (WE NEED TO BE ABLE TO ACCESS MENU ITEMS DURING THE PROGRAM, SO WE CAN'T JUST ADD THEM AND FORGET ABOUT
   THEM - IN PARTICULAR, IT WOULD BE NICE TO BE ABLE TO CHANGE THEIR LABELS SO THAT WE CAN REFLECT THE CURRENT
   STATE, e.g. "Undo Brush Translation", etc.)
  (ESSENTIALLY, WHAT I'M ADVOCATING IS STORING ALL THE MENU ITEMS BY NAME IN SOMETHING LIKE A HASHTABLE OR A
   BINARY SEARCH TREE)
  (DONE, SEE Commands.MenuManager)
* ArchitectureBrush.near_handle needs a rethink - we want to check the distance in pixels, not in level
  coordinates, otherwise it doesn't work when (say) we zoom in and decrease the grid size to minimum.
  (PROBABLY ALL THAT NEEDS DOING IS TO CHANGE near_handle TO TAKE THE APPROPRIATE IRenderer AND USE ITS
   distance_squared METHOD)
  (near_edge_x AND near_edge_y ALSO NEED CHANGING)
  (CHANGED THEM AND IT WORKS FINE, JUST NEED TO GET RID OF THE OLD CODE AND COMMENT THEM PROPERLY NOW)
  (DONE - IN ADDITION, I'M NOW USING A NEW pixel_distance METHOD IN IRenderer TO IMPROVE THE EFFICIENCY A BIT)
* Write all the comments promised by TODOs! I don't want to write them this evening, but they need doing and
  they'll be harder to write the longer I put it off...
  (IMPORTANT)
  (DONE)
* Finish adding undo and redo facilities.
  (THIS IS ACTUALLY SURPRISINGLY TRICKY, AND WILL INVOLVE CHANGING A LOT OF CODE)
  (DON'T FORGET THINGS LIKE RESETTING THE COMMAND MANAGER WHEN WE LOAD A NEW MAP, etc.)
  (STILL TO DO (PROBABLY NOT A COMPREHENSIVE LIST): Deletion, Resizing, Selection, Shearing, Splitting (probably tricky!), Translation)
  (TRANSIENT BRUSH COMPOSITES ARE POTENTIALLY HARD TO DEAL WITH - MORE THOUGHT NEEDED)
  (SELECTION'S PRETTY MUCH DONE AS FAR AS I CAN SEE)
  (SPLITTING'S ALSO DONE NOW - IT WASN'T PARTICULARLY TRICKY, AS IT TURNED OUT)
  (RESIZING'S DONE - JUST SHEARING, TRANSLATION AND DELETION TO GO!)
  (DONE SHEARING, ONLY TRANSLATION AND DELETION LEFT NOW)
  (DONE DELETION, ONLY TRANSLATION LEFT)
  (ALL DONE!)
* Make individual brush selection possible (TRICKY - WILL NEED TO IMPLEMENT A PROPER near_edge FUNCTION).
  (I'VE MADE A BIT OF PROGRESS WITH THIS - YOU CAN NOW SELECT BRUSHES BY CLICKING ON THEIR CENTRE CROSSES)
  (DONE - SEE GeomUtil.distance_square_from_linesegment AND THE CORRESPONDING METHOD IN IRenderer, AMONG
   OTHERS)
* When we select brushes at the moment, we can immediately transform them. This causes slight problems when
  we select a brush using an edge that is on the brush's bounding box, because then we end up unintentionally
  resizing the brush. This is annoying - it's probably worth sacrificing immediate transformations to prevent
  this happening.
  (FIXED - WE DIDN'T NEED TO SACRIFICE IMMEDIATE TRANSFORMATIONS, IT'S FIXABLE WITHOUT)
* Implement cut, copy and paste commands.
  (DONE)
* Write test code for Geom.GeomUtil.rotate_about_axis.
  (DONE)
* Examine all the repaint calls to see if any of them should be repaint_full calls.
  (DONE, THOUGH PERHAPS WE'RE DOING TOO MANY REPAINTS NOW? MIGHT BE WORTH EXAMINING AT SOME STAGE)
* Enable back-face culling.
  (DONE)
* Random zoom levels (e.g. 1.23) cause major problems with the grid - these aren't a problem as we're doing
  things at the moment, but it would be nice to be able to pick an arbitrary zoom level and have it work -
  I need to investigate what the problem is.
  (THE PROBLEM APPEARS TO BE IN THE DesignCanvas.find_nearest_grid_intersect_in_... METHODS - IT'S A ROUNDING
   ERROR AT SOME POINT)
  (SORTED, EVERYTHING NEEDED CHANGING TO USE doubles RATHER THAN ints)
* Reset the camera position in View3D when we create or load a map.
  (DONE)
* Write test code for Geom.GeomUtil.determine_line_intersection_with_plane and make the comment in
  Geom.GeomUtil.determine_linesegment_intersection_with_plane a bit more explanatory.
  (DONE)
* Finish working on the clipping stuff in BSP.TreeUtil and elsewhere.
  (I'VE GOT POLYGON CLIPPING WORKING WITH THE NEW FRAMEWORK, NOW FOR BRUSH CLIPPING!)
  (DONE!)
* Grouping and ungrouping of brushes.
  (SHOULDN'T BE TOO TRICKY, I HOPE! JUST NEED TO ADD A transient BOOLEAN TO ArchitectureBrushComposite AND
   CHANGE A BIT OF THE LOGIC)
  (DONE, THOUGH THE RENDERING COULD DO WITH SOME AESTHETIC IMPROVEMENTS)
  (SOMETHING REMAINS TO BE SAID ABOUT HOW BRUSHES WHICH WERE IN BRUSH GROUPS BEFORE PLANE SPLITTING SHOULD
   BE GROUPED AFTERWARDS)
  (SORTED THE GROUPING AFTER SPLITTING ISSUE)
* Think VERY carefully about how selection and rendering works in the Brushes package.
  (IT'S A HUGE MESS AT THE MOMENT AND NEEDS SERIOUS CLEARING UP)
  (WE IDEALLY WANT BRUSHES TO KNOW ABSOLUTELY NOTHING ABOUT MAPS, WHETHER VIA CONTAINERS OR OTHERWISE)
  (SORT OF DONE - SELECTION ISN'T QUITE SUCH AN ALMIGHTY MESS NOW, THOUGH RENDERING STILL NEEDS WORK)
  (THE select FUNCTIONS STILL NEED SERIOUS THOUGHT - FOR INSTANCE, WHEN WE SELECT A PolygonalBrush, WE NEED
   ITS select FUNCTION TO BE CALLED IN ORDER TO SET ITS m_fixedCorner_Coords VARIABLE TO null, BUT IT'S NOT
   CALLED WHEN WE SELECT IT AS A RESULT OF DEGHOSTING A SelectionBrush. WE CAN'T JUST CALL IT, BECAUSE OTHER
   select FUNCTIONS AREN'T DOING THINGS WE WANT AT THAT POINT!)
  (SORT OF FIXED)
* Was making the BSP tree internals public such a great plan? I remain unconvinced, so let's see if there's
  a better way to do it. Can't think of one offhand, but worth a bit of thought. There's no point in making
  lots of things depend on a not-especially-wonderful BSP tree implementation which I might want to change
  later.
  (THE WAY I'M GOING TO DO IT IS TO PROVIDE A GENERIC "clip_to_tree" FUNCTION IN BSP.TreeUtil, WHICH TAKES
   SOME SORT OF FUNCTOR SPECIFYING WHAT TO DO FOR A PARTICULAR TYPE OF ENTITY, e.g. A POLYGON OR A BRUSH)
* Fix problem: Picking is behaving weirdly after doing CSG. Might be a CSG problem, but weird if so.
  Could well be a general problem with the picking.
  (IT'S NOT (ENTIRELY, AT ANY RATE) A CSG PROBLEM - THE ORIGINAL WASN'T ACTUALLY WORKING, WRITING THE
   CSG CODE HAS JUST HIGHLIGHTED THAT FACT)
  (FIXED IT, I WAS MODIFYING THE DIRECTION VECTOR IN GeomUtil.determine_line_intersection_with_plane,
   RATHER THAN CLONING IT AND MODIFYING THE CLONE - THE TESTS DIDN'T PICK IT UP BECAUSE I WASN'T USING
   THE SAME DIRECTION VECTOR MORE THAN ONCE)
* Finish commenting the CSG code written so far.
  (DONE)
* Render polygon normals in the 3D view as well when the option's set.
  (DONE)
* Rewrite ArchitectureBrushComposite.save_MEF.
  (WE WANT TO PRESERVE BRUSH GROUPS WHEN WE SAVE, NOT MERELY SAVE THE INDIVIDUAL BRUSHES!)
  (FIXED, IT INVOLVED WRITING A load_MEF METHOD AS WELL, OBVIOUSLY)
* Doing CSG carve operations with group brushes might appear to work, but the code for it is actually wrong.
  One obvious way to fix things is to rewrite the CSG code in the brushes themselves, getting rid of the
  tree() function in favour of brush-specific code. The idea is that clipping to a group brush is like
  iteratively clipping to all its component brushes, and we could code that. The disadvantages of that idea
  are two-fold: firstly, it provides bad separation of concerns, in that we end up with CSG code intermingled
  with brush code; secondly, it's likely to result in many more resulting brushes than are actually needed.
  A better way to do it is to do a proper CSG union of the brushes in the group, then to carve with the
  unioned brush. This will require substantially more work, though.
  (WE CAN MAKE USE OF THE UBIQUITOUS BSP.TreeUtil.clip_to_tree METHOD TO DO THE CSG UNION OPERATION - OUR
   FIRST STEP IS TO MAKE THE COPLANAR CASE EXTERNALLY CUSTOMISABLE - SEE BSP.IClipFunctor.pass_coplanar_back
   AND THE OVERRIDDEN VERSION IN BSP.PolygonClipFunctor)
  (THE CSG UNIONING CODE IS WRITTEN - SEE BSP.TreeUtil.general_union, BUT THE COMPOSITE BRUSH TREE
   GENERATION STILL NEEDS WORK TO GET NESTED COMPOSITES WORKING)
  (ALL SORTED - I MOVED BSP.TreeUtil.general_union to CSG.CSGUtil.general_union_op, SINCE IT IS, AFTER ALL, A
   CSG OPERATION, AND COMPOSITE BRUSH TREE GENERATION NOW WORKS PROPERLY - SEE THE tree_polygons METHODS TO
   SEE HOW)
* Rewrite MainWindow.load_menus to allow nested menus.
  (DONE)
* Write BSP.TreeUtil.generate_optimal_convex_polygon_set. It should take a set of convex polygons such that
  GeomUtil.polygons_surround_convex_volume(...) returns true, and generate the optimal brush for them, in
  other words the minimal set of convex polygons which surround the same volume. Operationally, this
  involves building a universe polygon on each of the unique planes in which a parameter polygon lies
  and clipping it using all the other planes.
  (DONE)
* As a result of writing the CSG union code, it will be possible to try and combine convex brushes into
  larger ones. Checking for convexity is a relatively straightforward operation (just check that the brush
  is completely behind every plane defined by one of its polygons).
  (PARTIALLY DONE, JUST NEED TO TIDY UP THE RESULTING POLYGONS NOW)
  (DONE)
* Add brush hollowing.
  (DONE, THOUGH WE STILL NEED A DIALOG TO ALLOW USERS TO SET THE HOLLOWING THICKNESS)
  (ADDED THE DIALOG, SEE GUI.InputDialog)
* Sort out the movement speed issue - it's not that the map's rendering slowly (luckily), it's just that
  we're moving at a fixed speed which isn't great enough for large-scale maps.
  (TO FIX THIS, IMPLEMENT THE Camera MENU COMMANDS)
  (DONE)
* CSG (including brush carving) - first of all, implement CSG.ClipUtil.clip_brush_to_tree.
  (DONE BRUSH CARVING, ACTUALLY ENDED UP WRITING IT DIFFERENTLY - SEE CSG.CSGUtil.carve)
  (NOW FOR CSG INTERSECTION, ETC. - THE GUI IS THE REAL ISSUE WITH THAT)
  (DONE GENERAL CSG UNION, STILL NEED TO DO CONVEX CSG UNION AND EVERYTHING ELSE)
  (IT TURNS OUT THERE'S A BETTER WAY TO DO THINGS - I'VE IMPLEMENTED INTERSECTION, AND UNION WORKS IF THE RESULT IS
   CONVEX, BUT IF IT'S NOT THEN THERE'S STILL A BIT OF WORK TO DO - SEE CSG.CSGUtil.symmetric_op)
  (IT ALSO TURNS OUT THAT INTERSECTION NEEDS A BIT MORE THOUGHT THAN I'VE GIVEN IT - THE QUESTION IS ABOUT WHAT POLYGON
   CLIP FUNCTORS WE SHOULD BE USING)
  (FIXED THAT - I WAS USING THE WRONG CLIP FUNCTORS FOR INTERSECTION, OPPOSITE-FACING COPLANAR POLYGONS SHOULD BE PASSED
   DOWN THE FRONT OF THE TREE FOR INTERSECTION, NOT THE BACK)
  (FINALLY, CSG APPEARS TO BE WORKING, THOUGH IT MAY NEED A BIT OF A RETHINK WHEN I INTRODUCE TEXTURING -
   SEE LONG-TERM TODOs)
* Change SelectionBrush.render_selected so that the translation anchor gets rendered for
  selection brushes.
  (NOT VITAL, BUT NEEDS DOING AT SOME POINT)
  (DONE)
* Check over all the render, render_selected and render_bounds methods and check that everything's in the right place.
  (IMPORTANT, DO THIS BEFORE ANYTHING ELSE!)
  (DONE ONCE, BUT COULD DO WITH SOME LOOKING OVER AGAIN)
  (ArchitectureBrush.render MIGHT BE RENAMED TO render_centre_cross, FOR INSTANCE - THOUGHT NEEDED)
  (I'VE RENAMED THAT BUT THERE'S MORE STILL TO DO)
  (WELL IT'S TIDIER THAN IT WAS, I'VE GOT RID OF ALL THE super.render(...) STUFF)
  (DONE, IT'S NOW A FAIRLY CLEAN IMPLEMENTATION)
* I'm not sure SelectionBrush sits very easily as a subclass of ArchitectureBrush. It doesn't implement most
  of the things an ArchitectureBrush is supposed to be able to do, it's not splittable, ...
  (THE SOLUTION IS CLEARLY TO MOVE IT, BUT THE PROBLEM IS THAT A LOT OF THE FUNCTIONALITY IN ArchitectureBrush
   IS QUITE USEFUL IN THE IMPLEMENTATION OF SelectionBrush. IN PARTICULAR, THE RESIZING AND TRANSLATING ARE
   THINGS WE DON'T WANT TO REWRITE. THE SOLUTION IS PROBABLY TO MOVE ALL THAT FUNCTIONALITY INTO A HIGHER
   CLASS, AND INHERIT BOTH ArchitectureBrush AND SelectionBrush FROM THAT. THAT REPRESENTS A CONSIDERABLE
   AMOUNT OF WORK, THOUGH, SO I'M NOT KEEN ON TACKLING IT AT THE MOMENT WHEN THE EXISTING SOLUTION WILL
   SUFFICE FOR NOW.)
  (SORTED THIS, OUR HIERARCHY NOW HAS ResizableTranslatableBrush AND TransformableBrush - SelectionBrush IS A
   ResizableTranslatableBrush AND ArchitectureBrush IS A TransformableBrush)
* Have a look at whether we need to clear the state on both selection and deselection in ArchitectureBrush.
  It seems a bit wasteful.
  (WE DON'T, AND WE'RE NOT DOING IT ANY MORE IN ANY CASE FOR AN UNRELATED REASON TO DO WITH BRUSH STATES)
* We still need to check whether the undo and redo stacks get reset at appropriate times. I've done it for
  loading and creating new maps, there might be others. There are probably minor little things to fix, I
  need to keep a sharp eye out for them!
  (EVERYTHING SEEMS TO BE WORKING OK, AS FAR AS I CAN SEE)
* Finish implementing picking.
  (ArchitectureBrushComposite.pick, SelectionBrush.pick AND SplitterBrush.pick STILL NEED TO BE WRITTEN)
  (DONE ArchitectureBrushComposite.pick AND SplitterBrush.pick - ONLY SelectionBrush.pick REMAINING NOW,
   BUT THAT'S NOT ENTIRELY STRAIGHTFORWARD, UNFORTUNATELY, BECAUSE SelectionBrush DOESN'T HAVE POLYGONS)
  (ACTUALLY, WE DON'T NEED TO PICK A SelectionBrush IN ANY CASE, SO THERE'S NOTHING TO DO)
* Write code to allow flattening of nested composites. By this I mean that if we've got group brushes inside
  group brushes, it would be nice to be able to take all the individual brushes inside them and put them in
  a top-level group brush.
  (DONE)
* Sort out the rendering of group brushes, it's not ideal.
  (IT WOULD BE NICE, FOR INSTANCE, IF COMPONENT BRUSHES OF GROUPS WERE ALL RENDERED IN THE SAME COLOUR)
  (COMPONENT BRUSHES ARE NOW ALL RENDERED IN THE SAME COLOUR)
  (DO WE WANT TO RENDER THE CENTRE CROSS ALL THE TIME, EVEN WHEN GROUP BRUSHES AREN'T SELECTED? THERE ARE
   OTHER AESTHETIC THINGS TO CONSIDER AS WELL...)
  (I'VE DECIDED IT'S BETTER NOT TO RENDER THE CENTRE CROSS UNLESS THE GROUP BRUSH IS SELECTED)
* Fix problem: Multiselection involving a ghost brush is messed up. To reproduce the symptoms, do the following:
	i)		Create normal brush
	ii)		Create ghost brush
	iii)	Ctrl-click the normal brush in the 3D view
  (FIXED, I NEEDED TO ENSURE THAT THE GHOST BRUSH WAS DEGHOSTED BEFORE IT WAS GROUPED USING MULTISELECTION)
* Change the way shearing is rendered so that it's easier for the user to see what's going on.
  (WE WANT TO DRAW SOME SORT OF "SHEARED" BOUNDING BOX WHILE THE USER'S DOING THE SHEARING)
  (DONE)
* Take a look at Brushes.PolygonalBrush.tree_polygons - are we dealing with invalid brushes properly?
  (YOU CAN'T HAVE INVALID BRUSHES ANY MORE, SO THE ANSWER'S YES)
* Continue writing the texturing code.
  (IT'S COMING ON REALLY WELL - THE TEXTURES ARE THERE, IT'S JUST A CASE OF MAKING SURE THE TEXTURE PLANE
   GETS UPDATED CORRECTLY WHEN NECESSARY, AND SORTING OUT THE LOADING AND SAVING OF TEXTURES. THEN I CAN
   WRITE THE DIALOG TO ALLOW THE USER TO MODIFY THE TEXTURING, WHICH SHOULDN'T BE TOO TRICKY IF EVERYTHING
   ELSE WORKS)
  (THE TEXTURE PLANE'S NOW GETTING UPDATED CORRECTLY FOR SPLITTING, ROTATING, TRANSLATING, AND BRUSH CARVING,
   AT LEAST AS FAR AS I CAN TELL - THIS NEEDS VERIFYING. I STILL NEED TO WRITE CODE TO UPDATE IT FOR RESIZING
   AND IT'S WELL WORTH HAVING ANOTHER LOOK AT HOW I'M UPDATING IT FOR SHEARING - THE CURRENT METHOD I'M USING
   ISN'T IDEAL. I STILL NEED TO FIGURE OUT WHAT TO DO ABOUT TEXTURING AND CSG, I MIGHT JUST LEAVE IT AS IS,
   SINCE PEOPLE CAN ALWAYS REDO THE TEXTURES AND IT'S BETTER TO HAVE AS FEW POLYGONS AS POSSIBLE)
  (I THINK I'VE GOT IT DOING SOMETHING REASONABLE FOR RESIZING)
  (I'M NOT DOING TEXTURING THIS WAY ANY MORE, SO THIS IS REDUNDANT)
* Change the way texture planes work. At the moment, I've got the base u axis going from the first to the second
  vertex of a polygon. This is very arbitrary. What I really want is for all polygons which share a plane to have
  the same base u axis. I might be able to make use of GeomUtil.generate_arbitrary_coplanar_unit_vector somehow.
  (I'M NOT DOING TEXTURING THIS WAY ANY MORE, SO THIS IS REDUNDANT)
* Fix problem: When we change the texture parameters (e.g. scaling), then resize immediately afterwards, the texture plane reverts
  to what it was before the modification.
  (THIS TURNS OUT TO BE RELATED TO HOW WE'RE DOING OUR RESIZING - WE HAVE A CACHED REPRESENTATION OF THE POLYGONS IN PolyhedralBrush
   FROM WHICH WE'RE GENERATING OUR RESIZED VERSION EACH TIME. SINCE WE'RE NOT UPDATING THE INTERNAL REPRESENTATION WHEN WE CHANGE
   THE TEXTURES, RESIZING STRAIGHT AWAY BEFORE IT'S BEEN UPDATED CAUSES THE TEXTURES TO REVERT TO WHAT THEY WERE BEFORE. THE CORRECT
   THING TO DO HERE IS PROBABLY TO DO AWAY WITH THE DODGY INTERNAL REPRESENTATION IDEA, IT'S BEEN A PAIN RIGHT FROM THE START. TO DO
   THIS MEANS WE'LL HAVE TO PREVENT ANY OF THE BRUSH DIMENSIONS BECOMING ZERO, WHICH IS MY NEXT STEP. THE PROBLEM LAST TIME I TRIED
   TO DO THIS WAS THAT THE BRUSH DIMENSIONS WERE ZERO INITIALLY - THIS HAS NOW BEEN RECTIFIED (see DesignCanvas.generate_brush_creation_bounds
   AND ResizableTranslatableBrush.mouse_pressed)).
  (THIS APPEARS TO BE FIXED, BUT NOW WOULD BE A GOOD OPPORTUNITY TO TIDY UP THE VARIOUS begin_transform AND end_transform METHODS,
   IN PARTICULAR BY NO LONGER UPDATING CACHED VERSIONS OF A BRUSH WHEN THE TRANSFORMATION'S FINISHED)
  (FIXED)
* Sort out the initial texturing of polygons.
  (DONE)
* Sort out texture plane changes for shears - note that doing the reverse shear must return us to the original texture plane.
  (DONE)
* Sort out texture scaling/offsets - when we scale, the scaling should be from the edge of the polygon, not from the origin.
  (ACTUALLY, I NO LONGER THINK THIS IS THE CASE)
* Thinking about it more clearly, it may not be possible to maintain texture coordinates when resizing and rotating,
  since depending on what resize/rotate is done, the relevant angles may not be preserved (even though rotation is
  an "angle-preserving" transformation).
  (SORTED, I'M NOW PRESERVING TEXTURE COORDINATES IF POSSIBLE AND TEXTURE PARAMETERS IF NOT - SEE PolyhedralBrush.generate_reasonable_texture_plane)
* An invert mouse option for the camera would be nice.
  (DONE)
* Move the camera speed modifiers from View3D to Camera.
  (DONE)
* Reenable the "Preserve Texture Coordinates" option and write the code to get it working.
  (DONE)
* Fix problem: Brush carving as implemented is ludicrously inefficient: we're rebuilding the tree for the carve brush for every
  single other brush in the map! I don't know why I did it like that (it's incredibly dumb), but it needs fixing.
  (FIXED - IT DRAMATICALLY SPEEDS UP BRUSH CARVING)
* Write test code for MiscUtil.array_from_list.
  (DONE)
* Implement a dialog for map translation.
  (DONE)
* Write test code for Command.Seq.
  (DONE)

SHORT-TERM TODOs

* Check the commenting generally.
  (THIS IS A GOOD IDEA AFTER ANY MAJOR CODING SESSION!)
* Tidy up the GeomUtil class.
* Fix problem: When we click "Split Brush", if the cursor's not the normal pointer then it needs to be changed.
  (THERE ARE ISSUES WITH WHAT CURSOR IS DISPLAYED GENERALLY, ACTUALLY, IT NEEDS SORTING AT SOME POINT)
* Changing the comments for ArchitectureBrush.{rotate,shear,translate,etc.} might be beneficial - they don't
  actually do the specified operations permanently!
* We can optimise the selection metric in PolygonalBrush by only calculating it if our click's within an
  extended bounding box for the brush. Note that being inside the normal bounding box is not necessary,
  since clicking just outside the box next to an edge should still select the brush.
* Implement the Brush->Split menu item.
  (THE CORRECT THING TO DO HERE IS PROBABLY TO ASSOCIATE CODE FOR COMMANDS WITH MENU ITEMS IN GENERAL, AND
   ALLOW EVERYTHING ELSE TO RUN THE CODE ASSOCIATED WITH A GIVEN MENU ITEM - THIS WILL REQUIRE CHANGES TO
   Commands.MenuManager, BUT SHOULDN'T BE ESPECIALLY TRICKY TO IMPLEMENT)
* Implement the remaining render3D and render3D_selected methods.
  (IN PARTICULAR, SelectionBrush.render3D_selected AND SplitterBrush.render3D_selected NEED SOME WORK)
  (DONE SelectionBrush.render3D_selected)
  (SPLIT PLANES STILL NEED TO BE RENDERED PROPERLY IN SplitterBrush.render3D_selected)
* Allow multiple brush selection via Ctrl-clicking.
  (DONE IN View3D, ALSO NEEDS DOING IN DesignCanvas)
  (THE MULTIPLE BRUSH SELECTION IN View3D ISN'T ENTIRELY RIGHT AND NEEDS ANOTHER LOOK)
  (FIXED THAT, THOUGH IT COULD STILL DO WITH SOME TIDYING UP)
  (IT'S A BIT OF A MESS AT THE MOMENT, ACTUALLY - MUCH MORE THOUGHT NEEDED)
  (WE WANT IT TO BE POSSIBLE TO REMOVE BRUSHES FROM THE COMPOSITE BY Ctrl-clicking ON THEM AS WELL - THIS
   WILL REQUIRE CHANGES TO THE PICKING CODE AND A FAIR AMOUNT OF THOUGHT)
  (IT'S NOW POSSIBLE TO REMOVE BRUSHES AS DESCRIBED, THOUGH THE CODE TO DO IT IS VERY MESSY - IT'S HARD TO
   SEE HOW TO IMPROVE IT, THOUGH)
* Improve wireframe rendering to get rid of hidden edges.
  (WHEN I TRIED THIS, THERE WAS A CERTAIN AMOUNT OF EDGE FLICKERING - IT SEEMS I NEED TO USE SOMETHING LIKE
   glPolygonOffset TO SORT THIS OUT)
* Write test code for BSP.TreeUtil.generate_optimal_convex_polygon_set, and all the other functions I've
  just added. In particular, Tree.construct_right_linear_tree could do with a look.
  (BSP.TreeUtil.generate_optimal_convex_polygon_set NO LONGER EXISTS, NOTHING NEEDS DOING ANY MORE, BUT
   THE OTHER FUNCTIONS DO STILL NEED TEST CODE AT SOME STAGE)
* Rewrite the test code for BSP.TreeUtil.clip_to_tree in light of the changes to PolygonClipFunctor.
* We may need to rethink a fair bit of the CSG code when we do texturing - at the moment, I'm rebuilding polygons
  on planes, but this won't work very well when texturing is taken into account.
* Finish implementing LandscapeBrush and implement LandscapeModifierBrush.
  (IN PARTICULAR, ALL THE SPLINE STUFF NEEDS DOING. I ALSO NEED TO WRITE A RESCALING FUNCTION WHICH FINDS THE
   MAXIMUM AND MINIMUM HEIGHTS, SCALES THEM TO 1.0 AND 0.0 RESPECTIVELY, AND RESCALES ALL THE OTHER HEIGHTS TO
   THE NEW SCALE.)
  (THE SPLINE STUFF IS DEFINITELY GETTING THERE. I NEED TO CHECK WHAT'S GOING ON WITH PARAMETRISATION THOUGH -
   I SUSPECT IT'S SOMETHING TO DO WITH THE CONSTRAINTS I'M USING.)
  (LOOK *VERY* CAREFULLY AT WHAT GradientConstraint IS DOING, I'M NOT SURE I WAS CONCENTRATING HARD ENOUGH WHEN I
   THOUGHT ABOUT IT!)
  (SPLINE SURFACES WOULD BE BETTER OFF AS A SEPARATE CLASS, THEY SHOULDN'T BE CALCULATED EXPLICITLY IN LandscapeBrush)
  (GradientConstraint IS CONSTRAINING THE GRADIENT AT A CONTROL POINT, NOT AT AN INTERPOLATED POINT, I'VE CHANGED THE
   COMMENT ACCORDINGLY)
  (IMPLEMENTING ACTUAL SPLINE SURFACES IS QUITE HARD - LUCKILY I DON'T NEED TO, I ONLY NEED TO GENERATE AN APPROPRIATELY
   SUBDIVIDED MESH, WHICH IS MUCH EASIER. EVENTUALLY I'LL HAVE TO FIND A GOOD WAY OF GENERATING A UNIFORMLY-SPACED
   HEIGHTMAP FROM THE MESH, BUT ONE THING AT A TIME. THE UPSHOT OF ALL THIS IS THAT I DIDN'T WRITE A SPLINE SURFACE
   CLASS, I JUST WROTE A MESH-GENERATING UTILITY IN SplineUtil.)
  (I'M NOT ACTUALLY GOING TO WRITE LandscapeModifierBrush, I'M GOING TO PUT ITS FUNCTIONALITY IN LandscapeBrush)
  (YOU CAN NOW MODIFY LANDSCAPES! BUT LOTS OF THINGS STILL NEED WORKING ON:
	(i)		The cursor needs to change appropriately when you move the mouse over a manipulator
	(ii)	The extents of the brush need to be rendered even in MODIFYING mode (but note that rendering the bounding box isn't sufficient,
			we don't want the resize handles etc. rendered as well)
	(iii)	Being able to select multiple manipulators is an essential user-friendliness feature
	(iv)	The manipulators could do with being bigger and thus easier to manipulate (that goes for the ones in SplitterBrush too)
	(v)		Changing a landscape height should be a command and thus undoable
	(vi)	It would be useful to be able to easily set landscapes to start at the top of the bounding box - makes it easier to do
			terrain that's high at the edges
	(vii)	Adaptive subdivision when rendering would be very useful
	(viii)	The ability to increase and decrease the resolution of brushes needs implementing
	(ix)	We need to be able to texture landscapes - I need to fix texturing generally first, though
	(x)		Saving and loading of landscapes needs doing
	(xi)	It would be extremely useful to have some sort of auto-joining feature for adjacent landscapes, which
			would move the control points of all the selected landscapes until they all join up seamlessly - this
			is likely to be pretty difficult to do, though, so if the worst comes to the worst, I'll just skip it
			(and pretend I never intended to implement it!)
	(xii)	It would be quite nice to be able to load in external landscapes specified by arbitrary heightmaps -
			it's slightly problematic because the external heightmaps will tend to be large; my landscape scheme
			is geared towards storing a small number of points and interpolating between them, rather than storing
			a lot of points
	(xiii)	Allowing the user to specify the constraints would make the whole thing a lot more flexible, but would
			introduce quite a bit of extra complexity
	(xiv)	Resizing totally messes things up at the moment; it's going to need some more thought
	(xv)	It's very hard to create "useful" landscapes just by moving control points - what might be needed is something
			like a hill/valley creation tool, which moves the control points to the right places to generate the features
			in question. It's conceivable that it might not be possible to always generate the required features at the
			current resolution, in which case increasing the resolution automatically might be the way to go. Hill/valley
			creation is best done in top-down view, I think.
			If I were going for a radical approach, I might suggest that in many ways the whole spline idea is
			not ideal and it might be better to dispense with it in favour of a purely feature-based scheme, i.e.
			you would then design only in terms of geographical features. That would mean getting rid of a lot of
			stuff I've put major effort into, but it might be for the best. I could always keep the spline code I
			wrote for future projects, "just in case it comes in handy one day"! On the plus side, changing the
			landscape scheme I'm using would allow me to avoid implementing many of the things in the above list.
			The whole thing might be simpler and more useful.
			(Additional thoughts: there's nothing to stop you having spline landscapes and feature-based landscapes
			 in the same editor. If users end up only using one of them, so be it.)
	(xvi)	It would be useful to be able to reflect landscapes
	(xvii)	It would be useful to be able to select landscapes, and in particular manipulators, in the 3D view
   IN ADDITION, LandscapeBrush COULD DO WITH SOME GENERAL TIDYING UP AT SOME STAGE)
  (DONE (i))
  (DONE (iv))
  (DONE (v))
  ((vi) IS UNNECESSARY, SINCE WE CAN JUST FLIP THE TERRAIN IN THE Z DIRECTION)
  (SORT OF DONE (viii), BUT IT'S DODGY - I NEED TO REWRITE SplineUtil.intersect_spline_segment_with_plane SO THAT IT'S NOT SO RUBBISH.
   IN PARTICULAR, I NEED TO FIND A WAY TO GET RID OF THE knot PARAMETER WHICH MAKES IT DIFFICULT TO USE.)
  (DONE (x))
  (DONE (xvi))
* Disentangle the Geom and Geom.Splines packages. Geom.Splines shouldn't be using anything from Geom, but it needs Plane.
  My proposed solution is to make a new Geom.Planar package, put Plane and Polygon etc. in it, and let Geom.Splines depend
  on that instead.
  (DONE - IT'S PROBABLY WORTH CHANGING GeomConstants TO PlanarGeomConstants AND GeomUtil TO PlanarGeomUtil, NOW THAT THEY'RE
   IN THE Geom.Planar PACKAGE. IT'S ALSO WORTH CHECKING THEIR COMMENTING, AND THAT ALL THE METHODS IN WHAT WILL BE THE NEW
   PlanarGeomUtil ARE WHERE THEY SHOULD BE.)
* At the moment, the texture dimensions are hard-coded into TexturePlane, they need to be passed in.
* It's possible to tidy up PolyhedralBrush.TransformFunctor somewhat by making translate take a 2D vector and an AxisPair rather
  than a 3D vector - whether this is necessarily a good idea remains to be seen.
* Clicking empty space when attempting to multiselect should be a no-op, rather than deselecting everything.
* Change the parameters of PolyhedralBrush.generate_reasonable_texture_plane to take polygons instead of their components, it's
  tidier that way.
* Finish writing the Math.Vectors classes and then change the whole project to use them instead of the Java3D classes. Note that
  this will entirely remove our dependence on Java3D!
* Finish tidying up the Test package.
* Move the options sets out of Map and into their own class.
  (DONE, BUT I HAVEN'T YET TAKEN FULL ADVANTAGE OF IT BY GETTING RID OF ALL THE PASSING OF OPTIONS AROUND AS PARAMETERS)
* Check that the faces() methods in ArchitectureBrush subclasses are correct and see whether they can be combined with the
  tree_polygons() methods: the two share a lot in common.
* Take a look at the methods in Map: do we really need a select_face method AND a select_faces method, or could we combine them?
* Write test code for GeomUtil.classify_polygon_against_plane and remove the old implementation of it. It's worth running the tests
  on the old implementation first to ensure they give the same results for both.
* Fix bug: Somewhere in the code I'm not adding a brush to the map when I should, or something along those lines.
  (I'M AWARE THAT THAT DOESN'T NARROW IT DOWN A LOT, I NEED TO FIND A WAY TO REPRODUCE THE BUG! IT'S ONLY HAPPENED ONCE SO FAR)
* Tidy up textured landscape rendering and make it possible for users to edit the texture.
* Save the angular and linear camera speed being used with a given map file so that users don't have to type them in each time.
  Make it possible when editing to (a) revert to the defaults or (b) revert to the saved speeds.
* Write code to load and save .map files.
  (I'M IMPLEMENTING THIS IN THE Maps.MapFileMAP CLASS)
  (THE LOADER IS CURRENTLY AT THE STAGE OF LOADING THE worldspawn ENTITY, BUT IT DOESN'T TRY AND LOAD OTHER ENTITY TYPES YET -
   IT DOES SKIP THEM, HOWEVER, ALLOWING THE LOAD TO PROCEED WITHOUT PROBLEMS)
* Find a way of combining generate_convex_volume_polygons_from_surrounding_planes and generate_convex_volume_polygons_from_surrounding_polygons.
  (THEY'RE VERY SIMILAR IN MANY WAYS, BUT DO SUBTLY DIFFERENT THINGS)
* Implement proper flat-shaded rendering.
  (DONE, BUT COULD BE IMPROVED SO THAT GROUPS ARE RENDERED USING THE SAME COLOUR)
* Make sure the right textures are loaded at various times.
  (IN PARTICULAR, I NEED TO RELOAD THE landscape TEXTURE WHEN LOADING .map FILES)
  (ALSO, WHEN THE USER STARTS A NEW MAP, I NEED TO UNLOAD ALL THE CURRENTLY LOADED TEXTURES AND LOAD ONLY null AND landscape)

LONG-TERM TODOs

* Think about exactly how we're setting the brush creation anchor (MORE THOUGHT WOULD BE USEFUL, BUT IT'S OK).
* Implement View3D (HUGE TASK - APPROACH WITH CAUTION).
  (WELL IT MAY BE A HUGE TASK, BUT I'VE GOT THE BASICS OF IT WORKING AT LEAST. CURRENTLY ALL IT DOES IS DRAW
   THE BRUSHES AND ALLOW YOU TO MOVE AROUND A BIT, BUT IT'S A GOOD START - THE NEXT THINGS TO ADD ARE BETTER
   NAVIGATION, PICKING, etc. IT WOULD BE NICE TO BE ABLE TO REPAINT WHILE BRUSHES ARE BEING DRAGGED AROUND,
   BUT WHEN I'VE TRIED THAT IT RUNS PRETTY SLOWLY - IT MIGHT BE POSSIBLE, IF I CAN CUT DOWN ON THE NUMBER OF
   REPAINTS CALLED WHILE THE USER'S DRAGGING, BUT IT WILL NEED SOME THOUGHT)
  (I'VE GOT PICKING WORKING, AND THE NAVIGATION IS MUCH BETTER, BUT IT WOULD BE NICE TO NAVIGATE WITH THE MOUSE:
   THAT'S MY NEXT STEP)
  (YOU CAN NOW NAVIGATE WITH THE MOUSE, BUT I STILL NEED TO MAKE IT SO THAT YOU PICK WITH THE LEFT MOUSE BUTTON
   AND NAVIGATE WITH THE RIGHT)
* Log non-critical errors properly (THINGS LIKE TRYING TO ROTATE WHEN A BRUSH DIMENSION IS ZERO).
* Think of a better name for DesignCanvas.update_scrollbars, it doesn't reflect what it does very well. That's
  probably why I've been finding writing a description for it difficult.
* Add code to check whether a brush has been modified and prompt the user to save their work.
  (NOT THAT IMPORTANT AT THE MOMENT)
  (WILL BE FAIRLY EASY TO IMPLEMENT ONCE THE COMMAND SYSTEM IS IMPLEMENTED, SO IT SHOULD WAIT TILL THEN)
  (THE COMMAND SYSTEM'S NOW IMPLEMENTED, BUT I HAVEN'T GOT ROUND TO THIS YET)
* We clearly want to get rid of these dodgy instanceof tests at some stage!
  (I'VE MADE A LITTLE BIT OF PROGRESS, ALL THE instanceof SelectionBrush TESTS HAVE NOW GONE)
* Splitting brushes using spline cuts strikes me as an interesting idea. Something to think about, anyway.
* It would be nice to do zooming out properly - at the moment, when we zoom out, eventually the range of the
  scrollbars affects the zooming.
* Add a dialog to set a map's details.
  (NOT THAT IMPORTANT AT THE MOMENT)
  (ADDED THE DIALOG, BUT IT DOESN'T DO AN AWFUL LOT YET)
* We can probably optimise CSG.CSGUtil.carve using bounding boxes, and from an efficiency standpoint it's
  clearly worth doing.
* Should any other functions be moved from Geom.GeomUtil to Math.MathUtil? Anything which doesn't mention either
  planes or polygons is a candidate.
* Fix problem: CSG intersection with a group brush doesn't do what I'd prefer. If brushes are actually
  grouped together, we'd like to treat them as indivisible. At the moment, they're being treated as if
  they're still separate brushes for the purposes of intersection. This makes complex intersections
  trickier, particularly when, say, we want to intersect with a concave group.
* Should Camera really be in the GUI package, or would it be better somewhere else?
  (NOT OF HUGE IMPORTANCE, BUT THESE THINGS ARE ALWAYS WORTH A THOUGHT)
* Saving the boolean options to a configuration file might be a good idea.
* If I find myself copying the set_caret again, create a GUIUtil class to put it in instead.
  (IT WILL NEED RENAMING TO SOMETHING MORE SENSIBLE IF I DO THAT, LIKE set_caret_to_end)
* Devise a binary map file format.
  (THE EXISTING TEXT FORMAT IS VERBOSE - MAPS TAKE UP TOO MUCH DISK SPACE)

RANDOM OTHER STUFF

* Fix Geom.GeomUtil.polygons_enclose_convex_volume and write test code for it.
  (FIXED, I THINK, BUT I HAVEN'T WRITTEN THE TEST CODE YET)
  (I'M NOT ACTUALLY USING THIS FUNCTION ANY MORE, THOUGH I MIGHT CONCEIVABLY IN THE FUTURE)
  (IT'S BEEN MOVED TO Unneeded Code.txt, IN CASE I EVER NEED IT AGAIN)
* Reorganise the source files. The order I'm arbitrarily going to choose for things is (note that not all may be present):

	SINGLETON IMPLEMENTATION	(in the rare cases that a class is a singleton)
	CONSTANTS					(easier to modify constants if they're at the top of a file)
	PUBLIC ENUMERATIONS			(ditto)
	PROTECTED ENUMERATIONS		(ditto)
	PRIVATE ENUMERATIONS		(ditto)
	NESTED CLASSES				(if they're all small)
	(SHOULDN'T HAVE PUBLIC OR PACKAGE VARIABLES)
	PROTECTED VARIABLES			(what variables we're providing to subclasses should be easily visible)
	PRIVATE VARIABLES			(what variables we're using internally should be easily visible)
	CONSTRUCTORS				(how to construct an instance of class should be easily visible)
	FACTORY METHODS				(these are similar to constructors in that we can construct an instance of a class with them)
	PUBLIC ABSTRACT METHODS		(what methods need overriding should be easily visible)
	PACKAGE ABSTRACT METHODS	(ditto)
	PROTECTED ABSTRACT METHODS	(ditto)
	(CAN'T HAVE PRIVATE ABSTRACT METHODS)
	PUBLIC METHODS				(the interface of the class should be easily visible)
	PACKAGE METHODS				(ditto)
	PROTECTED METHODS			(ditto)
	PRIVATE METHODS				(purely implementation detail, has to go somewhere but not near the top)
	NESTED CLASSES				(if any of them are long)
	TEST HARNESS				(if there is one)

  So far, I've reorganised all the ones in the Brushes and BSP packages, but there's still a little way to go.

DODGY CODE ELIMINATION (a.k.a. HUNTING DOWN HACKERY WITH A BOW AND ARROW!)

* \(([A-Z][a-zA-Z0-9]+)\) is the regular expression for finding casts - note that match case must be on for it to work
* The pared down list of dodgy casts (i.e. excluding things like (Point?d)?.clone(), which can't be be helped) which
  need to be eliminated is as follows:

D:\Java\source\MapEditor\MainWindow.java(324):				final ArchitectureBrush ab = (ArchitectureBrush)b;
D:\Java\source\MapEditor\MainWindow.java(333):				final ArchitectureBrush ab = (ArchitectureBrush)b;
D:\Java\source\MapEditor\MainWindow.java(342):				final ArchitectureBrush ab = (ArchitectureBrush)b;
D:\Java\source\MapEditor\MainWindow.java(351):				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)b;
D:\Java\source\MapEditor\MainWindow.java(360):				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)b;
D:\Java\source\MapEditor\MainWindow.java(374):				CSGUtil.hollow((PolygonalBrush)b, thickness, m_map, this);
D:\Java\source\MapEditor\MainWindow.java(387):				ArchitectureBrush spb = (ArchitectureBrush)b;
D:\Java\source\MapEditor\MainWindow.java(411):				CSGUtil.carve((ArchitectureBrush)b, m_map);
D:\Java\source\MapEditor\MainWindow.java(419):				CSGUtil.intersect((ArchitectureBrushComposite)b, m_map, this);
D:\Java\source\MapEditor\MainWindow.java(427):				CSGUtil.union((ArchitectureBrushComposite)b, m_map, this);
D:\JAVA\SOURCE\MAPEDITOR\BRUSHES\SelectionBrush.java(94):	ArchitectureBrush ab = (ArchitectureBrush)b;
D:\JAVA\SOURCE\MAPEDITOR\BSP\Tree.java(242):				if(((ILeafNode)node.left()).is_solid()) pw.println(prefix + "--> solid");
D:\JAVA\SOURCE\MAPEDITOR\BSP\Tree.java(247):				output_subtree(pw, (IBranchNode)node.left(), prefix + "-->");
D:\JAVA\SOURCE\MAPEDITOR\BSP\Tree.java(251):				if(((ILeafNode)node.right()).is_solid()) pw.println(prefix + "--> solid");
D:\JAVA\SOURCE\MAPEDITOR\BSP\Tree.java(256):				output_subtree(pw, (IBranchNode)node.right(), prefix + "-->");
D:\JAVA\SOURCE\MAPEDITOR\BSP\Tree.java(350):				cur = (BranchNode)cur.m_rightChild;
D:\JAVA\SOURCE\MAPEDITOR\BSP\TreeUtil.java(44):				ILeafNode leafNode = (ILeafNode)node;
D:\JAVA\SOURCE\MAPEDITOR\BSP\TreeUtil.java(58):				IBranchNode branchNode = (IBranchNode)node;
D:\JAVA\SOURCE\MAPEDITOR\BSP\TreeUtil.java(172):			ILeafNode leafNode = (ILeafNode)node;
D:\JAVA\SOURCE\MAPEDITOR\BSP\TreeUtil.java(185):			IBranchNode branchNode = (IBranchNode)node;
D:\JAVA\SOURCE\MAPEDITOR\CSG\CSGUtil.java(219):				ArchitectureBrush ab = (ArchitectureBrush)b;
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(250):				if(selectedBrush instanceof ArchitectureBrushComposite && ((ArchitectureBrushComposite)selectedBrush).is_transient())
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(252):				final ArchitectureBrushComposite bc = (ArchitectureBrushComposite)selectedBrush;
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(258):				bc.add_brush((ArchitectureBrush)pickedBrush);
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(263):				bc.delete_brush((ArchitectureBrush)pickedBrush);
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(271):				bc.add_brush((ArchitectureBrush)m_map.get_selected_brush());
D:\JAVA\SOURCE\MAPEDITOR\GUI\View3D.java(272):				bc.add_brush((ArchitectureBrush)pickedBrush);

* The following ones aren't especially dodgy as far as I can tell, but could do with looking at just in case:

D:\JAVA\SOURCE\MAPEDITOR\BRUSHES\ArchitectureBrushComposite.java(598):	ret.add_brush((ArchitectureBrush)loadMethod.invoke(null, new Object[] {br}));
D:\JAVA\SOURCE\MAPEDITOR\BRUSHES\ArchitectureBrushComposite.java(623):	ret.add_brush((ArchitectureBrush)loadMethod.invoke(null, new Object[] {br}));
D:\JAVA\SOURCE\MAPEDITOR\MAPS\MapFileMEF.java(35):			map.add_brush((IBrush)loadMethod.invoke(null, new Object[] {br}));
D:\JAVA\SOURCE\MAPEDITOR\MAPS\MapFileMEF.java(76):			map.add_brush((IBrush)loadMethod.invoke(null, new Object[] {br}));
D:\JAVA\SOURCE\MAPEDITOR\GUI\AddScrollbars.java(34):		m_scrollableComp = (IScrollable)comp;
D:\JAVA\SOURCE\MAPEDITOR\GUI\DesignCanvas.java(192):		Graphics2D g2D = (Graphics2D)m_backGraphics;
D:\JAVA\SOURCE\MAPEDITOR\GUI\DesignCanvas.java(224):		Graphics2D g2D = (Graphics2D)m_backGraphics;

26-02-05	4318 lines of code
26-03-05	6680 lines of code
19-06-05	7340 lines of code
03-07-05	9456 lines of code
13-07-05	9759 lines of code
23-07-05	10376 lines of code
05-08-05	11287 lines of code
11-08-05	12401 lines of code
29-08-05	15044 lines of code
06-09-05	16064 lines of code
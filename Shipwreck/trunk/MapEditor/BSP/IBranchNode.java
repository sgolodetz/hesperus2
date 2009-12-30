package MapEditor.BSP;

import MapEditor.Geom.Planar.Plane;

interface IBranchNode extends INode
{
	INode left();
	INode right();
	Plane splitter();
}
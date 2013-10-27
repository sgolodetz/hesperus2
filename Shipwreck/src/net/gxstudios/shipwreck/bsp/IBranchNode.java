package net.gxstudios.shipwreck.bsp;

import net.gxstudios.shipwreck.geom.planar.Plane;

interface IBranchNode extends INode
{
	INode left();
	INode right();
	Plane splitter();
}
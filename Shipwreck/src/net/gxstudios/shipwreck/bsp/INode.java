package net.gxstudios.shipwreck.bsp;

interface INode
{
	boolean is_leaf();
	IBranchNode parent();
}
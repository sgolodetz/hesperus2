package MapEditor.BSP;

interface INode
{
	boolean is_leaf();
	IBranchNode parent();
}
package MapEditor.BSP;

interface ILeafNode extends INode
{
	boolean is_empty();
	boolean is_solid();
}
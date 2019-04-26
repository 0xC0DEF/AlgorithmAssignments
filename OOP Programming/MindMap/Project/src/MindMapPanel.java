import java.awt.*;
import javax.swing.*;

public class MindMapPanel extends JPanel
{
	public static MindMapPanel Instance()
	{
		if(instance == null)
			instance = new MindMapPanel();
		return instance;
	}
	
	public static void Reset()
	{
		instance = null;
	}
	
	public void Update(Tree tree)
	{
		panel.removeAll();
		panel.SetTree(tree);
		UpdateSub(tree.Root());
		SizeUpdate();
		validate();
		repaint();
	}
	
	public void Update()
	{
		panel.removeAll();
		UpdateSub(panel.GetTree().Root());
		SizeUpdate();
		validate();
		repaint();		
	}
	
	public void SizeUpdate()
	{
		panel.SizeUpdate();
	}
	
	private void UpdateSub(TreeNode cur)
	{
		panel.add(cur);
		for(TreeNode i : cur.Children())
			UpdateSub(i);
	}
	
	private MindMapPanel()
	{
		setLayout(new BorderLayout());
		add(new JLabel("View"), BorderLayout.NORTH);
		panel = new MindMapPanelBody();
		panel.setLayout(null);
		panel.setBackground(Color.WHITE);
		add(new JScrollPane(panel), BorderLayout.CENTER);
	}
	
	private static MindMapPanel instance;
	private static final long serialVersionUID = 1L;
	private MindMapPanelBody panel;
}

class MindMapPanelBody extends JPanel
{
	@Override
	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		if(tree!=null)
			tree.Root().Draw(g);
	}
	
	public void SetTree(Tree tree)
	{
		this.tree=tree;
	}
	
	public Tree GetTree()
	{
		return tree;
	}
	
	public void SizeUpdate()
	{
		setPreferredSize(new Dimension(tree.Root().MaxX()+100, tree.Root().MaxY()+100));
	}
	
	private Tree tree;
	private static final long serialVersionUID = 1L;	
}
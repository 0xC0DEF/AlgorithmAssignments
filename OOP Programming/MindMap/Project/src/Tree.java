import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.MouseInputAdapter;
import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreePath;

import java.util.LinkedList;

public class Tree implements TreeModel
{
	public static Tree last_instance;
	public static Tree LastInstance()
	{
		if(last_instance == null)
			last_instance = new Tree();
		return last_instance;
	}
	
	public Tree()
	{
		last_instance = this;
	}

	public void addTreeModelListener(TreeModelListener arg0) {}
	public void removeTreeModelListener(TreeModelListener l) {}
	public void valueForPathChanged(TreePath path, Object newValue) {}
	public Object getChild(Object arg0, int arg1) {
		TreeNode node = (TreeNode)arg0;
		return node.Children().get(arg1);
	}
	public int getChildCount(Object parent) {
		TreeNode node = (TreeNode)parent;
		return node.Children().size();
	}
	public int getIndexOfChild(Object parent, Object child) {
		TreeNode node = (TreeNode)parent;
		return node.Children().indexOf(child);
	}
	public boolean isLeaf(Object node) {
		TreeNode tnode = (TreeNode)node;
		return tnode.Children().size()==0;
	}
	public void Draw(Graphics g)
	{
		root.Draw(g);
	}
	public TreeNode getRoot()
	{
		return Root();
	}
	public void SetRoot(TreeNode root)
	{
		this.root = root;
	}
	
	public TreeNode Root()
	{
		return root;
	}
	public void Parse(String s, Vec2 root_pos)
	{
		String[] lines = s.split("\n");
		if(lines.length == 0)
			return;
		
		root=null;
		TreeNode cur = null;
		int cur_tc = -1;
		for(String i : lines)
		{
			TreeNode node = new TreeNode(EraseTab(i));
			if(TabCnt(i) == cur_tc + 1)
			{
				if(cur!=null)
				{
					cur.AddChild(node);
					cur = node;
				}
				else
				{
					root = cur = node;
					root.setLocation((int)root_pos.x, (int)root_pos.y);
				}
				cur_tc = TabCnt(i);
			}
			else if(TabCnt(i)<=cur_tc)
			{
				int cnt = TabCnt(i);
				while(cur!=null && cnt++ <= cur_tc)
					cur = cur.Parent();
				if(cur == null)
					return;
				cur.AddChild(node);
				cur=node;
				cur_tc = TabCnt(i);
			}
			else
				return;
		}
	}
	
	private String EraseTab(String s)
	{
		int i=0;
		while(s.charAt(i)=='\t')
			i++;
		return s.substring(i);
	}
	private int TabCnt(String s)
	{
		int i = 0;
		while(s.charAt(i) == '\t')
			i++;
		return i;
	}
	
	private TreeNode root;
}

class TreeNode extends JLabel
{
	public TreeNode(String s)
	{
		children = new LinkedList<TreeNode>();
		setText(s);
		setOpaque(true);
		setSize(100,50);
		setHorizontalAlignment(CENTER);
		setFont(new Font("Arial", Font.PLAIN, 15));
		MouseInputHandler h = new MouseInputHandler();
		setForeground(Color.WHITE);
		setBackground(Color.DARK_GRAY);
		c = getBackground();
		addMouseListener(h);
		addMouseMotionListener(h);
		addFocusListener(new FocusListener() {
			public void focusGained(FocusEvent e)
			{
				TreeNode node = (TreeNode)e.getSource();
				AttributePanel.Instance().Update(node);
				int r = c.getRed();
				int g = c.getGreen();
				int b = c.getBlue();
				Color nc = new Color(255-r, 255-g,255-b);
				node.setBackground(nc);
				if(node.getForeground() == Color.BLACK)
					node.setForeground(Color.WHITE);
				else
					node.setForeground(Color.BLACK);
			}
			public void focusLost(FocusEvent e)
			{
				TreeNode node = (TreeNode)e.getSource();
				node.setBackground(c);
				if(node.getForeground() == Color.BLACK)
					node.setForeground(Color.WHITE);
				else
					node.setForeground(Color.BLACK);
			}
		});
	}
	
	public void Draw(Graphics g)
	{
		for(TreeNode i:children)
			i.Draw(g);
		
		if(parent == null)
			return;
		
		g.setColor(Color.RED);
		Vec2[] pvs = new Vec2[4];
		Vec2[] cvs = new Vec2[4];
		pvs[0] = new Vec2(parent.getX()+parent.getWidth(), parent.getY()+parent.getHeight()/2);
		pvs[1] = new Vec2(parent.getX()+parent.getWidth()/2, parent.getY()+parent.getHeight());
		pvs[2] = new Vec2(parent.getX(), parent.getY()+parent.getHeight()/2);
		pvs[3] = new Vec2(parent.getX()+parent.getWidth()/2, parent.getY());
		cvs[0] = new Vec2(getX()+getWidth(), getY()+getHeight()/2);
		cvs[1] = new Vec2(getX()+getWidth()/2, getY()+getHeight());
		cvs[2] = new Vec2(getX(), getY()+getHeight()/2);
		cvs[3] = new Vec2(getX()+getWidth()/2, getY());
		Vec2 pv = pvs[0];
		Vec2 cv = cvs[0];
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				if(pvs[i].Sub(cvs[j]).Length() < pv.Sub(cv).Length())
				{
					pv=pvs[i];
					cv=cvs[j];
				}
		g.drawLine((int)cv.x, (int)cv.y, (int)pv.x, (int)pv.y);

		Vec2 nv = cv.Sub(pv);
		nv = nv.Normalize();
		nv = nv.Rotate(20);
		nv = nv.Multiply(16);
		g.drawLine((int)pv.x, (int)pv.y, (int)(nv.x+pv.x), (int)(nv.y+pv.y));
		nv = cv.Sub(pv);
		nv = nv.Normalize();
		nv = nv.Rotate(-20);
		nv = nv.Multiply(16);
		g.drawLine((int)pv.x, (int)pv.y, (int)(nv.x+pv.x), (int)(nv.y+pv.y));
	}
	
	public void AddChild(TreeNode node)
	{
		AddChild(node, true);
	}
	
	public void AddChild(TreeNode node, boolean auto_positioning)
	{
		if(auto_positioning)
		{
			Point pos = getLocation();
			pos.translate(children.size()*150, 150);
			node.setLocation(pos);
		}
		node.parent = this;
		children.add(node);
	}
	
	public void RemoveChild(TreeNode node)
	{
		node.parent = null;
		children.remove(node);
	}
	
	public TreeNode Parent()
	{
		return parent;
	}
	
	public LinkedList<TreeNode> Children()
	{
		return children;
	}
	
	public int MaxX()
	{
		int ret=getX() + getWidth();
		for(TreeNode i:children)
			ret = Math.max(ret, i.MaxX());
		return ret;
	}
	
	public int MaxY()
	{
		int ret=getY() + getHeight();
		for(TreeNode i:children)
			ret = Math.max(ret, i.MaxY());
		return ret;
	}
	
	public Color Color()
	{
		return c;
	}
	
	public void SetColor(Color c)
	{
		this.c=c;
		setBackground(c);
		int r = c.getRed();
		int g = c.getGreen();
		int b = c.getBlue();
		if(r+g+b<=0x77*3)
			setForeground(Color.WHITE);
		else
			setForeground(Color.BLACK);			
	}
	
	public void FindAndSetFocus(String name)
	{
		if(getText().equals(name))
		{		
			requestFocus();
		}
		else
		{
			for(TreeNode i:children)
				i.FindAndSetFocus(name);
		}
	}
	
	private Color c;
	private TreeNode parent;
	private LinkedList<TreeNode> children;
	private static final long serialVersionUID = 1L;
}

class MouseInputHandler extends MouseInputAdapter
{	
	public void mousePressed(MouseEvent e)
	{
		px = e.getX();
		py = e.getY();
		Component c = (Component)e.getSource();
		ox = c.getX();
		oy = c.getY();
		ow = c.getWidth();
		oh = c.getHeight();
		
		c.setFocusable(true);
		c.requestFocus();
		AttributePanel.Instance().Update((TreeNode)c);
	}
	
	public void mouseMoved(MouseEvent e)
	{
		Component c = (Component)e.getSource();
		final int thickness = 10;
		if(e.getX()<thickness && e.getY()<thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.NW_RESIZE_CURSOR));
			dragType = 1;
		}
		else if(e.getX()>c.getWidth()-thickness && e.getY()<thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.NE_RESIZE_CURSOR));
			dragType = 2;
		}
		else if(e.getX()<thickness && e.getY()>c.getHeight()-thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.SW_RESIZE_CURSOR));
			dragType = 3;
		}
		else if(e.getX()>c.getWidth()-thickness && e.getY()>c.getHeight()-thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.SE_RESIZE_CURSOR));
			dragType = 4;
		}
		else if(e.getX()<thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.W_RESIZE_CURSOR));
			dragType = 5;
		}
		else if(e.getX()>c.getWidth()-thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.E_RESIZE_CURSOR));
			dragType = 6;
		}
		else if(e.getY()<thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.N_RESIZE_CURSOR));
			dragType = 7;
		}
		else if(e.getY()>c.getHeight()-thickness)
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.S_RESIZE_CURSOR));
			dragType = 8;
		}
		else 
		{
			c.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
			dragType = 9;
		}
	}
	
	public void mouseExited(MouseEvent e)
	{
		Component c = (Component)e.getSource();
		c.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
	}
	
	@Override
	public void mouseReleased(MouseEvent e)
	{
		Component c = (Component)e.getSource();
		int curx=c.getX();
		int cury=c.getY();
		int curw=c.getWidth();
		int curh=c.getHeight();
		c.setLocation(ox, oy);
		c.setSize(ow, oh);
		TaskManager.Instance().Do(new ReleaseTask(c, curx, cury,curw,curh,ox,oy,ow,oh));
	}
	
	public void mouseDragged(MouseEvent e)
	{
		Component c = (Component)e.getSource();
		int dx=e.getX()-px;
		int dy=e.getY()-py;
		if(dragType == 1)
		{
			c.setLocation(c.getX()+dx, c.getY());
			c.setSize(c.getWidth()-dx, c.getHeight());

			c.setLocation(c.getX(), c.getY()+dy);
			c.setSize(c.getWidth(), c.getHeight() - dy);
		}
		else if(dragType == 2)
		{
			c.setSize(ow+dx, c.getHeight());

			c.setLocation(c.getX(), c.getY()+dy);
			c.setSize(c.getWidth(), c.getHeight() - dy);
		}
		else if(dragType == 3)
		{
			c.setLocation(c.getX()+dx, c.getY());
			c.setSize(c.getWidth()-dx, c.getHeight());

			c.setSize(c.getWidth(), oh+dy);
		}
		else if(dragType == 4)
		{
			c.setSize(ow+dx, c.getHeight());
			c.setSize(c.getWidth(), oh+dy);			
		}
		else if(dragType == 5)
		{
			c.setLocation(c.getX()+dx, c.getY());
			c.setSize(c.getWidth()-dx, c.getHeight());
		}
		else if(dragType == 6)
		{
			c.setSize(ow+dx, c.getHeight());
		}
		else if(dragType == 7)
		{
			c.setLocation(c.getX(), c.getY()+dy);
			c.setSize(c.getWidth(), c.getHeight() - dy);
		}
		else if(dragType == 8)
		{
			c.setSize(c.getWidth(), oh+dy);
		}
		else 
		{
			c.setLocation(c.getX()+dx, c.getY()+dy);
		}
		MindMapPanel.Instance().SizeUpdate();
		MindMapPanel.Instance().validate();
		MindMapPanel.Instance().repaint();
		AttributePanel.Instance().Update((TreeNode)c);
	}
	private int dragType;
	private int px, py;
	private int ox, oy;
	private int ow, oh;
};


class ReleaseTask implements Task
{
	public ReleaseTask(Component c, int curx, int cury, int curw, int curh, int ux, int uy, int uw, int uh)
	{
		this.c=c;
		this.curx = curx;
		this.cury=cury;
		this.curw=curw;
		this.curh=curh;
		this.ux = ux;
		this.uy = uy;
		this.uw = uw;
		this.uh = uh;
	}
	public void Do() {
		c.setLocation(curx, cury);
		c.setSize(curw, curh);

		MindMapPanel.Instance().SizeUpdate();
		MindMapPanel.Instance().validate();
		MindMapPanel.Instance().repaint();
		AttributePanel.Instance().Update((TreeNode)c);
	}
	public void Undo() {
		c.setLocation(ux, uy);
		c.setSize(uw, uh);

		MindMapPanel.Instance().SizeUpdate();
		MindMapPanel.Instance().validate();
		MindMapPanel.Instance().repaint();
		AttributePanel.Instance().Update((TreeNode)c);
	}
	private Component c;
	private int curx, cury, curw, curh;
	private int ux, uy, uw, uh;
}
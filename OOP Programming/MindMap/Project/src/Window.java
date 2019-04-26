import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

public class Window extends JFrame {
    public static void main(String[] args) {
    	w = new Window();
    }
    
	public Window()
    {
        setSize(1500, 900);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        nbh = new NewButtonHandler();
        obh = new OpenButtonHandler();
        sbh = new SaveButtonHandler();
        sabh = new SaveAsButtonHandler();
        ebh = new ExitButtonHandler();
        abh = new AddChildButtonHandler();
        ubh = new UndoButtonHandler();
        rbh = new RedoButtonHandler();
        fbh = new FindButtonHandler();
        htubh = new HowToUseButtonHandler();
        
		cp = getContentPane();
		InitMenuBar();
		InitToolBar();
		
		TextEditorPanel ta = TextEditorPanel.Instance();
		MindMapPanel mmp = MindMapPanel.Instance();
		AttributePanel ap = AttributePanel.Instance();
		JSplitPane sp2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, mmp, ap);
		sp2.setDividerLocation(getWidth()/5*3);
		JSplitPane sp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, ta, sp2);
		sp.setDividerLocation(getWidth()/5);
		cp.add(sp);

        setTitle("Mind Map");
        setIconImage(new ImageIcon("icon.png").getImage());
        setVisible(true);
    }
	
	void InitMenuBar()
	{
        JMenuBar mb = new JMenuBar();
        
        JMenuItem mi;
        
        JMenu m_file = new JMenu("File");
        
        mi = new JMenuItem("New");
        mi.addActionListener(nbh);
        m_file.add(mi);
        
        mi = new JMenuItem("Open");
        mi.addActionListener(obh);
        m_file.add(mi);
        
        mi = new JMenuItem("Save");
        mi.addActionListener(sbh);
        m_file.add(mi);
        
        mi = new JMenuItem("Save As...");
        mi.addActionListener(sabh);
        m_file.add(mi);
        
        m_file.addSeparator();
        
        mi = new JMenuItem("Exit");
        mi.addActionListener(ebh);
        m_file.add(mi);
        
        mb.add(m_file);

        
        
        JMenu m_edit = new JMenu("Edit");
        
        mi = new JMenuItem("Generate");
        mi.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				TextEditorPanel.Instance().Generate();
			}
		});
        m_edit.add(mi);
        
        mi = new JMenuItem("Apply");
        mi.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				AttributePanel.Instance().Apply();
			}
		});
        m_edit.add(mi);
        mi = new JMenuItem("Add Child");
        mi.addActionListener(abh);
        m_edit.add(mi);
        
        m_edit.addSeparator();
        
        mi = new JMenuItem("Undo");
        mi.addActionListener(ubh);
        m_edit.add(mi);
        mi = new JMenuItem("Redo");
        mi.addActionListener(rbh);
        m_edit.add(mi);
        m_edit.addSeparator();
        mi = new JMenuItem("Find");
        mi.addActionListener(fbh);
        m_edit.add(mi);
        mb.add(m_edit);

        JMenu m_help = new JMenu("Help");
        mi = new JMenuItem("How to use");
        mi.addActionListener(htubh);
        m_help.add(mi);
        mi=new JMenuItem("About develper");
        mi.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				JOptionPane.showMessageDialog(w, "숭실대학교 컴퓨터학부 20172605 김도현\n"+
			"숭실대학교 컴퓨터학부 2013???? 오현택", "About developer", JOptionPane.INFORMATION_MESSAGE);
			}
		});
        m_help.add(mi);
        mi = new JMenuItem("License");
        mi.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				JOptionPane.showMessageDialog(w, "MIT License", "License", JOptionPane.INFORMATION_MESSAGE);
			}
		});
        m_help.add(mi);
        mb.add(m_help);
        
        setJMenuBar(mb);
	}
	
	void InitToolBar()
	{
		JToolBar tb = new JToolBar("Tool Bar");
		tb.setBackground(Color.LIGHT_GRAY);
		JButton b;
		
		b = new JButton(new ImageIcon("new.png"));
		b.addActionListener(nbh);
		tb.add(b);
		
		b = new JButton(new ImageIcon("open.png"));
		b.addActionListener(obh);
		tb.add(b);
		
		b = new JButton(new ImageIcon("save.png"));
		b.addActionListener(sbh);
		tb.add(b);

		b = new JButton(new ImageIcon("saveas.png"));
		b.addActionListener(sabh);
		tb.add(b);

		b = new JButton(new ImageIcon("exit.png"));
		b.addActionListener(ebh);
		tb.add(b);
		
		tb.addSeparator();
		
		b=new JButton(new ImageIcon("generate.png"));
		b.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				TextEditorPanel.Instance().Generate();
			}
		});
		tb.add(b);
		
		b=new JButton(new ImageIcon("apply.png"));
		b.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				AttributePanel.Instance().Apply();
			}
		});
		tb.add(b);
		
		b=new JButton(new ImageIcon("addchild.png"));
		b.addActionListener(abh);
		tb.add(b);
		
		b = new JButton(new ImageIcon("undo.png"));
		b.addActionListener(ubh);
		tb.add(b);
		b = new JButton(new ImageIcon("redo.png"));
		b.addActionListener(rbh);
		tb.add(b);
		b = new JButton(new ImageIcon("find.png"));
		b.addActionListener(fbh);
		tb.add(b);

		tb.addSeparator();

		b=new JButton(new ImageIcon("howtouse.png"));
		b.addActionListener(htubh);
		tb.add(b);
		
		cp.add(tb, BorderLayout.NORTH);
	}
	
	private class NewButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			cp.removeAll();
			TextEditorPanel.Reset();
			MindMapPanel.Reset();
			AttributePanel.Reset();
			TaskManager.Reset();
			FileManager.Reset();
			
			InitToolBar();
			
			TextEditorPanel ta = TextEditorPanel.Instance();
			MindMapPanel mmp = MindMapPanel.Instance();
			AttributePanel ap = AttributePanel.Instance();
			JSplitPane sp2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, mmp, ap);
			sp2.setDividerLocation(getWidth()/5*3);
			JSplitPane sp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, ta, sp2);
			sp.setDividerLocation(getWidth()/5);
			cp.add(sp);
			cp.validate();
	        setTitle("Mind Map");
		}
	}
	
	private class OpenButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			cp.removeAll();
			TextEditorPanel.Reset();
			MindMapPanel.Reset();
			AttributePanel.Reset();
			TaskManager.Reset();
			
			InitToolBar();
			
			TextEditorPanel ta = TextEditorPanel.Instance();
			MindMapPanel mmp = MindMapPanel.Instance();
			AttributePanel ap = AttributePanel.Instance();
			JSplitPane sp2 = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, mmp, ap);
			sp2.setDividerLocation(getWidth()/5*3);
			JSplitPane sp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, ta, sp2);
			sp.setDividerLocation(getWidth()/5);
			cp.add(sp);
			cp.validate();
	        setTitle("Mind Map");
			
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(new FileNameExtensionFilter("XML", "xml"));
			int ret = fc.showOpenDialog(null);
			if(ret == JFileChooser.APPROVE_OPTION)
			{
				String path = fc.getSelectedFile().getPath();
				Tree t = FileManager.Load(path);
				if(t==null)
					return;
				MindMapPanel.Instance().Update(t);
				AttributePanel.Instance().Update(t.Root());
				w.setTitle("Mind Map - "+path);
			}
		}
	}
	
	private class SaveButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			String path = FileManager.WorkingFile();
			if(path == null)
			{
				JFileChooser fc = new JFileChooser();
				fc.setFileFilter(new FileNameExtensionFilter("XML", "xml"));
				int ret = fc.showSaveDialog(null);
				if(ret == JFileChooser.APPROVE_OPTION)
				{
					path = fc.getSelectedFile().getPath();
					if(!path.substring(path.length()-4).equals(".xml"))
						path=path+".xml";
				}
			}
			if(path!=null)
			{
				FileManager.Save(Tree.last_instance, path);
				w.setTitle("Mind Map - "+path);
			}
		}
	}
	
	private class SaveAsButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(new FileNameExtensionFilter("XML", "xml"));
			int ret = fc.showSaveDialog(null);
			if(ret == JFileChooser.APPROVE_OPTION)
			{
				String path = fc.getSelectedFile().getPath();
				if(!path.substring(path.length()-4).equals(".xml"))
					path=path+".xml";
				FileManager.Save(Tree.last_instance, path);
				w.setTitle("Mind Map - "+path);
			}
		}
	}
	
	private class ExitButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			w.dispatchEvent(new WindowEvent(w, WindowEvent.WINDOW_CLOSING));
		}
	}

	private class AddChildButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			String str = JOptionPane.showInputDialog(w, "추가할 노드의 이름을 입력하십시오", "");
			TaskManager.Instance().Do(new AddChildTask(AttributePanel.Instance().GetTarget(), str));
		}
	}
	
	private class UndoButtonHandler implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			TaskManager.Instance().Undo();
		}		
	}
	
	private class RedoButtonHandler implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			TaskManager.Instance().Redo();
		}		
	}
	
	private class FindButtonHandler implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			String str = JOptionPane.showInputDialog(w, "검색할 노드의 이름을 입력하십시오", "");
			Tree.last_instance.Root().FindAndSetFocus(str);
		}		
	}

	private class HowToUseButtonHandler implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JOptionPane.showMessageDialog(w, "트리 구조의 텍스트를 탭으로 구분하여 작성하고 Generate 버튼을 누릅니다.\n"+
		"만들어진 트리의 노드를 클릭하면 Properties창에서 값을 확인/변경할 수 있습니다.", "How to use", JOptionPane.INFORMATION_MESSAGE);
		}
	}
	ActionListener nbh, obh, sbh, sabh, ebh;
	ActionListener abh, ubh, rbh, fbh;
	ActionListener htubh;
	
	private static Window w;
	private Container cp;
	private static final long serialVersionUID = 1L;
}

class AddChildTask implements Task{
	public AddChildTask(TreeNode target, String name)
	{
		this.target = target;
		this.name = name;
	}
	public void Do() {
		if(target==null)
			return;
		target.AddChild(child = new TreeNode(name));
		MindMapPanel.Instance().Update();
	}
	public void Undo() {
		if(target==null)
			return;
		target.RemoveChild(child);
		MindMapPanel.Instance().Update();
	}
	private TreeNode target, child;
	private String name;
}
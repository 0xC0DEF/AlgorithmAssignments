import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class TextEditorPanel extends JPanel
{
	public static TextEditorPanel Instance()
	{
		if(instance == null)
			instance = new TextEditorPanel();
		return instance;
	}
	
	public void Generate()
	{
		btn.doClick();
	}

	public static void Reset()
	{
		instance = null;
	}
	
	private TextEditorPanel()
	{
		setLayout(new BorderLayout());
		add(new JLabel("Text Editor"), BorderLayout.NORTH);
		ta = new JTextArea("1\n	2\n		3\n	4\n		5");
		add(new JScrollPane(ta), BorderLayout.CENTER);
		btn = new JButton("Generate");
		btn.addActionListener(new GenerateButtonHandler());
		add(btn, BorderLayout.SOUTH);
	}
	
	private class GenerateButtonHandler implements ActionListener {
			public void actionPerformed(ActionEvent e) {
			Tree tree = new Tree();
			tree.Parse(ta.getText(), new Vec2(300,100));
			MindMapPanel.Instance().Update(tree);
			AttributePanel.Instance().Update(tree.Root());
		}
	}

	private JButton btn;
	private JTextArea ta;
	private static TextEditorPanel instance;
	private static final long serialVersionUID = 1L;
}
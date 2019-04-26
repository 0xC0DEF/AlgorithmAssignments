import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class AttributePanel extends JPanel
{
	public static AttributePanel Instance()
	{
		if(instance == null)
			instance = new AttributePanel();
		return instance;
	}
	public static void Reset()
	{
		instance = null;
	}
	
	public void Apply()
	{
		btn.doClick();
	}
	
	public void Update(TreeNode node)
	{
		target=node;
		tfs[0].setText(target.getText());
		tfs[1].setText(target.getLocation().x+"");
		tfs[2].setText(target.getLocation().y+"");
		tfs[3].setText(target.getWidth()+"");
		tfs[4].setText(target.getHeight()+"");
		tfs[5].setText("0x"+Integer.toHexString(target.Color().getRGB()).toString().substring(2));
	}
	
	public TreeNode GetTarget()
	{
		return target;
	}
	
	private AttributePanel()
	{
		setLayout(new BorderLayout());
		add(new JLabel("Properties"), BorderLayout.NORTH);
		JPanel body = new JPanel();
		body.setLayout(new GridLayout(6, 1, 0, 16));
		
		String[] label_strings = {"TEXT:", "X:", "Y:", "W:", "H:", "Color:"};
		tfs = new JTextField[6];
		for(int i=0;i<label_strings.length;i++)
		{
			JPanel item = new JPanel();
			item.setLayout(new BorderLayout());
			item.add(new JLabel(label_strings[i]), BorderLayout.WEST);
			tfs[i] = new JTextField("");
			tfs[i].setPreferredSize(new Dimension(200, 30));
			item.add(tfs[i], BorderLayout.EAST);
			if(i==0)
				tfs[i].setEditable(false);
			body.add(item);
		}
		
		JPanel layout = new JPanel();
		layout.setLayout(new BorderLayout());
		layout.add(body, BorderLayout.NORTH);
		
		add(new JScrollPane(layout), BorderLayout.CENTER);
		
		btn = new JButton("Apply");
		add(btn, BorderLayout.SOUTH);
		btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(target==null)
					return;
				target.setText(tfs[0].getText());
				target.setLocation(Integer.parseInt(tfs[1].getText()), Integer.parseInt(tfs[2].getText()));
				target.setSize(Integer.parseInt(tfs[3].getText()), Integer.parseInt(tfs[4].getText()));
				target.SetColor(Color.decode(tfs[5].getText().toUpperCase()));
				MindMapPanel.Instance().SizeUpdate();
				MindMapPanel.Instance().validate();
				MindMapPanel.Instance().repaint();
			}
		});
	}
	private static AttributePanel instance;
	private static final long serialVersionUID = 1L;
	
	private JButton btn;
	private TreeNode target;
	private JTextField[] tfs;
}
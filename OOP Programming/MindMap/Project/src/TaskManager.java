import java.util.Stack;

public class TaskManager
{
	static public TaskManager Instance()
	{
		if(instance==null)
			instance = new TaskManager();
		return instance;
	}
	
	static public void Reset()
	{
		instance = null;
	}
	
	public void Do(Task t)
	{
		undo.push(t);
		redo.clear();
		t.Do();
	}
	
	public void Undo()
	{
		if(undo.isEmpty())
			return;
		Task t = undo.pop();
		redo.push(t);
		t.Undo();
	}
	
	public void Redo()
	{
		if(redo.isEmpty())
			return;
		Task t = redo.pop();
		undo.push(t);
		t.Do();
	}

	private Stack<Task> undo = new Stack<Task>();
	private Stack<Task> redo = new Stack<Task>();
	
	static private TaskManager instance;
}

interface Task
{
	public void Do();
	public void Undo();
}
import java.util.*;

public class MapPartialSolution
{
	public String filename;
	public long startPos;
	public long endPos;
	
	public HashMap<String, Integer> statistics;
	
	public MapPartialSolution(String filename, long startPos, long endPos, HashMap<String, Integer> statistics)
	{
		this.filename = filename;
		this.startPos = startPos;
		this.endPos = endPos;
		
		this.statistics = statistics;
	}
}

import java.io.*;
import java.util.*;
import java.util.concurrent.*;

public class MapTask implements Callable<MapPartialSolution>
{
	private String filename;
	private long startPos;
	private long endPos;
	
	public MapTask(String filename, long startPos, long endPos)
	{
		this.filename = filename;
		this.startPos = startPos;
		this.endPos = endPos;
	}
	
	//returneaza rezultatul problemei partiale pe care a rezolvat-o
	public MapPartialSolution call() throws IOException
	{
		DocumentParser dp = new DocumentParser(filename, startPos, endPos);
		
		MapPartialSolution ps = new MapPartialSolution(filename, startPos, endPos, dp.getStatistics());
		
		return ps;
	}
}

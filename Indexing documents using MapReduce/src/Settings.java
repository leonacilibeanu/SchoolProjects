import java.io.*;
import java.util.*;

public class Settings
{
	public int NT;
	
	public int NC;
	public String[] words;
	
	public int D;
	public int N;
	public int X;
	
	public int ND;
	public String[] documents;
	
	public Settings(String inputFilename, int NT) throws IOException
	{
		this.NT = NT;
		
		//citeste setarile din fisierul de intrare
		Scanner scanner = new Scanner(new FileReader(inputFilename));
		
		NC = scanner.nextInt();
		words = new String[NC];
		
		for(int i = 0; i < NC; i++)
			words[i] = scanner.next("[a-zA-Z]+");
		
		D = scanner.nextInt();
		N = scanner.nextInt();
		X = scanner.nextInt();
		
		ND = scanner.nextInt();
		documents = new String[ND];
		
		for(int i = 0; i < ND; i++)
			documents[i] = scanner.next("[a-zA-Z0-9\\.\\-\\_\\:\\/\\\\]+");
		
		scanner.close();		
	}
}

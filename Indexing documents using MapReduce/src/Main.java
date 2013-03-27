import java.io.*;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.*;
import java.util.concurrent.*;

public class Main 
{	
	/**
	 * @param args
	 */
	public static void main(String[] args) 
	{
		if(args.length < 3)
		{
			System.out.println("Usage: java Main NT input.txt output.txt");
			return;
		}
		
		int NT = Integer.parseInt(args[0]);
		String inputFilename = args[1];
		String outputFilename = args[2];
		
		try
		{
			Settings settings = new Settings(inputFilename, NT);
			
			Master master = new Master(settings);
			
			LinkedList<MapPartialSolution> mapPartialSolutions = master.map();
			HashMap<String, HashMap<String, Double>> reduceSolution = master.reduce(mapPartialSolutions);
			LinkedList<String> searchResults = master.search(reduceSolution, settings.words);
			
			//scrierea rezultatelor in fisierul de iesire
			BufferedWriter bw = new BufferedWriter(new FileWriter(outputFilename, false));
		
			bw.write("Rezultate pentru: (");
			
			for(int i = 0; i < settings.words.length; i++)
			{
				bw.write(settings.words[i]);
				
				if(i + 1 < settings.words.length)
					bw.write(", ");
			}
			
			bw.write(")\n");

			DecimalFormat decimalFormat = new DecimalFormat();
			
			decimalFormat.setMaximumFractionDigits(2);
			decimalFormat.setMinimumFractionDigits(2);
			decimalFormat.setRoundingMode(RoundingMode.FLOOR);
			
			//afiseaza rezultatele cautarii in ordinea aparitiei lor in fisierul de intrare
			for(int k = 0; k < settings.documents.length; k++)
			{
				String document = settings.documents[k];
				
				if(!searchResults.contains(document))
					continue;
				
				bw.write("\n");
				bw.write(document + " (");
				
				for(int i = 0; i < settings.words.length; i++)
				{
					double frequency = reduceSolution.get(document).get(settings.words[i]);
					bw.write(decimalFormat.format(frequency * 100));
					
					if(i + 1 < settings.words.length)
						bw.write(", ");
				}
				
				bw.write(")");
			}
			
			bw.close();
			
			master.shutdown();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}

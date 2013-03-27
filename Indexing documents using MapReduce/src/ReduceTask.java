import java.util.*;
import java.util.Map.Entry;
import java.util.concurrent.*;

public class ReduceTask implements Callable<ReducePartialSolution>
{
	private Settings settings;
	private String document;
	private LinkedList<MapPartialSolution> mapPartialSolutions;
	
	public ReduceTask(Settings settings, String document, LinkedList<MapPartialSolution> mapPartialSolutions)
	{
		this.settings = settings;
		this.document = document;
		this.mapPartialSolutions = mapPartialSolutions;
	}

	private HashMap<String, Integer> getDocumentStatistics()
	{
		//cuvantul este folosit pe post de cheie
		//numarul de aparitii pe post de valoare
		HashMap<String, Integer> documentStatistics = new HashMap<String, Integer>();
		
		//centralizeaza numarul de aparitii pentru fiecare cuvant
		for(MapPartialSolution mps : mapPartialSolutions)
		{
			for(String word : mps.statistics.keySet())
			{
				if(!documentStatistics.containsKey(word))
					documentStatistics.put(word, 0);
				
				documentStatistics.put(word, documentStatistics.get(word) + mps.statistics.get(word));
			}
		}
		
		return documentStatistics;
	}
	
	//comparatorul va fi utilizat pentru a sorta cuvintele dupa numarul de aparitii (in ordine descrescatoare)
	private Comparator<Map.Entry<String, Integer>> getComparator()
	{
		Comparator<Map.Entry<String, Integer>> comparator = new Comparator<Map.Entry<String,Integer>>()
		{
			@Override
			public int compare(Entry<String, Integer> o1, Entry<String, Integer> o2)
			{
				return o2.getValue().compareTo(o1.getValue());
			}
		};
		
		return comparator;
	}
	
	//returneaza statisticile sortate dupa nr. de aparitii
	private List<Map.Entry<String, Integer>> getSortedDocumentStatistics(HashMap<String, Integer> documentStatistics)
	{
		//sorteaza cuvintele dupa numarul de aparitii
		
		ArrayList<Map.Entry<String, Integer>> sortedDocumentStatistics = new ArrayList<Map.Entry<String, Integer>>();
		sortedDocumentStatistics.addAll(documentStatistics.entrySet());
		
		Collections.sort(sortedDocumentStatistics, getComparator());
		
		return sortedDocumentStatistics;
	}
	
	public ReducePartialSolution call()
	{
		HashMap<String, Integer> documentStatistics = getDocumentStatistics();

		//calculeaza numarul total de cuvinte din document
		long wordCount = 0;
		for(Integer value : documentStatistics.values())
			wordCount += value;
		
		List<Map.Entry<String, Integer>> sortedDocumentStatistics = getSortedDocumentStatistics(documentStatistics);
		
		//acest HashMap va contine doar cuvintele care apar cel mai frecvent in document si frecventele lor
		HashMap<String, Double> retainedDocumentStatistics = new HashMap<String, Double>();
		
		Map.Entry<String, Integer> lastEntry = null;
		Iterator<Map.Entry<String, Integer>> it = sortedDocumentStatistics.iterator();
		
		for(int i = 0; it.hasNext() && settings.N > 0; i++) 
		{
			Map.Entry<String, Integer> entry = it.next();
			
			if(i >= settings.N)
			{
				//in cazul in care avem numar de aparitii egale le retinem pe toate cuvintele
				if(entry.getValue() < lastEntry.getValue())
					break;
			}
			
			//calculeaza frecventa pe baza numarului de aparitii si a numarului total de cuvinte
			double frequency = (double) entry.getValue() / wordCount;
			retainedDocumentStatistics.put(entry.getKey(), frequency);
			
			lastEntry = entry;
		}
		
		ReducePartialSolution rps = new ReducePartialSolution(document, retainedDocumentStatistics);
		
		return rps;
	}
}

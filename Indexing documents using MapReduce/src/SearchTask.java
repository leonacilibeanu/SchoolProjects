import java.util.*;
import java.util.concurrent.*;

public class SearchTask implements Callable<SearchPartialSolution>
{
	private String document;
	private HashMap<String, Double> documentStatistics;
	private String[] words;
	
	public SearchTask(String document, HashMap<String, Double> documentStatistics, String[] words)
	{
		this.document = document;
		this.documentStatistics = documentStatistics;
		this.words = words;
	}
	
	public SearchPartialSolution call()
	{
		boolean documentContainsAllWords = true;
		double documentRelevance = 0;
		
		//verifica daca toate cuvintele cautate sunt incluse in cele mai frecvente cuvinte
		//calculeaza relevanta ca fiind suma frecventelor cuvintelor cautate
		for(int i = 0; i < words.length; i++)
		{
			if(!documentStatistics.containsKey(words[i]))
			{
				documentContainsAllWords = false;
				break;
			}
			
			documentRelevance += documentStatistics.get(words[i]).doubleValue();
		}
		
		return new SearchPartialSolution(document, documentContainsAllWords, documentRelevance);
	}
}

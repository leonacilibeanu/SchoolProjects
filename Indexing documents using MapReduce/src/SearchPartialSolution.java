
public class SearchPartialSolution
{
	public String document;
	public boolean documentContainsAllWords;
	public double documentRelevance;
	
	public SearchPartialSolution(String document, boolean documentContainsAllWords, double documentRelevance)
	{
		this.document = document;
		this.documentContainsAllWords = documentContainsAllWords;
		this.documentRelevance = documentRelevance;
	}
}

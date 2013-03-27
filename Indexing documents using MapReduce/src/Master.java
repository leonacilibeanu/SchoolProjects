import java.io.*;
import java.util.*;
import java.util.Map.Entry;
import java.util.concurrent.*;


public class Master
{
	private Settings settings;
	private ExecutorService executorService;
	
	public Master(Settings settings)
	{
		this.settings = settings;
		
		executorService = Executors.newFixedThreadPool(settings.NT);
	}
	
	public LinkedList<MapPartialSolution> map() throws ExecutionException, InterruptedException
	{
		ExecutorCompletionService<MapPartialSolution> mapECS = new ExecutorCompletionService<>(executorService);
		
		//parcurge toate documentele si creaza task-uri
		long taskCount = 0;
		for(int i = 0; i < settings.ND; i++)
		{
			long filesize = new File(settings.documents[i]).length();
			
			//creaza task-uri pentru fiecare sectiune document
			for(long pos = 0; pos < filesize; pos += settings.D)
			{
				long endPos = pos + settings.D;
				
				if(endPos > filesize)
					endPos = filesize;

				MapTask mapTask = new MapTask(settings.documents[i], pos, endPos);

				//solutia partiala va putea fi accesata in viitor, dupa ce task-ul a fost rulat
				mapECS.submit(mapTask);
				
				taskCount++;
			}
		}
		
		LinkedList<MapPartialSolution> mapPartialSolutions = new LinkedList<MapPartialSolution>();
		
		for(long i = 0; i < taskCount; i++)
		{
			//in cazul in care task-ul nu a fost terminat, asteapta pana cand rezultatul partial devine disponibil
			MapPartialSolution mps = mapECS.take().get();
			
			mapPartialSolutions.add(mps);
		}
		
		return mapPartialSolutions;
	}
	
	//prima operatie de reducere
	HashMap<String, HashMap<String, Double>> reduce(LinkedList<MapPartialSolution> mapPartialSolutions) throws ExecutionException, InterruptedException
	{
		ExecutorCompletionService<ReducePartialSolution> reduceECS = new ExecutorCompletionService<>(executorService);
		
		//creaza cate o lista cu solutiile partiale pentru fiecare document
		//utilizeaza numele documentului pe post de cheie
		HashMap<String, LinkedList<MapPartialSolution>> documentPartialSolutions = new HashMap<String, LinkedList<MapPartialSolution>>();
		
		for(MapPartialSolution mps : mapPartialSolutions)
		{
			if(!documentPartialSolutions.containsKey(mps.filename))
				documentPartialSolutions.put(mps.filename, new LinkedList<MapPartialSolution>());

			documentPartialSolutions.get(mps.filename).add(mps);
		}
		
		//fiecare task realizeaza reducerea pentru un anumit document
		int taskCount = 0;
		for(String document : documentPartialSolutions.keySet())
		{
			ReduceTask reduceTask = new ReduceTask(settings, document, documentPartialSolutions.get(document));
			
			//solutia partiala va putea fi accesata in viitor, dupa ce task-ul a fost rulat
			reduceECS.submit(reduceTask);
			
			taskCount++;
		}
		
		//foloseste numele documentului pe post de cheie
		//valoarea este un HashMap care contine cele mai frecvente cuvinte si frecventa fiecaruia
		HashMap<String, HashMap<String, Double>> result = new HashMap<String, HashMap<String, Double>>();
		
		for(long i = 0; i < taskCount; i++)
		{
			//in cazul in care task-ul nu a fost terminat, asteapta pana cand rezultatul partial devine disponibil
			ReducePartialSolution rps = reduceECS.take().get();
			
			result.put(rps.document, rps.statistics);
		}
		
		return result;
	}
	
	//a doua operatie de reducere
	LinkedList<String> search(HashMap<String, HashMap<String, Double>> reduceSolution, String[] words) throws InterruptedException, ExecutionException
	{
		ExecutorCompletionService<SearchPartialSolution> searchECS = new ExecutorCompletionService<>(executorService);
		
		//fiecare task realizeaza cautarea in cate un document
		int taskCount = 0;
		for(String document : reduceSolution.keySet())
		{
			HashMap<String, Double> documentStatistics = reduceSolution.get(document);
			
			SearchTask searchTask = new SearchTask(document, documentStatistics, words);
			
			searchECS.submit(searchTask);
			
			taskCount++;
		}
		
		//documentul este cheia
		//valoarea reprezinta relevanta
		HashMap<String, Double> partialSolutions = new HashMap<String, Double>();
		
		for(long i = 0; i < taskCount; i++)
		{
			//in cazul in care task-ul nu a fost terminat, asteapta pana cand rezultatul partial devine disponibil
			SearchPartialSolution sps = searchECS.take().get();
			
			if(sps.documentContainsAllWords)
				partialSolutions.put(sps.document, sps.documentRelevance);
		}
		
		//comparatorul este utilizat pentru a sorta in ordine descrescatoare dupa relevanta
		Comparator<Map.Entry<String, Double>> comparator = new Comparator<Map.Entry<String, Double>>()
		{
			@Override
			public int compare(Entry<String, Double> o1, Entry<String, Double> o2)
			{
				return o2.getValue().compareTo(o1.getValue());
			}
		};
		
		//sorteaza documentele dupa relevanta
		ArrayList<Map.Entry<String, Double>> sortedPartialSolutions = new ArrayList<Map.Entry<String, Double>>();
		sortedPartialSolutions.addAll(partialSolutions.entrySet());
		Collections.sort(sortedPartialSolutions, comparator);
		
		//returneaza doar primele X documente
		LinkedList<String> result = new LinkedList<String>();
		Iterator<Map.Entry<String, Double>> it = sortedPartialSolutions.iterator();
		
		for(int i = 0; it.hasNext() && i < settings.X; i++) 
		{
			Map.Entry<String, Double> entry = it.next();
			
			result.add(entry.getKey());
		}
		
		return result;
	}
	
	void shutdown()
	{
		executorService.shutdown();
	}
}

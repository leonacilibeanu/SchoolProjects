import java.io.*;
import java.util.*;



public class Main {
	
	
	//structura words pastreaza intregul dictionar;
	 
		static HashMap<String,Integer> words = new HashMap<String,Integer>();
	
	/*
	 * @param Structura s pentru care vrem sa aflam nr de cuvinte
	 * 
	 * Numara cate cuvinte sunt intr-un string s
	 * 
	 * @return numarul de cuvinte
	 */
	public static int count(String s){
		int i;
		int nr=1;
		for (i=0;i<s.length();i++) if (s.charAt(i) == ' ') nr++;
		
		return nr;
	}
	
	/*
	 * @param
	 * 
	 * Sterge spatiile existente initial in sirul citit de la tastatura
	 * 
	 * @return Sirul primit ca paramentru, fara spatii
	 */
	public static String removeSpaces (String input){
		
		input = input.replaceAll("\\s", "");
		return input;
	}
	
	
	/*
	 * @return Minimul dintre 3 nr primite drept parametri
	 */
	  private static int getMinimum(int a, int b, int c) {
          return Math.min(Math.min(a, b), c);
	  }

	/*
	 * @param Cuvantul din dictionar fata de care se calculeaza distanta minima
	 * @param Cuvantul pentru care vrem sa aflam distanta minima
	 * 
	 * Calculeaza distanta minima dintre un cuvant din dictionar si cuvantul
	 * care trebuie corectat. Folosind un algoritm de tip programare dinamica,
	 * se obtine o matrice care are pe ultima linie valori ale distantelor minime 
	 * dintre un substring al input(extras de la 0 la i, unde i=1 to n) si cuvantul 
	 * din dictionar(target).
	 * 
	 * @return O lista de tip Integer, cu valori ale distantelor minime pentru fiecare substring.
	 */
	public static ArrayList<Integer> LevenshteinDistance(String target, String input){
		int n = target.length();
		int m = input.length();
		int M[][] = new int[n+1][m+1];
		ArrayList<Integer> list = new ArrayList<Integer>();
		int i,j, cost;
		
		 for (i = 0; i <= n; i++) {
		      M[i][0] = i;
		    }

		 for (j = 0; j <= m; j++) {
		      M[0][j] = j;
		    }
		 
		 for (i = 1; i <= n; i++){
			 for (j = 1; j <=  m;  j++){
				 if (target.charAt(i-1)==input.charAt(j-1)) cost = 0;
				 else cost = 1;
				 M[i][j] = getMinimum ((M[i-1][j] + 1), (M[i][j-1] + 1) ,(M[i-1][j-1] + cost));
			 }
		 }
		
		 for (i = 1; i<=m;i++) list.add(M[n][i]);
		 
		return list;
	}
	
	/*
	 * @param Cuvantul pentru care vrem sa calculam cel mai bun candidat
	 * 
	 * Apeland functia LevenshteinDistance, calculeaza, pentru fiecare cuvant din dictonar,
	 * distanta minima dintre acesta si cuvantul primit drept paramentru. 
	 * Actualizeaza valoarea din lista de candidati, bestChoice, daca e indeplinit cel putin
	 * un criteriu: distanta minima, frecventa maxima sau minim lexicografic;
	 * 
	 * @return Lista cu cei mai buni candidati calculati pentru input si substring-uri.
	 */
	
	public static ArrayList<Candidate> FindBest(String input){	
	
		/*
		 * se initializeaza un iterator pentru a parcurge dictionarul
		 */
		Iterator<String> it = words.keySet().iterator();
		
		/*
		 * lista cu distantele minime intermediare
		 */
		
		ArrayList<Integer> dist= new ArrayList<Integer>();
		
		/*
		 * lista de candidati pentru fiecare substring al input(extras de la 0 la i, unde i=1 to n) 
		 */
		ArrayList<Candidate> bestChoice = new ArrayList<Candidate>();
		
		dist.clear();
		bestChoice.clear();
		
		/*
		 * se initializeaza bestChoice, pentru fiecare substring din input, cu frecventa 0 si distanta 
		 * foarte mare:99999
		 */
		
		for (int i=1;i<=input.length();i++) 
			bestChoice.add(new Candidate(0,99999,input.substring(0,i)));
		
		String search;
		
		while (it.hasNext()){
			
			search = it.next();
			
			//lista cu distantele minime pentru input si substring-uri, fata de cuvantul din dictionar
			dist = LevenshteinDistance(search,input);
			
			/*
			 * se parcurge lista de distante minime si actualizeaza daca se exista o optimizare
			 * fata de valoarile deja existente in bestChoice
			 */
			
			for (int i=0;i<dist.size();i++)
				if (dist.get(i)<bestChoice.get(i).minDist) {
					bestChoice.get(i).minDist =dist.get(i);
					bestChoice.get(i).freqv = words.get(search);
					bestChoice.get(i).struct = search;
				}
				else 
					if (dist.get(i)==bestChoice.get(i).minDist)
						{if (words.get(search)>bestChoice.get(i).freqv){
							bestChoice.get(i).freqv = words.get(search);
							bestChoice.get(i).struct = search;
						}
						else
							if (words.get(search)==bestChoice.get(i).freqv)
								if (search.compareTo(bestChoice.get(i).struct) < 0)
									bestChoice.get(i).struct = search;
						}
			}
		return bestChoice;
	}
	
	/*
	 * @param Structura primita drept parametru de la tastatura
	 * 
	 * Functia dynamicSearch creeaza o matrice triunghiulara de candidati, fiecare linie
	 * din matricea superior triunghiulara, initializand-o cu valorile din lista de candidati 
	 * returnata la apelul functiei FindBest pentru substring-urile din input
	 * (extrase de la i la n, unde i=0 to n-1).
	 * Pe matricea astfel obtinuta, se aplica un algoritm de tip programare dinamica, combinand toate
	 * posibilitatile, pentru ca in final sa obtinem structura corectata(in pozitia M[1][n]).
	 * 
	 * @return Structura corectata
	 */
	public static String dynamicSearch(String input) {
		
		int n = input.length();
		int i,j,l,k;
		
		//lista de candidati careia i se atribuie ulterior rezultatul functiei FindBest
		ArrayList<Candidate> list = new ArrayList<Candidate>();
		
		//matricea de candidati
		Candidate[][] M = new Candidate[n+1][n+1];
		
		//candidat intermediar, folosit pentru o eventuala actualizare a matricei
		Candidate c;
		
		//se parcurge matricea si se initializeaza cu valorile aferente fiecarui substring
		
		for (i=1;i<(n+1);i++)
			{
			j=i;
			list.clear();
			list = FindBest(input.substring(i-1,n));
			
			for (k=0; k< list.size(); k++)
			{   M[i][j] = new Candidate(list.get(k).freqv,list.get(k).minDist,list.get(k).struct);
				j++;
			}
			}
		
		//algoritm programare dinamica
		
		 for (l=2;l<=n;l++){
			  
			  for (i=1;i<=(n-l+1);i++){
				  j=i+l-1;
				  
				  for (k=i;k<=(j-1);k++){
					  //se calculeaza un candidat intermediar, combinatie a structurilor de pe pozitiile
					  //(i,k) si (k+1,j)
					  c = new Candidate((M[i][k].freqv+M[k+1][j].freqv), 
							  (M[i][k].minDist+M[k+1][j].minDist), (M[i][k].struct + " " + M[k+1][j].struct));
					  
					  //daca exista o optimizare, aceasta va fi gasita si se va inlocui elementul de pe
					  //pozitia (i,j) cu valorile gasite de candidatul intermediar
					  
					  if (c.minDist<M[i][j].minDist) M[i][j]=new Candidate(c.freqv,c.minDist,c.struct);
					  else 
						  if (c.minDist==M[i][j].minDist)
						  { if (count(c.struct) < count(M[i][j].struct)) M[i][j]=new Candidate(c.freqv,c.minDist,c.struct);
						  else if (count(c.struct) == count(M[i][j].struct)) 
							  {if (c.freqv > M[i][j].freqv) M[i][j]=new Candidate(c.freqv,c.minDist,c.struct);
							  else if (c.freqv == M[i][j].freqv) 
								  if (c.struct.compareTo(M[i][j].struct) < 0) M[i][j] = new Candidate(c.freqv,c.minDist,c.struct);
							  }
						  }
				  }
			  }
		 }
		return M[1][n].struct;
	}

	/**
	 * @param args
	 */
	@SuppressWarnings("deprecation")
	public static void main(String[] args) throws FileNotFoundException  {
		// TODO Auto-generated method stub
	
		/*
		 *  initial, se citeste dictionarul, dict.txt, ale carui valori se vor pastra intr-o 
		 *  structura de tip HashMap<String,Integer>
		 */
		
		String[] line;
		 try 
		  {
		  FileInputStream fin = new FileInputStream("dict.txt");
		  BufferedInputStream bis = new BufferedInputStream(fin);
		  DataInputStream dis = new DataInputStream(bis);
		  
		  // Now read the buffered stream.
		  while (dis.available() > 0) 
		  {
			 line=dis.readLine().split(" ");
			 words.put(line[0],Integer.parseInt(line[1]));
			  }
		  fin.close();
	      bis.close();
	      dis.close();
		  } 
		  catch (Exception e) 
		  {
		  System.err.println("Error reading file: " + e);
		  }


	
		Scanner scanner = new Scanner(System.in);
		 
		//citirea structurii din linia de comanda
		String input = scanner.nextLine();
		 
		//eliminarea spatiilor
		input = removeSpaces(input);
	 
		//se afiseaza structura corectata, obtinuta prin apelul functiei dynamicSearch
	    System.out.println(dynamicSearch(input));
	}
	}

class Candidate{
	
	int freqv; 			//frecventa structurii
	int minDist; 		//distanta minima de editare a structurii
	String struct;		//cuvant/propozitia
	
	Candidate(int freqv, int min, String struct){
		this.freqv=freqv;
		this.minDist=min;
		this.struct=struct;
	}
	
	public void show(){
		System.out.println("Candidate");
		System.out.println("Structure : " + this.struct);
		System.out.println("Frequence : " + this.freqv);
		System.out.println("Distance : " + this.minDist);
	}
}

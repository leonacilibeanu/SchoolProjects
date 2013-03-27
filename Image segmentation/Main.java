import java.io.*;

import java.util.*;

public class Main {

	/**
	 * @param args
	 */
	
	/*
	 * Am declarat urmatoarele variabile globale:
	 * 
	 * maxVal = valoarea maxima a unui pixel din imagine
	 * lambda, treshold = din formule
	 * n = nr linii
	 * m = nr coloane
	 * nrMaskF, nrMaskB = nr de pixeli diferiti de zero din mask_fg, respectiv mask_bg
	 * img = lista cu pixelii din imagine
	 * maskF, maskB = liste cu pixelii marcati din fisierele mask_fg, respectiv mask_bg
	 * 
	 */
	static int maxVal = 255;
	static int lambda, treshold;
	static int n, m;
	static int nrMaskF, nrMaskB;
	static ArrayList<Integer> img = new ArrayList<Integer>();
	static ArrayList<Boolean> maskF = new ArrayList<Boolean>();
	static ArrayList<Boolean> maskB = new ArrayList<Boolean>();
	
	/*
	 * Se initializeaza graful prin apel de constructor.
	 * Se creaza o instanta a clasei Formule prin intermediul careia se calculeaza media valorilor,
	 * deviatia standard si in final se calculeaza capacitatile aferente fiecarei muchii, care apoi
	 * se adauga in graf prin apelul functiei addEdges din clasa Graph.
	 * 
	 * @return Returneaza graful corespunzator configuratiilor citite din fisiere
	 */
	static Graph createGraph(){
		
		Formule F = new Formule();
		
		//Graful ce urmeaza a fi creat are 2 elemente in plus fata de configuratia imaginii pentru a retine
		//sursa si drena. Astfel, sursa este nodul n*m din graf, iar drena este nodul (n*m + 1).
		
		Graph g= new Graph((n*m)+2);
		double miuf, miub, sigmaf, sigmab, fu;
		int fp;
		
		/*
		 * Se calculeaza media valorilor si deviatia standard pentru foreground si background
		 */
		
		miuf = F.calculateMiu(maskF, img, nrMaskF);
		miub = F.calculateMiu(maskB, img, nrMaskB);
		sigmaf = F.calculateSigma(maskF, img, nrMaskF, miuf);
		sigmab = F.calculateSigma(maskB, img, nrMaskB, miub);
		
		int i;
		int j1 = 0;
		int j2 = 0;

		/*
		 * Se adauga muchii intre nodurile din graf, mai putin sursa si drena.
		 * Deoarece functia addEdges adauga muchii in ambele sensuri, fiind graf neorientat,
		 * se urmareste sa nu se adauge de 2 ori aceeasi muchie. Astfel se restrictioneaza adaugarea
		 * de muchii in functie de pozitia in imagine a pixelului.
		 * 
		 */
		for (i = 0; i < n*m ; i++) {
			
			//ultimul element de pe ultima linie, va avea muchie
			
			if (i != (n*m - 1)){
			
			//daca e pe ultima coloana
			if (i % m == m-1) { 
				j1 = i + m; 
				j2 = 0;
				}
			
			else
				{
				//daca e pe ultima linie
				if (i+m >= n*m) {
					j1 = i + 1; 
					j2 = 0;
					}
				
				//daca nu e nici pe ultima linie, nici ultima coloana, nici ultimul element din imagine
				//atunci se adauga muchie doar dreapta si jos pentru fiecare nod
					else {
						j1 = i + 1; 
						j2 = i + m;
						}
				}
			}
			else { 
				j1 = 0; 
				j2 = 0;
				}
			
			if (j1 !=  0){
				
				//se calculeaza capacitatea
				fp = lambda * F.calculateFp(img.get(i), img.get(j1), treshold);
				
				//se adauga muchia in graf
				g.addEdge(i, j1, fp);
			}
			
			if (j2 !=  0){
				
				//se calculeaza capacitatea
				fp = lambda * F.calculateFp(img.get(i), img.get(j2),treshold);
				
				//se adauga muchia in graf
				g.addEdge(i, j2, fp);
			}
			
			//se adauga muchie de la sursa la fiecare element si de la drena la fiecare element
			
			//mai intai pentru sursa
			fu = F.calculateFu(img.get(i), miuf, sigmaf, miub, sigmab, 1);
			g.addEdge(n*m, i, fu);
			
			//si pentru drena
			fu = F.calculateFu(img.get(i), miuf, sigmaf, miub, sigmab, 0);
			g.addEdge(i, ((n*m)+1), fu);
			
		}
		
		return g;
	}
	
	/*
	 * @param Graful pe care se aplica BFS.
	 * 
	 * Se implementeaza algoritmul BFS pentru a gasi un drum intre sursa si destinatie.
	 * Se utilizeaza un vector de parinti pentru a verica daca un nod a fost sau nu
	 * adaugat in coada.
	 * 
	 * @return Lista cu nodurile care contin calea cea mai scurta de la sursa la destinatie.
	 */
	static ArrayList<Integer> bfs(Graph g){
	
		int source = n*m;
		int sink = n*m+1;
		int u,i,v;

		/*
		 * Lista in care se retin datele asociate unui nod u si anume, vecinii si capacitatile
		 * muchiilor. 
		 */
		ArrayList<Graph.Edge> tempList = new ArrayList<Graph.Edge>();
		
		/*
		 * Coada in care se retin nodurile ce urmeaza a fi prelucrate.
		 */
		Queue<Integer> q = new LinkedList<Integer>();
		q.add(source);
		
		 /* Initializare lista de parinti. */
		 for (i = 0; i < g.size; i++) 
			 g.parents[i] = -1;
			 

		/*Se implementeaza algoritmul BFS pentru a gasi un drum intre sursa si destinatie. */

		while ((g.parents[sink] == -1) && (!q.isEmpty())) {
			/* Se determina nodul din varful cozii. */
			u = q.peek();
			q.poll(); 
			
			/* Se obtine o lista cu informatii despre vecini ai nodului extras anterior. */
			tempList = g.edges.get(u);
			
			for (i = 0; i < tempList.size(); i++) {
				
				v = tempList.get(i).y;
				
				/* 
				 * Daca vecinul v, al lui u, are capacitatea mai mare ca zero si nu i s-a gasit un parinte,
				 * atunci se actualizeaza lista de parinti si v se adauga in coada.
				 */
				if ((tempList.get(i).c > 0) && (g.parents[v] == -1)){
					g.parents[v] = u;
					q.add(v);
				}
			}
		}
	
		/* 
		 * In cazul in care nu s-a gasit un drum inseamna ca nu mai exista drumuri de crestere
		 * si functia returneaza o lista vida.
		 */
		
		if (g.parents[sink] == -1) {
			return new ArrayList<Integer>();
		}
		
		
		/*
		 * In lista returnValue se retin nodurile care determina calea de la sursa la drena.
		 */
		ArrayList<Integer> returnValue = new ArrayList<Integer>();
		int node = sink;
				
		/* Se reconstituie drumul intre sursa si drena. */
		
		while (true) {
			returnValue.add(node);
			node = g.parents[node];
			if (node == source)
				break;
		}

		returnValue.add(source);
	
		return returnValue;

}
	/*
	 * @param Graful in curs de prelucrare.
	 * @param Calea de la sursa la drena.
	 * 
	 * Cat timp mai exista un drum de la sursa la destinatie, se gaseste fluxul maxim pentru cale 
	 * si se satureaza in ambele sensuri, graful creat fiind neorientat.
	 * 
	 * @return Fluxul cu care s-a saturat drumul.
	 */
	static double saturatePath(Graph g, ArrayList<Integer> path){
		
		int i,j;
		double c = 0;
		
		/* Initializare flux maxim. */
		double maxf = Double.MAX_VALUE;
		
		/*
		 * Lista in care se retin datele asociate unui nod si anume, vecinii si capacitatile
		 * muchiilor. 
		 */
		
		ArrayList<Graph.Edge> tempList = new ArrayList<Graph.Edge>();
		
		/* Se determina fluxul maxim pentru calea primita drept parametru. */
		
		for (i = path.size() - 1; i > 0; i--){
			
			/* Se determina lista de vecini si capacitati ale muchiilor pentru nodul de pe pozitia i din cale.*/
			tempList = g.edges.get(path.get(i));
			
			/* Se cauta valoarea capacitatii nodului vecin, de pe pozitia i-1 din cale. */
			for (j = 0; j < tempList.size(); j++)
				if (tempList.get(j).y == path.get(i-1)) 
					c = tempList.get(j).c;
			
			/* Daca valoarea capacitatii este mai mica decat a fluxului, variabila de flux maxim se actualizeaza. */
			
			if (c < maxf)
				maxf = c;
			
		}
		/* Urmeaza saturarea drumului in ambele sensuri. */
		for (i = path.size() - 1; i > 0; i--){
			
			/* Mai intai se satureaza de la i la i-1. */
			tempList = g.edges.get(path.get(i));
		
			for (j = 0; j<tempList.size(); j++) 
				if (tempList.get(j).y == path.get(i-1)) 
					g.edges.get(path.get(i)).get(j).c -=maxf;
			
			
			/* Apoi de la i-1 la i. */
			tempList = g.edges.get(path.get(i-1));
			for (j = 0; j<tempList.size(); j++) 
				if (tempList.get(j).y == path.get(i)) 
					g.edges.get(path.get(i-1)).get(j).c -=maxf;
		}
		
		return maxf;
	}
	/*
	 * @param Graful in curs de prelucrare.
	 * 
	 * Cat timp mai exista drumuri de crestere in graf (determinate cu functia bfs), acestea 
	 * se satureaza si se contorizeaza fluxul de saturare pentru fiecare astfel de drum.
	 * 
	 * @return Fluxul maxim prin graf.
	 */
	static double maxFlow (Graph g){
		
		double result = 0;
		
		/* Drumul de crestere. */
		ArrayList<Integer> path = new ArrayList<Integer>();
		
		while (true) {
			path = bfs(g);
			
			/* Daca s-a gasit o cale. */
			if (path.size() == 0) 
				return result;
			else 
				/* Se aduna la rezultatul final fluxul cu care s-a saturat calea. */
				result += saturatePath(g,path);
		}
	}
	
	/*
	 * @param Graful in curs de prelucrare.
	 * 
	 * Se realizeaza o parcurgere BFS din nodul sursa si se actualizeaza nodurile atinse de parcurgere
	 * cu valoarea true in lista in. Toate celelalte noduri din in vor avea valoarea false.
	 * 
	 * @return Vector cu toate nodurile din graf, fiind marcate cele de la care se poate ajunge de la sursa.
	 */
	
	static boolean[] minCut(Graph g){
		
		/* Lista in care se marcheaza nodurile pentru care exista o cale de la sursa. */
		boolean[] in = new boolean[g.size];
		
		
		/*
		 * Lista in care se retin datele asociate unui nod si anume, vecinii si capacitatile
		 * muchiilor. 
		 */
		ArrayList<Graph.Edge> tempList = new ArrayList<Graph.Edge>();
		Queue<Integer> q = new LinkedList<Integer>();
		
		int i,u,v;
		int source = n*m;
		
		for ( i = 0; i < g.size; i++)
			in[i] = false;
		
		q.add(source);
		in[source] = true;
		
		/* Se ruleaza BFS si se marcheaza nodurile atinse. */
		
		while (!q.isEmpty()){
			u = q.peek();
			q.poll();
			
			tempList = g.edges.get(u);
			for (i = 0; i < tempList.size(); i++){
				v = tempList.get(i).y;
				if ((tempList.get(i).c > 0) && (in[v] == false)){
					in[v] = true;
					q.add(v);
				}
			}
		}
		
	return in;
	}

	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		/* Numarul de pixeli marcati in mask_fg */
		nrMaskF=0;
		/* Numarul de pixeli marcati in mask_bg */
		nrMaskB=0;
	
		/* 
		 * Citirea pixelilor din imagine.pgm 
		 */
		
		String[] line;
		 try 
		  {
		  FileInputStream fin = new FileInputStream("imagine.pgm");
		  BufferedInputStream bis = new BufferedInputStream(fin);
		  DataInputStream dis = new DataInputStream(bis);
		 
		  /* Se ignora prima linie, pe care se afla marcajul "P2" */
		  dis.readLine();
		  
		  /* Se citesc dimensiunile imaginii */
		  line=dis.readLine().split(" ");
		  
		  /* 
		   * m = nr coloane;
		   * n = nr linii;
		   */
		  m = Integer.parseInt(line[0]);
		  n = Integer.parseInt(line[1]);
		  
		  /* Se ignora valoarea maxima a unui pixel, aceasta fiind intotdeauna 255 */
		  dis.readLine();
		  
		  /* Citirea si memorarea valorii fiecarui pixel in lista img. */
		  while (dis.available() > 0) 			 
			 img.add(Integer.parseInt(dis.readLine()));
		  
			  
		  fin.close();
	      bis.close();
	      dis.close();
		  } 
		  catch (Exception e) 
		  {
		  System.err.println("Error reading file: " + e);
		  }
		 
		 int x;
		 
		 /*
		  * Citirea mastii pentru foreground.
		  */
		 
		 try 
		  {
		  FileInputStream fin = new FileInputStream("mask_fg.pgm");
		  BufferedInputStream bis = new BufferedInputStream(fin);
		  DataInputStream dis = new DataInputStream(bis);
		  
		  /*
		   * Se ignora primele 3 linii, dimensiunea imaginii fiind deja setata,
		   * iar valoarea maxima a unui pixel se considera cunoscuta (255).
		   */
		  dis.readLine();
		  dis.readLine();
		  dis.readLine();
		  
		  /* 
		   * Se citeste marcajul pentru foreground.
		   * Daca valoarea pixelului este mai mare ca zero, se considera marcat, 
		   * se adauga valoarea true in masca si se incrementeaza nsMaskF,
		   * altfel se adauga false.
		   */
		  while (dis.available() > 0) {
			  x = Integer.parseInt(dis.readLine());
			if (x > 0) {
				maskF.add(true);
				nrMaskF++;
			} 
			else 
				maskF.add(false);
		  }
			  
		  fin.close();
	      bis.close();
	      dis.close();
		  } 
		  catch (Exception e) 
		  {
		  System.err.println("Error reading file: " + e);
		  }
		 
		 /*
		  * Citirea mastii pentru background.
		  */
		 try 
		  {
		  FileInputStream fin = new FileInputStream("mask_bg.pgm");
		  BufferedInputStream bis = new BufferedInputStream(fin);
		  DataInputStream dis = new DataInputStream(bis);
		  
		 /* Se ignora primele 3 linii ale fisierului. */
		  dis.readLine();
		  dis.readLine();
		  dis.readLine();
		  
		  /* 
		   * Se citeste marcajul pentru background.
		   * Daca valoarea pixelului este mai mare ca zero, se considera marcat, 
		   * se adauga valoarea true in masca si se incrementeaza nsMaskB,
		   * altfel se adauga false.
		   */
		  
		  while (dis.available() > 0) {
			  x = Integer.parseInt(dis.readLine());
			if (x > 0) {
				maskB.add(true);
				nrMaskB++;
			} 
			else 
				maskB.add(false);           
		  }
			  
		  fin.close();
	      bis.close();
	      dis.close();
		  } 
		  catch (Exception e) 
		  {
		  System.err.println("Error reading file: " + e);
		  }
		
		 /*
		  * Citirea parametrilor lambda si treshold din fisierul "parametri.txt".
		  * Observatie: Am considerat fiecare parametru scris pe cate o linie.
		  */
		 try 
		  {
		  FileInputStream fin = new FileInputStream("parametri.txt");
		  BufferedInputStream bis = new BufferedInputStream(fin);
		  DataInputStream dis = new DataInputStream(bis);
		  
		  lambda = Integer.parseInt(dis.readLine());
		  treshold = Integer.parseInt(dis.readLine());
		  			  
		  fin.close();
	      bis.close();
	      dis.close();
		  } 
		  catch (Exception e) 
		  {
		  System.err.println("Error reading file: " + e);
		  }
		 

		 /*
		  * Se apeleaza functia createGraph pentru a crea grafel aferent imaginii.
		  */
		 Graph g = createGraph();
		 
		 /*
		  * Determinarea fluxului maxim prin graf.
		  */
		 double flow = maxFlow(g);	
			
		/* Valoarea energiei se afiseaza in consola. */
		 
		 System.out.println("" + flow);

		/*
		 * Se determina taietura minima prin graf si rezultatul se memoreaza in vectorul in.
		 * Asa cum am precizat si in descrierea functiei minCut, vectorul returnat va avea
		 * valoarea true pentru nodurile la care se poate ajunge de la sursa si false in caz contrar.
		 * Mai exact, cele care vor avea valoarea true, vor fi printate in fisier cu valoarea "0"
		 * si cu "255" altfel.
		 */
		boolean[] in = minCut(g);
		int i;
		
		/* Printarea imaginii obtinute in fisierul "segment.pgm". */
		
		try{
			  /* Se creeaza fisierul. */ 
			  FileWriter fstream = new FileWriter("segment.pgm");
			  BufferedWriter out = new BufferedWriter(fstream);
			  out.write("P2\n");
			  out.write(m + " " + n + "\n");
			  out.write(255 + "\n");
			  
			  for ( i = 0; i < n*m; i++)
				  if (in[i])
						out.write(0 + "\n");
					else
						out.write(255 + "\n");
			  
			  /* Se inchide fisierul. */
			  out.close();
			  }
		catch (Exception e){
			  System.err.println("Error: " + e.getMessage());
			  }
	}
}

/*
 * Clasa Graph in care se stabilesc caracteristicile corespunzatoare cerintelor pentru graful imaginii.
 * 
 * size = dimensiunea grafului (nr linii*nr coloane +  2)
 * edges = HashMap pentru muchii. Cheia o reprezinta nodul sursa al muchiei si valoarea este o lista de tip
 * 			Edge in care se retin nodul destinatie al muchiei si capacitatea.
 * parents = vectorul de parinti
 */

class Graph
{
	
	int size;
	HashMap<Integer,ArrayList<Edge>> edges;
	int[] parents;
	
	
	class Edge{
		/* Nodul destinatie al muchiei. */
		int y;
		/* Capacitatea muchiei. */
		double c;
		
		
		Edge(int y, double c) {
			this.y = y;
			this.c = c;
		}
	}
	
	Graph(int size){
		int i,j;
		
		this.size = size;
		
		/* Initializare vector de parinti. */ 
		parents = new int[size];
		 
		/* 
		 * Initializare HashMap pentru muchii.
		 * Se adauga toate nodurile grafului care, initial,
		 * nu au niciun vecin.
		 */
		edges = new HashMap<Integer,ArrayList<Graph.Edge>>();
		 
		for (i = 0; i < size; i++)
			edges.put(i, new ArrayList<Edge>());

	}
	
	/*
	 * Functie de adaugare a unei muchii de capacitate c intre nodurile x si y.
	 * Se adauga muchie in ambele sensuri, graful fiind neorientat.
	 */
	void addEdge(int x, int y, double c){
	
		edges.get(x).add(new Edge(y,c));
		edges.get(y).add(new Edge(x,c));
			
	}
	
	/*
	 * Functie de afisare a grafului.
	 */
	void show(){
		int i,j;
		ArrayList<Graph.Edge> tempList = new ArrayList<Graph.Edge>();
		
		for (i = 0; i < this.edges.size(); i++)
		{
			System.out.println("nod: " + i);
			tempList = this.edges.get(i);
			for ( j = 0; j < tempList.size(); j++) 
				System.out.println("v: " + tempList.get(j).y  + " , c: " + tempList.get(j).c);
		}
	}
}

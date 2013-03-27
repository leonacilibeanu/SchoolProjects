/*
 * To change this template, choose Tools | Templates

 * and open the template in the editor.
 */
//package main;

/**
 *
 * @author Cilibeanu Leona Camelia
 */
import java.util.*;


// dorim sa avem  o relatie de ordine totala pentru obiectele clasei 
//folosind  interfata Comparable

abstract class ArboreHuffman implements Comparable<ArboreHuffman> {
    public final int frecventa; // frecventa arborelui
    public ArboreHuffman(int frec) { frecventa = frec; }
 
 /* comparare dupa frecventa doi arbori
 * @param   arbore  este un ArboreHuffman
 * @return diferenta de frecventa intre arborele curent si ArboreleHuffman arbore
 */

    public int compareTo(ArboreHuffman arbore) {
        return frecventa - arbore.frecventa;
    }
}
 
class FrunzaHuffman extends ArboreHuffman {
    public final char valoare; // codul ASCII al frunzei
 
    public FrunzaHuffman(int frec, char val) {
        super(frec);
        valoare = val;
    }
}
 
class NodHuffman extends ArboreHuffman {
    public final ArboreHuffman stang, drept; // subarbore 
    //constructor
    public NodHuffman(ArboreHuffman s, ArboreHuffman d) {
        super(s.frecventa + d.frecventa);
        stang = s;
        drept = d;
    }
}

public class Main {
    static int []v=new int[258];
    static String []cod= new String[10000];
    public static String newline = System.getProperty("line.separator","\n");
    static int ii=-1;
 
   
 /* 
 * @param   CharFrecv este un vector cu frecventele indexate dupa codul caracterului
 * @return radacina ArboreHuffman 
 */
    public static ArboreHuffman creazaArbore(int[] charFrecv) {
        PriorityQueue<ArboreHuffman> arbori = new PriorityQueue<ArboreHuffman>();
        // avem o padure de frunze - cate una pentru fiecare caracter
        for (int i = 0; i < charFrecv.length; i++)
            if (charFrecv[i] > 0)
                arbori.offer(new FrunzaHuffman(charFrecv[i], (char)i));
         // le introducem intr-o coada de prioritati
        assert arbori.size() > 0;
        // cat timp avem mai mult de un arbore in coada
        while (arbori.size() > 1) {
            // doi arbori cu frecventa cea mai mica
            ArboreHuffman a = arbori.poll();
            ArboreHuffman b = arbori.poll();
 
            // cream un nou nod din  cei doi  si il inseram in coada
            arbori.offer(new NodHuffman(a, b));
        }
        return arbori.poll();
    }
 /* 
 * @param  arbore care este chiar radacina
 * @param codul in el se calculeaza <secventa_biti_codare>
 * afiseaza codurile ASCII si <secventa_biti_codare>pentru toate frunzele
 * memoreaza in doi vectori codurile ASCII si <secventa_biti_codare>pentru toate frunzele
 * @return  
 */
    public static void afiseazaCoduri(ArboreHuffman arbore, StringBuffer codul) {
        assert arbore != null;
        if (arbore instanceof FrunzaHuffman) {
            FrunzaHuffman frunza = (FrunzaHuffman)arbore;
            // afiseaza codul ASCII si <secventa_biti_codare> a frunzei
            System.out.println((int)frunza.valoare + " "+ codul);
            ii++;
            v[ii]=(int)frunza.valoare; 
            cod[v[ii]]=codul.toString();
            
        } else if (arbore instanceof NodHuffman) {
            NodHuffman nod = (NodHuffman)arbore;
 
            // traversare stanga
            codul.append('0');
            afiseazaCoduri(nod.stang, codul);
            codul.deleteCharAt(codul.length()-1);
 
            // traversare dreapta
            codul.append('1');
            afiseazaCoduri(nod.drept, codul);
            codul.deleteCharAt(codul.length()-1);
        }
    }
  /* 
 * @param  text textul care trebuie codat
 *  determinarea si afisarea textului codat si la final pune un . pe o noua linie
 * @return  
 */
public static void afiseazaTextCodat(String text)
{String s="";
    for (char c : text.toCharArray ())
            s=s+cod[c];
 System.out.println(".\n"+s);
}

 /* 
 * @param  text textul care trebuie deocodat 
 * @param i numarul de caractere distincte din textul care trebuie decodat
 * din text
 *  determinarea si afisarea textului decodat si la final pune un . pe o noua linie
 * @return  
 */
// afisarea textului decodat 
public static void afiseazaTextDecodat(String text, int i)
{String s=""; 

while(1==1)
 {    for (int j=0; j<=i;j++)
        if(text.indexOf(cod[v[j]])==0)
       {   
           System.out.printf("%c",(char)v[j] );
           text=text.substring(cod[v[j]].length());
           break;
         }
    if(text.length()==0 || text==null) break;
 }
          System.out.print(newline+".");
}
    /**
     * @param args the command line arguments 
     * la apel vom folosi fie parametrul c fie d 
     */
    public static void main(String[] args) {
        // TODO code application logic here
        String text="", linie="";             
        int nrl=0;
      
       
        if (args.length == 0 ) System.out.println(" Eroare Nu exista parametrii!!");
        else
            if (args.length > 1 ) System.out.println(" Eroare Prea multi parametrii!!");
            else if(args[0].equals("c")) 
            { Scanner conin = new Scanner(System.in);
             
              while (conin.hasNext()) 
                { linie= conin.nextLine();
                  nrl++;
                  if (!linie.equals("."))  if (nrl==1) text=linie;
                                           else text=text+newline+linie;  
                  if (linie.equals(".")) break;
                }
              
        
     
        // consideram ca avem maximum 256 caractere (ar putea fi ASCII extins)
        int[] charFrecv = new int[256];
        // determinam frecventa de aparitie a caracterelor din textul citit de la consola
        for (char c : text.toCharArray())
            charFrecv[c]++;
 
        // construim arborele cunoscand caracterele si frecventa de aparitie
        ArboreHuffman arbore = creazaArbore(charFrecv);
        
        //afisam rezultatele la consola <cod ASCII> <cod binar>
        afiseazaCoduri(arbore, new StringBuffer());
        // afisam textul codat binar
        afiseazaTextCodat(text);
             }
                 
                 else if(args[0].equals("d")) 
                     {Scanner conin = new Scanner(System.in);
                      ii=-1;
                      while (conin.hasNext()) 
                        { linie= conin.nextLine();
                          ii++;
                          if (!linie.equals("."))  {
                                          Scanner lin_in = new Scanner(linie);
                                          v[ii]=lin_in.nextInt();
                                          cod[v[ii]]=lin_in.next();
                                                   }
                          if (linie.equals(".")) {text=conin.nextLine();
                                                   break;    }
                           }
                      // afisam rezultatul decodarii
                      afiseazaTextDecodat(text,ii);
                       }
                 
                      else System.out.println(" argument gresit");
                
        
    }
}


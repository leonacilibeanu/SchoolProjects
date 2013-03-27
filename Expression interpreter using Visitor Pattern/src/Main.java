//utilizat pentru citire

// @author Cilibeanu Leona
import java.util.Scanner;
//utilizat pentru stivele de operatori si operanzi pentru implementarea algoritmului la care face referire enuntul temei
import java.util.ArrayList;
//utilizat pentru afisare
import java.io.PrintWriter;
//utilizat pentru afisare-crearea unui PrintWriter
import java.io.FileOutputStream;
//utilizat pentru citire-crearea unui scanner
import java.io.File;
/*
 * Clasa Principala program-in care sunt implementate metodele ce reprezinta citirea,crearea arborelui prin algoritmul din enunt,si apelul
 * cizitatorilor
 */
public class Main {
    /*
     * Metoda principala Main in care se apeleaza metodele de citire, constructie arbore, vizitatori si afisarea rezultatelor
     * @param args numele fisierului corespunzator codului sursa
     */
    public static void main(String[] args){
        String numeFisier=args[0];
        //crearea radacina generica arbore parsare
        Node radacina=new Node();
        //constructie arbore
        Main.construiesteArbore(radacina,numeFisier);
        //deschidere fisier pentru scriere si afisarea indentata
        PrintWriter out=null;
        try{
            out=new PrintWriter(new FileOutputStream(numeFisier+"_pt"));
            //apel cu numarTaburi 0 pentru ca radacina generica sa fie afisata fara nici un tab inainte
            radacina.display(out,0);
        }catch(Exception e){}
        finally{
            try{
                out.close();
            } catch(Exception e){}
        }
        //deschidere fisier pentru scriere, instantiere vizitator semantic, si apel
        SemanticVisitor sv=null;
        try{
            out=new PrintWriter(new FileOutputStream(numeFisier+"_sv"));
            sv=new SemanticVisitor(out);
            //apel acceptare vizitator
            radacina.accept(sv);
        }catch(Exception e){}
        finally{
            try{
                out.close();
            } catch(Exception e){}
        }
        //testare existenta erori semantice si deschidere fisier pentru scriere, apel vizitator Rezultat si afisare rezultate
        if(sv.getOk()){
            ResultVisitor rv=null;
            try{
                out=new PrintWriter(new FileOutputStream(numeFisier+"_rv"));
                rv=new ResultVisitor(out);
                //apel acceptare vizitator pentru prelucrare arbore
                radacina.accept(rv);
                //afisare rezultate
                rv.afisareRezultate();
            }catch(Exception e){}
            finally{
                try{
                    out.close();
                } catch(Exception e){}
            }
        }
    }
    /*
     * Metoda statica de citire cod sursa si creare arbore parsare folosind algoritmul din enunt cu 2 stive.
     * @param radacina nodul radacina al arborelui de parsare
     * @param numeFisier numele fisierului corespunzator codului sursa
     */
    public static void construiesteArbore(Node radacina,String numeFisier){
        //stiva Operatori pentru implementare algoritm
        ArrayList<Node> stivaOperatori=new ArrayList<Node>();
        //stiva Operanzi pentru implementare algoritm
        ArrayList<Node> stivaOperanzi=new ArrayList<Node>();
        //deschidere fisier pentru citire
        File f=null;
        Scanner scan=null;
        try{
           f=new File(numeFisier); 
           scan=new Scanner(f);
        } catch(Exception e){}
        //variabile contorizare linie si coloana
        int linie=1,coloana=1;
        //citire cat timp exista linii noi de citit
        while(scan.hasNextLine()){
            Node x=null;
            //citire linie
            String instructiune=scan.nextLine();
            //impartire in functie de caracterul spatiu in string rezultant un array de stringuri care reprezonta operatorii si opearanzii
            String[] instructiuni=instructiune.split(" ");
            //initializare coloana la inceput de linie noua
            coloana=1;
            //ciclare prin array si creare Noduri corespunzatoare, parsand informatia de pe o linie si adaugandu-le in stivele corespunzatoare
            for(int i=0;i<instructiuni.length;i++){
                //daca reprezinta un egal
                if(instructiuni[i].contentEquals("=")){
                    //creare instanta Egal noua
                    x=new Egal(null,null,linie,coloana);
                    //adaugare in stiva operatori
                    Main.adaugaOperator(x,stivaOperatori,stivaOperanzi);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=2;
                    continue;
                }
                //daca reprezinta un plus
                if(instructiuni[i].contentEquals("+")){
                    //creare instanta plus noua
                    x=new Plus(null,null,linie,coloana);
                    //adaugare in stiva operatori
                    Main.adaugaOperator(x,stivaOperatori,stivaOperanzi);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=2;
                    continue;
                }
                //daca reprezinta o inmultire
                if(instructiuni[i].contentEquals("*")){
                    //creare instanta inmultire noua
                    x=new Inmultire(null,null,linie,coloana);
                    //adaugare in stiva operatori
                    Main.adaugaOperator(x,stivaOperatori,stivaOperanzi);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=2;
                    continue;
                }
                //daca reprezinta valoare booleana true
                if(instructiuni[i].contentEquals("true")){
                    //creare instanta MyBoolean noua
                    x=new MyBoolean(true);
                    //adaugare in stiva operanzi
                    stivaOperanzi.add(x);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=5;
                    continue;
                }
                //daca reprezinta valoare booleana false
                if(instructiuni[i].contentEquals("false")){
                    //creare instanta MyBoolean noua
                    x=new MyBoolean(false);
                    //adaugare in stiva Operanzi
                    stivaOperanzi.add(x);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=6;
                    continue;
                }
                //daca reprezinta un numar(incepe cu cifra)
                if(instructiuni[i].charAt(0)>='0'&&instructiuni[i].charAt(0)<='9'){
                    //creare instanta noua Intreg
                    x=new Intreg(Integer.parseInt(instructiuni[i]));
                    //adaugare in stiva operanzi
                    stivaOperanzi.add(x);
                    //incrementare coloana tinand cont de spatiile libere
                    coloana+=instructiuni[i].length()+1;
                    continue;
                }
                //altfel reprezinta o variabila
                //creare instanta Variabila noua
                x=new Variabila(instructiuni[i],linie,coloana);
                //adaugare in stiva operanzu
                stivaOperanzi.add(x);
                //incrementare coloana tinand cont de spatiile libere
                coloana+=instructiuni[i].length()+1;
            }
            //creare arbore prin initializarea legaturilor dintre noduri
            //ciclare prin stiva de operatori, extragand cate 2 operanzi si reunindu-i sub operatorul respectiv
            while(true) {
                //daca stiva este goala
                if(stivaOperatori.isEmpty())
                    break;
                //scoare un operatori din stivaOperatori
                Node z=stivaOperatori.remove(stivaOperatori.size()-1);
                //daca operatorul e null
                if(z==null)
                    break;
                //scoatere operanzi din stiva Operanzi
                Node dreapta=stivaOperanzi.remove(stivaOperanzi.size()-1);
                Node stanga=stivaOperanzi.remove(stivaOperanzi.size()-1);
                //initializare legaturi, adaugandu-i ca descendenti ai Operatorului
                z.adaugaDescendent(stanga);
                z.adaugaDescendent(dreapta);
                //adaugare subarbore creat in stiva Operanzi si repetarea pasilor anteriori
                stivaOperanzi.add(z);
            }
            //adaugare subarbore astfel creat(corespunzator unei linii de cod) la radacina generica
            radacina.adaugaDescendent(stivaOperanzi.remove(0));
            //incrementarea liniei si repetarea pasilor anteriori
            linie++;
        }
    }
    /*
     * Metoda adaugare operator la stiva de operatori pentru implementarea algoritmului precizat in enunt. La intalnirea unui operator +,
     * inaintea caruia(in ordinea din linia de cod) exista mai multe inmultiri, se efectueaza inmultirile, reunind operanzii sub o inmultire,
     * pentru respectarea ordinii operatiilor.
     * @param o Operatorul de adaugat
     * @param stivaOperatori stiva de operatori
     * @param stivaOperanzi stiva de operanzi
     */
     public static boolean adaugaOperator(Node o,ArrayList<Node> stivaOperatori, ArrayList<Node> stivaOperanzi){
         //daca stiva de operatori nu e goala si inaintea plusului se afla inmultiri, efectuarea inmultirilor
        if(!stivaOperatori.isEmpty()&&stivaOperatori.get(stivaOperatori.size()-1) instanceof Inmultire&&o instanceof Plus){
            Node x=null;
            //scoatere secventiala operatori inmultire si "efectuare operatii", reunind sub operator operanzii 
            while(true){
                //daca stiva e goala
                if(stivaOperatori.isEmpty()){
                    x=null;
                    break;
                }
                //scoatere operator
                x=stivaOperatori.remove(stivaOperatori.size()-1);
                //daca operatorul e null sau daca nu e de inmultire
                if(x==null||!(x instanceof Inmultire))
                    break;
                //scoatere operanzi din stiva
                Node dreapta=stivaOperanzi.remove(stivaOperanzi.size()-1);
                Node stanga=stivaOperanzi.remove(stivaOperanzi.size()-1);
                //initializare legaturi reunind astfel operanzii sub operator si se adauga operatorul(subarborele creat) la stiva operanzi
                x.adaugaDescendent(stanga);
                x.adaugaDescendent(dreapta);
                //adaugare la stiva operanzi arbore nou creat
                stivaOperanzi.add(x);
            }
            //daca ultimul operator ramas in x este diferit de null, deci a intalnit un operator plus sau egal care nu trebuia scos, se readauga in stiva operatori
            if(x!=null)
                stivaOperatori.add(x);
        }
        //adaugare operator nou in stiva de operatori
        return stivaOperatori.add(o);
    }
}

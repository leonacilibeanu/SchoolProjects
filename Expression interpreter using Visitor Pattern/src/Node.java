//utilizat pentru lista de copii al fiecarui Nod
import java.util.ArrayList;
//utilizat pentru afisare
import java.io.PrintWriter;
//utilizat pentru iterarea prin colectii
import java.util.Iterator;
/*
 * Clasa Nod generic in arborele de parsare
 */
public class Node implements Visitable{
    /*
     * Campuri in care se retin numarul liniei si coloanei corespunzatoare nodului din fisierul sursa
     */
    protected int linie, coloana;
    /*
     * Lista copii Nod(va contine 2 Noduri in cazul operatorilor, oricate pentru radacina generica si 0 pentru Operanzi) 
     */
    protected ArrayList<Node> descendenti;
    /*
     * Camp de tip pentru operatori - utilizat in semnatic visitor pentru usurarea verificarii semnantice - reprezinta tipul de date continut, pe
     * care il are un subordine operatorul (-1 pentru neinitializat, -2 pentru conflict de tip, 0 pentru boolean, 1 pentru intreg)
     */
    protected int tip=-1;
    /**
     * Constructor fara parametru care initializeaza campurile instantei clasei.
     */
    public Node(){
        this.linie=0;
        this.coloana=0;
        this.descendenti=new ArrayList<Node>();
    }
    /*
     * Constructor cu parametrii
     * @param linie numarul liniei corespunzatoare nodului in codul sursa
     * @param coloana numarul coloanei corespunzatoare nodului in codul sursa
     */
    public Node(int linie,int coloana){
        this.linie=linie;
        this.coloana=coloana;
        this.descendenti=new ArrayList<Node>();
    }
    /*
     * Metoda ce returneaza numarul liniei corespunzatoare nodului in codul sursa
     * @return numarul liniei corespunzatoare nodului in codul sursa
     */
    public int getLinie(){
        return this.linie;
    }
    /*
     * Metoda ce returneaza numarul coloanei corespunzatoare nodului in codul sursa
     * @return numarul coloanei corespunzatoare nodului in codul sursa
     */
    public int getColoana(){
        return this.coloana;
    }
    /*
     * Metoda ce actualizeaza numarul valoarea campului tip(utilizat pentru operatori in SemanticVisitor)
     * @param tip valoarea cu care va fi initializat campul tip
     */
     protected void setTip(int tip){
        this.tip=tip;
    }
     /*
     * Metoda ce returneaza tipul nodului(Utilizat pentru operatori in SemanticVisitor)
     * @return tipul nodului(-1 pentru neinitializat, -2 pentru conflict de tip, 0 pentru boolean, 1 pentru intreg)
     */
    protected int getTip(){
        return this.tip;
    }
    /*
     * Metoda adaugare copii pentru Nod
     * @param x nodul de adaugat ca descendent al Nodului curent
     */
    protected boolean adaugaDescendent(Node x){
        return this.descendenti.add(x);
    }
    /*
     * Metoda generica afisare continut Nod. Aceasta afiseaza numarul de taburi precizate de parametrul numarTaburi si verificand tipul nodului
     * afiseaza semnul =(pt Egal),+(Pt Plus),*(pt Inmultire),numele variabilei,valoarea retinut in tipurile de date,sau r pentru radacina generica
     * @param out streamul ce va fi utilizat pentru afisare
     * @param numarTaburi numarul de taburi de afisat pentru a obtine o afisare indentata
     */
    public void afiseaza(PrintWriter out,int numarTaburi){
        //afisare taburi
        while(numarTaburi>0){
            out.print("\t");
            numarTaburi--;
        }
        //daca nodul e instanta a Egal
        if(this instanceof Egal){
            out.println("=");
            return;
        }
        //daca nodul e instanta a Inmultire
        if(this instanceof Inmultire){
            out.println("*");
            return;
        }
        //daca nodul e instanta a Plus
        if(this instanceof Plus){
            out.println("+");
                return;
        }
        //daca nodul e instanta a Variabila
        if(this instanceof Variabila){
            out.println(((Variabila)this).getNume());
                return;
        }
        //daca nodul e instanta a Intreg
        if(this instanceof Intreg){
            out.println(((Intreg)this).getValoare());
                return;
        }
        //daca nodul e instanta a MyBoolean
        if(this instanceof MyBoolean){
            out.println(((MyBoolean)this).getValoare());
            return;
        }
        //daca nodul reprezinta radacina generica
        out.println("r");
    }
    /*
     * Metoda recursiva de afisare indentata a arborelui de parsare
     * @param out streamul de utilizat la afisare
     * @param numarTaburi numarul de taburi de afisat pentru a obtine afisarea indentata
     */
    public void display(PrintWriter out,int numarTaburi){
        //afisare continut nod curent
        this.afiseaza(out,numarTaburi);
        //iterare prin descendeti si apel recursiv display cu numarul de taburi incrementat
        Iterator iterare=this.descendenti.iterator();
        while(iterare.hasNext())
            ((Node)iterare.next()).display(out, numarTaburi+1);
    }
    /*
     * Implementare metoda accept din interfata pe care o implementeaza clasa Node. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra arborele de parsare incepand din radacina generica.
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
    /*
     * Metoda ce returneaza un iterator pentru lista de descendenti
     * @return iterator pentru lista de descendenti
     */
    protected Iterator getIterator(){
        return this.descendenti.iterator();
    }
}

//utilizat pentru afisare
import java.io.PrintWriter;
//utilizate pentru retinere variabile definite si tipul cu care au fost definite
import java.util.Map;
import java.util.TreeMap;
//utilizat pentru iterare prin colectii
import java.util.Iterator;
//utilizat pentru lista de operatii a caror semantica trebuie verificata
import java.util.ArrayList;
/*
 * Clasa ce implementeaza interfata Visitor, astfel reprezentand un vizitator ce parcurge arborele de parsare si verifica semantica operatiilro
 * ce trebuie efectuate
 */
public class SemanticVisitor implements Visitor{
    /*
     * streamul de utilizat la afisare
     */
    private PrintWriter out=null;
    /*
     * dictionarul de variabile declarate si tipurile lor
     */
    private Map<String,Integer> m=null;
    /*
     * lista de operatii de efectuat si operanzii corespunzatori
     */
    private ArrayList<Node> l=null;
    /*
     * camp de decizie asupra corectitudinii semantice dupa parcurgerea arborelui
     */
    private boolean ok=true;    
    /*
     * Constructor vizitator semantic cu parametru
     * @param out streamul de utilizat la afisare
     */
    public SemanticVisitor(PrintWriter out){
        this.out=out;
        this.m=new TreeMap<String,Integer>();
        this.l=new ArrayList<Node>();
    }
    /*
     * metoda ce returneaza campul ok corespunzator corectitudinii semantice dupa parcurgerea arborelui
     * @return true sau false daca nu exista sau exista erori semantice
     */
    public boolean getOk(){
        return this.ok;
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Radacina generica
     * @param x radacina generica de vizitat
     */
    @Override
    public void visit(Node x){
        //iterare prin descendenti deci linii de cod
        Iterator itereaza=x.descendenti.iterator();
        while(itereaza.hasNext()){
            //verificare corectitudine linie de cod
            //apel de accept vizitator pentru umplere lista de operatii si operanzi
            ((Node)itereaza.next()).accept(this);
            //iterare prin lista pana cand ramane doar operatorul =, deci la final fiind atribuit si tipul variabilei in dictionar
            while(this.l.size()>2){
                //scoare cate 2 operanzi din lista
                Node dreapta=this.l.remove(this.l.size()-1);
                Node stanga=this.l.remove(this.l.size()-1);
                Node operator=null;
                //cautare primul operator neprelucrat, deci cel care corespunde operanzilor 
                for(int i=this.l.size()-1;i>=0;i--)
                    if((this.l.get(i) instanceof Plus||this.l.get(i) instanceof Inmultire||this.l.get(i) instanceof Egal)&&this.l.get(i).getTip()!=-2){
                        operator=this.l.get(i);
                        break;
                    }
                //stabilire tip date stanga si dreapta
                int tipDreapta=-1, tipStanga=-1;
                if(dreapta instanceof MyBoolean)
                    tipDreapta=0;
                if(dreapta instanceof Intreg)
                    tipDreapta=1;
                //daca cea din dreapta e variabila trebuie sa fie declarata deja, deci verificare declaratie sau afisare eroare
                if(dreapta instanceof Variabila){
                    if(this.m.containsKey(((Variabila)dreapta).getNume()))
                            tipDreapta=this.m.get(((Variabila)dreapta).getNume());
                    else{
                        this.out.println(((Variabila)dreapta).getNume()+" nedeclarata la linia "+((Variabila)dreapta).getLinie()+" coloana "+((Variabila)dreapta).getColoana());                 
                        this.ok=false;
                        tipDreapta=-2;
                    }    
                }
                if(dreapta instanceof Plus)
                    tipDreapta=((Plus)dreapta).getTip();
                if(dreapta instanceof Inmultire)
                    tipDreapta=((Inmultire)dreapta).getTip();
                if(stanga instanceof Plus)
                    tipStanga=((Plus)stanga).getTip();
                if(stanga instanceof Inmultire)
                    tipStanga=((Inmultire)stanga).getTip();
                if(stanga instanceof MyBoolean)
                    tipStanga=0;
                if(stanga instanceof Intreg)
                    tipStanga=1;
                //daca cel din stanga e variabila dar nu corespunde unui operator egal, atunci trebuie sa fi fost declarat inainte, deci verificare
                //declaratie sau afisare eroare
                if(stanga instanceof Variabila&&!(operator instanceof Egal)){
                    if(this.m.containsKey(((Variabila)stanga).getNume()))
                            tipStanga=this.m.get(((Variabila)stanga).getNume());
                    else{
                        this.out.println(((Variabila)stanga).getNume()+" nedeclarata la linia "+((Variabila)stanga).getLinie()+" coloana "+((Variabila)stanga).getColoana());                 
                        this.ok=false;
                        tipStanga=-2;
                    }
                }
                //daca operatorul e egal, membrul stanga trebuie sa fie variabila, deci verificare sa fie variabila sau afisare eroare
                if(operator instanceof Egal){
                    if(stanga instanceof Variabila&&tipDreapta>=0)
                        this.m.put(((Variabila)stanga).getNume(),tipDreapta);
                    if(!(stanga instanceof Variabila)){
                        this.out.println("membrul stang nu este o variabila la linia "+operator.getLinie()+" coloana 1");
                        this.ok=false;
                    }
                    continue;
                }
                //daca tipurile nu corespund si sunt diferite de -2 deci sa nu fi afisat deja eroare pentru nedefinire
                if(tipStanga!=tipDreapta&&tipStanga!=-2&&tipDreapta!=-2) {
                    this.out.println((operator instanceof Plus?"+":"*")+" intre tipuri incompatibile la linia "+operator.getLinie()+" coloana "+operator.getColoana());
                    operator.setTip(-2);
                    this.ok=false;
                    continue;
                }
                //daca tipurile corespund modificare tip operator
                if(tipStanga==tipDreapta){
                    if(operator instanceof Plus)
                        ((Plus)operator).setTip(tipStanga);
                    if(operator instanceof Inmultire)
                        ((Plus)operator).setTip(tipStanga);
                }
            }
            //eliberare lista pentru linia urmatoare de cod
            this.l.clear();
        }
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Egal
     * @param x Egalul de vizitat
     */
    @Override
    public void visit(Egal x){
        //se adauga operatorul in lista
        this.l.add(x);
        //se itereaza prin descendenti care se adauga la lista, ca apoi sa se itereze prin descendenti si sa se faca apel acceptare vizita
        //astfel oricare 2 operanzi au ca operator corespunzator cel mai apropiat operator neprelucrat
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            this.l.add((Node)itereaza.next());
        itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Inmultire
     * @param x Inmultirea de vizitat
     */
    @Override
    public void visit(Inmultire x){
        //nu se adauga la lista fiind deja adaugat ori de egal ori de alti operatori
        //se itereaza prin descendenti care se adauga la lista, ca apoi sa se itereze prin descendenti si sa se faca apel acceptare vizita
        //astfel oricare 2 operanzi au ca operator corespunzator cel mai apropiat operator neprelucrat
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            this.l.add((Node)itereaza.next());
        itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Adunare
     * @param x Adunarea de vizitat
     */
    @Override
    public void visit(Plus x){
        //nu se adauga la lista fiind deja adaugat ori de egal ori de alti operatori
        //se itereaza prin descendenti care se adauga la lista, ca apoi sa se itereze prin descendenti si sa se faca apel acceptare vizita
        //astfel oricare 2 operanzi au ca operator corespunzator cel mai apropiat operator neprelucrat
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            this.l.add((Node)itereaza.next());
        itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Variabila
     * @param x Variabila de vizitat
     */
    @Override
    public void visit(Variabila x){
        //nu se adauga la lista fiind deja adaugat ori de egal ori de alti operatori
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru Intreg
     * @param x Intregul de vizitat
     */
    @Override
    public void visit(Intreg x){
        //nu se adauga la lista fiind deja adaugat ori de egal ori de alti operatori
    }
    /*
     * Metoda ce implementeaza metoda visit definita in interfata Visitor pentru MyBoolean
     * @param x MyBoolean-ul de vizitat
     */
    @Override
    public void visit(MyBoolean x){
        //nu se adauga la lista fiind deja adaugat ori de egal ori de alti operatori
    }
}

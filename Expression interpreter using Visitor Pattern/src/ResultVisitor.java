//utilizat pentru afisare
import java.io.PrintWriter;
//utilizate pentru un dictionar de valori a variabilelor
import java.util.Map;
import java.util.TreeMap;
//utilizat pentru iterarea prin colectii
import java.util.Iterator;
//utilizat pentru lista de operanzi de utilizand la efectuare operatii
import java.util.ArrayList;
/*
 * Clasa ce implementeaza interfata Visitor, astfel reprezentand un vizitator ce parcurge arborele de parsare si efectuand calculele obtine
 * valorile fiecarei variabile din codul sursa.
 */
public class ResultVisitor implements Visitor{
    /*
     * streamul de utilizat la afisare
     */
    private PrintWriter out=null;
    /*
     * colectia de tip map de utilizat pentru dictionar de valori
     */
    private Map<String,Object> m=null;
    /*
     * lista de operanzi utilizata la efectuare operatiilor in ordine
     */
    private ArrayList<Object> l=null;
    /*
     * Cosntructor cu parametru
     * @param out streamul de utilizat la afisare
     */
    public ResultVisitor(PrintWriter out){
        this.out=out;
        this.m=new TreeMap<String,Object>();
        this.l=new ArrayList<Object>();
    }
    /*
     * Metoda afisare rezultate finale
     */
    public void afisareRezultate(){
        for(Map.Entry<String,Object> entry:this.m.entrySet())
            this.out.println(entry.getKey()+" = "+entry.getValue());
    }
    /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe Tipul Node deci pe radacina generica
     * @param x Nodul de vizitat,prelucrat
     */
    @Override
    public void visit(Node x){
        //iterare prin descendenti si apel acceptare vizitator
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
    }
    /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe Operatorul Egal
     * @param x Egalul de vizitat,prelucrat
     */
    @Override
    public void visit(Egal x){
        //itereaza prin descendenti si apel acceptare vizitator in urma caruia ajungand in frunze deci in variabile sau valori intregi sau boolene
        //se adauga valorile acestora in lista de operanzi, iar extragand operanzii se efectueaza operatia specifica operatorului(de atribuire)
        Iterator itereaza=x.getIterator();//are 2 elemente, membru stang(Variabila) si membru drept
        Variabila mstanga=(Variabila)itereaza.next();
        ((Node)itereaza.next()).accept(this);
        this.m.put(mstanga.getNume(), this.l.remove(0));
        this.l.clear();
    }
    /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe Operatorul Inmultire
     * @param x Inmultirea de vizitat,prelucrat
     */
    @Override
    public void visit(Inmultire x){
        //itereaza prin descendenti si apel acceptare vizitator in urma caruia ajungand in frunze deci in variabile sau valori intregi sau boolene
        //se adauga valorile acestora in lista de operanzi, iar extragand operanzii se efectueaza operatia specifica operatorului(de inmultire sau si logic)
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
        Object dreapta=this.l.remove(this.l.size()-1);
        Object stanga=this.l.remove(this.l.size()-1);
        if(dreapta instanceof Integer)
            this.l.add((Integer)stanga*(Integer)dreapta);
        else
            this.l.add((Boolean)stanga&(Boolean)dreapta);
    }
     /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe Operatorul Plus
     * @param x Adunarea de vizitat,prelucrat
     */
    @Override
    public void visit(Plus x){
        //itereaza prin descendenti si apel acceptare vizitator in urma caruia ajungand in frunze deci in variabile sau valori intregi sau boolene
        //se adauga valorile acestora in lista de operanzi, iar extragand operanzii se efectueaza operatia specifica operatorului(sau logic sau adunare)
        Iterator itereaza=x.getIterator();
        while(itereaza.hasNext())
            ((Node)itereaza.next()).accept(this);
        Object dreapta=this.l.remove(this.l.size()-1);
        Object stanga=this.l.remove(this.l.size()-1);
        if(dreapta instanceof Integer)
            this.l.add((Integer)stanga+(Integer)dreapta);
        else{
            this.l.add((Boolean)stanga|(Boolean)dreapta);
        }
    }
     /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe tipul variabila
     * @param x Variabila de vizitat,prelucrat
     */
    @Override
    public void visit(Variabila x){    
        //adauga la lista de operanzi
        this.l.add(this.m.get(x.getNume()));
    }
     /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe tipul Intreg
     * @param x Intregul de vizitat,prelucrat
     */
    @Override
    public void visit(Intreg x){    
         //adauga la lista de operanzi
        this.l.add(x.getValoare());
    }
     /*
     * Metoda ce implementeaza metoda visit din interfata Visitor pe tipul MyBoolean
     * @param x MyBoolean-ul de vizitat,prelucrat
     */
    @Override
    public void visit(MyBoolean x){   
         //adauga la lista de operanzi
        this.l.add(Boolean.valueOf(x.getValoare()));
    }
}

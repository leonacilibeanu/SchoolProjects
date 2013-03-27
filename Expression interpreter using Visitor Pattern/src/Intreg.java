/*
 * Clasa tip date Intregi in arborele de parsare
 */
public class Intreg extends Node{
    /*
     * Valoarea nodului
     */
    protected int valoare;
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public Intreg(){}
    /*
     * Constructor cu parametru
     * @param valoare numarul cu care va fi initializat campul valoarea al instantei clasei
     */
    public Intreg(int valoare){
        this.valoare=valoare;
    }
    /*
     * Metoda care returneaza valoarea retinuta de Nod
     * @return valoarea retinuta de Nod
     */
    protected int getValoare(){
        return this.valoare;
    }
    /*
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra tipul de date Intreg
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

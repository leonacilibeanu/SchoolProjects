/*
 * Clasa tip date Boolene in arborele de parsare - numit MyBoolean pentru a nu ascunde wrapperul Boolean derivat din Object
 */
public class MyBoolean extends Node{
    /*
     * Valoarea Nodului
     */
    protected boolean valoare;
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public MyBoolean(){}
    /*
     * Constructor cu parametru 
     * @param valoare valoarea(adevarat sau fals) care va fi retinuta in noul Nod
     */
    public MyBoolean(boolean valoare){
        this.valoare=valoare;
    }
    /*
     * Metoda ce returneaza valoarea retinuta de Nod
     * @return valoarea retinuta de Nod
     */
    protected boolean getValoare(){
        return this.valoare;
    }
    /**
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra tipul de date MyBoolean
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

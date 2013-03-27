/*
 * Clasa ce reprezinta Tipul variabila in arborele de parsare
 */
public class Variabila extends Node{
    /*
     * Campul corespunaztor numelui variabilei
     */
    protected String nume;
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public Variabila(){}
    /**
     * Constructor cu parametrii ce utilizeaza operatorul superclasei pentru initializare linie si coloana
     * @param nume numele variabilei
     * @param linie linia corespunzatoare variabilei in codul sursa
     * @param linie coloana corespunzatoare variabilei in codul sursa
     */
    public Variabila(String nume,int linie,int coloana){
        super(linie,coloana);
        this.nume = nume;
    }
    /*
     * Metoda ce returneaza numele variabilei
     * @return numele variabilei
     */
    protected String getNume(){
        return this.nume;
    }
    /*
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra variabila
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

/*
 * Clasa Operator Egal in arbore parsare
 */
public class Egal extends Node{
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public Egal(){}
    /*
     * Constructor cu parametrii
     * @param stanga Nodul care va fi copilul stanga al noului Operator Egal(Operand stanga)
     * @param dreapta Nodul care va fi copilul dreapta al noului Operator Egal(Operand dreapta)
     * @param linie Linia de cod corespunzatoare Egalului
     * @param coloana  Coloana corespunzatoare Egalului
     */
    public Egal(Node stanga,Node dreapta,int linie,int coloana){
        //apel constructor superclasa pentru initializare linie si coloana
        super(linie,coloana);
        //adaugare operanzi in cazul in care acestia sunt definiti
        if(stanga!=null)
            super.adaugaDescendent(stanga);
        if(dreapta!=null)
            super.adaugaDescendent(dreapta);
    }
    /*
     * Metoda care returneaza operandul stanga al egalului
     * @return Nodul care reprezinta operandul stanga al egalului
     */
    protected Node getStanga(){
        //apel de get pe pozitia 0 intrucat un operator poate sa aiba doar 2 operanzi care sunt retinuti in memorie in ordinea stanga-pozitia 0 iar dreapta-pozitia 1
        return super.descendenti.get(0);
    }
    /*
     * Metoda care returneaza operandul dreapta al egalului
     * @return Nodul care reprezinta operandul dreapta al egalului
     */
    protected Node getDreapta(){
        //apel de get pe pozitia 0 intrucat un operator poate sa aiba doar 2 operanzi care sunt retinuti in memorie in ordinea stanga-pozitia 0 iar dreapta-pozitia 1
        return super.descendenti.get(1);
    }
    /*
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra operatorul egal
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

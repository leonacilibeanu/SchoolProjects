/*
 * Clasa Operator inmultire in arbore parsare
 */
public class Inmultire extends Node{
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public Inmultire(){}
    /*
     * Constructor cu parametrii
     * @param stanga Nodul care va fi copilul stanga al noului Operator Inmultire(Operand stanga)
     * @param dreapta Nodul care va fi copilul dreapta al noului Operator Inmultire(Operand dreapta)
     * @param linie Linia de cod corespunzatoare Inmultirii
     * @param coloana  Coloana corespunzatoare Inmultirii
     */
    public Inmultire(Node stanga,Node dreapta,int linie,int coloana){
        super(linie,coloana);
        if(stanga!=null)
            super.adaugaDescendent(stanga);
        if(dreapta!=null)
            super.adaugaDescendent(dreapta);
    }
    /*
     * Metoda care returneaza operandul stanga al inmultirii
     * @return Nodul care reprezinta operandul stanga al inmultirii
     */
    protected Node getStanga(){
        //apel de get pe pozitia 0 intrucat un operator poate sa aiba doar 2 operanzi care sunt retinuti in memorie in ordinea stanga-pozitia 0 iar dreapta-pozitia 1
        return super.descendenti.get(0);
    }
    /*
     * Metoda care returneaza operandul dreapta al inmultirii
     * @return Nodul care reprezinta operandul dreapta al inmultirii
     */
    protected Node getDreapta(){
        //apel de get pe pozitia 0 intrucat un operator poate sa aiba doar 2 operanzi care sunt retinuti in memorie in ordinea stanga-pozitia 0 iar dreapta-pozitia 1
        return super.descendenti.get(1);
    }
    /*
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra operatorul inmultire
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

/*
 * Clasa Operator Plus in arbore parsare
 */
public class Plus extends Node{
    /*
     * Constructor fara parametrii. Apeleaza super();
     */
    public Plus(){}
    /*
     * Constructor cu parametrii ce initializeaza linia coloana utilizand super(linie,coloana), si adauga operanzi stanga respectiv dreapta in
     * cazul in care acestia nu sunt nuli
     * @param stanga Nod de adaugat drept copil stanga(Operand stanga)
     * @param dreapta Nod de adaugat drept copil dreapta(Operand dreapta)
     * @param linie linia corespunzatoare nodului curent in codul sursa
     * @param coloana coloana corespunzatoare nodului curent in codul sursa
     */
    public Plus(Node stanga,Node dreapta,int linie,int coloana){
        //initializare linie si coloana utilizand constructor superclasa
        super(linie,coloana);
        //adaugare copii in cazul in care nu sunt nuli
        if(stanga!=null)
            super.adaugaDescendent(stanga);
        if(dreapta!=null)
            super.adaugaDescendent(dreapta);
    }
    /*
     * Metoda ce returneaza copilul stanga al operatorului plus
     * @return copilul stanga al operatorului(Operandul stanga)
     */
    protected Node getStanga(){
        return super.descendenti.get(0);
    }
    /*
     * Metoda ce returneaza copilul dreapta al operatorului plus
     * @return copilul dreapta al operatorului(Operandul dreapta)
     */
    protected Node getDreapta(){
        return super.descendenti.get(1);
    }
    /**
     * Suprascriere metoda accept din superclasa care implementeaza interfata visitor. Aceasta permite utiizarea patternului Visitor.
     * @param v Referinta catre Vizitatorul care urmeaza a prelucra operatorul Plus
     */
    @Override
    public void accept(Visitor v){
        v.visit(this);
    }
}

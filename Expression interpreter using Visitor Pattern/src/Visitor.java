/*
 * Interfata ce defineste forma unui vizitator
 */
public interface Visitor{
    /*
     * Metoda supraincarcata de vizitare radacina generica
     * @param x radacina generica de vizitat
     */
    public void visit(Node x);
    /*
     * Metoda supraincarcata de vizitare egal
     * @param x egalul de vizitat
     */
    public void visit(Egal x);
     /*
     * Metoda supraincarcata de vizitare inmultire
     * @param x inmultirea de vizitat
     */
    public void visit(Inmultire x);
    /*
     * Metoda supraincarcata de vizitare adunare
     * @param x adunarea de vizitat
     */
    public void visit(Plus x);
   /*
     * Metoda supraincarcata de vizitare variabila
     * @param x variabila de vizitat
     */
    public void visit(Variabila x);
     /*
     * Metoda supraincarcata de vizitare intreg
     * @param x intregul de vizitat
     */
    public void visit(Intreg x);
    /*
     * Metoda supraincarcata de vizitare MyBoolean
     * @param x MyBoolean-ul de vizitat
     */
    public void visit(MyBoolean x);
}

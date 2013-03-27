/*
 * Interfata ce determina forma unei clase ce poate fi vizitata
 */
public interface Visitable{
    /*
     * Metoda de acceptare vizitator v. Se utilizeaza pentru a transmite decizia la supraincarcare la runtime.
     */
    public void accept(Visitor v);
}

Pentru a afla tipul recursivitatii unei functii date am evaluat corpul acesteia in functie de varianta conditionala pe care o apeleaza.
Pentru inceput am retinut numele functiei in variabila name, corpul functiei in variabila body si am initializat "gradul de recursivitate"=nr_recursions cu 0 (echivalent cu functie nerecursiva). Se verifica de la inceput daca apare numele functiei in corpul acesteia. Daca nu, se returneaza mesajul si se iese din functie. 
Daca exista, se evalueaza pentru fiecare tip de conditie: if, cond cu ajutorul functiilor evaluate-if, evaluate-cond si se actualizeaza nr_recursions. Daca elementul analizat din lista nu apartine {if,cond} atunci se evalueaza cu ajutorul functiei evaluate-if care il trateaza ca pe o consecinta a lui if.


Functia evaluate-if poate verifica atat consecintele de pe ramurile then/else ale lui if, cat si consecintele de pe fiecare ramura a lui cond sau varianta neconditionata de reapelare a unei functii.
Evaluate-if, evalueaza in iter1 primul element din lista primita drept parametru si in functie de conditia indeplinita se reapeleaza pentru o alta structura conditionala (if, cond), apeleaza iter2 daca este prima consecinta si iter3 daca este a doua consecinta.
In cazul in care elementul verificat din lista nu este la randul lui o lista, este ignorat (cu siguranta functia nu se reapeleaza pe acea ramura).
Atat in iter2, cat si in iter3 se pastreaza nr de aparitii ale numelui functiei pe ramura in nr, respectiv nrr. In cazul in care numele functiei nu este primul element din lista, automat functia este un parametru pentru o alta functie care o apeleaza, crescand astfel "gradul de recursivitate" cu 1. 
In plus fata de iter2, iter3 verifica daca nr de aparitii de pe a doua ramura e mai mare decat "gradul de recursivitate" obtinut anterior. In caz afirmativ se pastreaza aceasta valoare si se incrementeaza doar daca indeplineste aceeasi conditie ca la iter2.
Se pastreaza acest maxim de pe fiecare ramura cu scopul de a determina tipul recursivitatii functiei in cazul cel mai nefavorabil, indiferent daca ea poate fi nerecursiva intr-un caz particular.

Functia evaluate-cond verifica ,folosindu-se de evaluate-if, nr de aparitii si tipul de apelare al functiei pentru fiecare din ramurile lui cond, pastrandu-se maximul din aceleasi conditii explicate mai sus.

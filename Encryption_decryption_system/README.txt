Pentru implementarea codificatorului:
 - am citit de la consola(intrarea standard) textul linie cu linie, folosindu-ma de clasa Scanner;
 - dupa citire am determinat si memorat frecventa fiecarui caracter din text; 
 - folosindu-ma de aceste frecvente am construit arborele Huffman(arbore binar) (cu clasele: ArboreHuffman
FrunzaHuffman, NodHuffman):
      -pentru fiecare caracter am creat un nod frunza (contine cod ASCII, frecventa) si
      l-am adaugat intr-o coada de prioritati;
      -cat timp coada nu este vida
           - scot doua noduri cu prioritate maxima ( cu frecventa de aparitie cea mai mica);
           - creez un nod intern din aceste doua noduri(cu frecventa suma frecventelor 
             celor doua noduri);
           - adaug nodul creat in coada;
       -nodul ultim ramas este radacina arborelui binar 
       
  - am parcurs arborele si am afisat informatiile din frunzele sale(nodurile terminale) si am 
    calculat pentru fiecare frunza <secventa_biti_codare> (in sensul ca, prin parcurgerea arborelui,
    ca am adaugat 0 la parcurgerea unui fiu stang si  1 la parcurgerea unui fiu drept pana am ajuns la frunza)
    iar din frunza am luat  <codul_ASCII> ;
  - am memorat codul ASCII si secventa biti codare in doi vectori;
  -am afisat textul codat. 

Pentru implementarea decodificatorului:
 - am citit de la consola(intrarea standard), folosindu-ma de clasa Scanner si am memorat
    codul ASCII al fiecarui caracter din text si <secventa_biti_codare> apoi am citit textul codat;
 - am construit o metoda  care decodifica secventa de biti si care apoi afiseaza textul decodat,
identificand caracter cu caracter textul in clar.
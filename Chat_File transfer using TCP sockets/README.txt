Am ales un format al mesajelor care sa fie cat se poate de uniform si sa lucreze in mod natural cu tipurile de date
din C.

Serverul central
------------------

Mesajele primite de catre serverul central au urmatorul format:
--------------------------------------------------------------
| marime_mesaj (2B) | tipul_cererii (1B) | date (0..65533 B) |
--------------------------------------------------------------

Campul marime_mesaj este codat pe 2 byti si corespunde marimii intregului mesaj, pentru a putea sti cand
mesajul a fost primit in intregime.
Tipul cererii este codat pe 1 byte, oferind o plaja suficient de mare pentru aplicatia curenta.
Datele depind de tipul mesajului.

In functie de tipul mesajului, datele au urmatoarea structura:

- pentru mesaj de tipul REQUEST_TYPE_SET_INFO, datele au urmatoarea structura:
----------------------------------------------
| port (2B) | numele (terminat cu octetul 0) |
----------------------------------------------

- pentru mesaj de tipul REQUEST_TYPE_LISTCLIENTS si REQUEST_TYPE_DISCONNECT, datele lipsesc, fiind suficienta
informatia continuta in octetul care indica tipul mesajului

- pentru mesaj REQUEST_TYPE_INFOCLIENT si REQUEST_TYPE_GETSHARE, datele au urmatoarea structura:
---------------------------------------
| nume_client (terminat cu octetul 0) |
---------------------------------------

- pentru mesaj de tipul REQUEST_TYPE_INFOCLIENT_SILENT structura este aceasi ca si pentru REQUEST_TYPE_INFOCLIENT

- pentru mesaj de tipul REQUEST_TYPE_SHARE si REQUEST_TYPE_UNSHARE, datele au urmatoarea structura:
---------------------------------------
| nume_fisier (terminat cu octetul 0) |
---------------------------------------

Raspunsurile serverului central au urmatoarea structura:
------------------------------------------------------------------
| marime_mesaj (2B) | tipul_raspunsului (1B) | date (0..65533 B) |
------------------------------------------------------------------

In functie de tipul raspunsului, datele pot avea urmatoarea structura:
- pentru raspunsuri de tipul RESPONSE_NAME_EXISTS, RESPONSE_SUCCESS datele au dimensiune 0
- pentru raspunsuri de tipul RESPONSE_LISTCLIENTS si RESPONSE_GETSHARE, datele sunt compuse din
siruri de caractere, iar elementele sunt separate prin caracterul nul:
----------------------------------------
|string1 |0| string2 |0| ... stringN |0|
----------------------------------------
Am ales acest format pentru a putea incapsula si decapsula datele cu usurinta tinand cont de faptul ca
in C se utilizeaza cel mai adesea siruri de caractere terminate prin caracterul nul.

- pentru raspunsuri de tipul RESPONSE_CLIENTINFO datele au urmatorul format:
----------------------------------------------------------------------------------------------
| portul_pe_care_asculta (2B) | de_cat_timp_e_conectat (4B) | numele (terminat cu octetul 0) |
----------------------------------------------------------------------------------------------

- pentru raspunsuri de tipul  RESPONSE_CLIENTINFO_SILENT datele au urmatorul format:
--------------------------------------------------------------------------------------------------------------
| portul_pe_care_asculta (2B) | de_cat_timp_e_conectat (4B) | numele (terminat '\0') | ip (terminat cu '\0') |
--------------------------------------------------------------------------------------------------------------

Am utilizat 2 bytes pentru port acoperind astfel toata plaja de valori posibile.
Pentru a stoca timpul scurs de la conectare am utilizat 4 octeti pentru a acoperi toate situatiile (tinand cont de
faptul ca timestamp-ul pe sistemele UNIX clasice este pe 32 de biti).
Numele si ip-ul le-am stocat ca siruri de caractere terminate cu '\0' pentru a usura incapsularea si decapsularea
datelor.

Clientul
---------
Clientul se utilizeaza astfel:
./client nume_client ip_server port_server [local_port]

Aplicatia client utilizeaza mesaje care au urmatorul format:
--------------------------------------------------------------
| marime_mesaj (2B) | tipul_cererii (1B) | date (0..65533 B) |
--------------------------------------------------------------

In functie de tipul cererii, structura datelor variaza.
- pentru mesaje de tipul REQUEST_TYPE_MESSAGE, datele au urmatoarea structura:
-----------------------------------------------------------------
|nume_client_sursa (terminat cu '\0') | mesaj (terminat cu '\0')|
-----------------------------------------------------------------

- pentru mesaje de tipul REQUEST_TYPE_FILE_WRITE_DATA, datele au urmatoarea structura:
-------------------
|date (1 .. 1024B)|
-------------------
Datele continute in acest tip de mesaje sunt de fapt continutul fisierelor transferate.

- pentru mesaje de tipul REQUEST_TYPE_EOF si REQUEST_TYPE_ABORT_UNABLE_TO_OPEN_FILE, datele au dimensiune nula,
deoarece acestea sunt notificari si este suficienta informatia continuta in octetul care indica tipul measjului.

- pentru mesaje de tipul REQUEST_TYPE_START_TRANSFER, datele au urmatoarea structura:
--------------------------------
|nume_fisier (terminat cu '\0')|
--------------------------------

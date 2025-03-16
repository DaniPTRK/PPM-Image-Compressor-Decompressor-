Descriere implementare - tema2.c
    Programul pe care l-am realizat conține 4 structuri diferite:
- struct colors care reprezintă conținutul unei celule din grid. Practic, în
această structură păstrez într-un vector rgb[] valorile din cele 3 canale
de culoare. Astfel, rgb[0] este RED, rgb[1] este GREEN, rgb[2] este BLUE.
- struct nod reprezintă conținutul unui nod din arborele cuaternar, așa
cum este prezentat în cerință. Astfel, salvez tipul nodului și, dacă este
un nod frunză, valorile din cele 3 canale.
- struct cnod reprezintă conținutul unui nod din coada pe care o folosesc
în parcurgerile de tip BFS. Fiecare nod conține un nod din arborele cuaternar.
- struct coada unde salvez începutul și capătul cozii.
    Inițial, verific ce cerință trebuie rezolvată păstrând opțiunile date în
linia de comandă într-o matrice de char-uri attr[3][4]. În funcție de cerința
citită, realizez citirea datelor din fișierul oferit ca input. Pentru
cerința 1 și 2, voi citi din fișierul PPM informațiile necesare pentru a
construi arborele cuaternar, iar pentru cerința 3 voi citi din fișierul
comprimat informații necesare pentru a construi arborele cuaternar folosind
metoda asemănătoare BFS.
    Pentru cerințele 1 și 2, mă folosesc de funcția read_PPM() pentru a realiza
matricea de culori grid[][] pe care o voi împărți, dacă rădăcina nu este nod
frunză, în funcția build_the_tree() cu scopul realizării arborelui cuaternar.
    Pentru a verifica dacă rădăcina este sau nu nod frunză, mă folosesc de
funcția check_if_color(), o funcție în care calculez media din întreaga
matrice de culori și calculez scorul similarității. Dacă acest scor este
mai mic decât pragul oferit, atunci avem un nod frunză, un nod de tip 1,
deci nu are sens să mai împărțim matricea în alte blocuri. În caz contrar,
avem un nod intern și este necesară împărțirea matricii, deci se va
executa build_the_tree().
    Construcția acestui arbore este realizată recursiv, bazându-mă pe pragul
oferit și pe scorul similarității din interiorul blocurilor. Astfel,
funcția build_the_tree() primește un pointer la adresa rădăcinii arborelui,
matricea de culori care reprezintă practic imaginea și alte 4 valori întregi
care desemnează, practic, colțul din stânga sus și cel din dreapta jos
al blocului pe care eu îl investighez. Deci, matricea pe care o verific
pornește de la (starti,startj) și se oprește la (sizei,sizej). După ce
calculez media în interiorul blocului în care mă aflu folosindu-mă de
un vector med[] pentru a salva valorile și de funcția calculate_median()
pentru a obține valorile efective, și calculez scorul similarității
în calculate_mean(), verific dacă mean-ul este mai mic decât factorul dat,
iar în caz contrar, mă îndrept spre blocul în care mean-ul este mai mare
decât factorul apelând, din nou, build_the_tree(). Se poate observa faptul că
matricea pe care o investighez este împărțită de la bun început în alte 4
blocuri, astfel că eu investighez, practic, componentele sale, nu matricea
în sine, dată ca input în funcție. Atunci când mean-ul este mai mare decât
factorul, eu apelez funcția și îmi schimb punctele extreme ce delimitează 
matricea, împărțind la 4, practic, aria pe care o voi analiza. sizei-starti
și sizej-startj îmi sugerează laturile acestei arii. Dacă găsesc un mean
mai mic decât factorul, inițializez fiul nodului în care mă aflu corespondent
ariei în care am găsit meanul, folosindu-mă de poz[4], și trec valorile medii
în nodul respectiv. Nodurile interne sunt realizate de la bun început când
apelez funcția build_the_tree() pentru că dacă funcția a fost apelată, nodul
în care mă aflu nu este un nod frunză.
    Dacă mă aflu în cazul cerinței 1, folosesc o parcurgere DFS pentru a obține
numărul de blocuri de culori și cea mai mare latură a unui bloc de culoare
verificând tipul nodului în care mă aflu (tipul trebuie să fie 1). Nivelul
îl calculez ca pe un maxim, în sensul că mă folosesc de un contor ce îmi indică
nivelul unde mă aflu și, dacă acest nivel este mai mare decât numărul de nivele
actual, trec acest contor în variabila de niveluri "lvls".1
    Dacă mă aflu în cazul cerinței 2, mai întâi trec dimensiunea imaginii
(aceeași cu cea citită din PPM) în fișierul comprimat și, după aceea, fac p
parcurgere BFS pentru a scrie, pe niveluri, conținutul arborelui în fișierul
binar. În parcurgere_BFS() mă folosesc de o coadă pentru a trece nodurile
de pe următorul nivel și pentru a elimina nodurile din nivelul actual după ce
le trec conținutul în fișierul binar.
    În cazul cerinței 3, citirea diferă, în sensul că de data aceasta citesc
dintr-un fișier comprimat. Astfel, voi folosi mai întâi funcția
binary_build_the_tree() unde, folosind un fel de parcurgere BFS, voi construi
arborele cuaternar. De data aceasta, coada pe care o folosesc va ține minte
de pe nivelul următor doar acele noduri care sunt interne, nu și cele frunză,
întrucât nodurile frunză nu mai au descendenți și, deci, următoarele noduri
de pe următoarea linie vor fi fiii nodurilor interne. Indiferent de tip, trec
valorile citite din fișierul binar folosindu-mă de init_nod(). După realizarea
arborelui, realizez matricea în build_the_grid(), recursiv, într-un mod similar
cu cel folosit în build_the_tree(), astfel că funcția primește matricea de culori,
arborele și 4 valori care, la fel ca în funcția de obținere a arborelui, reprezintă
colțul din stânga sus și colțul din dreapta jos a matricii pe care o asamblez.
Similitudinea dintre cele două funcții răsare din modul în care folosesc
recursivitatea, întrucât modul de găsire a matricii pe care urmează să o
asamblez "mimează" modul de găsire a blocului care are scorul de similaritatea
mai mic decât factorul dat. Practic, folosindu-mă de 2 vectori phasei[] și
phasej[], recreez cele 2 for-uri din build_the_tree(), iar apelările recursive la
build_the_grid() urmează aceeași formulă ca cele din build_the_tree(), cu
mici diferențe. Astfel, cu cât mă adâncesc mai mult în arbore, cu atât "sparg"
matricea în bucăți mai mici și merg spre bucata corespondentă nodului din arbore
în care mă aflu, până găsesc o frunză ce-mi dovedește faptul că matricea pe care
mă aflu este un bloc cu culoarea aflată în informația nodului. Găsind, deci,
un nod de tip 1, umplu matricea cu valoarea de roșu, verde și albastru din nod,
iar dacă găsesc un nod de tip 0, sparg matricea. După realizarea matricii,
practic, a imaginii comprimate, trec valorile ce se află în matrice în fișierul
ppm pe care îl realizez, folosindu-mă de write_PPM().
    În final, eliberez memoria, folosindu-mă, de asemenea, de o funcție
cut_the_tree() unde dau free nodurilor din arbore folosind o parcurgere DFS.

Punctaj obținut pe checker-ul local:

Cerinta 1 : 20
Cerinta 2 : 30.0
Cerinta 3 : 30.0
Total     : 80.0
Bonus     : 20.0
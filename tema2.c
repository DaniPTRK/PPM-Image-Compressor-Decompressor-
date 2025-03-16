/* ION Daniel - 315CC*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct colors
{
    //Conținutul unei celule din matricea de culori.
    unsigned char rgb[3];
}Colors;

typedef struct nod
{
    //Structură folosită pentru arborele cuaternar.
    unsigned char tipul_nodului;
    unsigned char valoare_pentru_rosu, valoare_pentru_verde, valoare_pentru_albastru;
    struct nod *v[4];
}Nod, *Cuater_Arb; 

typedef struct cnod
{
    //Structură folosită pentru coada de noduri din arborele cuaternar.
    Cuater_Arb info;
    struct cnod* next;
}Cnod, *Lista;

typedef struct coada
{
    //Structura efectivă a cozii.
    Lista start, end;
}Coada;

void enqueue(Coada *l, Cuater_Arb x)
{
    //Adaug în coadă nodul din arbore.
    Lista nou;
    nou=(Lista)malloc(sizeof(Cnod));
    nou->info=x;
    nou->next=NULL;
    if(l->start==NULL)
    {
        l->start=nou;
        l->end=nou;
    }
    else
    {
        l->end->next=nou;
        l->end=l->end->next;
    }
}

void dequeue(Coada *l)
{
    //Scot din coadă nodul din arbore.
    if(l->start!=NULL)
    {
        Lista s=l->start;
        l->start=l->start->next;
        free(s);
    }
}

void init_nod(Cuater_Arb *nou, unsigned char tip, unsigned char r, unsigned char g, unsigned char b)
{
    //Inițializez un nod din arbore.
    int i;
    (*nou)=(Cuater_Arb)malloc(sizeof(Nod));
    (*nou)->tipul_nodului=tip;
    (*nou)->valoare_pentru_rosu=r;
    (*nou)->valoare_pentru_verde=g;
    (*nou)->valoare_pentru_albastru=b;
    for(i=0; i<4; i++)
    {
        (*nou)->v[i]=NULL;
    }
}

void cut_the_tree(Cuater_Arb l)
{
    //Dau free nodurilor din arbore.
    int i;
    for(i=0; i<4; i++)
    {
        if(l->v[i]!=NULL)
        {
            cut_the_tree(l->v[i]);
        }
    }
    free(l);
}

unsigned char calculate_median(Colors **grid, int starti, int startj, int sizei, int sizej, int choice)
{
    /*Calculez media valorilor din submatricea delimitată de punctele (starti,startj) și (sizei,sizej).
    "choice" reprezintă canalul de culoare pe care calculez media: 0 = R, 1 = G, 2 = B.*/
    int i, j;
    unsigned long long int sum=0;
    unsigned char color;
    for(i=starti; i<sizei; i++)
    {
        for(j=startj; j<sizej; j++)
        {
            sum+=grid[i][j].rgb[choice];
        }
    }
    color=(int)sum/((sizei-starti)*(sizei-starti));
    return color;
}

unsigned long long int calculate_mean(Colors **grid, int starti, int startj, int sizei, int sizej, unsigned char *med)
{
    //Calculez scorul similarității în funcție de mediile din cele 3 canale de culoare.
    int i, j, k;
    unsigned long long int mean;
    unsigned long long int sum=0;
    for(i=starti; i<sizei; i++)
    {
        for(j=startj; j<sizej; j++)
        {
            for(k=0; k<3; k++)
            {
                sum+=(med[k]-grid[i][j].rgb[k])*(med[k]-grid[i][j].rgb[k]);
            }
        }
    }
    mean=sum/(3*(sizei-starti)*(sizei-starti));
    return mean;
}

int check_if_color(Cuater_Arb *l, Colors **grid, int prag, int size)
{
    //Verific dacă rădăcina este de tip 1.
    unsigned long long int mean;
    unsigned char med[3];
    med[0]=calculate_median(grid,0,0,size,size,0);
    med[1]=calculate_median(grid,0,0,size,size,1);
    med[2]=calculate_median(grid,0,0,size,size,2);
    mean=calculate_mean(grid,0,0,size,size,med);
    if(mean<=prag)
    {
        //Avem tip 1.
        init_nod(l,1,med[0],med[1],med[2]);
        return 1;
    }
    //Avem nod intern.
    return 0;
}

void build_the_tree(Cuater_Arb *l, Colors **grid, int prag, int starti, int startj, int sizei, int sizej)
{
    //Construcția arborelui cuaternar cu ajutorul matricii de culori grid.
    int i, j, cont, poz[4]={0,1,3,2};
    unsigned long long int mean;
    unsigned char med[3];
    init_nod(l,0,0,0,0);
    cont=0;
    for(i=starti; i<sizei; i=i+(sizei-starti)/2)
    {
        for(j=startj; j<sizej; j=j+(sizej-startj)/2)
        {
            /*Împart submatricea de la (starti,startj) la (sizei,sizej) în 4 matrici.
            Pentru fiecare matrice calculez media și scorul similiarității*/
            med[0]=calculate_median(grid,i,j,i+(sizei-starti)/2,j+(sizej-startj)/2,0);
            med[1]=calculate_median(grid,i,j,i+(sizei-starti)/2,j+(sizej-startj)/2,1);
            med[2]=calculate_median(grid,i,j,i+(sizei-starti)/2,j+(sizej-startj)/2,2);
            mean=calculate_mean(grid,i,j,i+(sizei-starti)/2,j+(sizej-startj)/2,med);
            if(mean<=prag)
            {
                //Similaritatea nu depășește pragul impus. Trecem nodul frunză.
                init_nod(&((*l)->v[poz[cont]]),1,med[0],med[1],med[2]);
            }
            else
            {
                //Similaritatea depășește pragul. Creăm un nou nod intern.
                build_the_tree(&((*l)->v[poz[cont]]),grid,prag,i,j,i+(sizei-starti)/2,j+(sizej-startj)/2);
            }
            cont++;
        }
    }
}

void binary_build_the_tree(FILE *f, Cuater_Arb *l)
{
    //Construcția arborelui cuaternar cu ajutorul fișierului comprimat.
    //Inițializez o coadă unde voi salva doar nodurile interne.
    Coada q;
    q.start=NULL;
    unsigned char aux, tip, r, g, b;
    int i;
    fread(&tip,sizeof(unsigned char),1,f);
    if(tip==1)
    {
        /*Dacă primul nod citit din fișierul binar este de tip 1, înseamnă că
        avem doar o culoare în toată matricea, deci nu are sens să construim coada 
        de noduri interne.*/
        fread(&r,sizeof(unsigned char),1,f);
        fread(&g,sizeof(unsigned char),1,f);
        fread(&b,sizeof(unsigned char),1,f);
        init_nod(l,tip,r,g,b);
    }
    else
    {
        //Trec rădăcina în coadă.
        init_nod(l,0,0,0,0);
        enqueue(&q,*l);
        while(q.start!=NULL)
        {
            for(i=0; i<4; i++)
            {
                fread(&tip,sizeof(unsigned char),1,f);
                if(tip==0)
                {
                    //Avem nod intern. Îl trec în arbore și în coadă.
                    init_nod(&(q.start->info->v[i]),0,0,0,0);
                    enqueue(&q,q.start->info->v[i]);
                }
                else
                {
                    //Avem nod frunză. Citesc RGB și trec nodul doar în arbore.
                    fread(&r,sizeof(unsigned char),1,f);
                    fread(&g,sizeof(unsigned char),1,f);
                    fread(&b,sizeof(unsigned char),1,f);
                    init_nod(&(q.start->info->v[i]),tip,r,g,b);
                }
            }
            dequeue(&q);
        }
    }
}

void build_the_grid(Colors **grid, Cuater_Arb l, int starti, int startj, int sizei, int sizej)
{
    //Asamblarea matricii de culori folosind arborele cuaternar.
    int i, j, startiaux, startjaux, phasei[4]={0,0,1,1}, phasej[4]={0,1,1,0};
    if(l->tipul_nodului==0)
    {
        /*Dacă am nod intern, atunci înseamnă că matricea desemnată de nodul pe care îl am este
        împărțită în alte 4 matrici mai mici.*/
        for(i=0; i<4; i++)
        {
            /*Calculez punctul din stânga sus și cel din dreapta jos asemănător cu cum am
            calculat în build_the_tree().*/
            startiaux=starti+phasei[i]*(sizei-starti)/2;
            startjaux=startj+phasej[i]*(sizej-startj)/2;
            build_the_grid(grid,l->v[i],startiaux,startjaux,startiaux+(sizei-starti)/2,startjaux+(sizej-startj)/2);
        }
    }
    else
    {
        //Dacă am nod frunză, atunci trec în spațiul din matricea de culori culoarea din nod.
        for(i=starti; i<sizei; i++)
        {
            for(j=startj; j<sizej; j++)
            {
                grid[i][j].rgb[0]=l->valoare_pentru_rosu;
                grid[i][j].rgb[1]=l->valoare_pentru_verde;
                grid[i][j].rgb[2]=l->valoare_pentru_albastru;
            }
        }
    }
}

void parcurgere_DFS(Cuater_Arb l, int *lvls, int *blocks, int *maxbl, int n, int count)
{
    //Parcurgere DFS pentru rezolvarea cerinței 1.
    int i;
    for(i=0; i<4; i++)
    {
        if(l->v[i]!=NULL)
        {
            parcurgere_DFS(l->v[i],lvls,blocks,maxbl,n/2,count+1);
        }
    }
    //count înseamnă nivelul la care mă aflu + 1. Nivelul maxim este numărul de niveluri din arbore.
    if(count>(*lvls))
    {
        *lvls=count;
    }
    if(l->tipul_nodului==1)
    {
        //Am nod frunză. Adun la numărul de blocuri de culori.
        (*blocks)++;
        //Verific dacă nodul frunză are lungimea laturii pătratului pe care îl reprezintă maximă.
        if(n>(*maxbl))
        {
            *maxbl=n;
        }
    }
}

void parcurgere_BFS(FILE *g, Cuater_Arb l)
{
    //Parcurgere pe nivel a arborelui cuaternar în scopul realizării fișierului comprimat.
    int i;
    Coada q;
    q.start=NULL;
    enqueue(&q,l);
    while(q.start!=NULL)
    {
        for(i=0; i<4; i++)
        {
            //Trec în coadă toți descendenții.
            if(q.start->info->v[i]!=NULL)
            {
                enqueue(&q,q.start->info->v[i]);
            }
        }
        if(q.start!=NULL)
        {
            //Trec în fișierul comprimat tipul nodului.
            fwrite(&(q.start->info->tipul_nodului),sizeof(unsigned char),1,g);
            if(q.start->info->tipul_nodului!=0)
            {
                //Pentru nod frunză, trec valorile de pe cele 3 canale de culoare.
                fwrite(&(q.start->info->valoare_pentru_rosu),sizeof(unsigned char),1,g);
                fwrite(&(q.start->info->valoare_pentru_verde),sizeof(unsigned char),1,g);
                fwrite(&(q.start->info->valoare_pentru_albastru),sizeof(unsigned char),1,g);
            }
        }
        dequeue(&q);
    }
}

void read_PPM(FILE *f, int n, Colors **grid)
{
    //Citesc din .ppm matricea de culori.
    int i,j,k;
    fgetc(f);
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            for(k=0; k<3; k++)
            {
                fread(&(grid[i][j].rgb[k]),sizeof(unsigned char),1,f);
            }
        }
    }
}

void write_PPM(FILE *g, Colors **grid, int n)
{
    //Scriu în .ppm matricea de culori.
    int i,j,k;
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            for(k=0; k<3; k++)
            {
                fwrite(&(grid[i][j].rgb[k]),sizeof(unsigned char),1,g);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    //Câteva inițializări.
    FILE *f=fopen(argv[argc-2],"rb");
    FILE *g=fopen(argv[argc-1], "wb");
    char attr[3][4]={"-c1", "-c2", "-d"}, type[3];
    int i, width, height, maxval;
    Cuater_Arb l;
    Colors **grid;
    for(i=0; i<3; i++)
    {
        //Verific ce cerință trebuie rezolvată.
        if(strcmp(argv[1],attr[i])==0)
        {
            switch(i)
            {
                case 0:
                {
                    //Avem cerința 1.
                    int factor, lvls=0, blocks=0, maxbl=-1, count=1;
                    factor=atoi(argv[2]);
                    //Citesc din PPM.
                    fscanf(f, "%s", type);
                    fscanf(f, "%d%d", &width, &height);
                    fscanf(f, "%d", &maxval);
                    grid=(Colors**)malloc(height*sizeof(Colors*));
                    for(i=0; i<height; i++)
                    {
                        grid[i]=(Colors *)malloc(width*sizeof(Colors));
                    }
                    read_PPM(f,width,grid);
                    //Verific dacă rădăcina este nod de tip 1.
                    if(check_if_color(&l,grid,factor,height)==0)
                    {
                        //Construiesc arborele.
                        build_the_tree(&l,grid,factor,0,0,height,width);
                    }
                    //Folosesc DFS și afișez rezultatele cerute.
                    parcurgere_DFS(l,&lvls,&blocks,&maxbl,height,count);
                    fprintf(g,"%d\n%d\n%d\n", lvls, blocks, maxbl);
                    break;
                }
                case 1:
                {
                    //Avem cerința 2.
                    unsigned int dimensiune_imagine;
                    int factor;
                    factor=atoi(argv[2]);
                    //Citesc din PPM.
                    fscanf(f, "%s", type);
                    fscanf(f, "%d%d", &width, &height);
                    fscanf(f, "%d", &maxval);
                    grid=(Colors**)malloc(height*sizeof(Colors*));
                    for(i=0; i<height; i++)
                    {
                        grid[i]=(Colors *)malloc(width*sizeof(Colors));
                    }
                    read_PPM(f,width,grid);
                    //Verific dacă rădăcina este nod de tip 1.
                    if(check_if_color(&l,grid,factor,height)==0)
                    {
                        //Construiesc arborele.
                        build_the_tree(&l,grid,factor,0,0,height,width);
                    }
                    dimensiune_imagine=width;
                    fwrite(&dimensiune_imagine,sizeof(unsigned int), 1, g);
                    //Folosesc BFS și trec nodurile pe linie în fișierul comprimat.
                    parcurgere_BFS(g,l);
                    break;
                }
                case 2:
                {
                    //Avem cerința 3.
                    fread(&width,sizeof(int),1,f);
                    grid=(Colors**)malloc(width*sizeof(Colors*));
                    for(i=0; i<width; i++)
                    {
                        grid[i]=(Colors *)malloc(width*sizeof(Colors));
                    }
                    //Construiesc arborele folosind fișierul binar.
                    binary_build_the_tree(f,&l);
                    //Realizez decomprimarea - construiesc PPM-ul.
                    fprintf(g,"P6\n");
                    fprintf(g,"%d %d\n",width,width);
                    fprintf(g, "255\n");
                    build_the_grid(grid,l,0,0,width,width);
                    write_PPM(g,grid,width);
                    break;
                }
            }
        }
    }
    //Eliberez memoria alocată și închid fișierele.
    for(i=0; i<width; i++)
    {
        free(grid[i]);
    }
    free(grid);
    cut_the_tree(l);
    fclose(f);
    fclose(g);
    return 0;
}
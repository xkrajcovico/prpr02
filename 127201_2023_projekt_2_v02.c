#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

struct ID { // struktura id, splitnuta lebo zadanie
    char first;
    int second;
    char third;
};

struct Position { // struktura pozicia, splitnuta lebo zadanie
    double longitude;
    double latitude;
};

struct Node { // definujem nod, v ktorom su data
    struct ID id;
    struct Position position;
    char typ[3];
    double hodnota;
    char cas[5];
    char datum[9];
    struct Node* prev;
    struct Node* next;
};

struct DATALIST { //pointer na head a tail linkedlistu
    struct Node* head;
    struct Node* tail;
};

void trim(char *str) { //podfunkcia rusi whitespace
    size_t length = strlen(str);    //pomocna premenna dlzka stringu
    if (length > 0 && str[length - 1] == '\n') { //ide v stringu po enter
        str[length - 1] = '\0';      //zmaze enter, nahradi koncom /o
    }
}

int porovnaj_nody(struct Node* node1, struct Node* node2) { //podfunkcia pre u
    int porovnaj_datum = strcmp(node1->datum, node2->datum); //uloz si ktory datum je vacsi
    
    if (porovnaj_datum != 0) {      //ked su rovnake returnuj 0
        return porovnaj_datum;
    } else {
        return strcmp(node1->cas, node2->cas); //porovnaj cas & returnuj
    }
}

void u_function(struct DATALIST* list) {
    if (list->head == NULL || list->head->next == NULL) {   //ak je linkedlist prazdny skonci
        return;
    }
    int pocet_pred = 0;     //zrata si pocet pred sortom (lebo zadanie)
    struct Node* temp = list->head;
    while (temp != NULL)
    {
        pocet_pred++;
        temp = temp->next;    
    }
    
    int swapped;    //kontrolka na while
    struct Node* current; //aktualnny: teraz ho sortujem
    struct Node* posledny = NULL;   //posledny

    do {    //pouzivam bubblesort algorithm  - https://www.geeksforgeeks.org/bubble-sort-on-doubly-linked-list/
        swapped = 0;
        current = list->head;        
        while (current->next != posledny) {
            if (porovnaj_nody(current, current->next) > 0) {        
                struct Node* tempPrev = current->prev;     //prehod nody ak su opacne
                struct Node* tempNext = current->next;

                if (tempPrev != NULL) {                     //updatni next & previous pre current
                    tempPrev->next = current->next;
                } else {
                    list->head = current->next;
                }

                if (tempNext != NULL) {
                    tempNext->prev = current->prev;
                } else {
                    list->tail = current->prev;
                }

                current->prev = tempNext;                  //updatni next & previous pre current
                current->next = tempNext->next;

                if (tempNext->next != NULL) {
                    tempNext->next->prev = current;
                } else {
                    list->tail = current;
                }

                tempNext->next = current;
                swapped = 1;                                //pokracujeme dalej
            } else {                                        
                current = current->next;                  
            }
        }
        posledny = current;
    } while (swapped == 1);
    int pocet_po = 0;                      //zrata po sorte
    struct Node* temp_po = list->head;
    while (temp_po != NULL)
    {
        pocet_po++;
        temp_po = temp_po->next;    
    }
    if (pocet_pred == pocet_po)         //return dobra
    {
        printf("Spajany  zoznam  bol  usporiadany.\n");
    }
    else                //return zle
    {
        printf("Chyba  usporiadania.\n");
    }
    
}

void v_function(struct DATALIST* list) {
    struct Node* current = list->head;
    if (current == NULL) //ak neni linkedlist, skonci
    {
        return;
    }
    
    int i = 1;
    while (current) {
        printf("%d:\n",i++);
        printf("ID: %c%03d%c\t%s\t%.2lf\n", current->id.first, current->id.second, current->id.third,current->typ,current->hodnota);
        if (current->position.longitude < 0)    //print minus/plus
            printf("Poz: %.4lf", current->position.longitude);
        else
            printf("Poz: +%.4lf", current->position.longitude);
        if (current->position.latitude < 0)
            printf("\t\t%.4lf\n", current->position.latitude);
        else
            printf("\t\t+%.4lf\n", current->position.latitude);
        printf("DaC: %s\t\t%s\n", current->datum, current->cas);
        current = current->next; //posunie sa na dalsi nod
    }
}

void r_function(struct DATALIST* list) {
    int pos1, pos2;
    scanf("%d %d", &pos1,&pos2);
    if (pos1 == pos2) { //ak su rovanke skonci
        return;
    }
    pos1 -= 1;
    pos2 -= 1;

    struct Node* node1 = list->head; //vytvori si nody podla vstupu
    struct Node* node2 = list->head;
    int count = 0;

    while (node1 != NULL && count < pos1) { //v cykle najde nod1
        node1 = node1->next;
        count++;
    }

    count = 0;

    while (node2 != NULL && count < pos2) { //v cykle najde nod2    
        node2 = node2->next;
        count++;
    }

    if (node1 == NULL || node2 == NULL) { //ak su mimo likedlistu skonci
        return;
    }

    struct Node* tempPrev1 = node1->prev;           // prehodi ich pomocou temp
    struct Node* tempNext1 = node1->next;
    struct Node* tempPrev2 = node2->prev;
    struct Node* tempNext2 = node2->next;

    
    if (tempPrev1 != NULL) {            // riesi head a tail pre nod1
        tempPrev1->next = node2;
    } else {
        list->head = node2;
    }

    if (tempNext1 != NULL) {
        tempNext1->prev = node2;
    } else {
        list->tail = node2;
    }

    if (tempPrev2 != NULL) {            // riesi head a tail pre nod2
        tempPrev2->next = node1;
    } else {
        list->head = node1;
    }

    if (tempNext2 != NULL) {
        tempNext2->prev = node1;
    } else {
        list->tail = node1;
    }

    node1->prev = tempPrev2;            // prehodi prev & next
    node1->next = tempNext2;
    node2->prev = tempPrev1;
    node2->next = tempNext1;
}

void z_function(struct DATALIST* list, int *n)
{
    char scan_id_del[6];            //nacitaj id na deletnutie
    scanf("%s",scan_id_del);
    struct ID id_del = { scan_id_del[0], atoi(scan_id_del+1), scan_id_del[4] };
    
    struct Node* current = list->head;
    while (current != NULL) { //prejdi list
    if (id_del.first == current->id.first &&
        id_del.second == current->id.second &&
        id_del.third == current->id.third) {        //ak nasiel zhodu, tak zmaze
        if (current->prev != NULL) {
            current->prev->next = current->next;
        } else {                                    //ak som na head-e tak updatni head
            list->head = current->next;
        }

        if (current->next != NULL) {
            current->next->prev = current->prev;
        } else {                                    //ak je tail, updatni tail
            list->tail = current->prev;
        }

        free(current);
        (*n)--;                                 // mame o nod menej
        printf("Zaznam pre ID:  %c%d%c bol vymazany.\n",id_del.first,id_del.second,id_del.third); 
    }
    current = current->next; // posun sa dalej
    }
}

void k_function(struct DATALIST* list, int *n) {
    struct Node* current = list->head;
    for(int i; i < (*n);i++){       //prejdi nody a zmaz kazdy
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = list->tail = NULL; //linkedlist zmen na prazdny
}

void p_function(struct DATALIST* list, int *n)
{
    int p;  //p -pozicia nodu co ideme pridat
    scanf("%d", &p);    
    p -= 1;

    if (p < 1 || p > (*n) + 1) {    //chechkni ci ma zmysel pridavat na poziciu
        printf("Invalid position.\n");
        return;
    }

    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));  //novy nod

    char ch_ter[16];    //charakter z terminalu - temprorary premenna ch_ter

    scanf("%s",ch_ter);     //napln id
    sscanf(ch_ter,"%c%d%c", &new_node->id.first, &new_node->id.second, &new_node->id.third);
    
    scanf("%s",ch_ter);        //napln poziciu
    sscanf(ch_ter,"%lf%lf", &new_node->position.longitude, &new_node->position.latitude);

    scanf("%s", new_node->typ); //napln typ
    
    scanf("%lf", &new_node->hodnota); //napln hodnota
    
    scanf("%s", new_node->cas);        //napln cas
    
    scanf("%s", new_node->datum);       //napln datum


    struct Node* current = list->head;  //sprav pomocny nod
    int current_position = 1;           //pomocna premenna na nasmerovanie pridaneho nodu

    while (current_position != p) {     //najdi pointer kde chces pridat nod
        current = current->next;
        current_position++;
    }

    new_node->prev = current;           // pomocnym sa implementuj do pozicie
    new_node->next = current->next;     // pointer na next preber

    if (current->next != NULL) {
        current->next->prev = new_node;
    } else {
        list->tail = new_node;          //zmen tail, ak sme na konci
    }

    current->next = new_node;           //zmen next aby pointoval na pridany nod
    
}

void n_function(struct DATALIST* list, int *n) {
    struct Node* current = list->head;  //reset linkedlistu - bsaiaclly k ale neslo mi ho sem volat tak som ho skopiroval
    for(int i; i < (*n);i++){       
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    (*n) = 0;                   
    list->head = list->tail = NULL; 
    FILE* file = fopen("dataloger_V2.txt", "r"); //otvor file
    if (file == NULL) {
        printf("Zaznamy neboli nacitane!\n");   //ak file neni skonci
        return;
    }

    char line[50] = "";
    while (fgets(line, sizeof(line), file) != NULL) { 
        if(strcmp(line,"$$$\n") == 0)                           // k najde $$$ prida dalsi node
        {
            trim(fgets(line, sizeof(line), file));                      //napln id
            struct ID id = { line[0], atoi(line + 1), line[4] };
            
            trim(fgets(line, sizeof(line), file));                  
            struct Position pos;
            sscanf(line + 1, "%lf%lf", &pos.longitude, &pos.latitude); //napln poziciu

            trim(fgets(line, sizeof(line), file));                  //napln typ
            char typ[3];
            sscanf(line, "%s", typ);

            trim(fgets(line, sizeof(line), file));                  //napln hodnota
            double hodnota;
            sscanf(line, "%lf", &hodnota);

            trim(fgets(line, sizeof(line), file));                  //napln cas
            char cas[5];
            sscanf(line, "%s", cas);

            trim(fgets(line, sizeof(line), file));                  //napln daatum
            char datum[9];
            sscanf(line, "%s", datum);

            (*n)++;                                 //novy nod - zvys pocet nodov
            
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node)); //novy nod

            newNode->id = id;       //napln novy nod pomocnymi premennymi
            newNode->position = pos;
            strcpy(newNode->typ, typ);
            newNode->hodnota = hodnota;
            strcpy(newNode->cas, cas);
            strcpy(newNode->datum, datum);
            newNode->prev = newNode->next = NULL;   //next, prev su zatial prazdne

            if (list->head == NULL ){                      //ak je linkedlist prazdny daj nod na zaciatok
                list->head = list->tail = newNode;
            } else {                                       //inak na konice
                list->tail->next = newNode;
                newNode->prev = list->tail;
                list->tail = newNode;
            }
                }
            }
    printf("nacitalo sa %d zaznamov\n",(*n));               //print kolko nodov si pridal
    fclose(file);
}

void f_function(struct DATALIST* list) {
    struct Node* current = list->head;
    if (current == NULL) //ak neni linkedlist, skonci
    {
        return;
    }
    FILE* sheet = fopen("sheet.csv", "w"); //otvor file

    int i = 1;
    while (current) {
        printf("%f, ",current->hodnota);
        printf("%c%03d%c\n", current->id.first, current->id.second, current->id.third,current->typ);
        fprintf(sheet,"%f\n",current->hodnota);
        current = current->next; //posunie sa na dalsi nod
    }
    fclose(sheet);
    system("C://Progra~1//gnuplot//bin//gnuplot < command.txt");
}

void vytvor_zoznam(struct DATALIST* list){

}

int main()
{
    struct DATALIST dataList; 
    char input;
    int n = 0;
    while ((input = getc(stdin))!= 'k')
    {
        switch (input)
        {
        case 'n':{
            n_function(&dataList,&n);
            break;
        }
        case 'v':{
            v_function(&dataList);
            break;
        }
        case 'p':{
            p_function(&dataList,&n);
            break;
        }
        case 'z':{
            z_function(&dataList,&n);
            break;
        }
        case 'u':{
            u_function(&dataList);
            break;
        }   
        case 'r':{
            r_function(&dataList);
            break;
        }
        case 'f':{
            f_function(&dataList);
            break;
        }
        case 'x':{
            vytvor_zoznam(&dataList);
            break;
        }
        default:
            break;
        }
    } 
    k_function(&dataList,&n);
    return 0;
}
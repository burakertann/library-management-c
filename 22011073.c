#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Author{
    char *name;
    int id;
    char *surname;
}Author;

typedef struct AuthorNode{
    Author *data;
    struct AuthorNode *next;
}AuthorNode;

typedef struct IDqueue{
    int id;
    struct IDqueue *next;
}IDqueue;

typedef struct Book{
    char *name;
    char isbn[14];
    int adet;
}Book;

typedef struct BookCopy{
    char *etiket;
    char *durum;
}BookCopy;

typedef struct BookCopyNode{
    BookCopy *data;
    struct BookCopyNode *next;
}BookCopyNode;

typedef struct BookNode{
    Book *data;
    BookCopyNode *copyHead;
    struct BookNode *next;
}BookNode;

typedef struct BookAuthorLink{
    char isbn[14];
    int authorID;
    struct BookAuthorLink *next;
}BookAuthorLink;

typedef struct Student {
    char *name;         
    char *surname;       
    int point;            
    char id[9];           
    struct Student *prev;
    struct Student *next; 
    struct BorrowHistoryNode *historyHead;
} Student;

typedef struct BorrowHistory {
    char *isbn;       
    char *etiket;      
    char *alisTarihi;  
    char *iadeTarihi;  
} BorrowHistory;


typedef struct BorrowHistoryNode {
    BorrowHistory data;
    struct BorrowHistoryNode *next;
} BorrowHistoryNode;



Author* createAuthor(char *name,int id,char *surname);
int addAuthor(AuthorNode **head, IDqueue **rear, char *name, char *surname, int *currentID);
int createID(IDqueue **rear, int *currentID);
int dequeueID(IDqueue **rear);
void enqueueID(IDqueue **rear,int value);
void refreshAuthorsCSV(AuthorNode *head, const char *filename);
void readAuthorsFromCSV(AuthorNode **head, IDqueue **recycledIDs, const char *filename, int *currentID);
void deleteAuthorFromCSV(int deleteid,const char* filename);
int deleteAuthor(AuthorNode **head, IDqueue **recycledIDs, int id, const char *authorCSV, const char *bookAuthorCSV);
int updateAuthor(AuthorNode *head, int id, const char *newName, const char *newSurname, const char *authorCSV);
void printIDQueue(IDqueue *rear);
char *generateEtiket(char *isbn,int sayi);
void appendCopiesToBook(BookNode *book, BookCopyNode *newCopies, int adet);
BookNode* findBookByISBN(BookNode *head, const char *isbn);
void insertSortedBookNode(BookNode **head, BookNode *newNode);
void addBook(BookNode **bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, int *currentAuthorID,IDqueue **rear);
void appendBookAuthorLink(BookAuthorLink **head, const char *isbn, int authorID);
Book* createBook(char *name, char *isbn, int adet);
BookCopyNode* createBookCopyList(char *isbn, int startIndex, int adet);
AuthorNode* findAuthorByID(AuthorNode *head, int id);
void saveBooksToCSV(BookNode *head, const char *filename);
void saveBookCopiesToCSV(BookNode *bookHead, const char *filename);
void saveBookAuthorLinksToCSV(BookAuthorLink *linkHead, const char *filename);
void readBooksFromCSV(BookNode **bookHead, const char *filename);
void readBookCopiesFromCSV(BookNode *bookHead, const char *filename);
void readBookAuthorLinksFromCSV(BookAuthorLink **linkHead, const char *filename);
int deleteBook(BookNode **bookHead, BookAuthorLink **linkHead, const char *isbn);
int updateBook(BookNode *bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, int *currentAuthorID, IDqueue **rear);
void listBookByStatus(BookNode *bookHead);
void showBook(BookNode *bookHead);
Student* createStudent(char *name, char *surname, char *id);
void addStudent(Student **head,Student *newStudent);
void listStudents(Student *head);
void saveStudentsToCSV(Student *head, const char *filename);
void readStudentsFromCSV(Student **head, const char *filename);
int updateStudent(Student *head, const char *id, const char *newName, const char *newSurname, const char *filename);
int deleteStudent(Student **head, const char *id, const char *filename);
void readBorrowHistory(Student *studentHead, const char *filename);
void appendBorrowHistory(Student *ogr, char *isbn, char *etiket, char *tarih, int islemTuru);
void writeToBorrowCSV(const char *filename, const char *ogrenciID, const char *etiket, int islemTuru, const char *tarih);
void borrowBook(Student *studentHead, BookNode *bookHead, const char *csvFile);
int daysBetween(const char *start, const char *end);
void returnBook(Student *studentHead, BookNode *bookHead, const char *csvFile);
void listUnreturnedStudents(Student *head);
void listLateReturns(Student *head);
void listPenalizedStudents(Student *head);
void showStudentInfo(Student *head);
void studentMenu(int secim, Student **studentHead, BookNode *bookHead, const char *csvFile);
void bookMenu(int secim, BookNode **bookHead, BookAuthorLink **linkHead, AuthorNode **authorHead, int *currentAuthorID, IDqueue **idQueue, Student *studentHead);
void authorMenu(int secim, AuthorNode **authorHead, IDqueue **idQueue, BookAuthorLink *linkHead,int *currentAuthorID);
void generalMenu(int secim, BookNode **bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, Student **studentHead, IDqueue **idQueue, int *currentAuthorID);
void freeAuthors(AuthorNode *head);
void freeBookCopies(BookCopyNode *head);
void freeBooks(BookNode *head);
void freeBookAuthorLinks(BookAuthorLink *head);
void freeIDQueue(IDqueue *rear);
void freeBorrowHistory(BorrowHistoryNode *head);
void freeStudents(Student *head);

int main() {
    BookNode *bookHead = NULL;
    AuthorNode *authorHead = NULL;
    BookAuthorLink *linkHead = NULL;
    IDqueue *idQueue = NULL;
    Student *studentHead = NULL;
    int currentAuthorID = 0;
        
    readBooksFromCSV(&bookHead, "Kitaplar.csv");
    readBookCopiesFromCSV(bookHead, "KitapKopya.csv");
    readBookAuthorLinksFromCSV(&linkHead, "KitapYazar.csv");
    readAuthorsFromCSV(&authorHead, &idQueue, "yazarlar.csv", &currentAuthorID);
    readStudentsFromCSV(&studentHead, "Ogrenciler.csv");
    readBorrowHistory(studentHead, "OduncIslemleri.csv");

    int secim;
    do {
        printf("\n==== KÜTÜPHANE ANA MENÜ ====\n");
        printf("1  - Öğrenci Ekle\n");
        printf("2  - Öğrenci Güncelle\n");
        printf("3  - Öğrenci Sil\n");
        printf("4  - Öğrenci Bilgisi Görüntüle\n");
        printf("5  - Teslim Etmeyen Öğrencileri Listele\n");
        printf("6  - Cezalı Öğrencileri Listele\n");
        printf("7  - Öğrenci Listesini Göster\n");
        printf("8  - Kitap Ödünç Ver\n");
        printf("9  - Kitap İade Al\n");

        printf("10 - Kitap Ekle\n");
        printf("11 - Kitap Güncelle\n");
        printf("12 - Kitap Sil\n");
        printf("13 - Kitap Ara ve Göster\n");
        printf("14 - Raftaki Kitapları Listele\n");
        printf("15 - Gecikmeli İadeleri Listele\n");

        printf("16 - Yazar Ekle\n");
        printf("17 - Yazar Güncelle\n");
        printf("18 - Yazar Sil\n");
        printf("19 - Yazarları Listele\n");
        printf("20 - Kitap-Yazar Eşleşmelerini Göster\n");

        printf("21 - Tüm Verileri Yeniden Yükle (CSV'den)\n");
        printf("22 - Geri Kazanılabilir Yazar ID'lerini Göster\n");

        printf("0  - Çıkış\n");

        printf("Seçiminiz: ");
        scanf("%d", &secim);

        if (secim >= 1 && secim <= 9)
            studentMenu(secim, &studentHead, bookHead, "OduncIslemleri.csv");
        else if (secim >= 10 && secim <= 15)
            bookMenu(secim, &bookHead, &linkHead, &authorHead, &currentAuthorID, &idQueue,studentHead);
        else if (secim >= 16 && secim <= 21)
            authorMenu(secim, &authorHead, &idQueue,linkHead,&currentAuthorID);
        else if (secim >= 22)
            generalMenu(secim, &bookHead, &authorHead, &linkHead, &studentHead, &idQueue, &currentAuthorID);
        else if (secim == 0)
            printf("Çıkılıyor...\n");
        else
            printf("Geçersiz seçim!\n");

    } while (secim != 0);
    
    freeBooks(bookHead);
    freeAuthors(authorHead);
    freeBookAuthorLinks(linkHead);
    freeIDQueue(idQueue);
    freeStudents(studentHead);
}

Author* createAuthor(char *name,int id,char *surname){
    Author *newAuthor  = (Author*)malloc(sizeof(Author));
    if(newAuthor == NULL){
        return NULL;
    }
    newAuthor->name = (char*)malloc(sizeof(char) * strlen(name)+1);
    newAuthor->surname = (char*)malloc(sizeof(char) * strlen(surname)+1);

    if (newAuthor->name == NULL || newAuthor->surname == NULL) {
        free(newAuthor->name);
        free(newAuthor->surname);
        free(newAuthor);
        return NULL;
    }
    strcpy(newAuthor->name,name);
    strcpy(newAuthor->surname,surname);
    newAuthor->id = id;
    return newAuthor;
}

int addAuthor(AuthorNode **head,IDqueue **rear,char *name,char *surname,int *currentID){
    int newID = createID(rear, currentID);
    Author *newAuthor = createAuthor(name,newID,surname);
    if (newAuthor == NULL) return -1;

    AuthorNode *newNode = (AuthorNode*)malloc(sizeof(AuthorNode));
    if (newNode == NULL) return -1;
    newNode->data = newAuthor;
    newNode->next = NULL;
    //Liste Boş veya başa eklemem gereken durum
    if(*head == NULL || (*head)->data->id > newNode->data->id){
        newNode->next = *head;
        *head = newNode;
    }
    //Arada bir yere ekleme veya sona ekleme durumu
    else{
        AuthorNode *temp = *head;
        while(temp->next != NULL && temp->next->data->id < newNode->data->id){
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
    return newID;
}

int createID(IDqueue **rear, int *currentID){
    int id = dequeueID(rear);
    if(id != -1){
        printf("Geri ID kullanıldı: %d\n", id);
        return id;
    }
    printf("Yeni ID verildi: %d\n", *currentID + 1);
    return ++(*currentID);
}

int dequeueID(IDqueue **rear){
    if (*rear == NULL)
        return -1;

    IDqueue *front = (*rear)->next;
    int id = front->id;

    if (front == *rear) {
        *rear = NULL;
    } else {
        (*rear)->next = front->next;
    }
    printf("ID kuyruktan alındı: %d\n", id);
    free(front);
    return id;
}

void enqueueID(IDqueue **rear,int value){
    IDqueue *newNode = (IDqueue*)malloc(sizeof(IDqueue));
    newNode->id = value;
    printf("ID kuyrukta saklanıyor: %d\n", value);

    if(*rear == NULL){
        newNode->next = newNode;
        *rear = newNode;
    } else {
        newNode->next = (*rear)->next;
        (*rear)->next = newNode;
        *rear = newNode;
    }
}

void refreshAuthorsCSV(AuthorNode *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Dosya yazma hatası!\n");
        return;
    }

    AuthorNode *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%s\n", temp->data->id, temp->data->name, temp->data->surname);
        temp = temp->next;
    }

    fclose(fp);
}

void readAuthorsFromCSV(AuthorNode **head, IDqueue **recycledIDs, const char *filename, int *currentID) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Dosya bulunamadı, başlangıç listesi boş.\n");
        return;
    }

    char line[256];
    int maxID = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *idStr = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *surname = strtok(NULL, "\n");

        if (idStr && name && surname) {
            int id = atoi(idStr);
            if (id > maxID) maxID = id;

            Author *author = createAuthor(name, id, surname);
            if (!author) continue;

            AuthorNode *node = (AuthorNode *)malloc(sizeof(AuthorNode));
            node->data = author;
            node->next = NULL;

            if (*head == NULL || (*head)->data->id > id) {
                node->next = *head;
                *head = node;
            } else {
                AuthorNode *temp = *head;
                while (temp->next != NULL && temp->next->data->id < id)
                    temp = temp->next;
                node->next = temp->next;
                temp->next = node;
            }
        }
    }

    fclose(fp);

    *currentID = maxID;

    int *used = (int*)calloc(maxID + 1, sizeof(int));
    if (used == NULL) {
        printf("used dizisi için bellek ayrılamadı!\n");
        return;
    }

    AuthorNode *temp = *head;
    while (temp != NULL) {
        used[temp->data->id] = 1;
        temp = temp->next;
    }

    for (int i = 1; i < maxID; i++) {
        if (!used[i]) {
            enqueueID(recycledIDs, i);
            printf("Eksik ID bulundu ve kuyruğa eklendi: %d\n", i);
        }
    }

    free(used);
}

void deleteAuthorFromCSV(int deleteid,const char* filename){
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (fp == NULL || temp == NULL) {
        printf("KitapYazar dosyası açılamadı.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (token == NULL) continue;

        fprintf(temp, "%s", token);  // ISBN

        while ((token = strtok(NULL, ",\n")) != NULL) {
            int id = atoi(token);
            if (id == deleteid) {
                fprintf(temp, ",-1");
            } else {
                fprintf(temp, ",%d", id);
            }
        }
        fprintf(temp, "\n");
    }
    fclose(fp);
    fclose(temp);
    remove(filename);
    rename("temp.csv", filename);
}

int deleteAuthor(AuthorNode **head, IDqueue **recycledIDs, int id, const char *authorCSV, const char *bookAuthorCSV){
    if (*head == NULL) {
        printf("Yazar listesi boş.\n");
        return 0;
    }

    AuthorNode *temp = *head;
    AuthorNode *prev = NULL;

    if (temp->data->id == id) {
        *head = temp->next;
    } else {
        while (temp != NULL && temp->data->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) {
            printf("Yazar ID %d bulunamadı.\n", id);
            return 0;
        }
        prev->next = temp->next;
    }

    enqueueID(recycledIDs, temp->data->id);

    free(temp->data->name);
    free(temp->data->surname);
    free(temp->data);
    free(temp);

    refreshAuthorsCSV(*head, authorCSV);
    deleteAuthorFromCSV(id, bookAuthorCSV);

    return 1;

}

int updateAuthor(AuthorNode *head, int id, const char *newName, const char *newSurname, const char *authorCSV) {
    AuthorNode *temp = head;
    while (temp != NULL) {
        if (temp->data->id == id) {
            free(temp->data->name);
            free(temp->data->surname);

            temp->data->name = (char*)malloc(strlen(newName) + 1);
            temp->data->surname = (char*)malloc(strlen(newSurname) + 1);

            if (temp->data->name == NULL || temp->data->surname == NULL) {
                printf("Bellek hatası!\n");
                return 0;
            }

            strcpy(temp->data->name, newName);
            strcpy(temp->data->surname, newSurname);

            refreshAuthorsCSV(head, authorCSV);

            return 1;
        }
        temp = temp->next;
    }

    printf("ID %d ile yazar bulunamadı.\n", id);
    return 0;
}

void printIDQueue(IDqueue *rear) {
    if (rear == NULL) {
        printf("[IDQueue] Boş\n");
        return;
    }

    printf("[IDQueue] Geri kazanılabilir ID'ler: ");
    IDqueue *temp = rear->next;
    do {
        printf("%d ", temp->id);
        temp = temp->next;
    } while (temp != rear->next);
    printf("\n");
}

char *generateEtiket(char *isbn,int sayi){
    int num_lenght = 11;
    int total_lenght = strlen(isbn) + 1 + num_lenght+1;
    char *don = (char*)malloc(sizeof(char) * total_lenght);
    if(don == NULL){
        printf("Bellek tahsis edilemedi.\n");
        return NULL;
    }
    sprintf(don,"%s_%d",isbn,sayi);
    return don;
}

void addBook(BookNode **bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, int *currentAuthorID, IDqueue **rear) {
    char name[100], isbn[14];
    int adet;

    printf("Kitap adı: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("ISBN (13 hane): "); scanf("%s", isbn);
    printf("Kaç adet kopya eklemek istiyorsunuz? "); scanf("%d", &adet);

    BookNode *existing = findBookByISBN(*bookHead, isbn);
    if (existing != NULL) {
        int startIndex = existing->data->adet + 1;
        BookCopyNode *newCopies = createBookCopyList(isbn, startIndex, adet);
        appendCopiesToBook(existing, newCopies, adet);
        printf("Kitap zaten vardı. %d yeni kopya eklendi.\n", adet);
        saveBooksToCSV(*bookHead, "Kitaplar.csv");
        saveBookCopiesToCSV(*bookHead, "KitapKopya.csv");
        return;
    }

    Book *newBook = createBook(name, isbn, adet);
    BookCopyNode *copyList = createBookCopyList(isbn, 1, adet);

    BookNode *newNode = malloc(sizeof(BookNode));
    newNode->data = newBook;
    newNode->copyHead = copyList;
    newNode->next = NULL;
    insertSortedBookNode(bookHead, newNode);
    saveBooksToCSV(*bookHead, "Kitaplar.csv");
    saveBookCopiesToCSV(*bookHead, "KitapKopya.csv");
    int yazarSayisi;
    printf("Kaç yazar var? "); scanf("%d", &yazarSayisi);

    for (int i = 0; i < yazarSayisi; i++) {
        int yazarID;
        printf("Yazar ID girin: "); scanf("%d", &yazarID);

        AuthorNode *yazar = findAuthorByID(*authorHead, yazarID);
        if (yazar == NULL) {
            char ad[50], soyad[50];
            printf("Bu ID’ye sahip yazar yok. Yeni yazar adı: "); scanf("%s", ad);
            printf("Yeni yazar soyadı: "); scanf("%s", soyad);
            addAuthor(authorHead, rear, ad, soyad, currentAuthorID);  // ✅ düzeltildi
        }

        appendBookAuthorLink(linkHead, isbn, yazarID);
    }
    saveBookAuthorLinksToCSV(*linkHead, "KitapYazar.csv");
    printf("Yeni kitap başarıyla eklendi.\n");
}

Book* createBook(char *name,char *isbn,int adet){
    Book *newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) return NULL;
    newBook->name = strdup(name);
    if (!newBook->name) {
        free(newBook);
        return NULL;
    }
    strcpy(newBook->isbn,isbn);
    newBook->adet = adet;
    return newBook;
}

BookCopyNode *createBookCopyList(char *isbn,int startIndex,int adet){
    int i;
    BookCopyNode *head = NULL;
    BookCopyNode *tail = NULL;
    for(i = startIndex;i < startIndex+adet;i++){
        char *etiket = generateEtiket(isbn,i);
        BookCopy *newCopy = (BookCopy*)malloc(sizeof(BookCopy));
        newCopy->durum = strdup("RAFTA");
        newCopy->etiket = etiket;
        BookCopyNode *newNode = (BookCopyNode*)malloc(sizeof(BookCopyNode));
        newNode->data = newCopy;
        newNode->next = NULL;
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

void appendCopiesToBook(BookNode *book, BookCopyNode *newCopies, int adet) {
    if (!book) return;

    if (book->copyHead == NULL) {
        book->copyHead = newCopies;
    } else {
        BookCopyNode *temp = book->copyHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newCopies;
    }

    book->data->adet += adet;
}

BookNode* findBookByISBN(BookNode *head, const char *isbn) {
    while (head != NULL) {
        if (strcmp(head->data->isbn, isbn) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void insertSortedBookNode(BookNode **head, BookNode *newNode) {
    if (*head == NULL || strcmp(newNode->data->name, (*head)->data->name) < 0 ||
        (strcmp(newNode->data->name, (*head)->data->name) == 0 &&
         strcmp(newNode->data->isbn, (*head)->data->isbn) < 0)) {
        
        newNode->next = *head;
        *head = newNode;
        return;
    }

    BookNode *current = *head;
    while (current->next != NULL) {
        int nameCmp = strcmp(newNode->data->name, current->next->data->name);
        int isbnCmp = strcmp(newNode->data->isbn, current->next->data->isbn);

        if (nameCmp < 0 || (nameCmp == 0 && isbnCmp < 0)) {
            break;
        }
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void appendBookAuthorLink(BookAuthorLink **head, const char *isbn, int authorID) {
    BookAuthorLink *newNode = (BookAuthorLink*)malloc(sizeof(BookAuthorLink));
    if (newNode == NULL) {
        printf("Bellek hatası: BookAuthorLink oluşturulamadı.\n");
        return;
    }

    strcpy(newNode->isbn, isbn);
    newNode->authorID = authorID;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        BookAuthorLink *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

AuthorNode* findAuthorByID(AuthorNode *head, int id) {
    while (head != NULL) {
        if (head->data->id == id)
            return head;
        head = head->next;
    }
    return NULL;
}

void saveBooksToCSV(BookNode *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Dosya açılamadı: %s\n", filename);
        return;
    }

    BookNode *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s,%s,%d\n", temp->data->isbn, temp->data->name, temp->data->adet);
        temp = temp->next;
    }

    fclose(fp);
}

void saveBookCopiesToCSV(BookNode *bookHead, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("KitapKopya.csv dosyası açılamadı.\n");
        return;
    }

    BookNode *book = bookHead;
    while (book != NULL) {
        BookCopyNode *copy = book->copyHead;
        while (copy != NULL) {
            fprintf(fp, "%s,%s\n", copy->data->etiket, copy->data->durum);
            copy = copy->next;
        }
        book = book->next;
    }

    fclose(fp);
}

void saveBookAuthorLinksToCSV(BookAuthorLink *linkHead, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("KitapYazar.csv dosyası açılamadı.\n");
        return;
    }

    BookAuthorLink *temp = linkHead;
    while (temp != NULL) {
        fprintf(fp, "%s,%d\n", temp->isbn, temp->authorID);
        temp = temp->next;
    }

    fclose(fp);
}

void readBooksFromCSV(BookNode **bookHead, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[readBooksFromCSV] %s dosyası bulunamadı.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *isbn = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *adetStr = strtok(NULL, "\n");

        if (!isbn || !name || !adetStr) continue;

        int adet = atoi(adetStr);
        Book *book = createBook(name, isbn, adet);
        BookNode *node = malloc(sizeof(BookNode));
        node->data = book;
        node->copyHead = NULL;
        node->next = NULL;

        insertSortedBookNode(bookHead, node);
    }

    fclose(fp);
}

void readBookCopiesFromCSV(BookNode *bookHead, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[readBookCopiesFromCSV] %s dosyası bulunamadı.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *etiket = strtok(line, ",");
        char *durum = strtok(NULL, "\n");
        if (!etiket || !durum) continue;

        char isbn[14];
        sscanf(etiket, "%13[^_]", isbn);

        BookNode *book = findBookByISBN(bookHead, isbn);
        if (!book) continue;

        BookCopy *copy = malloc(sizeof(BookCopy));
        copy->etiket = strdup(etiket);
        copy->durum = strdup(durum);

        BookCopyNode *copyNode = malloc(sizeof(BookCopyNode));
        copyNode->data = copy;
        copyNode->next = NULL;

        appendCopiesToBook(book, copyNode, 0);
    }

    fclose(fp);
}

void readBookAuthorLinksFromCSV(BookAuthorLink **linkHead, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[readBookAuthorLinksFromCSV] %s dosyası bulunamadı.\n", filename);
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        char *isbn = strtok(line, ",");
        char *idStr = strtok(NULL, "\n");
        if (!isbn || !idStr) continue;

        int authorID = atoi(idStr);
        appendBookAuthorLink(linkHead, isbn, authorID);
    }

    fclose(fp);
}

int deleteBook(BookNode **bookHead, BookAuthorLink **linkHead, const char *isbn) {
    if (*bookHead == NULL) return 0;

    BookNode *curr = *bookHead;
    BookNode *prev = NULL;

    while (curr != NULL && strcmp(curr->data->isbn, isbn) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Kitap bulunamadı.\n");
        return 0;
    }

    BookCopyNode *c = curr->copyHead;
    while (c != NULL) {
        if (strcmp(c->data->durum, "RAFTA") != 0) {
            printf("Kitap kopyalarından biri ödünçte. Silme işlemi iptal edildi.\n");
            return 0;
        }
        c = c->next;
    }

    if (prev == NULL) {
        *bookHead = curr->next;
    } else {
        prev->next = curr->next;
    }

    BookCopyNode *copyTemp = curr->copyHead;
    while (copyTemp != NULL) {
        BookCopyNode *next = copyTemp->next;
        free(copyTemp->data->etiket);
        free(copyTemp->data->durum);
        free(copyTemp->data);
        free(copyTemp);
        copyTemp = next;
    }

    free(curr->data->name);
    free(curr->data);
    free(curr);

    BookAuthorLink *l = *linkHead, *lprev = NULL;
    while (l != NULL) {
        if (strcmp(l->isbn, isbn) == 0) {
            BookAuthorLink *toDelete = l;
            if (lprev == NULL) {
                *linkHead = l->next;
                l = *linkHead;
            } else {
                lprev->next = l->next;
                l = l->next;
            }
            free(toDelete);
        } else {
            lprev = l;
            l = l->next;
        }
    }

    saveBooksToCSV(*bookHead, "Kitaplar.csv");
    saveBookCopiesToCSV(*bookHead, "KitapKopya.csv");
    saveBookAuthorLinksToCSV(*linkHead, "KitapYazar.csv");

    printf("Kitap başarıyla silindi.\n");
    return 1;
}

int updateBook(BookNode *bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, int *currentAuthorID, IDqueue **rear) {
    char isbn[14];
    printf("Güncellenecek kitabın ISBN numarasını girin: ");
    scanf("%s", isbn);

    BookNode *book = findBookByISBN(bookHead, isbn);
    if (book == NULL) {
        printf("Kitap bulunamadı.\n");
        return 0;
    }

    int secim;
    printf("\n=== Kitap Güncelleme Menüsü ===\n");
    printf("1. Kitap Adını Güncelle\n");
    printf("2. Yazar(lar)ı Güncelle\n");
    printf("0. Güncellemeyi İptal Et\n");
    printf("Seçiminiz: ");
    scanf("%d", &secim);

    switch (secim) {
        case 1: {
            char yeniAd[100];
            printf("Yeni kitap adını girin: ");
            getchar();  // newline temizliği
            fgets(yeniAd, sizeof(yeniAd), stdin);
            yeniAd[strcspn(yeniAd, "\n")] = 0;
            free(book->data->name);
            book->data->name = strdup(yeniAd);
            printf("Kitap adı başarıyla güncellendi.\n");
            saveBooksToCSV(bookHead, "Kitaplar.csv");
            break;
        }
        case 2: {
            BookAuthorLink *curr = *linkHead, *prev = NULL;
            while (curr != NULL) {
                if (strcmp(curr->isbn, isbn) == 0) {
                    BookAuthorLink *toDelete = curr;
                    if (prev == NULL) {
                        *linkHead = curr->next;
                        curr = *linkHead;
                    } else {
                        prev->next = curr->next;
                        curr = curr->next;
                    }
                    free(toDelete);
                } else {
                    prev = curr;
                    curr = curr->next;
                }
            }

            int yazarSayisi;
            printf("Kaç yeni yazar girilecek? ");
            scanf("%d", &yazarSayisi);

            for (int i = 0; i < yazarSayisi; i++) {
                int yazarID;
                printf("Yazar ID girin: "); scanf("%d", &yazarID);

                AuthorNode *yazar = findAuthorByID(*authorHead, yazarID);
                if (yazar == NULL) {
                    char ad[50], soyad[50];
                    printf("Bu ID’ye sahip yazar yok. Yeni yazar adı: "); scanf("%s", ad);
                    printf("Yeni yazar soyadı: "); scanf("%s", soyad);
                    yazarID = addAuthor(authorHead, rear, ad, soyad, currentAuthorID);
                }

                appendBookAuthorLink(linkHead, isbn, yazarID);
            }
            saveBookAuthorLinksToCSV(*linkHead, "KitapYazar.csv");
            printf("Yazar(lar) başarıyla güncellendi.\n");
            break;
        }
        case 0:
            printf("Güncelleme iptal edildi.\n");
            break;
        default:
            printf("Geçersiz seçim!\n");
    }

    return 1;
}

void listBookByStatus(BookNode *bookHead) {
    printf("\n=== RAFTA OLAN KİTAPLAR ===\n");

    BookNode *book = bookHead;
    while (book != NULL) {
        BookCopyNode *copy = book->copyHead;
        while (copy != NULL) {
            if (strcmp(copy->data->durum, "RAFTA") == 0) {
                printf("Kitap: %s | ISBN: %s | Etiket: %s\n",
                    book->data->name, book->data->isbn, copy->data->etiket);
            }
            copy = copy->next;
        }
        book = book->next;
    }
}

void showBook(BookNode *bookHead) {
    char kitapAdi[100];
    printf("Aranacak kitap adını girin: ");
    getchar();
    fgets(kitapAdi, sizeof(kitapAdi), stdin);
    kitapAdi[strcspn(kitapAdi, "\n")] = 0;

    int bulundu = 0;
    BookNode *book = bookHead;
    while (book != NULL) {
        if (strcmp(book->data->name, kitapAdi) == 0) {
            bulundu = 1;
            printf("\nKitap: %s | ISBN: %s | Adet: %d\n", book->data->name, book->data->isbn, book->data->adet);
            BookCopyNode *copy = book->copyHead;
            while (copy != NULL) {
                printf("  - Etiket: %s | Durum: %s\n", copy->data->etiket, copy->data->durum);
                copy = copy->next;
            }
        }
        book = book->next;
    }

    if (!bulundu)
        printf("Bu adda bir kitap bulunamadı.\n");
}

Student* createStudent(char *name, char *surname, char *id) {
    Student *s = malloc(sizeof(Student));
    s->name = strdup(name);
    s->surname = strdup(surname);
    strcpy(s->id, id);
    s->point = 100;
    s->next = NULL;
    s->prev = NULL;
    s->historyHead = NULL;
    return s;
}

void addStudent(Student **head,Student *newStudent){
    if(*head == NULL){
        (*head) = newStudent;
        return;
    }
    Student *temp = *head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newStudent;
    newStudent->prev = temp;
}

void listStudents(Student *head) {
    if (head == NULL) {
        printf("Öğrenci listesi boş.\n");
        return;
    }

    printf("\n--- Öğrenci Listesi ---\n");
    Student *temp = head;
    while (temp != NULL) {
        printf("ID: %s | Ad: %s | Soyad: %s | Puan: %d\n",
               temp->id, temp->name, temp->surname, temp->point);
        temp = temp->next;
    }
}

void saveStudentsToCSV(Student *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Dosya açılamadı: %s\n", filename);
        return;
    }

    Student *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s,%s,%s,%d\n", temp->id, temp->name, temp->surname, temp->point);
        temp = temp->next;
    }

    fclose(fp);
    printf("Öğrenciler başarıyla '%s' dosyasına kaydedildi.\n", filename);
}

void readStudentsFromCSV(Student **head, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Dosya açılamadı: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *surname = strtok(NULL, ",");
        char *pointStr = strtok(NULL, "\n");

        if (!id || !name || !surname || !pointStr) continue;

        int point = atoi(pointStr);
        Student *newStudent = malloc(sizeof(Student));
        newStudent->name = strdup(name);
        newStudent->surname = strdup(surname);
        strcpy(newStudent->id, id);
        newStudent->point = point;
        newStudent->next = NULL;
        newStudent->prev = NULL;
        newStudent->historyHead = NULL;

        addStudent(head, newStudent);
    }

    fclose(fp);
    printf("Öğrenciler başarıyla '%s' dosyasından yüklendi.\n", filename);
}

int updateStudent(Student *head, const char *id, const char *newName, const char *newSurname, const char *filename) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) {
            free(head->name);
            free(head->surname);
            head->name = strdup(newName);
            head->surname = strdup(newSurname);
            printf("Öğrenci bilgileri güncellendi.\n");

            // CSV'yi yeniden yaz
            FILE *fp = fopen(filename, "w");
            if (!fp) {
                printf("CSV dosyası açılamadı.\n");
                return 0;
            }

            Student *temp = head;
            while (temp->prev != NULL) temp = temp->prev; // listenin başına git

            while (temp != NULL) {
                fprintf(fp, "%s,%s,%s,%d\n", temp->id, temp->name, temp->surname, temp->point);
                temp = temp->next;
            }

            fclose(fp);
            return 1;
        }
        head = head->next;
    }

    printf("Öğrenci bulunamadı.\n");
    return 0;
}

int deleteStudent(Student **head, const char *id, const char *filename) {
    if (*head == NULL) {
        printf("Liste boş.\n");
        return 0;
    }

    Student *curr = *head;
    while (curr != NULL && strcmp(curr->id, id) != 0)
        curr = curr->next;

    if (curr == NULL) {
        printf("Öğrenci bulunamadı.\n");
        return 0;
    }

    // Liste bağlantılarını kopar
    if (curr->prev != NULL)
        curr->prev->next = curr->next;
    else
        *head = curr->next; // baştan siliniyorsa

    if (curr->next != NULL)
        curr->next->prev = curr->prev;

    // Bellek temizliği
    free(curr->name);
    free(curr->surname);
    BorrowHistoryNode *h = curr->historyHead;
    while (h != NULL) {
        BorrowHistoryNode *temp = h;
        h = h->next;
        free(temp->data.isbn);
        free(temp->data.etiket);
        free(temp->data.alisTarihi);
        free(temp->data.iadeTarihi);
        free(temp);
    }
    free(curr);

    // CSV'yi yeniden yaz
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("CSV dosyası açılamadı.\n");
        return 0;
    }

    Student *temp = *head;
    while (temp != NULL) {
        fprintf(fp, "%s,%s,%s,%d\n", temp->id, temp->name, temp->surname, temp->point);
        temp = temp->next;
    }

    fclose(fp);
    printf("Öğrenci silindi.\n");
    return 1;
}

void readBorrowHistory(Student *studentHead, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("OduncIslemleri.csv dosyası açılamadı.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *ogrenciID = strtok(line, ",");
        char *etiket = strtok(NULL, ",");
        char *turStr = strtok(NULL, ",");
        char *tarih = strtok(NULL, "\n");

        if (!ogrenciID || !etiket || !turStr || !tarih) continue;

        int tur = atoi(turStr);

        char isbn[14];
        sscanf(etiket, "%13[^_]", isbn);

        Student *ogr = studentHead;
        while (ogr != NULL && strcmp(ogr->id, ogrenciID) != 0) {
            ogr = ogr->next;
        }
        if (!ogr) continue;

        if (tur == 0) {
            BorrowHistoryNode *newNode = malloc(sizeof(BorrowHistoryNode));
            newNode->data.isbn = strdup(isbn);
            newNode->data.etiket = strdup(etiket);
            newNode->data.alisTarihi = strdup(tarih);
            newNode->data.iadeTarihi = NULL;
            newNode->next = NULL;

            if (ogr->historyHead == NULL) {
                ogr->historyHead = newNode;
            } else {
                BorrowHistoryNode *temp = ogr->historyHead;
                while (temp->next != NULL) temp = temp->next;
                temp->next = newNode;
            }
        } else if (tur == 1) {
            BorrowHistoryNode *temp = ogr->historyHead;
            while (temp != NULL) {
                if (strcmp(temp->data.etiket, etiket) == 0 && temp->data.iadeTarihi == NULL) {
                    temp->data.iadeTarihi = strdup(tarih);
                    break;
                }
                temp = temp->next;
            }
        }
    }

    fclose(fp);
}

void appendBorrowHistory(Student *ogr, char *isbn, char *etiket, char *tarih, int islemTuru) {
    if (!ogr) return;

    if (islemTuru == 0) {
        BorrowHistoryNode *newNode = malloc(sizeof(BorrowHistoryNode));
        newNode->data.isbn = strdup(isbn);
        newNode->data.etiket = strdup(etiket);
        newNode->data.alisTarihi = strdup(tarih);
        newNode->data.iadeTarihi = NULL;
        newNode->next = NULL;

        if (ogr->historyHead == NULL) {
            ogr->historyHead = newNode;
        } else {
            BorrowHistoryNode *temp = ogr->historyHead;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    } else if (islemTuru == 1) {
        BorrowHistoryNode *temp = ogr->historyHead;
        while (temp != NULL) {
            if (strcmp(temp->data.etiket, etiket) == 0 && temp->data.iadeTarihi == NULL) {
                temp->data.iadeTarihi = strdup(tarih);
                break;
            }
            temp = temp->next;
        }
    }
}

void writeToBorrowCSV(const char *filename, const char *ogrenciID, const char *etiket, int islemTuru, const char *tarih) {
    FILE *fp = fopen(filename, "a"); 
    if (!fp) {
        printf("Dosya yazma hatası: %s\n", filename);
        return;
    }

    fprintf(fp, "%s,%s,%d,%s\n", ogrenciID, etiket, islemTuru, tarih);
    fclose(fp);
}

void borrowBook(Student *studentHead, BookNode *bookHead, const char *csvFile) {
    char ogrID[9], etiket[20];
    printf("Öğrenci numarasını girin: ");
    scanf("%s", ogrID);

    Student *ogr = studentHead;
    while (ogr && strcmp(ogr->id, ogrID) != 0)
        ogr = ogr->next;

    if (!ogr) {
        printf("Öğrenci bulunamadı.\n");
        return;
    }

    if (ogr->point <= 0) {
        printf("Öğrencinin puanı yetersiz. İşlem iptal edildi.\n");
        return;
    }

    printf("Kitap etiketini girin (örn: 1234567891011_2): ");
    scanf("%s", etiket);

    char isbn[14];
    sscanf(etiket, "%13[^_]", isbn);

    BookNode *book = findBookByISBN(bookHead, isbn);
    if (!book) {
        printf("Kitap bulunamadı.\n");
        return;
    }

    BookCopyNode *copy = book->copyHead;
    while (copy != NULL && strcmp(copy->data->etiket, etiket) != 0)
        copy = copy->next;

    if (!copy) {
        printf("Etiket bulunamadı.\n");
        return;
    }

    if (strcmp(copy->data->durum, "RAFTA") != 0) {
        printf("Kitap şu anda başka bir öğrenci tarafından ödünç alınmış.\n");
        return;
    }

    char tarih[11];
    time_t now = time(NULL);
    strftime(tarih, sizeof(tarih), "%Y-%m-%d", localtime(&now));

    free(copy->data->durum);
    copy->data->durum = strdup(ogrID);

    appendBorrowHistory(ogr, isbn, etiket, tarih, 0);

    writeToBorrowCSV(csvFile, ogrID, etiket, 0, tarih);

    printf("Kitap başarıyla ödünç verildi.\n");
}

int daysBetween(const char *start, const char *end) {
    struct tm tm1 = {0}, tm2 = {0};
    strptime(start, "%Y-%m-%d", &tm1);
    strptime(end, "%Y-%m-%d", &tm2);
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    double diff = difftime(t2, t1);
    return (int)(diff / (60 * 60 * 24));
}

void returnBook(Student *studentHead, BookNode *bookHead, const char *csvFile) {
    char ogrID[9], etiket[20];
    printf("Öğrenci numarasını girin: ");
    scanf("%s", ogrID);

    Student *ogr = studentHead;
    while (ogr && strcmp(ogr->id, ogrID) != 0)
        ogr = ogr->next;

    if (!ogr) {
        printf("Öğrenci bulunamadı.\n");
        return;
    }

    printf("Teslim edilecek kitap etiketini girin: ");
    scanf("%s", etiket);

    char isbn[14];
    sscanf(etiket, "%13[^_]", isbn);

    BookNode *book = findBookByISBN(bookHead, isbn);
    if (!book) {
        printf("Kitap bulunamadı.\n");
        return;
    }

    BookCopyNode *copy = book->copyHead;
    while (copy && strcmp(copy->data->etiket, etiket) != 0)
        copy = copy->next;

    if (!copy) {
        printf("Etiket bulunamadı.\n");
        return;
    }

    if (strcmp(copy->data->durum, ogrID) != 0) {
        printf("Bu kitap bu öğrenciye ait değil.\n");
        return;
    }

    char tarih[11];
    time_t now = time(NULL);
    strftime(tarih, sizeof(tarih), "%Y-%m-%d", localtime(&now));

    BorrowHistoryNode *hist = ogr->historyHead;
    while (hist) {
        if (strcmp(hist->data.etiket, etiket) == 0 && hist->data.iadeTarihi == NULL) {
            hist->data.iadeTarihi = strdup(tarih);

            int gecikme = daysBetween(hist->data.alisTarihi, tarih);
            if (gecikme > 15) {
                ogr->point -= 10;
                printf("Teslim gecikmesi tespit edildi (%d gün). -10 puan uygulandı.\n", gecikme);
            }

            break;
        }
        hist = hist->next;
    }

    free(copy->data->durum);
    copy->data->durum = strdup("RAFTA");

    writeToBorrowCSV(csvFile, ogrID, etiket, 1, tarih);

    printf("Kitap başarıyla iade edildi.\n");
}

void listUnreturnedStudents(Student *head) {
    int bulundu = 0;
    printf("\n=== Teslim Etmemiş Öğrenciler ===\n");

    while (head != NULL) {
        BorrowHistoryNode *h = head->historyHead;
        while (h != NULL) {
            if (h->data.iadeTarihi == NULL) {
                printf("Öğrenci: %s %s | Numara: %s\n", head->name, head->surname, head->id);
                bulundu = 1;
                break;
            }
            h = h->next;
        }
        head = head->next;
    }

    if (!bulundu)
        printf("Tüm kitaplar teslim edilmiş.\n");
}

void listLateReturns(Student *head) {
    int bulundu = 0;
    printf("\n=== Gecikmeli Teslim Edilen Kitaplar ===\n");

    while (head != NULL) {
        BorrowHistoryNode *h = head->historyHead;
        while (h != NULL) {
            if (h->data.iadeTarihi != NULL) {
                int fark = daysBetween(h->data.alisTarihi, h->data.iadeTarihi);
                if (fark > 15) {
                    printf("Öğrenci: %s %s | Kitap: %s | Etiket: %s | Gecikme: %d gün\n",
                        head->name, head->surname, h->data.isbn, h->data.etiket, fark);
                    bulundu = 1;
                }
            }
            h = h->next;
        }
        head = head->next;
    }

    if (!bulundu)
        printf("Gecikmeli teslim edilen kitap yok.\n");
}

void listPenalizedStudents(Student *head) {
    int bulundu = 0;
    printf("\n=== Cezalı Öğrenciler (Puanı 0 veya altı) ===\n");

    while (head != NULL) {
        if (head->point <= 0) {
            printf("Öğrenci: %s %s | Numara: %s | Puan: %d\n",
                   head->name, head->surname, head->id, head->point);
            bulundu = 1;
        }
        head = head->next;
    }

    if (!bulundu)
        printf("Cezalı öğrenci yok.\n");
}

void showStudentInfo(Student *head) {
    char input[100];
    printf("Öğrenci ID veya Ad-Soyad girin: ");
    getchar();
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    Student *ogr = head;
    while (ogr != NULL) {
        if (strcmp(ogr->id, input) == 0 ||
            (strstr(ogr->name, input) && strstr(ogr->surname, input))) {
            break;
        }
        ogr = ogr->next;
    }

    if (!ogr) {
        printf("Öğrenci bulunamadı.\n");
        return;
    }

    printf("\n--- Öğrenci Bilgisi ---\n");
    printf("Ad: %s\nSoyad: %s\nID: %s\nPuan: %d\n", ogr->name, ogr->surname, ogr->id, ogr->point);

    printf("\n--- Kitap Hareketleri ---\n");
    BorrowHistoryNode *hist = ogr->historyHead;
    if (!hist) {
        printf("Hiç kitap ödünç almamış.\n");
        return;
    }

    while (hist != NULL) {
        printf("Kitap: %s | Etiket: %s | Alış: %s | İade: %s\n",
               hist->data.isbn,
               hist->data.etiket,
               hist->data.alisTarihi,
               hist->data.iadeTarihi ? hist->data.iadeTarihi : "TESLİM EDİLMEMİŞ");
        hist = hist->next;
    }
}

void studentMenu(int secim, Student **studentHead, BookNode *bookHead, const char *csvFile) {
    switch (secim) {
        case 1: {
            char name[100], surname[100], id[9];
            printf("Ad: "); scanf("%s", name);
            printf("Soyad: "); scanf("%s", surname);
            printf("Numara (8 hane): "); scanf("%s", id);
            Student *s = createStudent(name, surname, id);
            addStudent(studentHead, s);
            saveStudentsToCSV(*studentHead, "Ogrenciler.csv");
            break;
        }

        case 2: {
            char no[9], newName[100], newSurname[100];
            printf("Numara: "); scanf("%s", no);
            printf("Yeni Ad: "); scanf("%s", newName);
            printf("Yeni Soyad: "); scanf("%s", newSurname);
            updateStudent(*studentHead, no, newName, newSurname, "Ogrenciler.csv");
            break;
        }

        case 3: {
            char no[9];
            printf("Silinecek numara: "); scanf("%s", no);
            deleteStudent(studentHead, no, "Ogrenciler.csv");
            break;
        }

        case 4:
            showStudentInfo(*studentHead);
            break;

        case 5:
            listUnreturnedStudents(*studentHead);
            break;

        case 6:
            listPenalizedStudents(*studentHead);
            break;

        case 7:
            listStudents(*studentHead);
            break;

        case 8:
            borrowBook(*studentHead, bookHead, csvFile);
            break;

        case 9:
            returnBook(*studentHead, bookHead, csvFile);
            break;

        default:
            printf("Geçersiz öğrenci işlemi!\n");
    }
}

void bookMenu(int secim, BookNode **bookHead, BookAuthorLink **linkHead, AuthorNode **authorHead, int *currentAuthorID, IDqueue **idQueue, Student *studentHead) {
    switch (secim) {
        case 10:
            addBook(bookHead, authorHead, linkHead, currentAuthorID, idQueue);
            break;

        case 11:
            updateBook(*bookHead, authorHead, linkHead, currentAuthorID, idQueue);
            break;

        case 12: {
            char isbn[14];
            printf("Silinecek kitabın ISBN numarası: ");
            scanf("%s", isbn);
            deleteBook(bookHead, linkHead, isbn);
            break;
        }

        case 13:
            showBook(*bookHead);
            break;

        case 14:
            listBookByStatus(*bookHead);
            break;

        case 15:
            listLateReturns(studentHead);
            break;

        default:
            printf("Geçersiz kitap işlemi!\n");
    }
}

void authorMenu(int secim, AuthorNode **authorHead, IDqueue **idQueue, BookAuthorLink *linkHead,int *currentAuthorID) {
    switch (secim) {
        case 16: {
            char ad[100], soyad[100];
            printf("Ad: "); scanf("%s", ad);
            printf("Soyad: "); scanf("%s", soyad);
            addAuthor(authorHead, idQueue, ad, soyad, currentAuthorID);
            refreshAuthorsCSV(*authorHead, "yazarlar.csv");
            break;
        }

        case 17: {
            int id;
            char yeniAd[100], yeniSoyad[100];
            printf("Güncellenecek ID: "); scanf("%d", &id);
            printf("Yeni ad: "); scanf("%s", yeniAd);
            printf("Yeni soyad: "); scanf("%s", yeniSoyad);
            updateAuthor(*authorHead, id, yeniAd, yeniSoyad, "yazarlar.csv");
            break;
        }

        case 18: {
            int id;
            printf("Silinecek yazar ID: "); scanf("%d", &id);
            deleteAuthor(authorHead, idQueue, id, "yazarlar.csv", "KitapYazar.csv");
            break;
        }

        case 19: {
            AuthorNode *a = *authorHead;
            while (a != NULL) {
                printf("ID: %d | Ad: %s | Soyad: %s\n", a->data->id, a->data->name, a->data->surname);
                a = a->next;
            }
            break;
        }

        case 20: {
            printf("\n--- KİTAP-YAZAR BAĞLANTILARI ---\n");
            BookAuthorLink *l = linkHead;
            while (l != NULL) {
                printf("ISBN: %s <-> Yazar ID: %d\n", l->isbn, l->authorID);
                l = l->next;
            }
            break;
        }

        case 21:
            printf("Not: Kitabın yazarını güncelleme işlemi zaten kitap güncelleme (11) menüsündedir.\n");
            break;

        default:
            printf("Geçersiz yazar işlemi!\n");
    }
}

void generalMenu(int secim, BookNode **bookHead, AuthorNode **authorHead, BookAuthorLink **linkHead, Student **studentHead, IDqueue **idQueue, int *currentAuthorID) {
    switch (secim) {
        case 21:
            readBooksFromCSV(bookHead, "Kitaplar.csv");
            readBookCopiesFromCSV(*bookHead, "KitapKopya.csv");
            readBookAuthorLinksFromCSV(linkHead, "KitapYazar.csv");
            readAuthorsFromCSV(authorHead, idQueue, "yazarlar.csv", currentAuthorID);
            readStudentsFromCSV(studentHead, "Ogrenciler.csv");
            readBorrowHistory(*studentHead, "OduncIslemleri.csv");
            printf("Tüm CSV dosyaları belleğe yeniden yüklendi.\n");
            break;

        case 22:
            printIDQueue(*idQueue);
            break;

        default:
            printf("Geçersiz genel işlem!\n");
    }
}

void freeAuthors(AuthorNode *head) {
    while (head != NULL) {
        AuthorNode *temp = head;
        head = head->next;
        free(temp->data->name);
        free(temp->data->surname);
        free(temp->data);
        free(temp);
    }
}

void freeBookCopies(BookCopyNode *head) {
    while (head != NULL) {
        BookCopyNode *temp = head;
        head = head->next;
        free(temp->data->etiket);
        free(temp->data->durum);
        free(temp->data);
        free(temp);
    }
}

void freeBooks(BookNode *head) {
    while (head != NULL) {
        BookNode *temp = head;
        head = head->next;
        free(temp->data->name);
        free(temp->data);
        freeBookCopies(temp->copyHead);
        free(temp);
    }
}

void freeBookAuthorLinks(BookAuthorLink *head) {
    while (head != NULL) {
        BookAuthorLink *temp = head;
        head = head->next;
        free(temp);
    }
}

void freeIDQueue(IDqueue *rear) {
    if (!rear) return;

    IDqueue *curr = rear->next;
    rear->next = NULL;

    while (curr != NULL) {
        IDqueue *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void freeBorrowHistory(BorrowHistoryNode *head) {
    while (head != NULL) {
        BorrowHistoryNode *temp = head;
        head = head->next;
        free(temp->data.isbn);
        free(temp->data.etiket);
        free(temp->data.alisTarihi);
        if (temp->data.iadeTarihi)
            free(temp->data.iadeTarihi);
        free(temp);
    }
}

void freeStudents(Student *head) {
    while (head != NULL) {
        Student *temp = head;
        head = head->next;
        free(temp->name);
        free(temp->surname);
        freeBorrowHistory(temp->historyHead);
        free(temp);
    }
}


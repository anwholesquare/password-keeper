#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

int randomKeyGenerator(int min, int max);

typedef struct password
{
    char name[100];
    char username[100];
    int key;
    int value[500];
} pwd;


struct DBID {
    int id;
    char serviceName[100];
    char username[100];
} db_id;


char fileName[] = "db_loader.dat";

int keyMultiplier = 11;
int endingValue = 30240;

void encrypt(char password[100], int key, int value[100] );

char* substr(const char *src, int m, int n);

void decrypt(char password[100], int key, int value[100]);
void reverseString(char str[], int len);
void lastIDFinder(char str[], int len, int* id, char serviceName[100], char userName[100]);
void lastID();


char getChoice(char* choice);
void loadPasswords();
void savePasswords();
void addNewPassword();
void choiceList(char choice);
void choiceMaker();
void passwordCracker(char fileName[100]);

char chooser[5];
void choose() { fflush(stdin); fgets(chooser, 2, stdin);}

int main() {
    srand(time(NULL));
    choiceMaker();
    
    return 0;
}


char getChoice(char* choice) {
    printf("Enter Your Choice:\t");
    fflush(stdin);
    scanf("%c", choice);
    if(*choice == '1') {
        return '1';
    }else if (*choice == '2') {
        return '2';
    }else {
        int exitMode = 1;
        printf("Do you want to exit the application? (Y = 1 /N = 0):\t");
        choose();
        exitMode = atoi(chooser);
        switch (exitMode)
        {
        case 1:
            exit(0);
            break; 
        default:
            printf("\n\n");
            choiceMaker();
            break;
        }
    }
}

void choiceMaker() {
    system ("clear");
    printf("Password Keeper\nDeveloped by anwholesquare\n----------------------------\n\n1. Load Passwords\t2. Save New Password\n3. Exit Application\n");
    
    char choice;
    getChoice( &choice);
    choiceList(choice);
    int exitMode = 1;
    printf("Do you want to exit the application? (Y = 1 /N = 0):\t");
    choose();
    exitMode = atoi(chooser);
    switch (exitMode)
    {
    case 1:
        exit(0);
        break; 
    default:
        printf("\n\n");
        choiceMaker();
        break;
    }
}

void choiceList(char choice) {
    switch (choice)
    {
        case '1':
            loadPasswords();
            break;
        case '2':
            savePasswords();
            break;    
    }
}
void loadPasswords() {
    printf("Password Loader\n---------------\n");
    FILE *fpdbr;
    char serviceName[100];
    char username[100];

    if ((fpdbr = fopen(fileName, "r")) == NULL) 
    {
        printf("Can't open DB file\n");
    }
    
    lastID();

    struct loader {
        char serviceName[100];
        char userName [100];
    } ld[db_id.id];
    
    int loader_id = 0;

    char buffer[150];
    printf("Id        Service");
    for(int sp = 0; sp <= (40 - strlen("Service")); sp++) {printf(" ");}
    printf("Username\n");
    while (fgets(buffer, 150, fpdbr))
    {

        buffer[strcspn(buffer, "\n")] = 0;
        
        int i = 2;
        int cnt = 0;
        char ch_id[5];
        while(buffer[i] != '|') {
            ch_id[cnt] = buffer[i];
            i++;
            cnt++;
        }
        i++;
        int id = atoi(ch_id);
        cnt = 0;

        while(buffer[i] != '|') {
            serviceName[cnt] = buffer[i];
            cnt++;
            i++;
        }
        serviceName[cnt] = '\0';
        i++;
        cnt = 0;
        while(i < strlen(buffer)) {
            username[cnt] = buffer[i];
            cnt++;
            i++;
        }
        username[cnt] = '\0';
        
        printf("%d\t", id);
        for (int sp = 0; sp <= (snprintf(NULL, 0, "%d", id-1)); sp++) {printf(" ");}
        printf("%s", serviceName);
        for(int sp = 0; sp <= (40 - strlen(serviceName)); sp++) {printf(" ");}
        printf("%s\n", username);
        strcpy(ld[id].userName , username);
        strcpy(ld[id].serviceName , serviceName);
        if( id >= db_id.id) {
            break;
        }
    }
    
    int choice = 0;
    while( choice <= 0 || choice > db_id.id ) {
        printf("Enter service ID to retrieve password: \t");
        choose();
        choice = atoi(chooser);
        if ( choice > db_id.id) {
            printf("Warning: Invalid Choice\n");
        }
    }
    printf("\n\n");
    printf("-------------------------------------------------\n\n");
    printf("Id: %d\nService name: %s\nUsername: %s\nPassword: ", choice, ld[choice].serviceName, ld[choice].userName );
        
    fclose(fpdbr); 
    char passFile[150] = "assets\\";
    strcat(passFile, ld[choice].serviceName);
    char choicer[7];
    sprintf(choicer, "%d", choice);
    strcat(passFile,choicer );
    strcat(passFile,".dat" );
    passwordCracker(passFile);
    printf("-------------------------------------------------\n");
}

void savePasswords() {
    printf("Password Saver\n--------------\n1. Add new password\t2.Go back\n\n");
    int choice = -1;
    
    while(choice != 1 && choice != 2) {
        printf("enter your choice: ");
        choose();
        choice = atoi(chooser);
        printf("\n\n");
        switch (choice)
        {
        case 1:
            addNewPassword();
            break;
        
        case 2:
            choiceMaker();
            break;
        }
    }
    
}

void addNewPassword() {
    int flag = 0;
    pwd pwd1;
    char password[100];
    printf("-------------------------------------------------\n\n");
    printf("Enter Site Name: ");
    fflush(stdin);
    fgets(pwd1.name, 100, stdin);

    printf("Enter User Name: ");
    fflush(stdin);
    fgets(pwd1.username, 100, stdin);
    
    printf("Enter Password: ");
    fflush(stdin);
    fgets(password, 100, stdin);
    
    pwd1.key = randomKeyGenerator(2, 9);
    char keyer[2];
    keyer[0] = (char) (pwd1.key + '0');
    encrypt(password, pwd1.key, pwd1.value);    


    char file_maker[100];
    strcat(file_maker, keyer  );
    strcat(file_maker, "\n");
    
    int i = 0;
    while(pwd1.value[i] != endingValue) {
        char str[7];
        sprintf(str, "%d", pwd1.value[i]); 
        strcat(file_maker, str);
        strcat(file_maker, " ");
        i++;
    }
    char str[7];
    sprintf(str, "%d", pwd1.value[i]); 
    strcat(file_maker, str);
    strcat(file_maker, "\n");
   

    FILE *fp, *fpdbw;

    lastID();
    char* NewServiceName = substr(pwd1.name, 0, strlen(pwd1.name) -1);
    char* UserName = substr(pwd1.username, 0, strlen(pwd1.username) -1);

    sprintf(str, "%d", db_id.id+1);    
    char db_file_append[150] = "~ ";
    strcat(db_file_append, str);
    strcat(db_file_append, "|");
    strcat(db_file_append, NewServiceName);
    strcat(db_file_append, "|");
    strcat(db_file_append, UserName);
    strcat(db_file_append, "\n");

    
    char newFile[200] = "assets\\";
    strcat(newFile, NewServiceName);
    strcat(newFile, str);
    strcat(newFile, ".dat");

    
    if( (fpdbw = fopen(fileName, "a")) == NULL) { 
        printf("Cannot open DB file\n"); 
        flag = 1;
    }

    
    if( (fp = fopen(newFile, "w")) == NULL) { 
        printf("Cannot open Asset folder\n"); 
        flag = 1;
    }else{

    }

    fputs(db_file_append, fpdbw);
    fputs(file_maker, fp);
    fclose(fp);
    fclose(fpdbw);
    

    if(flag) {
        printf("Password can't be saved due to file permissions error\n");
    }else{
        printf("Password saved in our dictionary.\n");
    }

    printf("-------------------------------------------------\n\n");

}

char* substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}


int randomKeyGenerator(int min, int max) {
    
    return ((rand() % (int)(((max) + 1) - (min))) + (min));
}

void encrypt(char password[100], int key, int value[100] ) {
    int i = 0;
    int cnt = 0;
    while(password[i] != '\0') {
        if ( (int) password[i] >  31 && password[i] < 127) {
            value[cnt] = ((int) password[i]) + key * keyMultiplier;
            password[i] = '\0';
            cnt++;
        }
        i++;
        
    }
    value[cnt] = endingValue;
}

void passwordCracker(char fileName[100]) {

    
    FILE *fpdbr;
    char identity[150];

    if ((fpdbr = fopen(fileName, "r")) == NULL) 
    {
        printf("Can't open encrypted file\n");
        return;
    }
    char ch;
    int key = 0;
    int cnt = 0;
    int value [100];
    while (!feof (fpdbr))
    {   
        if(key != 0 ) {
            fscanf (fpdbr, "%d", &value[cnt]);
            cnt++;
        }else{
            fscanf (fpdbr, "%d", &key);
        }     
    }

    char passwords[100];
    decrypt(passwords,key,value);
    puts(passwords);
    printf("\n");
        
    fclose(fpdbr); 
}
void decrypt(char password[100], int key, int value[100]) {
    int i = 0;
    while(value[i] != endingValue) {
        password[i] = (char)  (value[i] - key*keyMultiplier);
        i++;
    }
    password[i] = '\0';
}

void reverseString(char str[], int len) {
    for(int i = 0; i < len/2; i++) {
        char temp = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = temp;
    }
}


void lastIDFinder(char str[], int len, int* id, char serviceName[100], char userName[100]) {
    char str1[150];
    memset(str1, '\0', 150);
    int cnt = 0;
    for(int i = len-1; str[i-1] != '~'; i--) {
        str1[cnt] = str[i];
        cnt++;
    }
    
    
    reverseString(str1, strlen(str1));
    strcpy(str, str1);
    str[strlen(str)] = '\0';
    char ch_id[5];
    int i;
    for(i = 0; str[i] != '|'; i++) {
        ch_id[i] = str[i];
    }
    
    
    *id = atoi(ch_id);
    i++;
    cnt = 0;
    
    
    while(str[i] != '|') {
        serviceName[cnt] = str[i];
        cnt++;
        i++;
    }
    
    serviceName[cnt] = '\0';
    i++;
    cnt = 0;
    while(str[i] != '\n') {
        userName[cnt] = str[i];
        cnt++;
        i++;
    }
}


void lastID() {

    FILE *fpdbr;
    char identity[150];

    if ((fpdbr = fopen(fileName, "r")) != NULL) 
    {
        fseek(fpdbr, -150, SEEK_END);            
        fread(identity, 150-1, 1, fpdbr);         
        fclose(fpdbr);  
    }
    lastIDFinder(identity,strlen(identity), &db_id.id, db_id.serviceName, db_id.username);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#define MAX 1000

typedef struct REGISTRANT{
    char name[50];
    char pin[7];
    char phoneNum[20];
    char birthdate[20];
    int accountNo;
    int balance;
    int last;
}list;

list l;
list accountInfo[MAX];

char reg_Name[50];
char reg_phoneNum[20];
char reg_birthdate[20];
char reg_Pin[7];
char confirmPin[7];
char* checkDrives[2] = {"D:\\checkDB.dbf", "E:\\checkDB.dbf"};
char* drives[2] = {"D:\\bank.dbf","E:\\bank.dbf"};
int reg_PinInt, reg_AccountNo, reg_Bal, ctr = -1, exist = 0, checkName = 0, checkPin = 0, pinCharacter = 0;

void makenull();
int insertCard();
void retrieve();
int checkFd();
void registration();
int isEmpty();
int isFull();
void insert();
int locate(int x);
void save();
void saveToCard();

int main(){
    int i;
    system("color 02");
    makenull();
    if(insertCard()){
        _getch();
        system("cls");
        retrieve();
        registration();
    }
    i = remove(checkDrives[ctr]);
     printf("Flores, Bastian Bragi M.   BSIS - 2AB\n");
    printf("DATA STRUCTURE - PRELIMS PROJECT\n");
    printf("Registration Module\n");
    _getch();
    return 0;
}

void makenull(){
    l.last = -1;
}

int insertCard(){
    FILE *fp;
    printf("\t\t\t\t\t=======================================\n");
    printf("\t\t\t\t\t| < < < Please Insert Your Card > > > |\n");
    printf("\t\t\t\t\t=======================================\n\n");
    do{
        ctr = (ctr + 1) % 2;
        fp = fopen(checkDrives[ctr],"w");
    }while(fp == NULL);
    fclose(fp);

    fp = fopen(drives[ctr],"r");
    if(fp == NULL){
        printf("\t\t\t\t\t          Not Yet Registered         \n");
        printf("\n\n\n\t\t\t\t\t        PRESS ANY KEY . . .");
        return 1;
    }
    else{
        printf("\t\t\t\t\t          Already Registered         \n");
        return 0;
    }
    fclose(fp);
}

int checkFd(){
    FILE *fp;
    fp = fopen(checkDrives[ctr],"r");
    if(fp == NULL)
        return 1;
    return 0;
    fclose(fp);
}

void retrieve(){
    FILE *fp;
    int i = 0;
    fp = fopen("accountDB.dbf","r+");

    if (fp==NULL){
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|   XxxX   DATABASE IS EMPTY    XxxX  |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        _getch();
        system("cls");
    }
    else{
        while(!feof(fp)){
            l.last++;
            fscanf(fp,"%d %[^\t]%d %[^\t] %[^\t] %[^\n]",&accountInfo[l.last].accountNo, accountInfo[l.last].name, &accountInfo[l.last].balance, accountInfo[l.last].phoneNum, accountInfo[l.last].birthdate, accountInfo[l.last].pin);
        }
    }
    fclose(fp);
}

void registration(){
    int end;
    do{
        end = 1;
        l.last++;
        checkName = 0;
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|        R E G I S T R A T I O N      |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        printf("\n\t\t\t\tNAME: ");
        scanf(" %[^\n]s",reg_Name);
        strlwr(reg_Name);
        do{
            srand(time(0));
            reg_AccountNo = 10000+(rand()%90000);
        }while(locate(1));
        printf("\t\t\t\tACCOUNT NUMBER: %d",reg_AccountNo);
        printf("\n\t\t\t\tPHONE NUMBER: ");
        scanf(" %[^\n]s",reg_phoneNum);
        strlwr(reg_phoneNum);
        printf("\t\t\t\tBIRTHDAY: ");
        scanf(" %[^\n]s",reg_birthdate);
        strlwr(reg_birthdate);

       do{
            printf("\t\t\t\tINITIAL DEPOSIT (min. 5000): ");
            scanf("%d",&reg_Bal);
            if(reg_Bal < 5000){
                printf("\n\n\t\t\t\tSorry Initial Deposit Must be 5000 Above.\n");
            }
            else{
                break;
            }
        }while(1);

        do{
            printf("\n\t\t\t\t(6-DIGIT)PIN NUMBER: ");
            checkPin = 0;
            pinCharacter = 0;
            for(int i = 0; i < 6; i++){
                reg_Pin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            reg_Pin[6] = '\0';
            printf("\n\t\t\t\t(6-DIGIT)CONFIRM PIN: ");
            for(int i = 0; i < 6; i++){
                confirmPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            confirmPin[6] = '\0';

            end = strcmp(confirmPin,reg_Pin);

            for(int i = 0; i < 6; i++){
                if(isdigit(confirmPin[i] - 9)  == 0){
                    pinCharacter = 1;
                    break;
                }
            }

            if(end != 0){
                printf("\n\n\t\t\t\tYour PIN does Not MATCH\n");
                printf("\n\n\t\t\t\tPLEASE PRESS ANY KEY\n");
                _getch();
            }

            else if(locate(0) && checkPin == 1){
                printf("\n\n\t\t\t\tYour PIN is Not Unique\n");
                _getch();
            }
            else if(pinCharacter == 1){
                printf("\n\n\t\t\t\tYour PIN Has a Character.\n");
                _getch();
            }

        }while(end != 0 || checkPin || pinCharacter);

        if(locate(0) && checkName == 1){
            l.last--;
            printf("\n\n\t\t\t\tThis Account Already Exists!");
            _getch();
            system("cls");
        }

    }while(checkName);

    if(isFull()){
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|  < < <    DATABASE IS FULL   > > >  |\n");
        printf("\t\t\t\t\t=======================================\n\n");
    }
    else
        insert();
}

int isEmpty(){
    return(l.last == 0);
}

int isFull(){
    return(l.last == MAX);
}

void insert(){
    if(checkFd()){
        printf("\n\n");
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|  < < <   FLASH DRIVE ERROR   > > >  |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        exit(0);
    }
    strcpy(accountInfo[l.last].name,reg_Name);
    strcpy(accountInfo[l.last].phoneNum,reg_phoneNum);
    strcpy(accountInfo[l.last].birthdate,reg_birthdate);
    accountInfo[l.last].accountNo = reg_AccountNo;
    accountInfo[l.last].balance = reg_Bal;
    strcpy(accountInfo[l.last].pin,reg_Pin);
    printf("\n\n");
    printf("\t\t\t\t\t=======================================\n");
    printf("\t\t\t\t\t| < <   REGISTERED SUCCESSFULLY   > > |\n");
    printf("\t\t\t\t\t=======================================\n\n");
    save();
    saveToCard();
}

int locate(int x){
    if(x == 0){
        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].name,reg_Name) == 0 || strcmp(accountInfo[i].pin,reg_Pin) == 0){
                if(strcmp(accountInfo[i].name,reg_Name) == 0)
                    checkName = 1;
                if(strcmp(accountInfo[i].pin,reg_Pin) == 0)
                    checkPin = 1;
                return 1;
            }
        }
        return 0;
    }
    else if(x == 1){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].accountNo == reg_AccountNo)
                return 1;
        }
        return 0;
   }
}

void save(){
    FILE *fp;
    fp = fopen("accountDB.dbf","w");
    for(int i = 0; i <= l.last; i++){
        fprintf(fp,"%d\t%s\t%d\t%s\t%s\t%s",accountInfo[i].accountNo,accountInfo[i].name,accountInfo[i].balance, accountInfo[i].phoneNum, accountInfo[i].birthdate, accountInfo[i].pin);
        if(i != l.last)
            fprintf(fp,"\n");
    }
    fclose(fp);
}

void saveToCard(){
    FILE *fp;
    fp = fopen(drives[ctr],"w");
    fprintf(fp,"%d\t%s",reg_AccountNo,reg_Pin);
    fclose(fp);
}


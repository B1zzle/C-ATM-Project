#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
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

char inputPin[7];
char reg_Pin[7];
char confirmPin[7];
char pinFromCard[7];
char* checkDrives[2] = {"D:\\checkFd.dbf", "E:\\checkFd.dbf"};
char* drives[2] = {"D:\\bank.dbf","E:\\bank.dbf"};
int accountNoFromCard, reg_AccountNo, reg_Bal, accountIndex, otherAccIndex, ctr = -1, exist = 0, checkName = 0, checkPin = 0, pinCharacter = 0;

void makenull();
void retrieve();
int insertCard();
int checkFd();
void pinEnter();
int transaction();
int isEmpty();
int isFull();
void insert();
int locate();
void save();
void saveToCard();
void checkPinFromCard();
int confirmTransaction(int x, int y, int len);

void gotoxy(int x, int y);
COORD coord = {0,0};
void clear(char *label);
void printToxy(int x, int y, char* label);

int main()
{
    int i;
    system("color 02");
    makenull();
    retrieve();
    if(insertCard()){
        pinEnter();
        accountIndex = locate(1);
        do{
            _getch();
            system("cls");
            system("color 02");

        }while(transaction());
    }
    i = remove(checkDrives[ctr]);
    printf("Flores, Bastian Bragi M.   BSIS - 2AB\n");
    printf("DATA STRUCTURE - PRELIMS PROJECT\n");
    printf("Transaction Module\n");
    _getch();
    return 0;
}

void makenull(){
    l.last = -1;
}

void retrieve(){
    FILE *fp;
    int i = 0;
    fp = fopen("accountDB.dbf","r+");

    if (fp==NULL){
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|   XxxX   DATABASE IS EMPTY    XxxX  |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        printf("\t\t\t\t\t          Must Register First.");
        _getch();
        exit(0);
    }
    else{
        while(!feof(fp)){
            l.last++;
            fscanf(fp,"%d %[^\t]%d %[^\t] %[^\t] %[^\n]",&accountInfo[l.last].accountNo,accountInfo[l.last].name,&accountInfo[l.last].balance,accountInfo[l.last].phoneNum, accountInfo[l.last].birthdate, accountInfo[l.last].pin);
        }
    }
    fclose(fp);
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
        printf("\t\t\t\t\t          Not Yet Registered         \n\n");

        return 0;
    }
    else{
        while(!feof(fp))
            fscanf(fp,"%d %[^\n]",&accountNoFromCard,pinFromCard);

        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].pin,pinFromCard) == 0 && accountInfo[i].accountNo == accountNoFromCard){
                printf("\t\t\t\t\t          Already Registered         \n\n");
                return 1;
            }
        }
        printf("\t\t\t\t\t            Not Registered           \n\n");
        return 0;
    }
    fclose(fp);
}

int checkFd(){
    FILE *fp;
    fp = fopen(checkDrives[ctr],"r");
    if(fp == NULL){
        system("47");
        printf("\n\n");
        printf("\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|  < < <   FLASH DRIVE ERROR   > > >  |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        return 1;
    }
    return 0;
    fclose(fp);
}

void pinEnter(){
    int i = 0;
    do{
        system("COLOR 02");
        if(i == 3){
            printf("\n\t\t\t\t\t     LOGGING OUT . . .");
            exit(0);
        }
        printf("\t\t\t\t\t      PIN NUMBER: ");
        for(int i = 0; i < 6; i++){
            inputPin[i] = _getch() + 9;
            putchar('*');
            putchar(' ');
        }
        inputPin[6] = '\0';

        if(strcmp(inputPin,pinFromCard) != 0){

        system("COLOR 47");
        printf("\n\t\t\t\t\t            Invalid Pin              \n\n");

            _getch();
        }
        i++;
    }while(strcmp(inputPin,pinFromCard) != 0);
    printf("\n\n\t\t\t\t\t    ");
    system("pause");
}

int locate(int x){

    if(x == 0){
        for(int i = 0; i < l.last + 1; i++){
            if(strcmp(accountInfo[i].pin,reg_Pin) == 0){
                checkPin = 1;
                return 1;
            }
        }
        return 0;
    }
    else if(x == 1){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].accountNo == accountNoFromCard)
                return i;
        }
        return 0;
   }
   else if(x == 2){
        for(int i = 0; i < l.last + 1; i++){
            if(accountInfo[i].accountNo == reg_AccountNo){
                otherAccIndex = i;
                return 0;
            }
        }
        return 1;
   }
}

int transaction(){
    char choice;
    int end;
    printf("\t\t\t\t\t======================================\n");
    printf("\t\t\t\t\t|         Welcome to TUP BANK        |\n");
    printf("\t\t\t\t\t|                                    |\n");
    printf("\t\t\t\t\t|        T R A N S A C T I O N       |\n");
    printf("\t\t\t\t\t======================================");
    printf("\n\t\t\t\t\t|\t                             |");
    printf("\n\t\t\t\t\t|\t PRESS                       |");
    printf("\n\t\t\t\t\t|\t  [1]Balance Inquiry         |");
    printf("\n\t\t\t\t\t|\t  [2]Deposit                 |");
    printf("\n\t\t\t\t\t|\t  [3]Withdraw                |");
    printf("\n\t\t\t\t\t|\t  [4]Transfer                |");
    printf("\n\t\t\t\t\t|\t  [5]Change PIN              |");
    printf("\n\t\t\t\t\t|\t  [6]Exit Program            |");
    printf("\n\t\t\t\t\t|\t                             |");
    printf("\n\t\t\t\t\t======================================\n\n");
    printf("\n\t\t\t\t\t\t Enter choice : ");
    scanf(" %d",&choice);
    if(choice == 3){
        if(accountInfo[accountIndex].balance != 0){
            do{
                system("color 02");
                printf("\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|          AMOUNT OF WITHDRAW         |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                printf("\t\t\t\t\t  Enter Amount: Php  ");
                scanf("%d",&reg_Bal);
                if(reg_Bal % 100 != 0){
                    printf("\n\n\t\t\t\t\tPLEASE ENTER THE AMOUNT IN MULTIPLES OF 100,");
                    printf("\n\t\t\t\t\tATM MACHINE CAN ONLY DISPENSE 100, 500, 1000.\n");
                    _getch();
                }
                else if(reg_Bal > accountInfo[accountIndex].balance){
                    system("E0");
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                AMOUNT               |\n");
                    printf("\t\t\t\t\t|         GREATER THAN BALANCE        |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                    _getch();
                }
                else if(reg_Bal < 0){
                    system("color E0");
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|           NEGATIVE BALANCE          |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                    _getch();
                }
                else if(reg_Bal == 0){
                    system("color 47");
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|               INVALID!              |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                    _getch();
                }
            }while(reg_Bal % 100 != 0 || reg_Bal > accountInfo[accountIndex].balance || reg_Bal < 0 || reg_Bal == 0);
            if(confirmTransaction(44,11,76)){
                if(checkFd()){
                    exit(0);
                }
                accountInfo[accountIndex].balance -= reg_Bal;
                system("color A0");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|        SUCCESSFULLY WITHDRAWN!      |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
            }
            else{
                system("color 47");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|              CANCELLED              |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
            }
        }
        else{
            printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|              NO BALANCE             |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t=======================================\n\n");
        }
    }
    else if(choice == 2){
        do{
            system("color 02");
            printf("\n\t\t\t\t\t=======================================\n");
            printf("\t\t\t\t\t|          AMOUNT OF DEPOSIT          |\n");
            printf("\t\t\t\t\t=======================================\n");
            printf("\t\t\t\t\tEnter Amount: Php  ");
            scanf("%d",&reg_Bal);
            if(reg_Bal < 0){
                system("color E0");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|           NEGATIVE BALANCE          |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                _getch();
            }
            else if(reg_Bal == 0){
                 system("color 47");
                 printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                 printf("\t\t\t\t\t|                                     |\n");
                 printf("\t\t\t\t\t|               INVALID!              |\n");
                 printf("\t\t\t\t\t|                                     |\n");
                 printf("\t\t\t\t\t=======================================\n\n");
                _getch();
            }
        }while(reg_Bal < 0 || reg_Bal == 0);

        if(confirmTransaction(44,11,76)){
                if(checkFd()){
                    exit(0);
                }
                system("color A0");
                accountInfo[accountIndex].balance += reg_Bal;
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|        SUCCESSFULLY DEPOSITED!      |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
            }
        else{
            system("color 47");
            printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|              CANCELLED              |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t|                                     |\n");
            printf("\t\t\t\t\t=======================================\n\n");
        }
    }
    else if(choice == 1){
        if(checkFd()){
            exit(0);
        }
        system("color 02");
        printf("\n\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|           BALANCE INQUIRY           |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        printf("\t\t\t\t\t     Your Balance is : Php  %d", accountInfo[accountIndex].balance);
    }

    else if(choice == 4){
        if(l.last > 0){
            if(accountInfo[accountIndex].balance != 0){
                do{
                    system("color 02");
                    otherAccIndex = -1;
                    printf("\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|            FUND TRANSFER            |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                    printf("\t\t\t\t Enter the Account No. you want to Transfer your money to:  ");
                    scanf("%d",&reg_AccountNo);
                    if(locate(2)){
                        system("color E0");
                        printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t|          ACCOUNT NUMBER DOES        |\n");
                        printf("\t\t\t\t\t|               NOT EXIST             |\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t=======================================\n\n");
                        _getch();
                    }
                    else if(accountInfo[accountIndex].accountNo == accountInfo[otherAccIndex].accountNo){
                        otherAccIndex = -1;
                        system("color 47");
                        printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t|               INVALID               |\n");
                        printf("\t\t\t\t\t|            ACCOUNT NUMBER           |\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t|                                     |\n");
                        printf("\t\t\t\t\t=======================================\n\n");
                        _getch();
                    }
                    else{
                        do{
                            system("color 02");
                            printf("\n\t\t\t\t\t=======================================\n");
                            printf("\t\t\t\t\t|            FUND TRANSFER            |\n");
                            printf("\t\t\t\t\t=======================================\n\n");
                            printf("\t\t\t\t\t  Enter Amount Balance:  ");
                            scanf("%d",&reg_Bal);
                            if(reg_Bal > accountInfo[accountIndex].balance){
                                system("color E0");
                                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t|         DOES NOT HAVE ENOUGH        |\n");
                                printf("\t\t\t\t\t|               BALANCE               |\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t|=======================================\n\n");
                                _getch();
                            }
                            else if(reg_Bal < 0){
                                system("color C7");
                                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                                printf("\t\t\t\t\t|                                      |\n");
                                printf("\t\t\t\t\t|                NEGATIVE              |\n");
                                printf("\t\t\t\t\t|                 ENTRY                |\n");
                                printf("\t\t\t\t\t|                                      |\n");
                                printf("\t\t\t\t\t|                                      |\n");
                                printf("\t\t\t\t\t=======================================\n\n");
                                _getch();
                            }
                            else if(reg_Bal == 0){
                                system("color 47");
                                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t|               INVALID!              |\n");
                                printf("\t\t\t\t\t|                                     |\n");
                                printf("\t\t\t\t\t=======================================\n\n");
                                _getch();
                            }
                        }while(reg_Bal < 0 || reg_Bal > accountInfo[accountIndex].balance || reg_Bal == 0);
                    }
                }while(otherAccIndex == -1);
                if(confirmTransaction(44,11,76)){
                    if(checkFd()){
                        exit(0);
                    }
                    system("color A0");
                    accountInfo[accountIndex].balance -= reg_Bal;
                    accountInfo[otherAccIndex].balance += reg_Bal;
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|       SUCCESSFULLY TRANSFERRED!     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                }
                else{
                    system("color 47");
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|              CANCELLED              |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                }
            }
            else{
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|              NO BALANCE             |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
            }
        }
        else if(l.last == 0){
                system("color E0");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|         THERE IS NO ACCOUNT         |\n");
                printf("\t\t\t\t\t|             IN THE LIST             |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
        }
    }

    else if(choice == 5){
        do{
            if(checkFd()){
                exit(0);
            }
            checkPinFromCard();
            do{
                system("color 02");
                printf("\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|            CHANGE PIN               |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                printf("\t\t\t\t\t  Enter Current PIN:  ");

                for(int i = 0; i < 6; i++){
                    inputPin[i] = _getch() + 9;
                    putchar('*');
                    putchar(' ');
                }
                printf("\n\t\t\t\t\t=======================================\n");
                if(strcmp(inputPin,pinFromCard) != 0){
                    system("color 47");
                    printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t|               INVALID               |\n");
                    printf("\t\t\t\t\t|                 PIN                 |\n");
                    printf("\t\t\t\t\t|                                     |\n");
                    printf("\t\t\t\t\t=======================================\n\n");
                    _getch();
                }
            }while(strcmp(inputPin,pinFromCard) != 0);
            printf("\t\t\t\t\t  Enter new PIN:  ");
            for(int i = 0; i < 6; i++){
                reg_Pin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            reg_Pin[6] = '\0';
            printf("\n\t\t\t\t\t  Confirm new PIN:  ");
            for(int i = 0; i < 6; i++){
                confirmPin[i] = _getch() + 9;
                putchar('*');
                putchar(' ');
            }
            confirmPin[6] = '\0';

            end = strcmp(confirmPin,reg_Pin);

            checkPin = 0;
            pinCharacter = 0;

            for(int i = 0; i < 6; i++){
                if(isdigit(confirmPin[i] - 9)  == 0){
                    pinCharacter = 1;
                    break;
                }
            }

            if(end != 0){
                system("color E0");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|           PIN DID NOT MATCH         |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                _getch();
            }

            else if(locate(0) && checkPin == 1){
                system("color 90");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|        YOUR PIN IS TOO COMMON       |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                _getch();
            }

            else if(pinCharacter == 1){
                system("color C7");
                printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t|               YOUR PIN HAS          |\n");
                printf("\t\t\t\t\t|               A CHARACTER           |\n");
                printf("\t\t\t\t\t|                                     |\n");
                printf("\t\t\t\t\t=======================================\n\n");
                _getch();
            }

        }while(end != 0 || checkPin || pinCharacter);
        if(checkFd()){
            exit(0);
        }
        system("cls");
        system("color A0");
        strcpy(accountInfo[accountIndex].pin,reg_Pin);
        printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|        SUCCESSFULLY CHANGED!        |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        saveToCard();
    }

    else if(choice == 6){
        system("color 02");
        printf("\n\t\t\t\t       Thank You For Using TUP BANK ATM SERVICE\n\n\n");
        return 0;
    }
    else{
        system("color 47");
        printf("\n\n\n\n\n\n\n\t\t\t\t\t=======================================\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|            INVALID CHOICE           |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t|                                     |\n");
        printf("\t\t\t\t\t=======================================\n\n");
        _getch();
    }
    save();
    return 1;
}

void checkPinFromCard(){
    FILE* fp;
    fp = fopen(drives[ctr],"r");
    while(!feof(fp))
            fscanf(fp,"%d %[^\n]",&accountNoFromCard,pinFromCard);
}

int confirmTransaction(int x, int y, int len){
    char input;
    for(int i = y; i < y + 8; i++)
        printToxy(x,i,"                                ");
    for(int i = x; i < len; i++){
        printToxy(i,y - 1,"_");
        printToxy(i,y + 2,"_");
        printToxy(i, y + 7,"_");
    }
    for(int i = y; i < y + 8; i++){
        printToxy(x - 1,i,"|");
        printToxy(len,i,"|");
    }
    printToxy(x + 6,y + 1,"CONFIRM TRANSACTION?");
    printToxy(x + 5,y + 5,"[Y] YES       [N] NO");
    do{
        input = _getch();
        if(input == 'Y' || input == 'y' || input == 'N' || input == 'n'){
            system("cls");
            if(input == 'Y' || input == 'y')
                return 1;
            else if(input == 'N' || input == 'n')
                return 0;
        }
    }while(input != 'Y' || input != 'y' || input != 'N' || input != 'n');
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
    fprintf(fp,"%d\t%s",accountInfo[accountIndex].accountNo,reg_Pin);
    fclose(fp);
}

//FOR UI DESIGN
void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void printToxy(int x, int y, char* label){
    gotoxy(x,y);
    printf("%s",label);
}

void clear(char *label){
    if(strcmp(label,"result") == 0){
        printToxy(25,17,"                                                       ");
        printToxy(53,17,"- - -");
    }
    else if(strcmp(label,"result2") == 0){
        printToxy(32,22,"                                                       ");
        printToxy(57,22,"- - -");
    }
    else if(strcmp(label,"enter") == 0)
        printToxy(31,18,"                                                       ");
    else if(strcmp(label,"pinBoxes") == 0)
        printToxy(52, 13,"                 ");
}

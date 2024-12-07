#include <stdio.h>
#include <string.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();

const char* ACCOUNT_FILE = "account.dat";

typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account;

int main() {
    while (1) {
        int choice;
        printf("\n\n*** Bank Management System ***");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Check Balance");
        printf("\n5. Exit");
        printf("\nEnter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_account();
                break;
            case 2:
                deposit_money();
                break;
            case 3:
                withdraw_money();
                break;
            case 4:
                check_balance();
                break;
            case 5:
                printf("\nClosing The Bank, Thanks For Your Visit.\n");
                return 0;
            default:
                printf("\nInvalid Choice! Please Try Again.\n");
        }
    }
}

void create_account() {
    Account acc;
    FILE* file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL) {
        printf("\nUnable To Open File!\n");
        return;
    }

    getchar(); // Clear buffer for fgets
    printf("\nEnter Your Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0'; // Remove newline character
    printf("\nEnter Your Account Number: ");
    scanf("%d", &acc.acc_no);
    acc.balance = 0;

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);
    printf("\nAccount Created Successfully!\n");
}

void deposit_money() {
    FILE* file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable To Open Account File!\n");
        return;
    }

    int acc_no;
    float money;
    Account acc_r;
    printf("\nEnter Your Account Number: ");
    scanf("%d", &acc_no);
    printf("\nEnter Amount To Deposit: ");
    scanf("%f", &money);

    while (fread(&acc_r, sizeof(acc_r), 1, file)) {
        if (acc_r.acc_no == acc_no) {
            acc_r.balance += money;
            fseek(file, -sizeof(acc_r), SEEK_CUR);
            fwrite(&acc_r, sizeof(acc_r), 1, file);
            fclose(file);
            printf("\nSuccessfully Deposited Rs.%.2f\nNew Balance Is Rs.%.2f\n", money, acc_r.balance);
            return;
        }
    }

    fclose(file);
    printf("\nAccount Number %d Not Found In Records.\n", acc_no);
}

void withdraw_money() {
    FILE* file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable To Open Account File!\n");
        return;
    }

    int acc_no;
    float money;
    Account acc_r;
    printf("\nEnter Your Account Number: ");
    scanf("%d", &acc_no);
    printf("\nEnter The Amount You Wish To Withdraw: ");
    scanf("%f", &money);

    while (fread(&acc_r, sizeof(acc_r), 1, file)) {
        if (acc_r.acc_no == acc_no) {
            if (acc_r.balance >= money) {
                acc_r.balance -= money;
                fseek(file, -sizeof(acc_r), SEEK_CUR);
                fwrite(&acc_r, sizeof(acc_r), 1, file);
                fclose(file);
                printf("\nSuccessfully Withdrawn Rs.%.2f\nRemaining Balance Is Rs.%.2f\n", money, acc_r.balance);
                return;
            } else {
                printf("\nInsufficient Balance!\n");
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
    printf("\nAccount Number %d Not Found In Records.\n", acc_no);
}

void check_balance() {
    FILE* file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable To Open File!\n");
        return;
    }

    int acc_no;
    Account acc_read;
    printf("\nEnter Your Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&acc_read, sizeof(acc_read), 1, file)) {
        if (acc_read.acc_no == acc_no) {
            printf("\nYour Current Balance Is Rs.%.2f\n", acc_read.balance);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("\nAccount Number %d Not Found In Records.\n", acc_no);
}


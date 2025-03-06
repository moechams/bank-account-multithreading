#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_ACCOUNTS 10
#define MAX_CLIENTS 10
#define MAX_TRANSACTIONS 10
#define BUFFER_SIZE 100

typedef struct {
    int balance;
    pthread_mutex_t mutex;
} Account;

typedef struct {
    char type; //'D' for deposit, 'W' for withdrawal
    int account_id;
    int amount;
} Transaction;

typedef struct {
    int num_transactions;
    Transaction transactions[MAX_TRANSACTIONS];
} Client;

Account accounts[MAX_ACCOUNTS];
Client clients[MAX_CLIENTS];
int num_accounts = 0, num_clients = 0;

void* process_transactions(void* arg) {
    int client_id = *(int*)arg;
    for (int i = 0; i < clients[client_id].num_transactions; i++) {
        Transaction *t = &clients[client_id].transactions[i];
        pthread_mutex_lock(&accounts[t->account_id].mutex);

        if (t->type == 'D' || t->type == 'd') { //deposit
            accounts[t->account_id].balance += t->amount;
            printf("Client %d deposited %d to account A%d. New balance: %d\n", client_id + 1, t->amount, t->account_id + 1, accounts[t->account_id].balance);
        } else if ((t->type == 'W' || t->type == 'w') && accounts[t->account_id].balance >= t->amount) { //withdraw
            accounts[t->account_id].balance -= t->amount;
            printf("Client %d withdrew %d from account A%d. New balance: %d\n", client_id + 1, t->amount, t->account_id + 1, accounts[t->account_id].balance);
        } else if (t->type == 'W' || t->type == 'w') {
            printf("Client %d attempted to withdraw %d from account A%d. Insufficient funds.\n", client_id + 1, t->amount, t->account_id + 1);
        }

        pthread_mutex_unlock(&accounts[t->account_id].mutex);
    }
    return NULL;
}


int main() {
    FILE *file = fopen("assignment_5_input.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'A') {
            int account_id, balance;
            if (sscanf(line, "A%d balance %d", &account_id, &balance) == 2) {
                accounts[num_accounts].balance = balance;
                pthread_mutex_init(&accounts[num_accounts].mutex, NULL);
                printf("Account %d initialized with balance %d\n", account_id, balance);
                num_accounts++;
            } else {
                fprintf(stderr, "Error parsing account line: %s", line);
            }
        } else if (line[0] == 'C') {
            int client_id;
            if (sscanf(line, "C%d", &client_id) == 1) {
                client_id--; //adjust for zero-based indexing
                Client *client = &clients[client_id];

                char *ptr = strtok(line, " "); //first token (Client ID)
                while (ptr != NULL) {
                    ptr = strtok(NULL, " "); // get the next token
                    if (ptr == NULL || *ptr == '\n') break; // end of line

                    char type = ptr[0]; //operation type (D/W)

                    ptr = strtok(NULL, " "); //get the account ID
                    if (ptr == NULL || *ptr == '\n') break; //end of line

                    int account_id;
                    sscanf(ptr, "A%d", &account_id);
                    account_id--; // adjust for zero-based indexing

                    ptr = strtok(NULL, " "); // get the amount
                    if (ptr == NULL || *ptr == '\n') break; //end of line

                    int amount;
                    sscanf(ptr, "%d", &amount);

                    Transaction t = {type, account_id, amount};
                    client->transactions[client->num_transactions++] = t;
                    printf("Client %d transaction added: %c A%d %d\n", client_id + 1, type, account_id + 1, amount);
                }
                num_clients++;
            } else {
                fprintf(stderr, "Error parsing client line: %s\n", line);
            }
        }
    }
    fclose(file);

    //create and join threads
    pthread_t threads[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];
    for (int i = 0; i < num_clients; i++) {
        client_ids[i] = i;
        pthread_create(&threads[i], NULL, process_transactions, &client_ids[i]);
    }
    for (int i = 0; i < num_clients; i++) {
        pthread_join(threads[i], NULL);
    }

    //output results
    printf("No. of Accounts: %d\n", num_accounts);
    printf("No. of Clients: %d\n", num_clients);
    for (int i = 0; i < num_accounts; i++) {
        printf("A%d balance %d\n", i + 1, accounts[i].balance);
    }

    for (int i = 0; i < num_accounts; i++) {
        pthread_mutex_destroy(&accounts[i].mutex);
    }

    return 0;
}


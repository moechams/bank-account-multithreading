# Bank Account Thread Synchronization

A C program that implements mutual exclusion and thread synchronization for a banking system, developed for CS3305 Operating Systems course at Western University.

## Project Overview

This program simulates a banking system where multiple clients (implemented as threads) perform concurrent transactions (deposits and withdrawals) on shared bank accounts. The implementation uses mutex locks to ensure that only one thread can access a bank account at a time, preventing race conditions.

## Features

- **Multithreaded Design**: Each client operates as a separate thread
- **Mutex Synchronization**: Prevents race conditions when accessing shared resources
- **Transaction Processing**: Supports deposit and withdrawal operations
- **Input File Parsing**: Reads account information and client transactions from a file
- **Transaction Validation**: Prevents withdrawals that would result in negative balances

## Implementation Details

### Data Structures

- `Account`: Represents a bank account with a balance and a mutex for thread synchronization
- `Transaction`: Represents a single transaction with type (deposit/withdrawal), account ID, and amount
- `Client`: Represents a client with a list of transactions to perform

### Synchronization Mechanism

The program uses POSIX thread mutexes (`pthread_mutex_t`) to ensure that only one thread can access an account at a time. Before performing a transaction on an account, the thread locks the account's mutex, performs the transaction, and then unlocks the mutex.

```c
pthread_mutex_lock(&accounts[t->account_id].mutex);
// Perform transaction
pthread_mutex_unlock(&accounts[t->account_id].mutex);
```

This approach prevents race conditions where multiple threads might try to modify an account's balance simultaneously, potentially leading to incorrect results.

## How It Works

1. The program reads account information and client transactions from a file named "assignment_5_input.txt"
2. For each client, it creates a separate thread to process that client's transactions
3. Each thread processes its client's transactions sequentially, using mutex locks to ensure exclusive access to accounts
4. After all threads have completed, the program outputs the final account balances

## Sample Input Format

```
A1 balance 5000
A2 balance 5000
A3 balance 5000
A4 balance 5000
A5 balance 5000
C1 deposit A2 1000
C2 withdraw A1 300 deposit A4 200
C3 deposit A3 500 withdraw A4 400 withdraw A1 100
C4 withdraw A1 40000 withdraw A2 800
C5 withdraw A5 5000
```

## Sample Output

```
No. of Accounts: 5
No. of Clients: 5
A1 balance 4600
A2 balance 5200
A3 balance 5500
A4 balance 4800
A5 balance 0
```

## Compilation and Execution

To compile the program:
```
gcc -o bank_system assignment5.c -pthread
```

To run the program:
```
./bank_system
```

Note: The program expects an input file named "assignment_5_input.txt" to be present in the same directory.

## Educational Value

This project demonstrates several important concepts in operating systems and concurrent programming:

- **Thread Creation and Management**: Using POSIX threads to model concurrent clients
- **Mutual Exclusion**: Preventing race conditions in critical sections
- **Resource Synchronization**: Ensuring safe access to shared resources (bank accounts)
- **File I/O in C**: Reading and parsing structured input from files
- **Concurrent Transaction Processing**: Handling multiple operations happening in parallel

## Skills Demonstrated

- C programming
- POSIX threads (pthreads)
- Mutex synchronization
- File parsing
- Concurrent programming

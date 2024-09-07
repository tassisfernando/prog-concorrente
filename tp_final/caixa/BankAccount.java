package caixa;

class BankAccount {
    private int balance;

    public BankAccount(int balance) {
        this.balance = balance;
    }

    public synchronized void deposit(int amount) {
        balance += amount;
        System.out.println("Depósito realizado: " + amount + ", Saldo atual: " + balance);
    }

    public synchronized boolean withdraw(int amount) {
        if (balance >= amount) {
            balance -= amount;
            System.out.println("Saque realizado: " + amount + ", Saldo restante: " + balance);
            return true;
        }
        System.out.println("Saldo insuficiente para realizar o saque de: " + amount);
        return false;
    }

    public int getBalance() {
        return balance;
    }
}


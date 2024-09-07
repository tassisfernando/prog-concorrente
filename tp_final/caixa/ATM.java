package caixa;

class ATM extends Thread {
    private BankAccount account;
    private int amount;

    public ATM(BankAccount account, int amount) {
        this.account = account;
        this.amount = amount;
    }

    @Override
    public void run() {
        if (!account.withdraw(amount)) {
           account.deposit(amount);
        }
    }
}


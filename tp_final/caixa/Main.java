package caixa;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        BankAccount account = new BankAccount(1000);

        // Vários ATMs tentando sacar dinheiro simultaneamente
        ATM atm1 = new ATM(account, 700);
        ATM atm2 = new ATM(account, 500);
        ATM atm3 = new ATM(account, 200);

        // Iniciar os ATMs
        atm1.start();
        atm2.start();
        atm3.start();

        // Esperar que todos os ATMs terminem
        atm1.join();
        atm2.join();
        atm3.join();
    }
}

class Main2 {
    public static void main(String[] args) throws InterruptedException {
        final int SIZE = 1000000;
        BankAccount account = new BankAccount(SIZE);


        // Cria 1000 ATMs que tentam sacar 1 cada
        List<ATM> atms = new ArrayList<>(SIZE);
        for (int i = 0; i < SIZE; i++) {
            atms.add(new ATM(account, 1));
        }

        atms.parallelStream().forEach(ATM::start);

//        // Espera todos os ATMs terminarem
//        for (ATM atm : atms) {
//            atm.join();
//        }

        System.out.println("Saldo final: " + account.getBalance());
    }
}

package pool;

import java.util.LinkedList;

/**
 * A classe FilaTarefas gerencia um pool de threads de trabalho e uma fila de tarefas a serem executadas por essas threads.
 */
class FilaTarefas {

    private final int nThreads; // Número de threads no pool
    private final MyPoolThreads[] threads; // Array de objetos MyPoolThreads
    private final LinkedList<Runnable> queue; // Fila de tarefas
    private boolean shutdown; // Flag para indicar se o pool está sendo desligado

    /**
     * Construtor que inicializa o pool de threads com o número especificado de threads.
     *
     * @param nThreads Número de threads no pool
     */
    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i = 0; i < nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        }
    }

    /**
     * Adiciona uma nova tarefa à fila e notifica uma thread em espera.
     *
     * @param r A tarefa a ser adicionada
     */
    public void execute(Runnable r) {
        synchronized (queue) {
            if (this.shutdown) {
                return;
            }
            queue.addLast(r);
            queue.notify();
        }
    }

    /**
     * Define o flag shutdown como verdadeiro e notifica todas as threads em espera.
     * Em seguida, aguarda que todas as threads terminem a execução.
     */
    public void shutdown() {
        synchronized (queue) {
            this.shutdown = true;
            queue.notifyAll();
        }
        for (int i = 0; i < nThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                return;
            }
        }
    }

    /**
     * Classe interna que representa uma thread de trabalho que continuamente busca tarefas na fila e as executa até que o pool seja desligado.
     */
    private class MyPoolThreads extends Thread {

        public void run() {
            Runnable r;
            while (true) {
                synchronized (queue) {
                    while (queue.isEmpty() && (!shutdown)) {
                        try {
                            queue.wait();
                        } catch (InterruptedException ignored) {
                        }
                    }
                    if (queue.isEmpty() && shutdown) {
                        return;
                    }
                    r = (Runnable) queue.removeFirst();
                }
                try {
                    r.run();
                } catch (RuntimeException e) {
                }
            }
        }
    }
}

/**
 * Classe Hello que implementa a interface Runnable e define uma tarefa que imprime uma mensagem no console.
 */
class Hello implements Runnable {

    String msg; // Mensagem a ser impressa

    /**
     * Construtor que inicializa a mensagem.
     *
     * @param m Mensagem a ser impressa
     */
    public Hello(String m) {
        msg = m;
    }

    /**
     * Método executado pela thread que imprime a mensagem no console.
     */
    public void run() {
        System.out.println(msg);
    }
}

/**
 * Classe Primo que implementa a interface Runnable e verifica se um dado número é primo.
 */
class Primo implements Runnable {
    private final int numero;

    /**
     * Construtor que inicializa o número a ser verificado.
     *
     * @param numero Número a ser verificado se é primo
     */
    public Primo(int numero) {
        this.numero = numero;
    }

    /**
     * Método executado pela thread que verifica se o número é primo e imprime o resultado no console.
     */
    public void run() {
        if (ehPrimo(numero)) {
            System.out.println(numero + " e primo.");
        } else {
            System.out.println(numero + " nao e primo.");
        }
    }

    /**
     * Método para determinar se um número é primo.
     *
     * @param n Número a ser verificado
     * @return true se o número for primo, false caso contrário
     */
    private boolean ehPrimo(long n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i <= Math.sqrt(n) + 1; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
}

/**
 * Classe da aplicação que contém o método main, demonstrando o uso do pool de threads FilaTarefas.
 */
class MyPool {

    private static final int NTHREADS = 10; // Número de threads no pool

    public static void main(String[] args) {
        // Cria o pool de threads
        FilaTarefas pool = new FilaTarefas(NTHREADS);

        // Dispara a execução dos objetos runnable usando o pool de threads
        for (int i = 0; i < 100; i++) {
            final String m = "Hello da tarefa " + i;
            Runnable hello = new Hello(m);
            pool.execute(hello);
            Runnable primo = new Primo(i);
            pool.execute(primo);
        }

        // Espera pelo término das threads
        pool.shutdown();
        System.out.println("Terminou");
    }
}
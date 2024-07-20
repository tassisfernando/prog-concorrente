// Classe que representa o vetor compartilhado
class SharedVector {
    private int[] vector;

    public SharedVector(int size) {
        this.vector = new int[size];
    }

    public synchronized void increment(int index) {
        this.vector[index] += 10;
    }

    public void print() {
        System.out.print("[ ");
        for (int i : this.vector) {
            System.out.print(i + " ");
        }
        System.out.println("]");
    }

    public int size() {
        return this.vector.length;
    }
}

// Classe que representa a tarefa de cada thread
class IncrementTask extends Thread {
    private SharedVector sharedVector;
    private int start;

    public IncrementTask(SharedVector sharedVector, int start) {
        this.sharedVector = sharedVector;
        this.start = start;
    }

    @Override
    public void run() {
        for (int i = start; i < sharedVector.size(); i += 2) {
            sharedVector.increment(i);
        }
    }
}

// Classe principal do programa
public class Atividade05 {
    public static void main(String[] args) throws InterruptedException {
        final int N = 20; // Tamanho do vetor
        final int M = 2; // Número de threads

        SharedVector sharedVector = new SharedVector(N);

        // Imprime os valores iniciais do vetor
        System.out.println("Valores iniciais do vetor:");
        sharedVector.print();

        // Cria e inicia as threads
        Thread[] threads = new Thread[M];
        for (int i = 0; i < M; i++) {
            threads[i] = new IncrementTask(sharedVector, i);
            threads[i].start();
        }

        // Aguarda todas as threads terminarem
        for (int i = 0; i < M; i++) {
            threads[i].join();
        }

        // Imprime os valores finais do vetor
        System.out.println("Valores finais do vetor após incremento pelas threads:");
        sharedVector.print();
    }
}
package leitorescritororiginal;

public class LeitorEscritorOriginal {

    static final int L = 6;
    static final int E = 3;

    public static void main(String[] args) {
        int i;
        Monitor monitor = new Monitor();  // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];       // Threads leitores
        Escritor[] e = new Escritor[E];   // Threads escritores

        for (i = 0; i < L; i++) {
            l[i] = new Leitor(i + 1, (i + 1) * 500, monitor);
            l[i].start();
        }
        for (i = 0; i < E; i++) {
            e[i] = new Escritor(i + 1, (i + 1) * 500, monitor);
            e[i].start();
        }
    }
}

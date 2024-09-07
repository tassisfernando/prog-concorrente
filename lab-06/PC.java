// Monitor
class Buffer {

    static final int N = 10;    //tamanho do buffer
    int[] buffer = new int[N];  //reserva espaco para o buffer
    int count = 0, in = 0, out = 0;   //variaveis compartilhadas

    // Construtor
    Buffer() {
        for (int i = 0; i < N; i++) {
            buffer[i] = -1;
        }
    } // inicia o buffer

    // Insere um item
    public synchronized void Insere(int item) {
        while (count == N) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        buffer[in] = item;
        System.out.println("Produtor inseriu: " + item);
        in = (in + 1) % N;
        count++;
        notifyAll();
    }

    public synchronized int[] Remove(int item) {
        while (count < N) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return new int[0];
            }
        }
        int[] items = new int[N];
        for (int i = 0; i < N; i++) {
            items[i] = buffer[(out + i) % N];
            buffer[(out + i) % N] = -1;
        }
        System.out.printf("Consumidor %s removeu todos os itens\n", item);
        out = (out + N) % N;
        count -= N;
        notifyAll();
        return items;
    }
}

//--------------------------------------------------------
// Consumidor
class Consumidor extends Thread {

    int id;
    int delay;
    Buffer buffer;
    int count = 0; // Contador de itens consumidos
    final int MAX_COUNT = 30;

    // Construtor
    Consumidor(int id, int delayTime, Buffer b) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = b;
    }

    // Metodo executado pela thread
    public void run() {
        int[] items;
        try {
            for (;;) {
                items = this.buffer.Remove(this.id - 1);
                if (items.length > 0) {
                    count += items.length; // Incrementa o contador pelo número de itens removidos
                    sleep(this.delay); // Simula o tempo para fazer algo com os itens retirados
                }
            }
        } catch (InterruptedException e) {
            return;
        }
    }

    public int getCount() {
        return count;
    }
}

//--------------------------------------------------------
// Produtor
class Produtor extends Thread {

    int id;
    int delay;
    Buffer buffer;
    int count = 0; // Contador de itens produzidos
    final int MAX_COUNT = 30;

    // Construtor
    Produtor(int id, int delayTime, Buffer b) {
        this.id = id;
        this.delay = delayTime;
        this.buffer = b;
    }

    // Metodo executado pelo thread
    public void run() {
        try {
            while (count < MAX_COUNT) {
                this.buffer.Insere(this.id); //simplificacao: insere o proprio ID
                count++; // Incrementa o contador após a inserção bem-sucedida
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            return;
        }
    }

    public int getCount() {
        return count;
    }
}

//--------------------------------------------------------
// Classe principal
class PC {

    static final int P = 5;
    static final int C = 5;

    public static void main(String[] args) throws InterruptedException {
        int i;
        Buffer buffer = new Buffer();      // Monitor
        Consumidor[] cons = new Consumidor[C];   // Consumidores
        Produtor[] prod = new Produtor[P];       // Produtores

        for (i = 0; i < C; i++) {
            cons[i] = new Consumidor(i + 1, 1000, buffer);
            cons[i].start();
        }
        for (i = 0; i < P; i++) {
            prod[i] = new Produtor(i + 1, 1000, buffer);
            prod[i].start();
        }

        for (i = 0; i < C; i++) {
            cons[i].join(); // Aguarda a thread terminar
            System.out.println("Consumidor " + i + " consumiu: " + cons[i].getCount());
        }
        for (i = 0; i < P; i++) {
            prod[i].join(); // Aguarda a thread terminar
            System.out.println("Produtor " + i + " produziu: " + prod[i].getCount());
        }
    }
}
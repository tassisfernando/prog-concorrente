package leitorescritororiginal;

public class Escritor extends Thread {

    int id; //identificador da thread
    int delay; //atraso bobo...
    Monitor monitor; //objeto monitor para coordenar a logica de execucao das threads

    // Construtor
    Escritor(int id, int delayTime, Monitor m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Metodo executado pela thread
    @Override
    public void run() {
        try {
            for (;;) {
                this.monitor.entraEscritor(this.id);
                sleep(500); //...para simbolizar o tempo de escrita
                this.monitor.saiEscritor(this.id);
                sleep(this.delay); //atraso bobo...
            }
        } catch (InterruptedException e) {
        }
    }
}

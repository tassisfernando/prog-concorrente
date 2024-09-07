package atividade02;

public class Leitor extends Thread {

    int id; //identificador da thread
    int delay; //atraso bobo
    Monitor monitor;//objeto monitor para coordenar a logica de execucao das threads

    // Construtor
    Leitor(int id, int delayTime, Monitor m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Metodo executado pela thread
    @Override
    public void run() {
        try {
            for (;;) {
                this.monitor.entraLeitor(this.id);
                sleep(500); //...para simbolizar o tempo de leitura
                this.monitor.saiLeitor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
        }
    }
}
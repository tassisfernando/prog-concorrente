package leitorescritororiginal;

public class Monitor {

    private int leit, escr;

    // Construtor
    Monitor() {
        this.leit = 0;
        this.escr = 0;
    }

    // Entrada para leitores
    public synchronized void entraLeitor(int id) {
        try {
            while (this.escr > 0) {
                System.out.println("L[" + id + "]: esta esperando");
                wait();  //bloqueia pela condicao logica da aplicacao 
            }
            this.leit++;
            System.out.println("L[" + id + "]: entrou, total de " + this.leit + " lendo");
        } catch (InterruptedException e) {
        }
    }

    // Saida para leitores
    public synchronized void saiLeitor(int id) {
        this.leit--;
        if (this.leit == 0) {
            notifyAll(); //libera os escritores (caso exista escritor bloqueado)
        }
        System.out.println("L[" + id + "]: saiu, resta(m) " + this.leit + " leitor(es) lendo");
    }

    // Entrada para escritores
    public synchronized void entraEscritor(int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                System.out.println("E[" + id + "]: esta esperando");
                wait();  //bloqueia pela condicao logica da aplicacao 
            }
            this.escr++;
            System.out.println("E[" + id + "]: entrou, total de " + this.escr + " escrevendo"); //se aparecer mais de um escritor escrevendo, ERRO!
        } catch (InterruptedException e) {
        }
    }

    // Saida para escritores
    public synchronized void saiEscritor(int id) {
        this.escr--;
        notifyAll(); //libera os leitores e outros escritores no caso de estarem bloqueados
        System.out.println("E[" + id + "]: saiu, resta(m) " + this.escr + " escritor(es) escrevendo"); //se aparecer mais de um escritor escrevendo, ERRO!
    }
}

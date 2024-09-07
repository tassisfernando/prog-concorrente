package atividade03;

public class Monitor {

    private int leit, escr, escrEsperando;

    // Construtor
    Monitor() {
        this.leit = 0;
        this.escr = 0;
        this.escrEsperando = 0;
    }

    // Entrada para leitores
    public synchronized void entraLeitor(int id) {
        try {
            while (this.escr > 0 || this.escrEsperando > 0) {
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

    // Operação de escrita
    public synchronized void escreve(int id) {
        try {
            this.escrEsperando++;
            while ((this.leit > 0) || (this.escr > 0)) {
                System.out.println("E[" + id + "]: esta esperando");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escrEsperando--;
            this.escr++;
            System.out.println("E[" + id + "]: entrou, total de " + this.escr + " escrevendo"); //se aparecer mais de um escritor escrevendo, ERRO!

            // Simula o tempo de escrita
            Thread.sleep(500);

            this.escr--;
            notifyAll(); //libera os leitores e outros escritores no caso de estarem bloqueados
            System.out.println("E[" + id + "]: saiu, resta(m) " + this.escr + " escritor(es) escrevendo"); //se aparecer mais de um escritor escrevendo, ERRO!
        } catch (InterruptedException e) {
        }
    }
}
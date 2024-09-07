package pool;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

//--PASSO 1: criar uma classe que implementa a interface Runnable 
class Hello2 implements Runnable {

    String msg;

    public Hello2(String m) {
        msg = m;
    }

    //--metodo executado pela thread
    public void run() {
        System.out.println(msg);
    }
}

//--classe do metodo main
class HelloPool {

    private static final int NTHREDS = 1000;

    public static void main(String[] args) {
        //--PASSO 2: criar o pool de threads
        ExecutorService pool = Executors.newFixedThreadPool(NTHREDS);

        //--PASSO 3: disparar a execução dos objetos runnable usando o pool de threads
        for (int i = 0; i < 5000; i++) {
            final String m = "Hello da tarefa " + i;
            Runnable r = new Hello2(m);
            pool.execute(r);
        }
        //--PASSO 4: encerrar o pool de threads
        //termina a execução das threads no pool (não permite que o executor aceite novos objetos)
        pool.shutdown();
        //espera todas as threads terminarem
        while (!pool.isTerminated()) {
        }
        System.out.println("Terminou");
    }
}

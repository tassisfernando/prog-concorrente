class BanheiroUni {

    private int mulherE, homemE, vezHomem, vezMulher, homemB, mulherB;

    // Construtor
    BanheiroUni() {
        this.mulherE = 0; //numero de mulheres esperando
        this.homemE = 0;  //numero de homens esperando
        this.mulherB = 0; //numero de mulheres no banheiro
        this.homemB = 0;  //numero de homens no banheiro
        this.vezMulher = 0; //sinaliza vez das mulheres esperando usarem o banheiro
        this.vezHomem = 0; //sinaliza vez dos homens esperando usarem o banheiro
    }

    // Entrada para mulheres
    public synchronized void EntraMulher(int id) {
        try {
            while ((this.homemB > 0) || (this.homemE > 0)) {
                this.mulherE++;
                wait();  //bloqueia pela condicao logica da aplicacao
                this.mulherE--;
                if (this.vezMulher > 0) { //permite que as mulheres esperando usem o banheiro
                    this.vezMulher--;
                    break;
                }
            }
            this.mulherB++;
        } catch (InterruptedException e) {
        }
    }

    // Saida para mulheres
    public synchronized void SaiMulher(int id) {
        this.mulherB--;
        if (this.mulherB == 0) {
            vezMulher = 0; //encerra o intervalo de acesso ao banheiro para as mulheres
            if (homemE > 0) { //passa a vez para os homens esperando pelo banheiro
                vezHomem = homemE;
                notifyAll(); //desbloqueia os homens esperando pelo banheiro
            }
        }
    }

    // Entrada para homens
    public synchronized void EntraHomem(int id) {
        try {
            while ((this.mulherB > 0) || (this.mulherE > 0)) {
                this.homemE++;
                wait();  //bloqueia pela condicao logica da aplicacao
                this.homemE--;
                if (this.vezHomem > 0) {
                    this.vezHomem--;
                    break;
                }
            }
            this.homemB++;
        } catch (InterruptedException e) {
        }
    }

    // Saida para homens
    public synchronized void SaiHomem(int id) {
        this.homemB--;
        if (this.homemB == 0) {
            vezHomem = 0;
            if (mulherE > 0) {
                vezMulher = mulherE;
                notifyAll(); //desbloqueia as mulheres esperando pelo banheiro
            }
        }
    }
}

class Homem implements Runnable {
    private BanheiroUni banheiro;
    private int id;

    public Homem(BanheiroUni banheiro, int id) {
        this.banheiro = banheiro;
        this.id = id;
    }

    @Override
    public void run() {
        while (true) {
            banheiro.EntraHomem(id);
            System.out.println("Homem " + id + " entrou no banheiro.");
            try {
                Thread.sleep((int) (Math.random() * 1000)); // Simula o tempo no banheiro
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            banheiro.SaiHomem(id);
            System.out.println("Homem " + id + " saiu do banheiro.");
            try {
                Thread.sleep((int) (Math.random() * 1000)); // Simula o tempo fora do banheiro
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

class Mulher implements Runnable {
    private BanheiroUni banheiro;
    private int id;

    public Mulher(BanheiroUni banheiro, int id) {
        this.banheiro = banheiro;
        this.id = id;
    }

    @Override
    public void run() {
        while (true) {
            banheiro.EntraMulher(id);
            System.out.println("Mulher " + id + " entrou no banheiro.");
            try {
                Thread.sleep((int) (Math.random() * 1000)); // Simula o tempo no banheiro
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            banheiro.SaiMulher(id);
            System.out.println("Mulher " + id + " saiu do banheiro.");
            try {
                Thread.sleep((int) (Math.random() * 1000)); // Simula o tempo fora do banheiro
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

public class BanheiroUnissex {
    public static void main(String[] args) {
        BanheiroUni banheiro = new BanheiroUni();

        // Criar e iniciar threads de homens
        for (int i = 0; i < 5; i++) {
            new Thread(new Homem(banheiro, i)).start();
        }

        // Criar e iniciar threads de mulheres
        for (int i = 0; i < 5; i++) {
            new Thread(new Mulher(banheiro, i)).start();
        }
    }
}
package reserva;

public class Passenger implements Runnable {

    private Flight flight;
    private String name;

    public Passenger(Flight flight, String name) {
        this.flight = flight;
        this.name = name;
    }

    @Override
    public void run() {
        if (flight.reserveSeat()) {
            System.out.println(name + " reservou um assento.");
        } else {
            System.out.println("Não há assentos disponíveis para " + name + ".");
        }
    }

}

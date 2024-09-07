package reserva;

public class Flight {

    private int seatsAvailable;

    public Flight(int seats) {
        this.seatsAvailable = seats;
    }

    public synchronized boolean reserveSeat() {
        if (seatsAvailable > 0) {
            seatsAvailable--;
            return true;
        }
        return false;
    }
}

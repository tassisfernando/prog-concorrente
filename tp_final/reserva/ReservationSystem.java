package reserva;

import java.util.ArrayList;
import java.util.List;

public class ReservationSystem {
    public static void main(String[] args) {
        final int CAPACITY = 500;
        Flight flight = new Flight(CAPACITY);

        List<Passenger> passengers = new ArrayList<>();

        for (int i = 0; i < CAPACITY * 3/2; i++) {
            passengers.add(new Passenger(flight, "Passenger " + i));
        }

        passengers.parallelStream().forEach(Passenger::run);
    }
}

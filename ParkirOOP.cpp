#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cstdlib> // untuk menggunakan fungsi system()

using namespace std;
using namespace std::chrono;

class Vehicle {
protected:
    string plateNumber;
    string type;
    steady_clock::time_point arrivalTime;

public:
    Vehicle(const string& plate, const string& t, const steady_clock::time_point& arrival)
        : plateNumber(plate), type(t), arrivalTime(arrival) {}

    const string& getPlateNumber() const {
        return plateNumber;
    }

    const string& getType() const {
        return type;
    }

    const steady_clock::time_point& getArrivalTime() const {
        return arrivalTime;
    }

    virtual double calculateParkingFee() const = 0; // Fungsi virtual murni untuk menghitung biaya parkir
};

class Car : public Vehicle {
public:
    Car(const string& plate, const steady_clock::time_point& arrival)
        : Vehicle(plate, "Car", arrival) {}

    double calculateParkingFee() const override {
        // Hitung biaya parkir untuk mobil
        duration<double> duration = steady_clock::now() - arrivalTime;
        double hours = duration.count() / 3600.0;
        return hours * 5000; // Biaya parkir mobil: Rp 5000 per jam
    }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(const string& plate, const steady_clock::time_point& arrival)
        : Vehicle(plate, "Motorcycle", arrival) {}

    double calculateParkingFee() const override {
        // Hitung biaya parkir untuk motor
        duration<double> duration = steady_clock::now() - arrivalTime;
        double hours = duration.count() / 3600.0;
        return hours * 2000; // Biaya parkir motor: Rp 2000 per jam
    }
};

class ParkingSpot {
private:
    int spotNumber;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(int number) : spotNumber(number), parkedVehicle(nullptr) {}

    bool isAvailable() const {
        return parkedVehicle == nullptr;
    }

    void parkVehicle(Vehicle* vehicle) {
        if (isAvailable()) {
            parkedVehicle = vehicle;
            time_t currentTime = system_clock::to_time_t(system_clock::now());
            tm* localTime = localtime(&currentTime);
            cout << "Kendaraan dengan nomor plat " << vehicle->getPlateNumber() << " berhasil diparkir di tempat parkir #" << spotNumber << endl;
            cout << "Waktu parkir: " << put_time(localTime, "%Y-%m-%d %H:%M:%S") << " WIT" << endl;
        } else {
            cout << "Tempat parkir #" << spotNumber << " sudah terisi." << endl;
        }
    }

    void removeVehicle() {
        if (!isAvailable()) {
            time_t currentTime = system_clock::to_time_t(system_clock::now());
            tm* localTime = localtime(&currentTime);
            cout << "Kendaraan dengan nomor plat " << parkedVehicle->getPlateNumber() << " berhasil keluar dari tempat parkir #" << spotNumber << endl;
            cout << "Waktu keluar: " << put_time(localTime, "%Y-%m-%d %H:%M:%S") << " WIT" << endl;
            double parkingFee = parkedVehicle->calculateParkingFee();
            cout << "Biaya parkir: Rp " << fixed << setprecision(0) << parkingFee << endl;
            delete parkedVehicle;
            parkedVehicle = nullptr;
        } else {
            cout << "Tempat parkir #" << spotNumber << " kosong." << endl;
        }
    }

    const Vehicle* getParkedVehicle() const {
        return parkedVehicle;
    }

    int getSpotNumber() const {
        return spotNumber;
    }

    ~ParkingSpot() {
        if (!isAvailable()) {
            delete parkedVehicle;
        }
        cout << "Tempat parkir #" << spotNumber << " telah dihapus." << endl;
    }
};

class ParkingArea {
private:
    vector<ParkingSpot*> spots;

public:
    ParkingArea(int totalSpots) {
        for (int i = 0; i < totalSpots; i++) {
            ParkingSpot* spot = new ParkingSpot(i + 1);
            spots.push_back(spot);
        }
    }

    void parkVehicle(Vehicle* vehicle) {
        for (ParkingSpot* spot : spots) {
            if (spot->isAvailable()) {
                spot->parkVehicle(vehicle);
                return;
            }
        }
        cout << "Semua tempat parkir penuh." << endl;
    }

    void removeVehicle() {
        int spotNumber;
        cout << "Masukkan nomor tempat parkir: ";
        cin >> spotNumber;

        if (spotNumber >= 1 && spotNumber <= spots.size()) {
            ParkingSpot* spot = spots[spotNumber - 1];
            spot->removeVehicle();
        } else {
            cout << "Nomor tempat parkir tidak valid." << endl;
        }
    }

    void displayParkingArea() const {
        cout << "Data Parkiran:\n";
        for (ParkingSpot* spot : spots) {
            if (spot->isAvailable()) {
                cout << "Tempat parkir #" << spot->getSpotNumber() << ": Kosong" << endl;
            } else {
                const Vehicle* parkedVehicle = spot->getParkedVehicle();
                cout << "Tempat parkir #" << spot->getSpotNumber() << ": " << parkedVehicle->getType() << " - " << parkedVehicle->getPlateNumber() << endl;
            }
        }
    }

    ~ParkingArea() {
        for (ParkingSpot* spot : spots) {
            delete spot;
        }
        cout << "Area parkir telah dihapus." << endl;
    }
};

int main() {
    ParkingArea parking(20); // Inisialisasi objek ParkingArea dengan 20 tempat parkir

    string plateNumber;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Parkir kendaraan\n";
        cout << "2. Keluarkan kendaraan\n";
        cout << "3. Tampilkan data parkiran\n";
        cout << "4. Selesai\n";
        cout << "Pilih menu: ";
        int choice;
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                cout << "Masukkan nomor plat kendaraan: ";
                cin >> plateNumber;

                cout << "Jenis Kendaraan:\n";
                cout << "1. Mobil\n";
                cout << "2. Motor\n";
                cout << "Pilih jenis kendaraan: ";

                int vehicleChoice;
                cin >> vehicleChoice;
                system("cls");
                if (vehicleChoice == 1) {
                    Car* car = new Car(plateNumber, steady_clock::now());
                    parking.parkVehicle(car);
                } else if (vehicleChoice == 2) {
                    Motorcycle* motorcycle = new Motorcycle(plateNumber, steady_clock::now());
                    parking.parkVehicle(motorcycle);
                } else {
                    cout << "Pilihan jenis kendaraan tidak valid." << endl;
                }

                break;

            case 2:
                parking.removeVehicle();
                break;

            case 3:
                parking.displayParkingArea();
                break;

            case 4:
                return 0;

            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.get();
        system("cls");
    }
}
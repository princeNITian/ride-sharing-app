#include <iostream>
#include <vector>

using namespace std;

enum RideStatus { IDLE, CREATED, WITHDRAWN, COMPLETED};

class Ride {
public:
	static const int AMT_PER_KM = 20;
	Ride();
	int calculateFare(bool);
	void setDest(int dest);
	int getId() const;
	void setId(int id);
	void setOrigin(int origin);
	RideStatus getRideStatus() const;
	void setRideStatus(RideStatus rideStatus);
	void setSeats(int seats);

private:
	int id;
	int origin, dest;
	int seats;
	RideStatus rideStatus;
};

class Person {
	public:
		string name;
};

class Driver : private Person {
public:
	Driver(string);
};

class Rider : private Person {
private:
	vector<Ride> allRides;
public:
	Rider(string);
	void createRide(int , int , int, int);
	void updateRide(int , int, int, int);
	void withdrawRide(int);
	int closeRide(int);

};

Ride::Ride(){
	id = origin = dest = seats = 0;
	rideStatus = RideStatus::IDLE;
};

Driver::Driver(string name){
	this->name = name;
}

int Ride::calculateFare(bool isPriorityRider){

	int dist = dest - origin;

	if(seats < 2) {
		return dist * AMT_PER_KM * (isPriorityRider ? 0.75 : 1);
	}
	return dist * seats * AMT_PER_KM * (isPriorityRider ? 0.5 : 0.75);
}


void Ride::setDest(int dest) {
	this->dest = dest;
}

int Ride::getId() const {
	return id;
}

void Ride::setId(int id) {
	this->id = id;
}

void Ride::setOrigin(int origin) {
	this->origin = origin;
}

RideStatus Ride::getRideStatus() const {
	return rideStatus;
}

void Ride::setRideStatus(RideStatus rideStatus) {
	this->rideStatus = rideStatus;
}

void Ride::setSeats(int seats) {
	this->seats = seats;
}

Rider::Rider(string name){
	this->name = name;
}

void Rider::createRide(int id,int origin, int dest, int seats){

	if(origin>=dest){
		cout << "Wrong values of Origin and Destination provided. Can't create ride\n";
		return ;
	}

	Ride currentRide;
	currentRide.setId(id);
	currentRide.setOrigin(origin);
	currentRide.setDest(dest);
	currentRide.setSeats(seats);
	currentRide.setRideStatus(RideStatus::CREATED);
	allRides.push_back(currentRide);

}

void Rider::updateRide(int id,int origin, int dest, int seats){

	auto iter = allRides.rbegin();
	for(; iter != allRides.rend(); ++iter){
		if(iter->getId() == id)
		{
			break;
		}
	}
	if(iter->getRideStatus() != RideStatus::CREATED){
		cout << "Ride wasn't in progress. Can't update ride\n";
		return;
	}
	iter->setOrigin(origin);
	iter->setDest(dest);
	iter->setSeats(seats);

}

void Rider::withdrawRide(int id){

	auto iter = allRides.rbegin();
	for(; iter != allRides.rend(); ++iter){
		if(iter->getId() == id)
		{
			break;
		}
	}
	if(iter->getRideStatus() != RideStatus::CREATED)
	{
		cout<<"Ride wasn't in progress. Can't withdraw ride\n";
		return;
	}
	iter->setRideStatus(RideStatus::WITHDRAWN);
	allRides.erase((iter+1).base());

}

int Rider::closeRide(int id){

	auto iter = allRides.rbegin();
	for(; iter != allRides.rend(); ++iter){
		if(iter->getId() == id)
		{
			break;
		}
	}

	if(iter->getRideStatus() != RideStatus::CREATED){
		cout<<"Ride wasn't in progress. Can't close ride\n";
		return 0;
	}
	iter->setRideStatus(RideStatus::COMPLETED);
	return iter->calculateFare(allRides.size()>=10);
}


int main(){
	Rider rider("Prince");
	Driver driver("Rehman");

	rider.createRide(1,50,60,1);
	cout<<rider.closeRide(1)<<endl;
	rider.updateRide(1,50,60,2);
	cout<<rider.closeRide(1)<<endl;

	cout<<"**************************************************"<<endl;

	rider.createRide(1,50,60,1);
	rider.withdrawRide(1);
	rider.updateRide(1,50,60,2);
	cout<<rider.closeRide(1)<<endl;

	cout<<"****************************************************"<<endl;

	rider.createRide(1,50,60,1);
	rider.updateRide(1, 50, 60, 2);
	cout<<rider.closeRide(1)<<endl;

	return 0;
}

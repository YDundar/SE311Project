//Yigit Can Dündar (20130601019) - Selin Önal () - Header file for SE 311 Term Project (2016)
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include<cstdlib>

using namespace std;

//Forward declarations for Zigbee, Cattle, DairyCattle and BeefCattle classes.
class Zigbee;
class Cattle;
class DairyCattle;
class BeefCattle;

//Singleton Database that holds the Location information of all farm animals using Zigbee signals.
class LivestockDatabase {
private:
	LivestockDatabase() {}; //Private constructor.
	vector<Zigbee*> deviceList; //The list that contains ONLY Zigbee Devices.
	static LivestockDatabase* database; //Singleton unique instance.
public:
	//Method that Adds the ZBDevice to the deviceList.
	void AddDevice(Zigbee* ZBDevice) {
		deviceList.push_back(ZBDevice);
	}

	//Database Method that lists all the connected devices.
	void listDevices();

	//Database Method that lists the location info for each monitored cattle.
	void listLocations();

	//Accessor method for the unique instance.
	static LivestockDatabase* GetDatabase() {
		if (database == NULL) {
			database = new LivestockDatabase();
		}
		return database;
	}
};

//Abstract Zigbee class that provides an interface for Zigbee Devices and Bluetooth adapter.
class Zigbee {
public:
	//Virtual methods that MUST be overridden by subclasses.
	virtual void sendLocationToDatabase() = 0;
	virtual void printLocationInfo() = 0;
	virtual int getDeviceHolderID() = 0;
	virtual void updateLocation() = 0;
	virtual double * getCurrentLocation() = 0;
};

//Concrete Zigbee Class that is used on Beef Cattle.
class ZigbeeDevice :public Zigbee {
private:
	int deviceHolderID; //The ID of the cattle that this device is attached to.
	double location[3] = { 0,1,0 }; //Location information of the cattle.
public:
	//Unique ID is set and the device info is sent to the Database
	ZigbeeDevice(int id) { deviceHolderID = id; sendLocationToDatabase(); }

	//Accessor method for the private data member "deviceHolderID".
	int getDeviceHolderID() { return deviceHolderID; }

	//Updates the location of the cattle based on a random number within the range [0,19], for x and z axes.
	void updateLocation() {
		double x = rand() % 20;
		location[0] = x;
		x = rand() % 20;
		location[2] = x;
	}
	//A pretty print method for the location info.
	void printLocationInfo() {
		cout << "(" << location[0] << "," << location[1] << "," << location[2] << ")";
	}

	//Accessor method for the private data member "location".
	double * getCurrentLocation() {
		return location;
	}

	//Method for sending the device info to the Database.
	void sendLocationToDatabase() {
		LivestockDatabase::GetDatabase()->AddDevice(this);
	}
};

//Bluetooth Class that is used on Dairy Cattle.
class BluetoothDevice {
private:
	int deviceHolderID; //The ID of the cattle that this device is attached to.
	double location[3] = { 0,1,0 }; //Location information of the cattle.
public:

	//Unique ID is set.
	BluetoothDevice(int id) { deviceHolderID = id; }

	//Accessor method for the private data member "deviceHolderID".
	int getDeviceHolderID() { return deviceHolderID; }

	//Updates the location of the cattle based on a random number within the range [0,19], for x and z axes.
	void updateLocation() {
		double x = rand() % 20;
		location[0] = x;
		x = rand() % 20;
		location[2] = x;
	}

	//A pretty print method for the location info.
	void printLocationInfo() {
		cout << "(" << location[0] << "," << location[1] << "," << location[2] << ")";
	}

	//Accessor method for the private data member "location".
	double * getCurrentLocation() { return location; }
};

//Bluetooth signal to Zigbee signal Adapter
class BluetoothToZigbee :public Zigbee {
private:
	BluetoothDevice* adaptee; //Adaptee Bluetooth Device instance.
public:
	//Adaptee is set and the adapters information is sent to the database.
	BluetoothToZigbee(BluetoothDevice* bTooth) { adaptee = bTooth; sendLocationToDatabase(); };

	//Adapter method to print the location info from the adaptee.
	void printLocationInfo() {
		adaptee->printLocationInfo();
	}
	//Adapter method to update the location info from the adaptee
	void updateLocation() {
		adaptee->updateLocation();
	}
	//Sends the adapter information to the Database.
	void sendLocationToDatabase() {
		LivestockDatabase::GetDatabase()->AddDevice(this);
	}
	//Adapter method to get the deviceHolderID from the adaptee.
	int getDeviceHolderID() {
		return adaptee->getDeviceHolderID();
	}
	//Adapter method to get the location info from the adaptee.
	double * getCurrentLocation() { return adaptee->getCurrentLocation(); }
};

//Abstract Product Class Carbohydrate
class Carbohydrate {
public:
	Carbohydrate() {}
	//Virtual method that must be overridden by subclasses.
	virtual string getName() = 0;
protected:
	//Protected data member that holds the name of the product.
	string name;
};

//Concrete Carbohydrate product class Corn
class Corn : public Carbohydrate {
public:
	//Name is set.
	Corn() { name = "Corn"; }
	//Accessor method for the protected data member "name".
	string getName() { return name; }
};

//Concrete Carbohydrate product class Wheat
class Wheat :public Carbohydrate {
public:
	Wheat() { name = "Wheat"; }
	string getName() { return name; }
};

//Abstract Product Class Protein
class Protein {
public:
	Protein() {}
	virtual string getName() = 0;
protected:
	string name;
};

//Concrete Protein product class Soybean
class Soybean :public Protein {
public:
	Soybean() { name = "Soybean"; }
	string getName() { return name; }
};

//Concrete Protein product class Canola
class Canola : public Protein {
public:
	Canola() { name = "Canola"; }
	string getName() { return name; }
};

//Abstract Factory Class CattleFeeder
class CattleFeeder {
public:
	//Virtual Methods that must be overridden by subclasses.
	virtual Carbohydrate* feedCHydrate() = 0;
	virtual Protein* feedProtein() = 0;
};

//Concrete Factory Class Dairy Feeder
class DairyCattleFeeder : public CattleFeeder {
public:
	//Factory method that returns the appropriate Carbohydrate object.
	Carbohydrate* feedCHydrate() {
		return new Corn();
	}

	//Factory method that returns the appropriate Protein object.
	Protein* feedProtein() {
		return new Soybean();
	}
};

//Concrete Factory Class Beef Feeder
class BeefCattleFeeder : public CattleFeeder {
public:
	//Factory method that returns the appropriate Carbohydrate object.
	Carbohydrate* feedCHydrate() {
		return new Wheat();
	}

	//Factory method that returns the appropriate Carbohydrate object.
	Protein* feedProtein() {
		return new Canola();
	}
};

//Abstract Visitor Class
class Visitor {
public:
	//Virtual methods that must be overridden by subclasses.
	virtual void Visit(DairyCattle* cattle) = 0;
	virtual void Visit(BeefCattle* cattle) = 0;
};

//Abstract Cattle Class
class Cattle {
public:
	Cattle() {}
	//Virtual methods that must be overridden by subclasses.
	virtual void eatFood(CattleFeeder* feeder) = 0;
	virtual void Accept(Visitor* visitor) = 0;
	virtual int getID() = 0;
	virtual void receiveVaccination() = 0;
protected:
	Carbohydrate* cHydrate; //Carbohydrate product instance.
	Protein* protein; //Protein product instance.
};

//Concrete Dairy Cattle Class. Tracked using Bluetooth. That also serves as the Abstract Factory methods client.
class DairyCattle :public Cattle {
private:
	int uniqueID; //Unique identifier.
	BluetoothDevice* TrackingDevice; //Tracker for dairy cattle.
	Zigbee* bluetoothToZigbeeAdapter; //Bluetooth to Zigbee adapter.
public:
	//Unique id is set, Bluetooth tracker is initialized, adapter is initialized with the tracker.
	DairyCattle(int id) {
		uniqueID = id;
		TrackingDevice = new BluetoothDevice(uniqueID);
		bluetoothToZigbeeAdapter = new BluetoothToZigbee(TrackingDevice);
	}

	//Method that simulates eating habits of dairy cattle.
	void eatFood(CattleFeeder* feeder) {
		cHydrate = feeder->feedCHydrate();
		protein = feeder->feedProtein();
		cout << "Dairy Cattle #" << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}

	//Visitor accept class.
	void Accept(Visitor * visitor) {
		visitor->Visit(this);
	}

	//Accessor method for the private data member "uniqueID".
	int getID() {
		return uniqueID;
	}

	//Method that the Veterinarian visitor can use.
	void receiveVaccination() {
		cout << "Dairy Cattle #" << uniqueID << " is receiving Vaccination." << endl;
	}
};

//Concrete Beef Cattle Class. Tracked using Zigbee. That also serves as the Abstract Factory methods client.
class BeefCattle :public Cattle {
private:
	int uniqueID; //Unique identifier.
	ZigbeeDevice* TrackingDevice; //Tracker for beef cattle.
public:
	//Unique ID is set, tracker is initialized.
	BeefCattle(int id) { uniqueID = id; TrackingDevice = new ZigbeeDevice(uniqueID); }

	//Method that simulates eating habits of beef cattle.
	void eatFood(CattleFeeder* feeder) {
		cHydrate = feeder->feedCHydrate();
		protein = feeder->feedProtein();
		cout << "Beef Cattle #" << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}

	//Visitor accept class.
	void Accept(Visitor * visitor) {
		visitor->Visit(this);
	}

	//Accessor method for the private data member "uniqueID".
	int getID() {
		return uniqueID;
	}

	//Method that the Veterinarian visitor can use.
	void receiveVaccination() {
		cout << "Beef Cattle #" << uniqueID << " is receiving Vaccination." << endl;
	}
};

//Concrete Veterinary Physician Visitor Class
class Veterinarian :public Visitor {
public:
	//Visit method for Dairy Cattle.
	void Visit(DairyCattle* cattle) {
		cattle->receiveVaccination();
	}

	//Visit method for Beef Cattle.
	void Visit(BeefCattle* cattle) {
		cattle->receiveVaccination();
	}
};

//Concrete Ministry of Agriculture Visitor Class
class MinistryRepresentative :public Visitor {
public:
	//Visit Method for Dairy Cattle.
	void Visit(DairyCattle* cattle) {
		cout << "Dairy Cattle #" << cattle->getID() << " has an ear tag." << endl;
	}

	//Visit Method for Beef Cattle.
	void Visit(BeefCattle* cattle) {
		cout << "Beef Cattle #" << cattle->getID() << " has an ear tag." << endl;
	}
};

//Facade Farm Class
class Farm {
private:
	vector <Cattle*> dairyLivestock; //List that holds Dairy Cattle informations.
	vector <Cattle*> beefLivestock; //List that holds Beef Cattle informations.
	DairyCattleFeeder * dairyFeeder = new DairyCattleFeeder(); //Dairy Feeder instance.
	BeefCattleFeeder * beefFeeder = new BeefCattleFeeder(); //Beef Feeder instance.
public:
	//Method that adds a new Dairy Cattle to the dairyLiveStock list with id.
	void AddNewDairyCattle(int id) {
		dairyLivestock.push_back(new DairyCattle(id));
	}

	//Method that adds a new Beef Cattle to the dairyLiveStock list with id.
	void AddNewBeefCattle(int id) {
		beefLivestock.push_back(new BeefCattle(id));
	}

	//Method that feeds all farm animals.
	void FeedLivestock() {
		for (int i = 0; i<dairyLivestock.size(); i++) {
			dairyLivestock[i]->eatFood(dairyFeeder);
		}
		for (int i = 0; i<beefLivestock.size(); i++) {

			beefLivestock[i]->eatFood(beefFeeder);
		}
	}

	//Method that accepts a visitor for all farm animals.
	void AcceptVisitor(Visitor* visitor) {
		for (int i = 0; i<dairyLivestock.size(); i++) {
			dairyLivestock[i]->Accept(visitor);
		}
		for (int i = 0; i<beefLivestock.size(); i++) {
			beefLivestock[i]->Accept(visitor);
		}
	}
};

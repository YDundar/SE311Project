#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

class Zigbee;

//Database that holds the Location information of all farm animals using Zigbee signals.
class LivestockDatabase {
private:
	LivestockDatabase() {};
	vector<Zigbee*> deviceList;
	static LivestockDatabase* database;
public:
	void AddDevice(Zigbee* ZBDevice) {
		deviceList.push_back(ZBDevice);
	}
	void listDevices();
	void listLocations();
	static LivestockDatabase* GetDatabase() { return database; }
};

//Abstract Zigbee class
class Zigbee {
public:
	virtual void sendLocationToDatabase() = 0;
	virtual void getLocationInfo() = 0;
	virtual int getDeviceHolderID() = 0;
	virtual void updateLocation() = 0;
};

//Concrete Zigbee Class
class ZigbeeDevice:public Zigbee {
private:
	int deviceHolderID;
	double location[3] = { 0,0,0 };
protected:
	ZigbeeDevice() {}
public:
	ZigbeeDevice(int id) { deviceHolderID = id; sendLocationToDatabase(); }
	int getDeviceHolderID() { return deviceHolderID; }
	void updateLocation() {
		double x = rand() % 20;
		location[0] = x;
		x = rand() % 20;
		location[1] = 1;
		x = rand() % 20;
		location[2] = x;
	}
	void getLocationInfo() {
		cout << "(" << location[0] << "," << location[1] << "," << location[2] << ")";
	}
	void sendLocationToDatabase(){
		LivestockDatabase::GetDatabase()->AddDevice(this);
	}
};

//Bluetooth Class
class BluetoothDevice {
private:
	int deviceHolderID;
	double location[3] = { 0,0,0 };
public:
	BluetoothDevice(int id) { deviceHolderID = id; }
	int getDeviceHolderID() { return deviceHolderID; }
	void updateLocation() {
		double x = rand() % 20;
		location[0] = x;
		x = rand() % 20;
		location[1] = 1;
		x = rand() % 20;
		location[2] = x;
	}
	void getLocationInfo() {
		cout << "(" << location[0] << "," << location[1] << "," << location[2] << ")";
	}
};

//Bluetooth signal to Zigbee signal Adapter
class BluetoothToZigbee :public Zigbee {
private:
	BluetoothDevice* adaptee;
public:
	BluetoothToZigbee(BluetoothDevice* bTooth) {
		adaptee = bTooth;
		sendLocationToDatabase();
	};
	void getLocationInfo() {
		adaptee->getLocationInfo();
	}
	void updateLocation() {
		adaptee->updateLocation();
	}
	void sendLocationToDatabase() {
		LivestockDatabase::GetDatabase()->AddDevice(this);
	}
	int getDeviceHolderID() {
		return adaptee->getDeviceHolderID();
	}
};

//Product Classes
class Carbonhydrate{//Abstract Product Class Carbonhydrate
public:
	Carbonhydrate() {}
	virtual string getName() = 0;
protected:
	string name;
};
class Corn: public Carbonhydrate{//Concrete Carbonhydrate product class #1
public:
	Corn() { name = "Corn"; }
	string getName() {
		return name;
	}
};
class Wheat :public Carbonhydrate{//Concrete Carbonhydrate product class #2
public:
	Wheat() { name = "Wheat"; }
	string getName() {
		return name;
	}
};
class Protein{//Abstract Product Class Protein
public:
	Protein() {}
	virtual string getName() = 0;
protected:
	string name;
};
class Soybean :public Protein{//Concrete Protein product class #1
public:
	Soybean() { name = "Soybean"; }
	string getName() {
		return name;
	}
};
class Canola : public Protein{//Concrete Protein product class #2
public:
	Canola() { name = "Canola"; }
	string getName() {
		return name;
	}
};

//Abstract Feeder Factory Class
class CattleFeeder {
public:
	virtual Carbonhydrate* feedCHydrate() = 0;
	virtual Protein* feedProtein() = 0;
};

class DairyCattleFeeder : public CattleFeeder {//Concrete Dairy Feeder Factory Class
public:
	Carbonhydrate* feedCHydrate() {
		return new Corn();
	}
	Protein* feedProtein() {
		return new Soybean();
	}
};
class BeefCattleFeeder : public CattleFeeder {//Concrete Beef Feeder Factory Class
public:
	Carbonhydrate* feedCHydrate() {
		return new Wheat();
	}
	Protein* feedProtein() {
		return new Canola();
	}
};

//Abstract Cattle Class also serves as the Abstract Factory methods client
class Cattle {
public:
	Cattle() {}
	virtual void eatFood(CattleFeeder* feeder) = 0;
protected:
	Carbonhydrate* cHydrate;
	Protein* protein;
};
//Concrete Dairy Cattle Class. Tracked using Bluetooth.
class DairyCattle:public Cattle {
private:
	int uniqueID;
	Zigbee* bluetoothToZigbeeAdapter;
public:
	DairyCattle(int id) { uniqueID = id; bluetoothToZigbeeAdapter = new BluetoothToZigbee(new BluetoothDevice(uniqueID)); }
	void eatFood(CattleFeeder* feeder) {
		cHydrate = feeder->feedCHydrate();
		protein = feeder->feedProtein();
		cout << "Dairy Cattle " << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}
};
//Concrete Beef Cattle Class. Tracked using Zigbee.
class BeefCattle:public Cattle {
private:
	int uniqueID;
	ZigbeeDevice* TrackingDevice;
public:
	BeefCattle(int id) { uniqueID = id; TrackingDevice = new ZigbeeDevice(uniqueID); }
	void eatFood(CattleFeeder* feeder) {
		cHydrate = feeder->feedCHydrate();
		protein = feeder->feedProtein();
		cout << "Beef Cattle " << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}
};

//Farm Facade Class
class Farm {
private:
	vector <Cattle*> dairyLivestock;
	vector <Cattle*> beefLivestock;
	DairyCattleFeeder * dairyFeeder = new DairyCattleFeeder();
	BeefCattleFeeder * beefFeeder = new BeefCattleFeeder();
public:
    Farm(){}
	void AddNewDairyCattle(int id) {
		dairyLivestock.push_back(new DairyCattle(id));
	}
	void AddNewBeefCattle(int id) {
		beefLivestock.push_back(new BeefCattle(id));
	}
	void FeedLivestock() {
		for each ( Cattle* cattle in dairyLivestock)
		{
			cattle->eatFood(dairyFeeder);
		}
		for each (Cattle* cattle in beefLivestock)
		{
			cattle->eatFood(beefFeeder);
		}
	}
};

//Database Method definitions
LivestockDatabase* LivestockDatabase::database = new LivestockDatabase();
void LivestockDatabase::listDevices() {
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		cout << i + 1 << "# device is on cattle id: " << deviceList[i]->getDeviceHolderID() << endl;
	}
}
void LivestockDatabase::listLocations() {
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		deviceList[i]->updateLocation();
		cout << "The animal with ID:" << deviceList[i]->getDeviceHolderID() << " is at location: ";
		deviceList[i]->getLocationInfo();
		cout << endl;
	}
}


//Farmer Client
int main() {
	srand(time(NULL));
	Farm* farm = new Farm();

	farm->AddNewBeefCattle(31234);
	farm->AddNewBeefCattle(43569);
	farm->AddNewDairyCattle(45435);
	farm->AddNewDairyCattle(35433);

	LivestockDatabase::GetDatabase()->listDevices();
	LivestockDatabase::GetDatabase()->listLocations();

	farm->FeedLivestock();

	getchar();
	return 0;
}

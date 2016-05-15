#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

class Zigbee;
class Cattle;
class DairyCattle;
class BeefCattle;

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
    virtual double * getcurrentlocation(){  }
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
	double * getcurrentlocation(){return location;}
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
	double * getcurrentlocation(){return adaptee->getcurrentlocation();}
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

//Abstract Visitor Class
class Visitor {
public:
	virtual void Visit(DairyCattle* cattle) = 0;
	virtual void Visit(BeefCattle* cattle) = 0;
};
//Abstract Cattle Class also serves as the Abstract Factory methods client
class Cattle {
public:
	Cattle() {}
	virtual void eatFood(CattleFeeder* feeder) = 0;
	virtual void Accept(Visitor* visitor) = 0;
	virtual int getID() = 0;
	virtual void receiveVaccination() = 0;
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
		cout << "Dairy Cattle #" << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}
	void Accept(Visitor * visitor) {
		visitor->Visit(this);
	}
	int getID() {
		return uniqueID;
	}
	void receiveVaccination() {
		cout << "Dairy Cattle #" << uniqueID << " is receiving Vaccination." << endl;
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
		cout << "Beef Cattle #" << uniqueID << " is eating " << cHydrate->getName() << " and " << protein->getName() << endl;
	}
	void Accept(Visitor * visitor) {
		visitor->Visit(this);
	}
	int getID() {
		return uniqueID;
	}
	void receiveVaccination() {
		cout << "Beef Cattle #" << uniqueID << " is receiving Vaccination." << endl;
	}
};

//Concrete Veterinary Physician Visitor Class
class Veterinarian :public Visitor {
public:
	void Visit(DairyCattle* cattle) {
		cattle->receiveVaccination();
	}
	void Visit(BeefCattle* cattle) {
		cattle->receiveVaccination();
	}
};

//Concrete Ministry of Agriculture Visitor Class
class MinistryRepresentative :public Visitor {
public:
	void Visit(DairyCattle* cattle) {
		cout << "Dairy Cattle #" << cattle->getID()<<" has an ear tag." << endl;
	}
	void Visit(BeefCattle* cattle) {
		cout << "Beef Cattle #" << cattle->getID() << " has an ear tag." << endl;
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
	void AcceptVisitor(Visitor* visitor) {
		for each(Cattle* cattle in dairyLivestock) {
			cattle->Accept(visitor);
		}
		for each(Cattle* cattle in beefLivestock) {
			cattle->Accept(visitor);
		}
	}
};


//Database Method definitions
LivestockDatabase* LivestockDatabase::database = new LivestockDatabase();
void LivestockDatabase::listDevices() {
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		cout <<"Device #"<< i + 1 << "is on cattle id: " << deviceList[i]->getDeviceHolderID() << endl;
	}
}
void LivestockDatabase::listLocations() {
    double * p;
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		deviceList[i]->updateLocation();
		cout << "Cattle #" << deviceList[i]->getDeviceHolderID() << " is at location: ";
		deviceList[i]->getLocationInfo();
		cout << endl;
		p=deviceList[i]->getcurrentlocation();
            if(*p>10 || *(p+1) >10 || *(p+2) >10){
            cout<< "--------->"<<"WARNING: CATTLE exit from the farm boundaries!!!"<<endl ;
        }
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
	cout << endl;
	LivestockDatabase::GetDatabase()->listLocations();
	cout << endl;
	farm->FeedLivestock();
	cout << endl;
	farm->AcceptVisitor(new Veterinarian());
	cout << endl;
	farm->AcceptVisitor(new MinistryRepresentative());

	getchar();
	return 0;
}

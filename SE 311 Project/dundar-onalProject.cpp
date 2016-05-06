#include <iostream>
#include <vector>
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


class Carbonhydrate{
    public:
    Carbonhydrate(){}
    virtual void Food()=0;
};
class Protein{
    public:
    Protein(){}
    virtual void Food()=0;

};

class Corn: public Carbonhydrate{
    public:
      void  Food(){
    cout<<" Corn is gaved to dairycattle "<<endl;}
};

class Wheat :public Carbonhydrate{
    public:
      void  Food(){
    cout<<" wheat is gaved to beefcattle"<<endl;}
};
class Soybean :public Protein{
    public:
      void  Food(){
    cout<<" Soybean is gaved to dairycattle"<<endl;}
};
class Canola : public Protein{
    public:
    void Food(){cout<<" Canola is gaved to beef cattle"<<endl;}
};

//Abstract Cattle Class
class Cattle {
public:
	Cattle() {}
	 virtual Carbonhydrate * createcarbonhdyrate()=0;
	 virtual Protein * createprotein()=0;
};

//Concrete Dairy Cattle Class. Tracked using Bluetooth.
class DairyCattle:public Cattle {
private:
	int uniqueID;
	Zigbee* bluetoothToZigbeeAdapter;
public:
	DairyCattle(int id) { uniqueID = id;
	 bluetoothToZigbeeAdapter = new BluetoothToZigbee(new BluetoothDevice(uniqueID)); }

    Carbonhydrate * createcarbonhdyrate(){
        return new Corn() ;}
    Protein * createprotein(){
        return new Soybean();}
};
//Concrete Beef Cattle Class. Tracked using Zigbee.
class BeefCattle:public Cattle {
private:
	int uniqueID;
	ZigbeeDevice* TrackingDevice;
public:
	BeefCattle(int id) { uniqueID = id; TrackingDevice = new ZigbeeDevice(uniqueID); }

    Carbonhydrate* createcarbonhdyrate(){
        return new Wheat();}
     Protein* createprotein(){
        return new Canola();}
};

//Abstract Feeder Factory Class

class CattleFeeder {
protected:
    Carbonhydrate * carbon_foods;
    Protein* prot_foods;
public:
    virtual void feed_cattle(vector<Cattle *> c)=0;
};

class DairyCattleFeeder: public CattleFeeder{
public:
    void feed_cattle(vector<Cattle *> c){

        for(int i=0; i< c.size();i++){
        carbon_foods=c[i]->createcarbonhdyrate();
        prot_foods=c[i]->createprotein();
        carbon_foods->Food();
        prot_foods->Food();
        }
    }
};
class BeefCattleFeeder: public CattleFeeder{
public:
    void feed_cattle(vector<Cattle *> c){

        for(int i=0; i< c.size();i++){
        carbon_foods=c[i]->createcarbonhdyrate();
        prot_foods=c[i]->createprotein();
        carbon_foods->Food();
        prot_foods->Food();
        }
    }
};
//Farm Facade Class
class Farm {
private:
	vector <Cattle*> dairyLivestock;
	vector <Cattle*> beefLivestock;
	DairyCattleFeeder *feeddairycattle;
	BeefCattleFeeder * feedbeefcattle;
public:
    Farm(){
        feeddairycattle=new DairyCattleFeeder();
        feedbeefcattle=new BeefCattleFeeder();
        }
	void AddNewDairyCattle(int id) {
		dairyLivestock.push_back(new DairyCattle(id));
	}
	void AddNewBeefCattle(int id) {
		beefLivestock.push_back(new BeefCattle(id));
	}
	vector<Cattle*> getDairyCattle() { return dairyLivestock; }
	vector<Cattle*> getBeefCattle() { return beefLivestock; }

    void feeding_cattle(){
        feeddairycattle->feed_cattle(dairyLivestock);
        feedbeefcattle->feed_cattle(beefLivestock);
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
    farm->feeding_cattle();
	getchar();
	return 0;
}

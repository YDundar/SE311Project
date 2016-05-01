#include <iostream>
#include <vector>
#include <time.h>

using namespace std;
//a
class ZigbeeDevice;
class Farm;
class LivestockDatabase {
private:
	LivestockDatabase() {};
	vector<ZigbeeDevice*> deviceList;
	static LivestockDatabase* database;
public:
	void AddDevice(ZigbeeDevice* ZBDevice) {
		deviceList.push_back(ZBDevice);
	}
	void listDevices();
	void listLocations();
	static LivestockDatabase* GetDatabase() { return database; }
	virtual void BluetoothToZigbee() {};
};

class ZigbeeDevice {
private:
	int deviceHolderID;
	double location[3] = { 0,0,0 };
	
public:
	ZigbeeDevice(int id) {LivestockDatabase::GetDatabase()->AddDevice(this); deviceHolderID = id; }
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
class BluetoothDevice {
private:
	int deviceHolderID;
	double location[3] = { 0,0,0 };
public:
	BluetoothDevice(int id) { deviceHolderID = id; }
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

class BluetoothToDatabaseAdapter :public LivestockDatabase {
private:
	BluetoothDevice* adaptee;
public:
	void BluetoothToZigbee() {

	}
};

class Cattle {
public:
	Cattle() {}
private:
	
};

class DairyCattle:public Cattle {
private:
	int uniqueID;
	BluetoothDevice* TrackingDevice;
public:
	DairyCattle(int id) { uniqueID = id; TrackingDevice = new BluetoothDevice(uniqueID); }

};

class BeefCattle:public Cattle {
private:
	int uniqueID;
	ZigbeeDevice* TrackingDevice;
public:
	BeefCattle(int id) { uniqueID = id; TrackingDevice = new ZigbeeDevice(uniqueID); }
};

class Farm {
private:
	vector <Cattle*> dairyLivestock;
	vector <Cattle*> beefLivestock;
public:
	
	void AddNewDairyCattle(int id) {
		dairyLivestock.push_back(new DairyCattle(id));
	}
	void AddNewBeefCattle(int id) {
		beefLivestock.push_back(new BeefCattle(id));
	}
	vector<Cattle*> getDairyCattle() { return dairyLivestock; }
	vector<Cattle*> getBeefCattle() { return beefLivestock; }
};


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
LivestockDatabase* LivestockDatabase::database = new LivestockDatabase();

int main() {
	srand(time(NULL));
	Farm* farm = new Farm();

	farm->AddNewBeefCattle(31234);
	farm->AddNewBeefCattle(43569);

	LivestockDatabase::GetDatabase()->listDevices();

	LivestockDatabase::GetDatabase()->listLocations();

	getchar();
	return 0;
}
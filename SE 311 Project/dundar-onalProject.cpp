//Yigit Can Dündar (20130601019) - Selin Önal () - Source file for SE 311 Term Project (2016)
#include "dundar-onalProject.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

//Database singleton initialization.
LivestockDatabase* LivestockDatabase::database = new LivestockDatabase();

//Definition for Database Method "listDevices".
void LivestockDatabase::listDevices() {
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		cout <<"Device #"<< i + 1 << " is on Cattle ID: " << deviceList[i]->getDeviceHolderID() << endl;
	}
}

//Definition for Database Method "listLocations".
void LivestockDatabase::listLocations() {
    double * p;
	for (unsigned int i = 0; i < deviceList.size(); i++) {
		deviceList[i]->updateLocation();
		cout << "Cattle #" << deviceList[i]->getDeviceHolderID() << " is at location: ";
		deviceList[i]->printLocationInfo();
		cout << endl;
		p=deviceList[i]->getCurrentLocation();
		if(*p>15 || *(p+2) >15){
			cout<< "--------->"<<"WARNING: Cattle #"<<deviceList[i]->getDeviceHolderID() <<" is outside the farm boundaries!!!"<<endl ;
		}
	}
}

//The Farmer Client
int main() {
	srand(time(NULL)); //Seed for location randomizer

	Farm* farm = new Farm(); // Farm facade is created

	//Cattle are added to the farm
	farm->AddNewBeefCattle(31234);
	farm->AddNewBeefCattle(43569);
	farm->AddNewDairyCattle(45435);
	farm->AddNewDairyCattle(35433);

	cout << "--->Connected devices are listed." << endl;
	LivestockDatabase::GetDatabase()->listDevices();

	cout << endl;

	cout << "--->Locations of Cattle animals are listed." << endl;
	LivestockDatabase::GetDatabase()->listLocations();

	cout << endl;

	cout << "--->Catlle is being fed." << endl;
	farm->FeedLivestock();

	cout << endl;

	cout << "--->Veterinary Physician visits the farm." << endl;
	farm->AcceptVisitor(new Veterinarian());

	cout << endl;

	cout << "--->Ministry of Food, Agriculture and Livestock visits the farm." << endl;
	farm->AcceptVisitor(new MinistryRepresentative());

	getchar();
	return 0;
}

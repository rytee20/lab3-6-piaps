#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <assert.h>
using namespace std;

// Component
class Unit
{
public:
	//virtual int getStrength() = 0;
	virtual void addUnit(Unit* p) {
			assert(false);
	}
	virtual ~Unit() {}
};


// Primitives 

class Pasenger:public Unit
{
private:
	int laggage;
public:
	Pasenger(int l) {
		laggage = l;
	}

	void setLaggege(int l) {
		laggage = l;
	}

	int getLaggege() {
		return laggage;
	}
};

class Stewardess : public Unit 
{
public:
	Stewardess() {
		cout << "Создан стюард" << endl;
	}
};

class Pilot :public Unit 
{
public:
	Pilot() {
		cout << "Создан пилот" << endl;
	}
};


// Composite

enum MaxPassengers { EconomPass = 150, BusinessPass = 20, FirstPass = 10, StewardessPass = 6, PilotPass = 2};
enum MaxWeight { EconomWeight = 20, BusinessWeight = 35, AllWeight = 60, MaxWeight = 7200};

class CompositeFirstClass : public Unit
{
private:
	vector<Pasenger*> fc;
	int sumWeight = 0;
public:
	void addPassenger(Pasenger* p) {
		if (fc.size() < FirstPass) {
			fc.push_back(p);
			sumWeight += p->getLaggege();
			cout << "Добавлен пассажир первого класса с весом багажа " << p->getLaggege() << endl;
		}
	}

	int getWeight() {
		return sumWeight;
	}

	int getPassengersAmount() {
		return fc.size();
	}

	~CompositeFirstClass() {
		for (int i = 0; i < fc.size(); ++i)
			delete fc[i];
	}
};

class CompositeBusinessClass : public Unit
{
private:
	vector<Pasenger*> bc;
	int sumWeight = 0;

public:
	void addPassenger(Pasenger* p) {
		if (bc.size() < BusinessPass) {
			bc.push_back(p);
			sumWeight += p->getLaggege();
			cout << "Добавлен пассажир бизнес класса с весом багажа " << p->getLaggege() << endl;
			if (overpayment(p)) cout << "Пассажир должен заплатить за перевес" << endl;
		}
	}

	int getWeight() {
		return sumWeight;
	}

	int getPassengersAmount() {
		return bc.size();
	}

	bool overpayment(Pasenger* p) {
		if (p->getLaggege() > BusinessWeight) return true;
		else return false;
	}

	~CompositeBusinessClass() {
		for (int i = 0; i < bc.size(); ++i)
			delete bc[i];
	}
};

class CompositeEconomClass : public Unit
{
private:
	vector<Pasenger*> ec;
	int sumWeight = 0;

public:
	void addPassenger(Pasenger* p) {
		if (ec.size() < EconomPass) {
			ec.push_back(p);
			sumWeight += p->getLaggege();
			cout << "Добавлен пассажир эконом класса с весом багажа " << p->getLaggege() << endl;
			if (overpayment(p)) cout << "Пассажир должен заплатить за перевес" << endl;
		}
	}

	int getWeight() {
		return sumWeight;
	}

	int getPassengersAmount() {
		return ec.size();
	}

	int passengerWithMaxWeight() {
		int max = 0;
		int index;

		for (int i = 0;i < ec.size();i++) {
			if (ec[i]->getLaggege() > max) {
				max = ec[i]->getLaggege();
				index = i;
			}
		}

		return index;
	}

	void laggageRemoval(int index) {
		sumWeight -= ec[index]->getLaggege();
		ec[index]->setLaggege(0);
	}

	bool overpayment(Pasenger* p) {
		if (p->getLaggege() > EconomWeight) return true;
		else return false;
	}

	~CompositeEconomClass() {
		for (int i = 0; i < ec.size(); ++i)
			delete ec[i];
	}
};

class Airplane :public Unit
{
private:
	CompositeBusinessClass* business = new CompositeBusinessClass;
	CompositeEconomClass* econom = new CompositeEconomClass;
	CompositeFirstClass* first = new CompositeFirstClass;
	vector<Stewardess*> stewardess;
	vector<Pilot*> pilot;
	int weight;

public:

	void addBusiness(Pasenger* p) {
		business->addPassenger(p);
	}

	void addEconom(Pasenger* p) {
		econom->addPassenger(p);
	}

	void addFirst(Pasenger* p) {
		first->addPassenger(p);
	}

	void addStewardess(Stewardess* p) {
		stewardess.push_back(p);
	}

	void addPilot(Pilot* p) {
		pilot.push_back(p);
	}

	void setWeight() {
		weight = first->getWeight() + econom->getWeight() + business->getWeight();
	}

	int getWeight() {
		return weight;
	}

	bool excessWeight() {
		if (weight > MaxWeight) {
			cout << endl << "Превышен максимальный вес багажа" << endl;
			return true;
		}
		else return false;
	}

	bool isReady() {
		if (pilot.size() != 2 || stewardess.size() != 6 ||
			(econom->getPassengersAmount() == 0 && business->getPassengersAmount() == 0 && first->getPassengersAmount() == 0)) {
			cout << endl << "Самолет не готов к вылету (нет экипажа и/или пассажиров)" << endl;
			return false;
		}
		else if (excessWeight()) {
			cout << "Самолет не готов к вылету" << endl;
			return false;
		}
		else {
			cout << endl << "Самолет готов к вылету" << endl;
			return true;
		}
	}

	void laggageRemoval() {
		while (weight > MaxWeight) {
			econom->laggageRemoval(econom->passengerWithMaxWeight());
		}
	}
};

int randWeight() {
	int end = 60;
	int start = 5;
	return rand() % (end - start + 1) + start;
}

Airplane* createAirplane()
{
	Airplane* airplane = new Airplane;

	for (int i = 0; i < FirstPass; ++i)
		airplane->addFirst(new Pasenger(randWeight()));

	for (int i = 0; i < BusinessPass; ++i)
		airplane->addBusiness(new Pasenger(randWeight()));

	for (int i = 0; i < EconomPass; ++i)
		airplane->addEconom(new Pasenger(randWeight()));

	for (int i = 0; i < StewardessPass; ++i)
		airplane->addStewardess(new Stewardess);

	for (int i = 0; i < PilotPass; ++i)
		airplane->addPilot(new Pilot);

	airplane->setWeight();

	cout << endl << "Создан самолет с общим весом багажа " << airplane->getWeight() << endl;

	return airplane;
}
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
#include<math.h>
using namespace std;

int totalPointsCovered = 0;
int ptsBought = 0;
const double budget = 10.00;
double totalCost = 0.00;
double totalValue = 0.0;

int sizeHI = 0;
int sizeHM = 0;
int sizeML = 0;
int sizeLI = 0;
 
struct Point{
	int xPos = rand() % 100 + 1;
	int yPos = rand() % 100 + 1;

	int radLX = xPos - 2;
	int radHX = xPos + 2;
	int radLY = yPos - 2;
	int radHY = yPos + 2;

	int weight = 1;
	double cost = 0.0;
	double value = 0.0;	

	int arrPos = 0;;
	bool isCovered = false;
	bool isBought = false;
};

//Sorts point AND assigns costs
void sortPoints(Point arr[], int size, Point arrHI[], Point arrHM[], Point arrML[], Point arrLI[]) {

	for (int i = 0; i < size; i++) {
		//High income
		if (arr[i].xPos >= 50 && arr[i].yPos >= 50) {
			arrHI[sizeHI] = arr[i];
			arrHI[sizeHI].arrPos = i;
			sizeHI++;
			arr[i].cost = .01 * (rand() % 25 + 100.00);
		}
		//High Mid income
		else if (arr[i].xPos <= 50 && arr[i].yPos >= 50) {
			arrHM[sizeHM] = arr[i];
			arrHM[sizeHM].arrPos = i;
			sizeHM++;
			arr[i].cost = .01 * (rand() % 25 + 70.00);
		}
		//Mid Low income
		else if (arr[i].xPos <= 50 && arr[i].yPos <= 50) {
			arrML[sizeML] = arr[i];
			arrML[sizeML].arrPos = i;
			sizeML++;
			arr[i].cost = .01 * (rand() % 25 + 40.00);
		}
		//Low income
		else if (arr[i].xPos >= 50 && arr[i].yPos <= 50) {
			arrLI[sizeLI] = arr[i];
			arrLI[sizeLI].arrPos = i;
			sizeLI++;
			arr[i].cost = .01 * (rand() % 25 + 10.00);
		}
		else {
			cout << "\nERROR\n";
		}
	}

	return;
}

void calculateWeight(Point arrCluster[], int size, Point arr[]) {
	Point* temp = arrCluster;
	int pos = 0;
	//Checks if there are points within a point's radius
	while (pos < size) {
		//Searches through all the points in the HIcluster
		for (int i = 0; i < size; i++) {
			//Ignores the point of interest
			if ((temp->xPos == arrCluster[i].xPos && temp->yPos == arrCluster[i].yPos)) {
				continue;
			}
			//Checks if the point is within the point of interest's radius. 
			//If the points' radii intersect on either the x-plane or y-plane it counts as being within.
			else if (abs(temp->radLX - arrCluster[i].radLX) <= 2 || abs(temp->radHX - arrCluster[i].radHX) <= 2
				|| abs(temp->radLY - arrCluster[i].radLY) <= 2 || abs(temp->radHY - arrCluster [i] .radHY) <= 2) {
				int j = arrCluster[i].arrPos;
				arr[j].weight++;
			}
		}
		pos++;
		temp = &arrCluster[pos];
	}
	return;
}

void setValues(Point arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i].value = arr[i].weight / arr[i].cost;
	}
}

//Sort array by value
int partition(Point arr[], int first, int last) {

	double pivot = arr[first].value;
	int count = 0;
	
	//Counts the elements on the left until reaching one that is higher than our first element
	for (int i = first + 1; i <= last; i++) {
		if (arr[i].value <= pivot) {
			count++;
		}
	}

	//Move pivotPos to a better location
	int pivotPos = first + count;
	swap(arr[pivotPos], arr[first]);

	//Sorting the left side and right side of the pivotPos
	for (int i = first, j = last; i < pivotPos && j > pivotPos;){
		if (arr[i].value <= pivot) {
			i++;
		}
		if (arr[j].value > pivot) {
			j--;
		}
		if (i < pivotPos && j > pivotPos) {
			swap(arr[i++], arr[j--]);
		}
	}

	return pivotPos;
}
void quickSort(Point arr[], int first, int last) {
	
	//Base Case
	if (first >= last) {
		return;
	}

	//Partition
	int part = partition(arr, first, last);

	//Sort left side
	quickSort(arr, first, part - 1);

	//Sort right side
	quickSort(arr, part + 1, last);
	
	return;
}

//"Buy data" from sorted array
void buyData(Point arr[], int size) {

	for (int i = size - 1; i >= 0; i--) {
		if (totalCost > (budget - arr[i].cost)) {
			continue;
		}

		totalPointsCovered += arr[i].weight;
		ptsBought++;
		totalCost += arr[i].cost;
		totalValue += arr[i].value;
	}
	return;
}

void calculateWeightCoverage(Point arrSorted[], int size) {
	Point* temp = &arrSorted[size-1];
	int pos = 99;

	//Resets weights
	for (int i = 0; i < size; i++) {
		arrSorted[i].weight = 1;
	}

	//Checks if there are points within a point's radius
	while (pos >= 0) {
		//Checks each point in the sorted array against the point of interest
		for (int i = 0; i < size; i++) {
			//Ignores the point of interest
			if ((temp->xPos == arrSorted[i].xPos && temp->yPos == arrSorted[i].yPos)) {
				arrSorted[i].isCovered = true;
				continue;
			}
			//Checks if the point is within the point of interest's radius. 
			//If the points' radii intersect on either the x-plane or y-plane it counts as being within.
			else if ((abs(temp->radLX - arrSorted[i].radLX) <= 2 || abs(temp->radHX - arrSorted[i].radHX) <= 2
				|| abs(temp->radLY - arrSorted[i].radLY) <= 2 || abs(temp->radHY - arrSorted[i].radHY) <= 2) && arrSorted[i].isCovered == false) {
				//increase the weight of Point temp points to
				arrSorted[pos].weight++;
				//Change the Point being added to isCovered = true
				arrSorted[i].isCovered = true;
			}
		}
		pos--;
		temp = &arrSorted[pos];
	}
	return;
}

//Prints summary of the data
void printData(double dataSize) {
	double percentCovered = (totalPointsCovered / dataSize) * 100;
	double percentSpent = (totalCost / budget) * 100.00;

	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "Points covered: " << totalPointsCovered << "\n";
	cout << "Points bought: " << ptsBought << "\n";
	cout << "Total Spent: " << setprecision(4) << totalCost << "\n";
	cout << "Total Value: " << totalValue << "\n";
	cout << "|\n";
	cout << "Percent of data covered: " << setprecision(4) << percentCovered << "%\n";
	cout << "Percent of budget spent: " << setprecision(4) << percentSpent << "%\n";
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
}

//Greedy//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int greedyPart(Point arr[], int first, int last) {

	double pivot = arr[first].value;
	int count = 0;

	//Counts the elements on the left until reaching one that is higher than our first element
	for (int i = first + 1; i <= last; i++) {
		if (arr[i].cost <= pivot) {
			count++;
		}
	}

	//Move pivotPos to a better location
	int pivotPos = first + count;
	swap(arr[pivotPos], arr[first]);

	//Sorting the left side and right side of the pivotPos
	for (int i = first, j = last; i < pivotPos && j > pivotPos;) {
		if (arr[i].cost <= pivot) {
			i++;
		}
		if (arr[j].cost > pivot) {
			j--;
		}
		if (i < pivotPos && j > pivotPos) {
			swap(arr[i++], arr[j--]);
		}
	}

	return pivotPos;
}
void greedyQS(Point arr[], int first, int last) {

	//Base Case
	if (first >= last) {
		return;
	}

	//Partition
	int part = greedyPart(arr, first, last);

	//Sort left side
	greedyQS(arr, first, part - 1);

	//Sort right side
	greedyQS(arr, part + 1, last);

	return;
}

//Random/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void buyRandom(Point arr[], int size) {
	
	bool canBuy = true;

	for (int i = 0; i < size; i++) {
		//Chooses a random Point
		int randPos = rand() % size;
		if (totalCost > (budget - arr[randPos].cost)) {
			//If the Point will cause the totalCost to exceed the budget then we don't buy it.
			canBuy = false;
		}
		//If the point hasn't been bought already and it will not make us go over budget, then we buy it.
		else if (arr[randPos].isBought == false && canBuy == true) {
			totalPointsCovered += arr[randPos].weight;
			ptsBought++;
			totalCost += arr[randPos].cost;
			totalValue += arr[randPos].value;
			arr[randPos].isBought = true;
		}
	}
}

int main() {
	
	//Intialize random seed
	srand(time(NULL));

	//Initializing arrays
	Point arr[100] = {};
	Point* arrHI = new Point[100];
	Point* arrHM = new Point[100];
	Point* arrML = new Point[100];
	Point* arrLI = new Point[100];

	Point* greedyArr = new Point[100];

	//Group points into clusters
	sortPoints(arr, 100, arrHI, arrHM, arrML, arrLI);

	//Calculate the weight for each point in each cluster
	calculateWeight(arrHI, sizeHI, arr);
	calculateWeight(arrHM, sizeHM, arr);
	calculateWeight(arrML, sizeML, arr);
	calculateWeight(arrLI, sizeLI, arr);

	//Clean up arrays that won't be used anymore. 
	delete[] arrHI;
	delete[] arrHM;
	delete[] arrML;
	delete[] arrLI;

	//Set values
	setValues(arr, 100);

	//Sorts array purely based on weight of each point (not accounting for redundancy)
	//Sort array by value
	quickSort(arr, 0, 99);

	//Redo weights starting with highest coverage to avoid redundancy
	calculateWeightCoverage(arr, 99);

	//Reset values
	setValues(arr, 100);

	//Resort data based on new weights
	quickSort(arr, 0, 99);

	//Buy data from the now correctly weighted & sorted array
	buyData(arr, 100);

	//Print the data summary
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "Weighted Set Cover\n";
	printData(100);


	//Greedy//////////////////////////////////////////////////////////////
	//Reset Data
	totalPointsCovered = 0;
	ptsBought = 0;
	totalCost = 0.0;
	totalValue = 0.0;

	//Preform greedy ops
	greedyQS(arr, 0, 99);
	buyData(arr, 100);
	cout << "Greedy\n";
	printData(100);

	//Random/////////////////////////////////////////////////////////////
	//Reset Data
	totalPointsCovered = 0;
	ptsBought = 0;
	totalCost = 0.0;
	totalValue = 0.0;

	//Preform random ops
	buyRandom(arr, 100);
	cout << "Random\n";
	printData(100);

	return 0;
}
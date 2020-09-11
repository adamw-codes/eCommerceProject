	// Program created by Adam M. Wajahat
	//Problem: Ask user for products, quantities, and user information to take orders
	//Input : Products information file(quantities, prices, product names)
	//Output : The invoice of the order with a total and the customers name and address

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

void readFile(double[145][4]);
void fillEmptyArray(double[145][5]);
int userChoice();
int searchProduct(double[145][4], double, int);
void displayProduct(double[145][4], int);
int quantityAvailable(double[145][4], int);
void cart(double[145][4], double[145][5], int, int, int);
void userInfo(string &, string &, string &);
void outputToScreen(double[145][5], string, string, string);
int productOutput(double[145][5], int index);
void totalOutput(double[145][5]);
void outputToFile(double[145][5], string, string, string);
int productOutputFile(double[145][5], int, ofstream &);
void totalOutputFile(double[145][5], ofstream &);


int main()
{
	// declaring arrays for products and cart
	double productArr[145][4];
	double cartArr[145][5];
	// declaring a counter and set it to 0
	int counter = 0;
	// call on readFile function to fill productArr with values from file
	readFile(productArr);
	// call on fillEmptyArray to fill cartArr with -1
	fillEmptyArray(cartArr);
	// call on userChoice to get the user choice's on product and store the returned value in productNumber
	double productNumber = userChoice();
	// if the user picks 0 on as their firs entry
	if (productNumber == 0) {
		// program ends
		return 0;
	}
	// while the productNumber value from the user is not equal to 0 (0 being when the user is done) and is less than the max amount of values in a one invoice
	while (productNumber != 0 && counter <= 144) {
		// declare and set index, itemLocation, quantity to 0
		int index = 0;
		int itemLocation = 0;
		int quantity = 0;
		// calls on the searchProduct function to find the item's location in the array and stores it in itemLocation
		itemLocation = searchProduct(productArr, productNumber, index);
		// if the itemLocation is not 145 (145 being not found returned value)
		if (itemLocation != 145) {
			// calls on displayProduct function to display the product details
			displayProduct(productArr, itemLocation);
			// calls on quantityAvailable function to get the quantity, check if it is available, and then stores the returned value in quantity
			quantity = quantityAvailable(productArr, itemLocation);
			// if the quantity is 0 (0 being the item is not in stock)
			if (quantity == 0) {
				// asks user for a new choice
				productNumber = userChoice();
				// drops this iteration of the loop and uses new productNumber
				continue;
			}
			// calls on the cart function to store this order details
			cart(productArr, cartArr, itemLocation, quantity, counter);
			// increases the counter to go the next row
			counter++;
			// asks the user for a new productNumber for the next iriteration
			productNumber = userChoice();
		}
		else {
			cout << "Item could not be found. Please enter a valid number." << endl;
			productNumber = userChoice();
		}
	}
	// declare strings that would be passed by reference to the functions
	string userName;
	string streetAddress;
	string cityStateZip;
	// calls on the userInfo function to get the user's name, street address, city, state, and zip code
	userInfo(userName, streetAddress, cityStateZip);
	// calls on the outputToScreen function to display all the information
	outputToScreen(cartArr, userName, streetAddress, cityStateZip);
	// calls on the outputToFile function to output all the information to the file output
	outputToFile(cartArr, userName, streetAddress, cityStateZip);

	return 0;
}

void readFile(double productArr[145][4]) {
	// declares productFile as input file and opens it
	ifstream productFile;
	productFile.open("ProductData.csv", ios::in);
	
	// if the productFile opens successfully
	if(productFile.is_open()) {
		// skip the first line
		productFile.seekg(27L, ios::beg);
		// start from row 1 all the way to 145
		for (int i = 0; i < 145; i++) {
			// start from column 1 to 4
			for (int j = 0; j < 4; j++) {
				// if the column is 1
				if (j == 0) {
					// read in that value
					productFile >> productArr[i][j];
				}
				// if column is 2
				if (j == 1) {
					// skip the word "Product " and read in that value
					productFile.seekg(9L, ios::cur);
					productFile >> productArr[i][j];
				}
				// if column is 3
				if (j == 2) {
					// skip the comma and read in that value
					productFile.seekg(1L, ios::cur);
					productFile >> productArr[i][j];
				}
				// if column is 4
				if (j == 3) {
					// skip the comma, read in the value, then skip the next comma and new line character
					productFile.seekg(1L, ios::cur);
					productFile >> productArr[i][j];
					productFile.seekg(2L, ios::cur);
				}
			}
		}
	}
	// if the file does not open output error message
	else{
		cout << "Error in processing file." << endl;
	}
	// close file
	productFile.close();
}

void fillEmptyArray(double cartArr[145][5]) {
	// start from row 1 to 145
	for (int i = 0; i < 145; i++) {
		// start from row 1 to 5
		for (int j = 0; j < 5; j++) {
			// store -1 in that location
			cartArr[i][j] = -1;
		}
	}
}

int userChoice() {
	// declare and set productNumber to 0
	int productNumber = 0;
	// give user the prompt to enter in a product number and store it in productNumber
	cout << "Please enter in a product number. When you are done selecting your items, please press 0. Product number: ";
	cin >> productNumber;
	// return the productNumber
	return productNumber;
}

int searchProduct(double productArr[145][4], double productNumber, int index) {
	// if that product is found at that index
	if (productArr[index][0] == productNumber) {
		// return that index
		return index;
	}
	// if the function reaches to the index 145
	else if (index == 145) {
		// returns 145, which signifies the item has not been found
		return index;
	}
	// if the product is not found in that iriteration
	else {
		// increment index by 1
		index = index + 1;
		// recursively call on the searchProduct again with a new index
		searchProduct(productArr, productNumber, index);
	}
}

void displayProduct(double productArr[145][4], int itemLocation) {
	// display the product's name in that location and cost in the productArr
	cout << "Product Name: " << productArr[itemLocation][1] << endl;
	cout << "Cost: " << productArr[itemLocation][2] << endl;
}

int quantityAvailable(double productArr[145][4], int itemLocation) {
	// get the available quantity by going to that location and going to the 3rd column
	double quantityIn = productArr[itemLocation][3];
	// declare quantityOut for user's wanted quantity
	double quantityOut;
	// if the product is not in stock
	if (quantityIn == 0) {
		// give user an error message and return 0
		cout << "Sorry, we are out of this item" << endl;
		return 0;
	}
	// prompt the user for a quantity and store it in quantityOut
	cout << "Please enter how many of this item you would like." << endl;
	cin >> quantityOut;
	// if the quantity given by the user is less than or equal to the quantity in stock
	if (quantityOut <= quantityIn) {
		// give the user a success message and return the quantity the user wants
		cout << "Item has been added to your shopping cart!" << endl;
		return quantityOut;
	}
	// if the user picks more than the quantity in stock
	else {
		// display error message and recursively call on the function for a new iriteration
		cout << "Sorry, we do not have that many items in stock." << endl;
		quantityAvailable(productArr, itemLocation);
	}
}

void cart(double productArr[145][4], double cartArr[145][5], int itemLocation, int quantity, int counter) {
	// set up variables to transfer information from productArr into cartArr
	double tempProductNumber;
	double tempProductName;
	double tempPrice;
	double totalPrice;

	// set each of the variables equal to the location of the array with each corresponding value
	tempProductNumber = productArr[itemLocation][0];
	tempProductName = productArr[itemLocation][1];
	tempPrice = productArr[itemLocation][2];
	// set total price equal to the price of one unit times the quantity the user wants
	totalPrice = tempPrice * quantity;

	// store each of the values into the respective locations in the cartArr
	cartArr[counter][0] = tempProductNumber;
	cartArr[counter][1] = tempProductName;
	cartArr[counter][2] = quantity;
	cartArr[counter][3] = tempPrice;
	cartArr[counter][4] = totalPrice;
}

void userInfo(string &userName, string &streetAddress, string &cityStateZip) {
	// clears the buffer on the lines
	cin.ignore(1000, '\n');
	// prompts the user to enter their first and last name
	cout << "Please enter in your first and last name: " << endl;
	// getline to read in the entire string until the new line character
	getline(cin, userName);
	// prompts the user to enter their street address
	cout << "Please enter in your street address: " << endl;
	// getline to read in the entire string until the new line character
	getline(cin, streetAddress);
	// prompts the user to enter their city, state, and zip code
	cout << "Please enter in your city, state, and zip code: " << endl;
	// getline to read in the entire string until the new line character
	getline(cin, cityStateZip);
}

void outputToScreen(double cartArr[145][5], string userName, string streetAddress, string cityStateZip) {
	// declare run and index and set index to 0
	int run;
	int index = 0;
	// display out the Customer's name, address, city, state, and zip code
	cout << "Customer Name: " << userName << endl;
	cout << "Customer Address: " << streetAddress << endl;
	cout << "\t          " << cityStateZip << endl;
	cout << endl;
	// set the title of the table
	cout << "Item Number		Item Name		Qty		Unit Cost		Total" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	// gets the returned value from productOutput
	run = productOutput(cartArr, index);
	// if the returned value is 1 (1 being item quantity 0)
	if (run == 1) {
		// increment index
		index++;
	}
	// while the run is not equal to 0 (0 being that there are no more items in the cart)
	while (run != 0) {
		// if the returned value is 1
		if (run == 1) {
			// increment index and skip that iriteration
			index++;
			continue;
		}
		// increment index
		index++;
		// call run on the next item in the cart
		run = productOutput(cartArr, index);
	}
	cout << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	// call on totalOutputFile to display total output
	totalOutput(cartArr);
}

int productOutput(double cartArr[145][5], int index) {
	// set each variable equal to the respective value in the array
	double productNumber = cartArr[index][0];
	double productName = cartArr[index][1];
	double quantity = cartArr[index][2];
	double unitCost = cartArr[index][3];
	double total = cartArr[index][4];

	// if it reaches the end of the cart array, return 0
	if (cartArr[index][0] == -1) {
		return 0;
	}
	// if the quantity is 0, return 1
	else if (cartArr[index][2] == 0){
		return 1;
	}
	// if all conditions met, then display the product information and return 2
	else {
		cout << setprecision(0) << fixed << "Item Number " << productNumber << "\t" << setw(18) << left << productName << "\t" << quantity << "\t\t" << setprecision(2) << fixed << setw(13) << left << unitCost << "\t\t" << setw(8) << left << total << endl;
		cout << "...." << endl;
		return 2;
	}
}

void totalOutput(double cartArr[145][5]) {
	// declare variables and intitalize them to 0
	int index = 0;
	double sum = 0;
	// while the value is not -1
	while (cartArr[index][4] != -1) {
		// add up the total price from that row
		sum += cartArr[index][4];
		// increment index for the next row
		index++;
	}
	// display out total
	cout << setw(95) << right << "Total: " << sum << endl;
}

void outputToFile(double cartArr[145][5], string userName, string streetAddress, string cityStateZip) {
	// declare run and index and set index to 0
	int run;
	int index = 0;
	// declare outputFile as an output file stream
	ofstream outputFile;
	// open the txt file invoice
	outputFile.open("invoice.txt");
	// // display out the Customer's name, address, city, state, and zip code
	outputFile << "Customer Name: " << userName << endl;
	outputFile << "Customer Address: " << streetAddress << endl;
	outputFile << "\t          " << cityStateZip << endl;
	outputFile << endl;
	// set the title of the table
	outputFile << "Item Number		Item Name		Qty		Unit Cost		Total" << endl;
	outputFile << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	// gets the returned value from productOutput
	run = productOutputFile(cartArr, index, outputFile);
	// if the returned value is 1 (1 being item quantity 0)
	if (run == 1) {
		index++;
	}
	// while the run is not equal to 0 (0 being that there are no more items in the cart)
	while (run != 0) {
		// if the returned value is 1
		if (run == 1) {
			// increment index and skip that iriteration
			index++;
			continue;
		}
		// increment index
		index++;
		// call run on the next item in the cart
		run = productOutputFile(cartArr, index, outputFile);
	}
	outputFile << "------------------------------------------------------------------------------------------------------------------------------------------" << endl;
	// call on totalOutputFile to display total output
	totalOutputFile(cartArr, outputFile);
	// file close
	outputFile.close();
}

int productOutputFile(double cartArr[145][5], int index, ofstream &outputFile){
	// set each variable equal to the respective value in the array
	double productNumber = cartArr[index][0];
	double productName = cartArr[index][1];
	double quantity = cartArr[index][2];
	double unitCost = cartArr[index][3];
	double total = cartArr[index][4];

	// if it reaches the end of the cart array, return 0
	if (cartArr[index][0] == -1) {
		return 0;
	}
	// if the quantity is 0, return 1
	else if (cartArr[index][2] == 0) {
		return 1;
	}
	// if all conditions met, then display the product information and return 2
	else {
		outputFile << setprecision(0) << fixed << "Item Number " << productNumber << "\t" << setw(18) << left << productName << "\t" << quantity << "\t\t" << setprecision(2) << fixed << setw(13) << left << unitCost << "\t\t" << setw(8) << left << total << endl;
		outputFile << "...." << endl;
		return 2;
	}
	}

void totalOutputFile(double cartArr[145][5], ofstream &outputFile) {
	// declare variables and intitalize them to 0
	int index = 0;
	double sum = 0;
	// while the value is not -1
	while (cartArr[index][4] != -1) {
		// add up the total price from that row
		sum += cartArr[index][4];
		// increment index for the next row
		index++;
	}
	// display out total to file
	outputFile << setw(95) << right << "Total: " << sum << endl;
}
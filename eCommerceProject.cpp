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

struct cartInfo {
	int productNum;
	int productName;
	int quantity;
	double cost;
	double total;
};

struct products {
	int productNum;
	int productName;
	double cost;
	int quantity;
};

int numberOfItems();
void readFile(products*);
void fillEmptyArray(cartInfo*, int);
int userChoice(cartInfo*);
void showCart(cartInfo*);
int searchProduct(products*, double, int, int);
void displayProduct(products*, int);
int quantityAvailable(products*, int, cartInfo*, int);
int searchCart(cartInfo*, double, int, int);
void cartIn(products*, cartInfo*, int, int, int, int);
void userInfo(string &, string &, string &);
void outputToScreen(cartInfo *, string, string, string);
int productOutput(cartInfo *, int index);
void totalOutput(cartInfo *);
void outputToFile(cartInfo *, string, string, string);
int productOutputFile(cartInfo *, int, ofstream &);
void totalOutputFile(cartInfo *, ofstream &);
void updateFile(cartInfo*);


int main()
{
	// call on numberOfItems and stores it in itemsInFile
	int itemsInFile = numberOfItems();
	// creates pointer arrays with dimensions of itemsInFile
	products *productArr = new products[itemsInFile];
	cartInfo *cartArr = new cartInfo[itemsInFile];
	// declaring a counter and set it to 0
	int counter = 0;
	// call on readFile function to fill productArr with values from file
	readFile(productArr);
	// call on fillEmptyArray to fill cartArr with -1
	fillEmptyArray(cartArr, itemsInFile);
	// call on userChoice to get the user choice's on product and store the returned value in productNumber
	double productNumber = userChoice(cartArr);
	// if the user picks 0 on as their first entry
	if (productNumber == 0) {
			// program ends
			return 0;
		}
	// while the productNumber value from the user is not equal to 0 (0 being when the user is done) and is less than the max amount of values in a one invoice
	while (productNumber != 0 && counter <= itemsInFile-1) {
		if (productNumber == -2) {
			productNumber = userChoice(cartArr);
			continue;
		}
		// declare and set index, itemLocation, quantity to 0
		int index = 0;
		int itemLocation = 0;
		int quantity = 0;
		// calls on the searchProduct function to find the item's location in the array and stores it in itemLocation
		itemLocation = searchProduct(productArr, productNumber, index, itemsInFile);
		// if the itemLocation is not 145 (145 being not found returned value)
		if (itemLocation != itemsInFile) {
			// calls on displayProduct function to display the product details
			displayProduct(productArr, itemLocation);
			// calls on quantityAvailable function to get the quantity, check if it is available, and then stores the returned value in quantity
			quantity = quantityAvailable(productArr, itemLocation, cartArr, itemsInFile);
			// if the quantity is 0 (0 being the item is not in stock)
			if (quantity == 0) {
				// asks user for a new choice
				productNumber = userChoice(cartArr);
				// drops this iteration of the loop and uses new productNumber
				continue;
			}


			// calls on the cart function to store this order details
			cartIn(productArr, cartArr, itemLocation, quantity, counter, itemsInFile);
			// increases the counter to go the next row
			counter++;
			// asks the user for a new productNumber for the next iriteration
			productNumber = userChoice(cartArr);
		}
		else {
			// gives error and asks user for a new productNumber
			cout << "Item could not be found. Please enter a valid number." << endl;
			productNumber = userChoice(cartArr);
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
	// calls on the updateFile function to update all teh quantities
	updateFile(cartArr);
	// deletes pointers
	delete[] productArr;
	delete[] cartArr;
	return 0;
}

int numberOfItems() {
	// declares productFile as input file and opens it
	ifstream productFile;
	int numOfProducts;
	productFile.open("ProductData.csv", ios::in);

	// if the productFile opens successfully
	if (productFile.is_open()) {
		// reads in number of products
		productFile >> numOfProducts;
	}
	else {
		// prompts error and ends program
		cout << "Error in processing file." << endl;
		exit(0);
	}
	// closes productFile and return numOfProducts
	productFile.close();
	return numOfProducts;
}

void readFile(products *productArr) {
	// declares productFile as input file and opens it
	ifstream productFile;
	int numOfProducts;
	productFile.open("ProductData.csv", ios::in);

	// if the productFile opens successfully
	if (productFile.is_open()) {
		productFile.seekg(0L, ios::beg);
		productFile >> numOfProducts;
		// skip the first line
		productFile.seekg(29L, ios::cur);
		// start from row 1 all the way to numOfProducts -1
		for (int i = 0; i < numOfProducts-1; i++) {
			int productNum = 0;
			int productName = 0;
			double productCost = 0;
			int productQuantity = 0;

			// reads in every value with respect to where it goes to in the struct
			productFile >> productNum;
			productArr[i].productNum = productNum;
			productFile.seekg(9L, ios::cur);
			productFile >> productName;
			productArr[i].productName = productName;
			productFile.seekg(1L, ios::cur);
			productFile >> productCost;
			productArr[i].cost = productCost;
			productFile.seekg(1L, ios::cur);
			productFile >> productQuantity;
			productArr[i].quantity = productQuantity;
			productFile.seekg(2L, ios::cur);
		}
		}

	// if the file does not open output error message
	else{
		cout << "Error in processing file." << endl;
	}
	// close file
	productFile.close();
}

void fillEmptyArray(cartInfo *cartArr, int itemsInFile) {
	// start from row 1 to itemsInFile
	for (int i = 0; i < itemsInFile-1; i++) {
		int temp = -1;
		double temp1 = -1;
		cartArr[i] = { temp, temp, temp, temp1 , temp1};
	}
}

int userChoice(cartInfo *cartArr) {
	// declare and set productNumber to 0
	string userInputTemp;
	int userInput;
	// give user the prompt to enter in a product number and store it in productNumber
	cout << "Please enter in a product number. When you are done selecting your items, please press 0. Product number: ";
	cin >> userInputTemp;
	// if the user enters cart
	if (userInputTemp == "cart") {
		// show the cart
		showCart(cartArr);
		return -2;
	}
	 // if the user enters a product number
	else {
		// convert string into int and return
		userInput = stoi(userInputTemp);
		return userInput;
	}
}

void showCart(cartInfo *cartArr) {
	// declare run and index and set index to 0
	int run = 0;
	int index = 0;
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

int searchProduct(products *productArr, double productNumber, int index, int itemsInFile) {
	// if that product is found at that index
	if (productArr[index].productNum == productNumber) {
		// return that index
		return index;
	}
	// if the function reaches to the itemsInFile
	else if (index == itemsInFile) {
		// returns 145, which signifies the item has not been found
		return index;
	}
	// if the product is not found in that iriteration
	else {
		// increment index by 1
		index = index + 1;
		// recursively call on the searchProduct again with a new index
		searchProduct(productArr, productNumber, index, itemsInFile);
	}
}

void displayProduct(products *productArr, int itemLocation) {
	// display the product's name in that location and cost in the productArr
	cout << "Product Name: " << productArr[itemLocation].productName << endl;
	cout << "Cost: " << productArr[itemLocation].cost << endl;
}

int quantityAvailable(products *productArr, int itemLocation, cartInfo *cartArr, int itemsInFile) {
	// get the available quantity by going to that location and going to the quantity location
	double quantityInStock = productArr[itemLocation].quantity;
	// get the productNumber by going to that location and going to the number location
	int productNumber = productArr[itemLocation].productNum;
	// declare quantityOut for user's wanted quantity
	double quantityWanted;
	// if the product is not in stock
	if (quantityInStock == 0) {
		// give user an error message and return 0
		cout << "Sorry, we are out of this item" << endl;
		return 0;
	}
	// prompt the user for a quantity and store it in quantityOut
	cout << "Please enter how many of this item you would like." << endl;
	cin >> quantityWanted;
	int index = 0;
	// searchCart function to see if item has been selected before and store returned value in variable
	int selectedQuantity = searchCart(cartArr, productNumber, index, itemsInFile);
	// add to qunatityWanted
	if (selectedQuantity == -1) {
		// if the quantity given by the user is less than or equal to the quantity in stock
		if (quantityWanted <= quantityInStock) {
			// give the user a success message and return the quantity the user wants
			cout << "Item has been added to your shopping cart!" << endl;
			return quantityWanted;
		}

		// if the user picks more than the quantity in stock
		else {
			// display error message and recursively call on the function for a new iriteration
			cout << "Sorry, we do not have that many items in stock." << endl;
			quantityAvailable(productArr, itemLocation, cartArr, itemsInFile);
		}
	}
	else {
		// set variable temp equal to the quantity wanted and the quantity already in the cart
		int temp = 0;
		temp = cartArr[selectedQuantity].quantity + quantityWanted;
		quantityWanted = temp;
		// if the total is less than the stock
		if (quantityWanted <= quantityInStock) {
			// give the user a success message and return the quantity the user wants
			cout << "Item has been added to your shopping cart!" << endl;
			return quantityWanted;
		}

		// if the user picks more than the quantity in stock
		else {
			// display error message and recursively call on the function for a new iriteration
			cout << "Sorry, we do not have that many items in stock." << endl;
			quantityAvailable(productArr, itemLocation, cartArr, itemsInFile);
		}
	}
	
}

int searchCart(cartInfo* cartArr, double productNumber, int index, int itemsInFile) {
	// if that product is found at that index
	if (cartArr[index].productNum == productNumber) {
		// return that index
		return index;
	}
	// if the function reaches to the index -1
	else if (cartArr[index].productNum == -1) {
		// returns -1, which signifies the item has not been found
		return -1;
	}
	// if the product is not found in that iriteration
	else {
		// increment index by 1
		index = index + 1;
		// recursively call on the searchCart again with a new index
		searchCart(cartArr, productNumber, index, itemsInFile);
	}
}

void cartIn(products *productArr, cartInfo *cartArr, int itemLocation, int quantity, int counter, int itemsInFile) {
	// set up variables to transfer information from productArr into cartArr
	int tempProductNumber;
	int tempProductName;
	double tempPrice;
	double totalPrice;

	// set each of the variables equal to the location of the array with each corresponding value
	tempProductNumber = productArr[itemLocation].productNum;
	tempProductName = productArr[itemLocation].productName;
	tempPrice = productArr[itemLocation].cost;
	// set total price equal to the price of one unit times the quantity the user wants
	totalPrice = tempPrice * quantity;

	// search the cart for the product number and see if its available
	int index = 0;
	int available = searchCart(cartArr, tempProductNumber, index, itemsInFile);
	// if the value is not available
	if (available == -1) {
		// store each of the values into the respective locations in the cartArr
		cartArr[counter].productNum = tempProductNumber;
		cartArr[counter].productName = tempProductName;
		cartArr[counter].quantity = quantity;
		cartArr[counter].cost = tempPrice;
		cartArr[counter].total = totalPrice;
	}
	// if the value has already been entere
	else {
		// update the quantities
		cartArr[available].quantity = quantity;
		cartArr[available].total = totalPrice;
	}
	
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

void outputToScreen(cartInfo *cartArr, string userName, string streetAddress, string cityStateZip) {
	// display out the Customer's name, address, city, state, and zip code
	cout << "Customer Name: " << userName << endl;
	cout << "Customer Address: " << streetAddress << endl;
	cout << "\t          " << cityStateZip << endl;
	cout << endl;
	// calls on showCart
	showCart(cartArr);
}

int productOutput(cartInfo *cartArr, int index) {
	// set each variable equal to the respective value in the array
	double productNumber = 0;
	productNumber = cartArr[index].productNum;
	double productName = 0;
	productName = cartArr[index].productName;
	double quantity = 0;
	quantity = cartArr[index].quantity;
	double unitCost = 0;
	unitCost = cartArr[index].cost;
	double total = 0;
	total = cartArr[index].total;

	// if it reaches the end of the cart array, return 0
	if (cartArr[index].productNum == -1) {
		return 0;
	}
	// if the quantity is 0, return 1
	else if (cartArr[index].quantity == 0){
		return 1;
	}
	// if all conditions met, then display the product information and return 2
	else {
		cout << setprecision(0) << fixed << "Item Number " << productNumber << "\t" << setw(18) << left << productName << "\t" << quantity << "\t\t" << setprecision(2) << fixed << setw(13) << left << unitCost << "\t\t" << setw(8) << left << total << endl;
		cout << "...." << endl;
		return 2;
	}
}

void totalOutput(cartInfo *cartArr) {
	// declare variables and intitalize them to 0
	int index = 0;
	double sum = 0;
	// while the value is not -1
	while (cartArr[index].total != -1) {
		// add up the total price from that row
		sum += cartArr[index].total;
		// increment index for the next row
		index++;
	}
	// display out total
	cout << setw(95) << right << "Total: " << sum << endl;
}

void outputToFile(cartInfo *cartArr, string userName, string streetAddress, string cityStateZip) {
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

int productOutputFile(cartInfo *cartArr, int index, ofstream &outputFile){
	// set each variable equal to the respective value in the array
	double productNumber = cartArr[index].productNum;
	double productName = cartArr[index].productName;
	double quantity = cartArr[index].quantity;
	double unitCost = cartArr[index].cost;
	double total = cartArr[index].total;

	// if it reaches the end of the cart array, return 0
	if (cartArr[index].productNum == -1) {
		return 0;
	}
	// if the quantity is 0, return 1
	else if (cartArr[index].quantity == 0) {
		return 1;
	}
	// if all conditions met, then display the product information and return 2
	else {
		outputFile << setprecision(0) << fixed << "Item Number " << productNumber << "\t" << setw(18) << left << productName << "\t" << quantity << "\t\t" << setprecision(2) << fixed << setw(13) << left << unitCost << "\t\t" << setw(8) << left << total << endl;
		outputFile << "...." << endl;
		return 2;
	}
	}

void totalOutputFile(cartInfo *cartArr, ofstream &outputFile) {
	// declare variables and intitalize them to 0
	int index = 0;
	double sum = 0;
	// while the value is not -1
	while (cartArr[index].total != -1) {
		// add up the total price from that row
		sum += cartArr[index].total;
		// increment index for the next row
		index++;
	}
	// display out total to file
	outputFile << setw(95) << right << "Total: " << sum << endl;
}

void updateFile(cartInfo *cartArr) {
	// declares productFile as output file and opens it
	fstream productFile;
	productFile.open("ProductData.csv");
	// declares skipVariable to skip the first value in the file
	int skipVariable;
	// brings the cursor to the front
	productFile.seekg(0L, ios::beg);
	// reads in
	productFile >> skipVariable;
	// skips the header
	productFile.seekg(29L, ios::cur);
	// declares i which would start at 0
	int i = 0;
	// while the productName in the array is not equal to -1
	while (cartArr[i].productName != -1) {
		// brings the cursor to the front
		productFile.seekg(0L, ios::beg);
		// reads in
		productFile >> skipVariable;
		// skips the header
		productFile.seekg(29L, ios::cur);
		// stores the productName in from the cartArr
		int tempProductName = cartArr[i].productName;
		// stores the quantity in from the cartArr
		int tempQuantity = cartArr[i].quantity;
		int tempQuantity1;
		double long position = 0;
		// position is equal to 29 times the line the product is on -5 to bring it infront of the quantity
		position = (tempProductName * 29) -5;
		// tells the program theres going to be input
		productFile.seekg(position, ios::cur);
		// stores in the current quantity
		productFile >> tempQuantity1;
		// calculates the left over quantity
		int quantityLeft = tempQuantity1 - tempQuantity;
		// moves the cursor back infront of the quantity
		productFile.seekp(-3L, ios::cur);
		
		// depending on how small or large the quantity is, the program will put in filler 0's
		if (quantityLeft == 0) {
			productFile << "00";
		}
		if ((quantityLeft > 0) && (quantityLeft <= 9)) {
			productFile << "00";
		}
		 if ((quantityLeft >= 10) && (quantityLeft <= 99)) {
			productFile << "0";
		}
		// display the actual quantity
		 productFile << quantityLeft;
		// increment the i to get the next iteration
		 i++;
	}
	// close productFile
	productFile.close();
}
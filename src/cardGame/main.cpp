#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<algorithm>
#include<random>
#include<cmath>

using namespace std;

//Constant quantities in this program
const size_t N {52};
const vector<string> suits {"Club", "Diamond", "Heart", "Spade"};
const vector<string> ranks {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
const size_t suits_no {suits.size()};
const size_t ranks_no {ranks.size()}; 

//Mandatory sub-routines for this program
vector<int> gen_deck();
void shuffle_deck(vector<int> &deck);
vector<vector<int>> distribute_deck(const size_t &n_players, const size_t &n_cards, const vector<int> &deck);
void card_dictionary (const int &card_ID);
void print_cards (const vector<vector<int>> &deck_table);
int input_int();
bool user_prompt();

//Optional sub-routines for debugging
void print_vector(const vector<int> &vec);
void print_vector_2d(const vector<vector<int>> &vec);
void vector_operate(vector<int> &vec, const int &value, const string &oper); 

int main(){
	bool quit {false};
	
	do {
		
		vector<int> deck {gen_deck()};
		shuffle_deck(deck);

		cout << endl << "Enter the number of players: ";
		size_t n_players {abs(input_int())};
		cout << endl << "Enter the number of cards per player (0 for all cards to be distributed): ";
		size_t n_cards {abs(input_int())};
	
		cout << endl << "Assigning cards to players..." << endl << flush;
		vector<vector<int>> deck_table {distribute_deck(n_players, n_cards, deck)};
		print_cards(deck_table);

		cout << endl << "Do you want to quit?";
		quit = user_prompt();
		if (quit)
			cout << endl << "Application has been terminated!" << endl;
	} while (!quit);

	return 0;	
}

//Sub-routine to generate ordered deck of cards
vector<int> gen_deck(){
	vector<int> deck (N,0);
	for (size_t i {1}; i < N+1; i++)
		deck.at(i-1) = i;
	return deck;
}

//Sub-routine to shuffle deck of cards
void shuffle_deck(vector<int> &deck){
	//Using random_device number generator as seed
	random_device rd;
	//Using Mersenne Twister 19937 random number engine as random number generator for shuffle
	mt19937_64 g(rd());
	shuffle(deck.begin(),deck.end(),g);
}

//Sub-routine to distribute the deck to players 
vector<vector<int>> distribute_deck(const size_t &n_players, const size_t &n_cards, const vector<int> &deck){
	vector<vector<int>> deck_table;
	//Counter for allocating cards to players
	size_t count_max {};
	//Iniatilizing deck_table
	if (n_cards != 0){ //If number of cards per player is specified
		count_max = n_players * n_cards;
		vector<int> temp (n_cards,0);
       		for (size_t i {}; i < n_players; i++)
	 		deck_table.push_back(temp);
	}
	else {
		size_t min_cards {N / n_players};
		size_t balance_cards {N % n_players};
		count_max = N;
		vector<int> temp (min_cards,0);
		//Iniatialize minimum number of cards to each player
		for (size_t i {}; i < n_players; i++)
			deck_table.push_back(temp);
		//Initialize balance cards to some players
		for (size_t i {}; i < balance_cards; i++)
			deck_table.at(i).push_back(0);
	}

	//Assigning cards from deck to deck_table
	size_t count {};
	size_t i {};
	size_t j {};

	while (count < count_max){
		if (i == n_players){
			i = 0;
			j++;
		}
		deck_table.at(i).at(j) = deck.at(count);
		i++;
		count++;
	}
	return deck_table;
}

//Sub-routine to translate cardID to card rank and suit
void card_dictionary(const int &card_ID){
	string card_suit {};
	string card_rank {};
	size_t rank_index {};

	for (size_t i {}; i < suits_no; i++){
	       if (card_ID <= (i+1)*ranks_no && card_ID >= (i*ranks_no)+1){
		       card_suit = suits.at(i);
		       rank_index = card_ID - (i * ranks_no) - 1;
		       card_rank = ranks.at(rank_index);
	       }
	}

	cout << card_rank + " of " + card_suit;
}

//Sub-routine to print cards assigned to all players
void print_cards(const vector<vector<int>> &deck_table){
	string temp {"Player "};
	cout << endl;
	cout << left;
		for (size_t i {}; i < deck_table.size(); i++){
			cout << setw(13) << temp + to_string(i+1);
			for (auto num : deck_table.at(i)){
				cout << setw(20);
				card_dictionary(num);
			}
			cout << endl;
		}
}
		        
//Sub-routine to receive integer input from user
int input_int(){
	string temp {};
	const string minus {"-"};
	bool check {false};
	int value {};
	while (!check){
		cin >> temp;
		//check if input is negative integer
		if (!temp.substr(0,1).compare(minus)){
			for (size_t i {1}; i < temp.size(); i++){
				if (!isdigit(temp.at(i))){
					cout << endl << "Input is not an integer! Please enter again: ";
					break;
				}
				else if (i == temp.size() - 1){
					value = stoi(temp.substr(1,string::npos)) * -1;
					check = true;
				}
			}
		}
		//check if input is non negative integer
		else{
			for (size_t i {}; i < temp.size(); i++){
				if (!isdigit(temp.at(i))){
					cout << endl << "Input is not an integer! Please enter again: ";
					break;
				}
				else if (i == temp.size() - 1){
					value = stoi(temp);
					check = true;
				}
			}
		}
	}
	return value;
}

//Sub-routine to receive boolean input from user
bool user_prompt(){
	cout << " (Y/n): ";
	char temp {};
	cin >> temp;
	while (toupper(temp) != 'Y' && toupper(temp) != 'N'){
		cout << endl << "Invalid input! Please enter again. (Y/n): ";
		cin >> temp;
	}
	if (toupper(temp) == 'Y')
		return true;
	else
		return false;
}

//Sub-routine to print 1D vector
void print_vector(const vector<int> &vec){
	if (vec.size() != 0){
		cout << "[ "; 
			for (auto comp : vec)
				cout << comp << " ";
			cout << "]";
	}
	else
		cout << "[] - No entries are stored! Please add an entry." << endl;
}

//Sub-routine to print 2D vector
void print_vector_2d(const vector<vector<int>> &vec){
	for (size_t i {}; i < vec.size(); i++){
		for (auto card : vec.at(i))
			cout << card << " ";
		cout << endl;
	}
}

//Sub-routine to perform basic element wise vector operations
void vector_operate(vector<int> &vec, const int &value, const string &oper){
	if (oper == "+"){
		for (auto num : vec)
			num += value;
	}
	else if (oper == "-"){
		for (auto num : vec)
			num -= value;
	}
	else if (oper == "*"){
		for (auto num : vec)
			num *= value;
	}
}


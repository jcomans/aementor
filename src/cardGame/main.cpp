#include<iostream>
#include<iomanip> // setw
#include<vector> // at, begin, end
#include<string> // to_string
#include<algorithm> // shuffle, all_off, replace, sort, unique, count, max_element, equal
#include<random> // random_device, mt19937_64
#include<cmath> // abs 
#include<numeric> // adjacent_difference
#include<iterator> // back_inserter, distance

using namespace std;

//Constant quantities in this program
const size_t N {52};
const vector<string> suits {"Clubs", "Diamonds", "Hearts", "Spades"};
const vector<string> ranks {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
const size_t suits_no {suits.size()};
const size_t ranks_no {ranks.size()};
const size_t hex_base {16};
const vector<string> hands {"High card", "Pair", "Two pair", "Three of a kind", "Straight", "Flush", "Full house", "Four of a kind", "Straight flush", "Royal flush"};  
const vector<size_t> hex_key {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

//Mandatory sub-routines for this program
vector<int> gen_deck();
void shuffle_deck(vector<int> &deck);
vector<vector<int>> distribute_deck(const size_t &n_players, const size_t &n_cards, const vector<int> &deck);
void card_index(const int &card_ID, size_t &suit_index, size_t &rank_index);
void card_dictionary(const size_t suit_index, const size_t rank_index);
void print_cards (const vector<vector<int>> &deck_table);
void hand_indices(const vector<int> &hand, vector<size_t> &hand_suits, vector<size_t> &hand_ranks);
bool is_flush(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params);
bool is_straight(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params);
bool is_straightFlush(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params);
size_t hand_strength(const vector<size_t> &hand_params);
void highest_hand(const vector<int> &hand);
int pow_int(int a, int b);
size_t pow_int(size_t a, size_t b);
int input_int();
bool user_prompt();

//Optional sub-routines for debugging
void print_vector(const vector<int> &vec);
void print_vector(const vector<size_t> &vec);
void print_vector_2d(const vector<vector<int>> &vec);
void vector_operate(vector<int> &vec, const int &value, const string &oper); 

int main(){
	bool quit {false};
	
	do {
		
		vector<int> deck {gen_deck()};
		shuffle_deck(deck);

		cout << endl << "Enter the number of players: ";
		size_t n_players {abs(input_int())};
		size_t max_cards {N/n_players};
		
		size_t n_cards {};
		bool check {false};
		do {
			cout << endl << "Enter the number of cards per player (0 for all cards to be distributed. Max allowed cards is " << dec << max_cards << "): ";
			n_cards = abs(input_int());
			if (n_cards <= max_cards)
				check = true;
			else
				cout << endl << "Invalid value! Please enter a value below " << max_cards + 1 << "." << endl;
		} while (!check);
	
		cout << endl << "Assigning cards to players..." << endl << flush;
		vector<vector<int>> deck_table {distribute_deck(n_players, n_cards, deck)};
		print_cards(deck_table);

		//Testing highest hand with player 1. For testing purposes, Specify 5 for number of cards per player 
		//highest_hand(deck_table.at(0));

		//Manual testing of highest hand
		highest_hand({13,1,12,10,11});

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

//Sub-routine to obtain suit_index and rank_index of card_ID in suits and cards vectors respectively
void card_index(const int &card_ID, size_t &suit_index, size_t &rank_index){

	for (size_t i {}; i < suits_no; i++){
	       if (card_ID <= (i+1)*ranks_no && card_ID >= (i*ranks_no)+1){
		       suit_index = i;
		       rank_index = card_ID - (i * ranks_no) - 1;
	       }
	}
}

//Sub-routine to translate suit_index and rank_index into their string equivalents
void card_dictionary(const size_t suit_index, const size_t rank_index){
	string card_suit {};
	string card_rank {};
	card_suit = suits.at(suit_index);
	card_rank = ranks.at(rank_index);

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
				size_t suit_index {}, rank_index {};
				card_index(num,suit_index,rank_index);
				card_dictionary(suit_index,rank_index);
			}
			cout << endl;
		}
}

//Sub-routine to obtain hand indicees
void hand_indices(const vector<int> &hand, vector<size_t> &hand_suits, vector<size_t> &hand_ranks){

	for (auto num : hand){
		size_t suit_index {}, rank_index {};
		card_index(num,suit_index,rank_index);
		hand_suits.push_back(suit_index);
		hand_ranks.push_back(rank_index);
	}
}

bool is_flush(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params){
	//Check if all the elements of hand_suits are same. The third parameter is a lambda function to check if all elements are same 
	if (all_of(hand_suits.begin(), hand_suits.end(),[&] (size_t i) {return i == hand_suits.at(0);})){
		//Store the hand rating. For flush it is 5
		hand_params.push_back(5);
		//Local copy of hand_ranks
		auto temp {hand_ranks};
		//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
		replace(temp.begin(),temp.end(),0,13);
		//Sort the ranks in decending order. Noticed that greater<>() works without including functional library in preprocessor. Why?
		sort(temp.begin(),temp.end(),greater<>());
	        //Store the ranks of flush
		for (auto num : temp)
			hand_params.push_back(num);
		return true;
	}
	else
		return false;
}

bool is_straight(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params){
	//Local copy of hand_ranks
	auto temp {hand_ranks};
	//Model 10 to ace stragiht
	vector <size_t> best_straight {0,9,10,11,12};
	//Sort the ranks in ascending order
	sort(temp.begin(),temp.end());
	vector<size_t> delta_temp;
	//First element of adjacent_difference is simply the first element of container
	adjacent_difference(temp.begin(),temp.end(),back_inserter(delta_temp));
	//Check if all the cards are separated by single rank
	if (all_of(delta_temp.begin()+1, delta_temp.end(),[&] (size_t i) {return i == 1;})){
		//Store the hand rating. For straight it is 4
		hand_params.push_back(4);
		//Sort the ranks in decending order
		sort(temp.begin(),temp.end(),greater<>());
		//Store the ranks of straight
		for (auto num : temp)
			hand_params.push_back(num);
		return true;
	}
	//Check the hand for 10 to Ace straight
	else if (equal(temp.begin(),temp.end(),best_straight.begin())){		
		//Store the hand rating. For straight it is 4
		hand_params.push_back(4);
		//Replace 0 in hand_ranks with 13 to denote ace rank as highest value
		replace(temp.begin(),temp.end(),0,13);
		//Sort the ranks in decending order
		sort(temp.begin(),temp.end(),greater<>());
			//Store the ranks of flush
		for (auto num : temp)
			hand_params.push_back(num);
		return true;
	}
	else
		return false;
}

bool is_straightFlush(const vector<size_t> &hand_suits, const vector<size_t> &hand_ranks, vector<size_t> &hand_params){
	auto flush_params {hand_params};
	auto straight_params {hand_params};
	//Check if hand is both flush and straight
	if (is_flush(hand_suits,hand_ranks,flush_params) && is_straight(hand_suits,hand_ranks,straight_params)){
		//Check if hand is royal flush
		if (straight_params.at(1) == 13){
			hand_params = straight_params;
			//Replace the hand rating of straight with Royal flush. Royal flush rating is 9
			hand_params.at(0) = 9;
		}
		else{
			hand_params = straight_params;
			//Replace the hand rating of straight with Straight flush. Straight flush rating is 8
			hand_params.at(0) = 8;
		}
		return true;
	}
	else
		return false;
}

//Sub-routine to obtain unique ranks of a hand
void count_unique_ranks(size_t &unique_ranks_size, size_t &max_freq, const vector<size_t> &hand_ranks, vector<size_t> &freq_indices){
	auto temp {hand_ranks};
	vector<size_t>::iterator temp_it;
	//Sorting temp will ensure all duplicate entries are removed by unique function
	sort(temp.begin(),temp.end());
	//Unique returns iterator to last unique element 
	temp_it = unique(temp.begin(),temp.end());
	//Range between temp.begin() and temp_it contains only the unique elements of temp
	//Distance gives the number of elements between iterators
	unique_ranks_size = distance(temp.begin(),temp_it);
	//Vector for storing frequency of ranks
	vector <size_t> freq_ranks;
	for (auto num : hand_ranks)
		freq_ranks.push_back(count(hand_ranks.begin(),hand_ranks.end(),num));
	//max_element returns the iterator with maximum value in a container. max_element needs to be dereferenced
		max_freq = *max_element(freq_ranks.begin(),freq_ranks.end());
	//Sort the indices of freq_ranks in descending order. Implemented a nice method by Lucasz Wiklendt from stackoverflow
		stable_sort(freq_indices.begin(), freq_indices.end(), [&freq_ranks](size_t it1, size_t it2) {return freq_ranks.at(it1) > freq_ranks.at(it2);});
}

//Sub-routine for hand with two unqiue cards(four of a kind and full house)
bool is_2unique(const size_t unique_ranks_size, const size_t max_freq, const vector<size_t> &hand_ranks, const vector<size_t> &freq_indices, vector<size_t> &hand_params){
	if (unique_ranks_size == 2){
		auto temp {hand_ranks};
		//For four of a kind max_freq is 4. 
		if (max_freq == 4){			
			//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
			replace(temp.begin(),temp.end(),0,13);
			//Store the hand rating. For four of a kind it is 7
			hand_params.push_back(7);
			//Store the ranks in descending order of frequency using freq_indices
			for (auto i : freq_indices)
				hand_params.push_back(temp.at(i));
		}
		//For full house max_freq is 3
		else if (max_freq == 3){			
			//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
			replace(temp.begin(),temp.end(),0,13);
			//Store the hand rating. For full house it is 6
			hand_params.push_back(6);
			//Store the ranks in descending order of frequency using freq_indices
			for (auto i : freq_indices)
				hand_params.push_back(temp.at(i));	
		}
		return true; 
	}
	else
		return false;
}

//Sub-routine for hand with three unique cards(three of a kind and two pair)
bool is_3unique(const size_t unique_ranks_size, const size_t max_freq, const vector<size_t> &hand_ranks, const vector<size_t> &freq_indices, vector<size_t> &hand_params){
	if (unique_ranks_size == 3){
		auto temp {hand_ranks};
		//For three of a kind max_freq is 3. 
		if (max_freq == 3){			
			//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
			replace(temp.begin(),temp.end(),0,13);
			//Store the hand rating. For three of a kind it is 3
			hand_params.push_back(3);
			//Store the ranks in descending order of frequency using freq_indices
			for (auto i : freq_indices)
				hand_params.push_back(temp.at(i));
			//For three of a kind last 2 ranks in hand needs to be further sorted in descending order
			sort(hand_params.begin()+4,hand_params.end(),greater<>());
		}
		//For two pair max_freq is 2
		else if (max_freq == 2){			
			//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
			replace(temp.begin(),temp.end(),0,13);
			//Store the hand rating. For two pair it is 2
			hand_params.push_back(2);
			//Store the ranks in descending order of frequency using freq_indices
			for (auto i : freq_indices)
				hand_params.push_back(temp.at(i));
			//For two pair first 4 ranks need to be further sorted in descending order
			sort(hand_params.begin()+1,hand_params.begin()+5,greater<>());	
		}
		return true; 
	}
	else
		return false;
}

//Sub-routine for hand with four unique cards(one pair)
bool is_4unique(const size_t unique_ranks_size, const size_t max_freq, const vector<size_t> &hand_ranks, const vector<size_t> &freq_indices, vector<size_t> &hand_params){
	if (unique_ranks_size == 4){
		auto temp {hand_ranks};
		//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
		replace(temp.begin(),temp.end(),0,13);
		//Store the hand rating. For one pair it is 1
		hand_params.push_back(1);
		//Store the ranks in descending order of frequency using freq_indices
		for (auto i : freq_indices)
			hand_params.push_back(temp.at(i));
		//For one pair last 3 ranks need to be further sorted in descending order
		sort(hand_params.begin()+3,hand_params.end(),greater<>()); 
		return true;
	}
	else
		return false;
}

//Sub-routine for hand with a high card
void high_card(const size_t unique_ranks_size, const size_t max_freq, const vector<size_t> &hand_ranks, const vector<size_t> &freq_indices, vector<size_t> &hand_params){
	auto temp {hand_ranks};
	//Replace 0 in hand_ranks if available with 13 to denote ace rank as highest value
	replace(temp.begin(),temp.end(),0,13);
	//Store the hand rating. For high card it is 0
	hand_params.push_back(0);
	//Sort the hand in decending order
	sort(temp.begin(),temp.end(),greater<>());
	//Store the ranks
	for (auto num : temp)
		hand_params.push_back(num);
}	

//Sub-routine to generate a hex value for strength of hand
size_t hand_strength(const vector<size_t> &hand_params){
	size_t hand_score {};
	size_t count {hand_params.size() - 1};
	for (auto num : hand_params){
		hand_score += hex_key.at(num) * pow_int(hex_base,count);
		count -= 1;
	}
	return hand_score;
}

//Sub-routine to find highest possible hand combination
void highest_hand(const vector<int> &hand){
	vector<size_t> hand_suits {};
	vector<size_t> hand_ranks {};
	/*hand_params vector is used to compute the hand score.
	The first element is the hand rating ranging between 0 and 9.
	The remaining elements are hand_ranks sorted in the appropropriate
	descending order for a hand in question. More information about this
	sorting can be seen in the sub-routines of the respective hands.
	hand_strength function will generate a hexadecimal number for hand_score
	using the below vector*/ 
	vector<size_t> hand_params {};
	size_t temp {};
	/*Split the hand vector into two vectors containing the suit and rank indices
	 in global suits and ranks vector*/ 
	hand_indices(hand,hand_suits,hand_ranks);
	//Unsorted indices of frequency_ranks vector. count_unique_ranks function argument
	vector<size_t> freq_indices {0,1,2,3,4};
	//number of unique ranks in hand. count_unique_ranks function argument
	size_t unique_ranks_size {};
	//Highest frequency of repeating cards in a hand. count_unique_ranks function argument
	size_t max_freq {};
	/*Updates the above three listed arguments. These arguments are required
	for non flush/straight hand subroutines*/
	count_unique_ranks(unique_ranks_size, max_freq, hand_ranks, freq_indices);
	
	if (is_straightFlush(hand_suits,hand_ranks,hand_params))
		 temp = hand_params.at(0);
	else if (is_2unique(unique_ranks_size, max_freq, hand_ranks, freq_indices, hand_params))
		temp = hand_params.at(0);
	else if (is_flush(hand_suits,hand_ranks,hand_params))
		 temp = hand_params.at(0);
	else if (is_straight(hand_suits,hand_ranks,hand_params))
		 temp = hand_params.at(0);
	else if (is_3unique(unique_ranks_size, max_freq, hand_ranks, freq_indices, hand_params))
		 temp = hand_params.at(0);
	else if (is_4unique(unique_ranks_size, max_freq, hand_ranks, freq_indices, hand_params))
		 temp = hand_params.at(0);
	else {
		high_card(unique_ranks_size, max_freq, hand_ranks, freq_indices, hand_params);
		temp = hand_params.at(0);
	}

	cout << endl << "Highest Hand: " << hands.at(temp);
	cout << endl << "Hand Score: " << hex << hand_strength(hand_params);
}

//Sub-routine for computing integer power
size_t pow_int(size_t a, size_t b){
	size_t value {1};
	for (size_t i {b}; i > 0; i--)
		value *= a;
	return value;
}

int pow_int(int a, int b){
	int value {1};
	for (int i {b}; i > 0; i--)
		value *= a;
	return value;
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

void print_vector(const vector<size_t> &vec){
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


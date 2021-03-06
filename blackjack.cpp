#include <iostream>
#include <string>
#include <array>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>
using namespace std;

//Used as array size throughout
const int deck_size = 52;

//Scores tracked throughout game
int player_score = 0; 
int dealer_score = 0;

enum CardRank
{
	R_2,
	R_3,
	R_4,
	R_5,
	R_6,
	R_7,
	R_8,
	R_9,
	R_10,
	R_JACK,
	R_QUEEN,
	R_KING,
	R_ACE,
	MAX_RANK
};

enum CardSuit
{
	CLUBS,
	DIAMONDS,
	HEARTS,
	SPADES,
	MAX_SUIT
};

struct Card
{
	CardRank rank;
	CardSuit suit;
};

//Declarations
void deal(int &p, int &d, Card *&cardPtr);
void playerTurn(int &p, Card *&cardPt);
void dealerTurn(int &p, int &d, Card *&cardPtr);
void winner(const int p, const int d);

//Populate array with a Card of each Rank & Suit
void populateDeck(array<Card, deck_size> &deck)
{
	int place = 0;
	for (int suit = 0; suit < MAX_SUIT; ++suit)
	{
		for (int rank = 0; rank < MAX_RANK; ++rank)
		{
			deck[place].suit = static_cast<CardSuit>(suit);
			deck[place].rank = static_cast<CardRank>(rank);
			++place;
		}
	}
}

//Print "RANK of SUIT" for a given Card
void printCard(const Card &card)
{
	switch (card.rank)
	{
	case R_2:
		cout << "Two";
		break;
	case R_3:
		cout << "Three";
		break;
	case R_4:
		cout << "Four";
		break;
	case R_5:
		cout << "Five";
		break;
	case R_6:
		cout << "Six";
		break;
	case R_7:
		cout << "Seven";
		break;
	case R_8:
		cout << "Eight";
		break;
	case R_9:
		cout << "Nine";
		break;
	case R_10:
		cout << "Ten";
		break;
	case R_JACK:
		cout << "Jack";
		break;
	case R_QUEEN:
		cout << "Queen";
		break;
	case R_KING:
		cout << "King";
		break;
	case R_ACE:
		cout << "Ace";
		break;
	default:
		cout << "Oops";
		break;
	}

	cout << " of ";

	switch (card.suit)
	{
	case HEARTS:
		cout << "Hearts";
		break;
	case SPADES:
		cout << "Spades";
		break;
	case DIAMONDS:
		cout << "Diamonds";
		break;
	case CLUBS:
		cout << "Clubs";
		break;
	default:
		cout << "error";
		break;
	}
}

//Swap two cards, used for shuffling
void swapCards(Card &a, Card &b)
{
	Card temp = a;
	a = b;
	b = temp;
}

//Generate an int value based on card Rank
int getCardValue(const Card &c)
{
	switch (c.rank)
	{
	case R_2:
		return 2;
		break;
	case R_3:
		return 3;
		break;
	case R_4:
		return 4;
		break;
	case R_5:
		return 5;
		break;
	case R_6:
		return 6;
		break;
	case R_7:
		return 7;
		break;
	case R_8:
		return 8;
		break;
	case R_9:
		return 9;
		break;
	case R_10: case R_JACK: case R_QUEEN: case R_KING:
		return 10;
		break;
	case R_ACE:
		return 11;
		break;
	default:
		break;
	}
}

//From LearnCPP.com lesson
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  // static used for efficiency, so we only calculate this value once
																				 // evenly distribute the random number across our range
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

//Initilize Random numbers for Windows
void initilizeRand()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();
}

void shuffleDeck(array<Card, deck_size> &deck)
{
	for (int index = 0; index < 52; ++index)
	{
		int swapIndex = getRandomNumber(0, 51);
		swapCards(deck[index], deck[swapIndex]);
	}
}

//Player promted to hit or stand
bool hitStand()
{
	cout << "> ";
	char choice;
	cin >> choice;
	if (choice == 'h' || choice == 'H') return true;
	else if (choice == 's' || choice == 'S') return false;

}

//A full round of Black Jack the goes until there is a winner
void playBlackJack(array<Card, deck_size> deck)
{
	Card *cardPtr = &deck[0]; //Point to first card in deck

	//Keeps track of numeric Rank totals for one round
	int dealer_total = 0;
	int player_total = 0;

	deal(player_total, dealer_total, cardPtr);

	//End game if 2 Aces = bust on deal
	if (player_total > 21 || dealer_total > 21) return;
	
	playerTurn(player_total, cardPtr);

	//New game if player busts
	if (player_total > 21) return;

	dealerTurn(player_total, dealer_total, cardPtr);

	//New game if dealer busts
	if (dealer_total > 21) return;

	//If neither busts, determine winner
	cout << "Dealer stays." << endl;
	winner(player_total, dealer_total);
}

//Gives the dealer one "face up" card and the player two
void deal(int &p, int &d, Card *&cardPtr)
{
	cout << "\nDealer is showing: ";
	printCard(*cardPtr);
	d += getCardValue(*cardPtr++);
	cout << '\n';

	cout << "You have: ";
	printCard(*cardPtr);
	p += getCardValue(*cardPtr++);
	cout << " & ";
	printCard(*cardPtr);
	p += getCardValue(*cardPtr++);
	cout << '\n';
}

void playerTurn(int &p, Card *&cardPtr)
{
	while (hitStand())
	{
		printCard(*cardPtr);
		p += getCardValue(*cardPtr++);
		cout << '\n';
		if (p > 21)
		{
			this_thread::sleep_for(1s);
			cout << "Bust!" << endl;
			++dealer_score;
			break;
		}
	}
}

void dealerTurn(int &p, int &d, Card *&cardPtr)
{
	cout << "Dealer flips a ";
	printCard(*cardPtr);
	d += getCardValue(*cardPtr++);
	cout << '\n';

	this_thread::sleep_for(1s);

	while (d < p && d < 17)
	{
		cout << "Dealer hits." << endl;
		this_thread::sleep_for(1s);
		printCard(*cardPtr);
		d += getCardValue(*cardPtr++);
		cout << '\n';
		this_thread::sleep_for(1s);
		if (d > 21)
		{
			cout << "Dealer busts! You win." << endl;
			++player_score;
			break;
		}
	}
}

void winner(const int p, const int d)
{
	if (p > d)
	{
		this_thread::sleep_for(1s);
		cout << "You win!" << endl;
		++player_score;
	}
	else
	{
		this_thread::sleep_for(1s);
		cout << "Dealer wins." << endl;
		++dealer_score;
	}
}


int main()
{
	initilizeRand();

	//Create array for deck of cards
	array<Card, deck_size> game_deck{};
	populateDeck(game_deck);

	cout << "Black Jack\nPress 'h' to hit and 's' to stand\n"
		 << "Ace is 11\nDeck is shuffled each round\n" << endl;

	//An infinite loop of rounds
	while (true)
	{
		shuffleDeck(game_deck);
		this_thread::sleep_for(2s);
		playBlackJack(game_deck);
		cout << "You: " << player_score << "\tDealer: " << dealer_score << endl;
	}

}
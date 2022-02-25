#include <iostream>
#include <time.h>
#include <windows.h>
#define DECK_SIZE 52
#define HALF_DECK_SIZE 26
#define COLORS_AMOUNT 4
#define SYMBOLS_AMOUNT 13
#define PLAYERS_AMOUNT 2

using namespace std;

class Card			//klasa obsługująca kartę
{
private:
	int value;
	const char* symbol = new char[5];
	const char* color = new char[5];
public:

	Card* next = NULL;
	Card* under = NULL;

	Card() { this->value = 0; this->symbol = "0"; this->color = "0"; }
	Card(int v, const char* s, const char* c) { this->value = v; this->symbol = s; this->color = c; }

	int getValue()
	{
		return value;
	}
	const char* getSymbol()
	{
		return symbol;
	}
	const char* getColor()
	{
		return color;
	}
	void show() {
		cout << getSymbol() << " " << getColor() << endl;
	}
}; 

class Queue			//implementacja struktury danych(kolejki)
{
public:
	Card* first = NULL;
	Card* last = NULL;

	void enqueue(Card* card)
	{
		Card* n = new Card(card->getValue(), card->getSymbol(), card->getColor());

		if (this->first == NULL)
		{
			this->first = this->last = n;

		}
		else
		{
			this->last->next = n;
			this->last = n;
		}
	}

	void enqueue(int value, const char* symbol, const char* color)
	{
		Card* n = new Card(value, symbol, color);
		enqueue(n);
	}

	void dequeue()
	{
		if (this->first != NULL)
		{
			Card* temp = this->first;
			this->first = this->first->next;
			delete temp;
		}
	}

	void show()
	{
		Card* temp = this->first;
		while (temp != NULL)
		{
			temp->show();
			temp = temp->next;
		}
	}

	int size()
	{
		int i = 0;
		Card* temp = this->first;
		while (temp != NULL)
		{
			temp = temp->next;
			i++;
		}
		return i;
	}

	bool empty()
	{
		if (size() == 0)
			return true;
		else
			return false;
	}

	bool full()
	{
		if (size() == DECK_SIZE)
			return true;
		else
			return false;
	}
	Card* getQueueAsArray(int size) //przerobienie kolejki na tablicę
	{
		Card* temp = this->first;

		for (int i = 0; i < size; i++)
		{
			temp = temp->next;
		}
		Card* tab = temp;
		return tab;
	}

};

class Stack //implementacja stosu (zastosowanie podczas potyczek kart)
{

public:
	Card* top = NULL;

	void push(Card* card)
	{
		Card* n = new Card(card->getValue(), card->getSymbol(), card->getColor());

		if (this->top != NULL)
		{
			n->under = this->top;
			this->top = n;
		}
		else
		{
			this->top = n;

		}

	}

	void push(int value, const char* symbol, const char* color)
	{
		Card* n = new Card(value, symbol, color);
		push(n);
	}

	void pop()
	{
		if (this->top != NULL)
		{
			Card* temp = this->top;
			this->top = this->top->under;
			delete temp;
		}
	}

	void showStack()
	{
		Card* temp = this->top;
		while (temp != NULL)
		{
			temp->show();
			temp = temp->under;
		}
	}

	bool empty()
	{
		if (this->top == NULL)
			return true;
		else
			return false;
	}

};

class Deck			//klasa talii (oparta na kolejce)
{
private:
	Queue queue;
	const char* symbols[SYMBOLS_AMOUNT] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jopek", "Dama", "Krol", "As" };
	const char* colors[COLORS_AMOUNT] = { "PIK", "TREFL", "KARO", "KIER" };
public:

	Deck(int deckVariant = 0)			//stworzenie talii
	{
		for (int i = 0; i < COLORS_AMOUNT; i++)
		{
			for (int j = deckVariant; j < SYMBOLS_AMOUNT; j++) //2-10
			{
				Card tmp(j + 2, symbols[j], colors[i]);
				queue.enqueue(j + 2, symbols[j], colors[i]);
			}
		}
	}
	void shuffle(int deckVariant = 0)			//tasowanie talii
	{
		Card* tab[DECK_SIZE];
		for (int i = 0; i < (DECK_SIZE - deckVariant * 4); i++)
		{
			tab[i] = queue.getQueueAsArray(i);
		}

		Queue temp_queue;

		for (int i = DECK_SIZE - 1 - (deckVariant * 4); i >= 0; i--)
		{
			int j = 0;
			if (i > 0)
			{
				j = rand() % (i + 1);
				temp_queue.enqueue(tab[j]);
				tab[j] = tab[i];
			}
			else
				temp_queue.enqueue(tab[j]);

			queue = temp_queue;
		}
	}

	void showDeck()
	{
		queue.show();
	}

	int firstCardValue()
	{
		return queue.first->getValue();
	}
	Card* firstCard()
	{
		return queue.first;
	}

	void popCard()
	{
		queue.dequeue();
	}

	void pushbackCard(Card* temp)
	{
		Card* n = new Card();

		queue.enqueue(temp->getValue(), temp->getSymbol(), temp->getColor());
	}

	int size()
	{
		return queue.size();
	}

	bool empty()
	{
		if (queue.empty())
			return true;
		else
			return false;
	}
	void clearDeck()
	{
		while (!queue.empty())
		{
			queue.dequeue();
		}
	}
	void dealCards(int num_of_players, Deck playerDeck[], int deckVariant = 0)			//rozdanie kart graczom
	{
		for (int j = 0; j < num_of_players; j++)
		{
			for (int i = 0; i < ((DECK_SIZE - (deckVariant * 4)) / num_of_players); i++)
			{
				playerDeck[j].pushbackCard(queue.first);
				queue.dequeue();
			}
		}

	}
	int checkRank()			//funkcja rankingowa dla talii
	{
		Card* temp = queue.first;
		int rank = 0;
		while (temp != NULL)
		{
			if (temp->getValue() == 14)
				rank += 20;
			else if (temp->getValue() == 13)
				rank += 7;
			else if (temp->getValue() == 12)
				rank += 2;
			else if (temp->getValue() == 11)
				rank += 1;

			temp = temp->next;
		}
		return rank;
	}

	int checkRank_Final()
	{
		Card* temp = queue.first;
		int rank = 0;
		while (temp != NULL)
		{
			if (temp->getValue() == 14)
				rank += 20;
			else if (temp->getValue() == 13)
				rank += 4;
			else if (temp->getValue() == 12)
				rank += 1;

			temp = temp->next;
		}
		return rank;
	}
};

class Game			//główna klasa obsługująca rozgrywkę
{
protected:
	Deck players[PLAYERS_AMOUNT];
	Stack stos[PLAYERS_AMOUNT];
	int tourCounter = 1;
	int player1 = 0;
	int player2 = 0;
public:
	Game(int deckVariant = 0, int p1 = 0, int p2 = 0)			//przygotowanie do gry
	{
		Deck deck(deckVariant);
		deck.shuffle(deckVariant);

		for (int i = 0; i < PLAYERS_AMOUNT; i++)
		{
			players[i].clearDeck();
		}
		deck.dealCards(PLAYERS_AMOUNT, players, deckVariant);
		/*for (int i = 0; i < PLAYERS_AMOUNT; i++)
		{
			players[i].showDeck();
			cout << endl << endl;
		}*/
	}

	virtual void play()			//rozpoczęcie rozgrywki
	{

		for (int i = 0; i < PLAYERS_AMOUNT; i++)
		{
			cout << "Player " << i + 1 << ": " << endl << endl;
			players[i].showDeck();
			cout << endl << endl;
		}

		cout << "Player 1 rank: " << players[0].checkRank() << endl;
		cout << "Player 2 rank: " << players[1].checkRank() << endl << endl;

		while (!(players[0].empty() || players[1].empty()))
		{

			Card* card1 = players[0].firstCard();
			Card* card2 = players[1].firstCard();
			cout << "Tura " << (tourCounter) << ": " << endl << endl;

			stos[0].push(card1);
			stos[1].push(card2);
			players[0].popCard();
			players[1].popCard();
			tourCounter++;
			compare();

			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
			players[0].showDeck();
			cout << endl << endl;
			players[1].showDeck();
			cout << endl << endl;
		}

		if (players[0].empty() && players[1].size() > 0)
		{
			cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
		}
		else if (players[1].empty() && players[0].size() > 0)
		{
			cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
		}

	}

	virtual void compare()			//porównanie dwóch kart na stole
	{
		if (stos[0].top->getValue() > stos[1].top->getValue())
		{
			stos[0].top->show();
			cout << " vs " << endl;
			stos[1].top->show();
			cout << endl;

			while (!stos[0].empty())
			{
				players[0].pushbackCard(stos[0].top);
				stos[0].pop();
			}
			while (!stos[1].empty())
			{
				players[0].pushbackCard(stos[1].top);
				stos[1].pop();
			}
			cout << "Player 1 WIN BATTLE" << endl << endl;
		}
		else
			if (stos[0].top->getValue() < stos[1].top->getValue())
			{
				stos[0].top->show();
				cout << " vs " << endl;
				stos[1].top->show();
				cout << endl;

				while (!stos[1].empty())
				{
					players[1].pushbackCard(stos[1].top);
					stos[1].pop();
				}
				while (!stos[0].empty())
				{
					players[1].pushbackCard(stos[0].top);
					stos[0].pop();
				}
				cout << "Player 2 WIN BATTLE" << endl << endl;

			}
			else
			{
				cout << "player1 size: " << players[0].size() << endl;
				cout << "player2 size: " << players[1].size() << endl << endl;
				stos[0].top->show();
				cout << " vs " << endl;
				stos[1].top->show();
				cout << endl;

				battle();
			}
	}


	virtual void battle()			//funkcja obsługująca wojnę
	{
		if (!(players[0].empty() || players[1].empty()))
		{
			cout << "BATTLE!" << endl << endl;

			Card* card3 = players[0].firstCard();
			Card* card4 = players[1].firstCard();

			cout << "X X" << endl << endl;

			stos[0].push(card3);
			stos[1].push(card4);
			tourCounter++;

			players[0].popCard();
			players[1].popCard();

			if (!(players[0].empty() || players[1].empty()))
			{
				Card* card5 = players[0].firstCard();
				Card* card6 = players[1].firstCard();

				stos[0].push(card5);
				stos[1].push(card6);
				tourCounter++;

				players[0].popCard();
				players[1].popCard();
				compare();
			}
			else if ((players[0].empty() && players[1].empty()))
			{
				cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
				cout << "player1 size: " << players[0].size() << endl;
				cout << "player2 size: " << players[1].size() << endl << endl;
				return;
			}

		}
		else if ((players[0].empty() && players[1].empty()))
		{
			cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
			return;
		}


	}
};

class Game_A : public Game		//wariant A wojny
{

public:
	Game_A(int deckVariant = 0) :Game(deckVariant)
	{

	}

};

class Game_B : public Game		//wariant B
{
public:
	Game_B(int deckVariant = 0) :Game(deckVariant)
	{

	}

	void battle()			//wojna w wariancie B
	{
		if (players[0].size() >= 2 && players[1].size() >= 2)
			Game::battle();
		else
		{
			cout << "BATTLE!" << endl << endl;

			if ((players[0].size()) == 1 || (players[1].size() == 1))
			{
				Card* card3 = players[0].firstCard();
				stos[0].push(card3);
				players[0].popCard();

				Card* card4 = players[1].firstCard();
				stos[1].push(card4);
				players[1].popCard();
				tourCounter++;
				cout << "X X" << endl << endl;
				cout << "player1 size: " << players[0].size() << endl;
				cout << "player2 size: " << players[1].size() << endl << endl;

				if (players[0].empty() && players[1].empty())
				{
					cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
					cout << "player1 size: " << players[0].size() << endl;
					cout << "player2 size: " << players[1].size() << endl << endl;
					return;
				}

				if (players[0].size() == 0)
				{
					Card* card5 = players[1].firstCard();
					stos[0].push(card5);
					players[1].popCard();

					if (!players[1].empty())
					{
						Card* card6 = players[1].firstCard();
						stos[1].push(card6);
						players[1].popCard();
						tourCounter++;
					}
					else
					{
						cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1;
						cout << "player1 size: " << players[0].size() << endl;
						cout << "player2 size: " << players[1].size() << endl << endl;
						return;
					}
				}
				else if (players[1].size() == 0)
				{
					Card* card6 = players[0].firstCard();
					stos[1].push(card6);
					players[0].popCard();

					if (!(players[0].empty()))
					{
						Card* card5 = players[0].firstCard();
						stos[0].push(card5);
						players[0].popCard();
						tourCounter++;
					}
					else
					{
						cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1;
						cout << "player1 size: " << players[0].size() << endl;
						cout << "player2 size: " << players[1].size() << endl << endl;
						return;
					}
				}
				compare();
			}
			else if (players[0].size() == 0 || players[1].size() == 0)
			{
				if (players[0].empty() && players[1].empty())
				{
					cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
					cout << "player1 size: " << players[0].size() << endl;
					cout << "player2 size: " << players[1].size() << endl << endl;
					return;
				}


				if (players[0].size() == 0)
				{
					Card* card3 = players[1].firstCard();
					stos[0].push(card3);
					players[1].popCard();

					if (!(players[1].empty()))
					{
						Card* card5 = players[1].firstCard();
						stos[0].push(card5);
						players[1].popCard();

						if (!(players[1].empty()))
						{
							Card* card4 = players[1].firstCard();
							stos[1].push(card4);
							players[1].popCard();

							if (!(players[1].empty()))
							{
								Card* card6 = players[1].firstCard();
								stos[1].push(card6);
								players[1].popCard();
								tourCounter++;
								cout << "X X" << endl << endl;
								cout << "player1 size: " << players[0].size() << endl;
								cout << "player2 size: " << players[1].size() << endl << endl;
							}
							else
							{
								cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
								cout << "player1 size: " << players[0].size() << endl;
								cout << "player2 size: " << players[1].size() << endl << endl;
								return;
							}

						}
						else
						{
							cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
							cout << "player1 size: " << players[0].size() << endl;
							cout << "player2 size: " << players[1].size() << endl << endl;
							return;
						}

					}
					else
					{
						cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
						cout << "player1 size: " << players[0].size() << endl;
						cout << "player2 size: " << players[1].size() << endl << endl;
						return;
					}
				}
				else if (players[1].size() == 0)
				{
					Card* card4 = players[0].firstCard();
					stos[1].push(card4);
					players[0].popCard();

					if (!(players[0].empty()))
					{
						Card* card6 = players[0].firstCard();
						stos[1].push(card6);
						players[0].popCard();

						if (!(players[0].empty()))
						{
							Card* card3 = players[0].firstCard();
							stos[0].push(card3);
							players[0].popCard();

							if (!(players[0].empty()))
							{
								Card* card5 = players[0].firstCard();
								stos[0].push(card5);
								players[0].popCard();
								tourCounter++;
								cout << "X X" << endl << endl;
								cout << "player1 size: " << players[0].size() << endl;
								cout << "player2 size: " << players[1].size() << endl << endl;
							}
							else
							{
								cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
								cout << "player1 size: " << players[0].size() << endl;
								cout << "player2 size: " << players[1].size() << endl << endl;
								return;
							}

						}
						else
						{
							cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
							cout << "player1 size: " << players[0].size() << endl;
							cout << "player2 size: " << players[1].size() << endl << endl;
							return;
						}

					}
					else
					{
						cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
						cout << "player1 size: " << players[0].size() << endl;
						cout << "player2 size: " << players[1].size() << endl << endl;
						return;
					}
				}
				compare();
			}
		}

	}
};

class Game_SMART : public Game			//klasa obsługująca mądrą wojnę
{
private:
	int choice = 0;
	int battleChecker;
	int random1 = 0;
	int random2 = 0;		//zmienne pomocnicze do losowego powrotu kart do talii
	Card remainedCard_v;		//obiekt przechowujący kartę, która pozostała w ręce gracza wybierającego kartę
	Card* remainedCard = NULL;	
public:
	Game_SMART(int deckVariant, int p1, int p2) :Game(deckVariant)
	{
		player1 = p1;
		player2 = p2;
	}

	void play()
	{
		players[0].showDeck();
		cout << endl << endl;
		players[1].showDeck();
		cout << endl << endl; 

		for (int i = 0; !(players[0].empty() || players[1].empty()); i++)
		{
			cout << "Tura " << (tourCounter) << ": " << endl << endl;

			battleChecker = i;

			Card* card1;
			if (remainedCard != NULL)
			{
				card1 = remainedCard;
			}
			else
			{
				card1 = players[i % 2].firstCard();
			}

			remainedCard = &remainedCard_v;
			Card card2_v = *(players[(i + 1) % 2].firstCard());
			Card* card2 = &card2_v;
			players[(i + 1) % 2].popCard();

			Card* card3;
			if (!players[(i + 1) % 2].empty())
				card3 = players[(i + 1) % 2].firstCard();
			else
			{
				card3 = &card2_v;
			}


			if ((i + 1) % 2 == 0)		
			{
				switch (player1)
				{
				case 0:
					realPlayer(card1, card2, card3);
					break;
				case 1:
					choice = randomBot(card1, card2, card3);
					break;
				case 2:
					choice = battleBot(card1, card2, card3);
					break;
				case 3:
					choice = peaceBot(card1, card2, card3);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (player2)
				{
				case 0:
					realPlayer(card1, card2, card3);
					break;
				case 1:
					choice = randomBot(card1, card2, card3);
					break;
				case 2:
					choice = battleBot(card1, card2, card3);
					break;
				case 3:
					choice = peaceBot(card1, card2, card3);
					break;
				default:
					break;
				}
			}


			switch (choice)
			{
			case 1:
				stos[i % 2].push(card1);
				stos[(i + 1) % 2].push(card2);
				tourCounter++;
				players[i % 2].popCard();
				if (compare_SMART())
				{
					remainedCard_v = *card3;
					battle();
				}
				else
					remainedCard_v = *card3;
				break;
			case 2:
				stos[i % 2].push(card1);
				stos[(i + 1) % 2].push(card3);
				tourCounter++;
				players[i % 2].popCard();
				if (compare_SMART())
				{
					remainedCard_v = *card2;
					battle();
				}
				else
					remainedCard_v = *card2;
				break;
			default:
				cout << "zly wybor" << endl;
				return;
			}

		}

		if (players[0].empty() && players[1].size() > 0)
		{
			cout << "Player 2 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
		}
		else if (players[1].empty() && players[0].size() > 0)
		{
			cout << "Player 1 WINS GAME!" << endl << "Liczba tur: " << tourCounter - 1 << endl;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
		}
	}

	void realPlayer(Card* card1, Card* card2, Card* card3)		//funkcja gry dla graczy
	{
		cout << "Karta przeciwnika: " << endl;
		card1->show();
		cout << endl;
		cout << "Wybierz: " << endl;
		card2->show();
		card3->show();
		cout << endl << "Aby wybrac pierwsza karte wprowadz '1', aby wybrac druga wprowadz '2': ";
		cin >> choice;
		cout << endl;
	}

	int randomBot(Card* card1, Card* card2, Card* card3)		//bot losowy
	{
		int randchoice = rand() % 2;
		if (randchoice)
			return 2;
		else
			return 1;
	}

	int battleBot(Card* card1, Card* card2, Card* card3)		//bot wściekły
	{
		if (card2->getValue() == card1->getValue())
			return 1;
		else if (card3->getValue() == card1->getValue())
			return 2;
		else if (card2->getValue() <= card3->getValue())
		{
			if (card2->getValue() > card1->getValue() && card3->getValue() > card1->getValue())
				return 1;
			else if (card2->getValue() < card1->getValue() && card3->getValue() < card1->getValue())
				return 1;
			else if (card3->getValue() > card1->getValue() && card2->getValue() < card1->getValue())
				return 2;
		}
		else if (card2->getValue() > card3->getValue())
		{
			if (card2->getValue() > card1->getValue() && card3->getValue() > card1->getValue())
				return 2;
			else if (card2->getValue() < card1->getValue() && card3->getValue() < card1->getValue())
				return 2;
			else if (card2->getValue() > card1->getValue() && card3->getValue() < card1->getValue())
				return 1;
		}
	}

	int peaceBot(Card* card1, Card* card2, Card* card3)			//bot pokojowy
	{
		if (card2->getValue() == card1->getValue())
			return 2;
		else if (card3->getValue() == card1->getValue())
			return 1;
		else if (card2->getValue() <= card3->getValue())
		{
			if (card2->getValue() > card1->getValue() && card3->getValue() > card1->getValue())
				return 1;
			else if (card2->getValue() < card1->getValue() && card3->getValue() < card1->getValue())
				return 1;
			else if (card3->getValue() > card1->getValue() && card2->getValue() < card1->getValue())
				return 1;
		}
		else if (card2->getValue() > card3->getValue())
		{
			if (card2->getValue() > card1->getValue() && card3->getValue() > card1->getValue())
				return 2;
			else if (card2->getValue() < card1->getValue() && card3->getValue() < card1->getValue())
				return 2;
			else if (card2->getValue() > card1->getValue() && card3->getValue() < card1->getValue())
				return 2;
		}
	}

	int compare_SMART()			//porównanie kart na stole
	{
		random1 = rand() % 2;
		if (random1 % 2 == 0)
			random2 = 1;
		else
			random2 = 0;

		if (stos[0].top->getValue() > stos[1].top->getValue())
		{
			stos[0].top->show();
			cout << " vs " << endl;
			stos[1].top->show();
			cout << endl;

			while (!stos[random1].empty())
			{
				players[0].pushbackCard(stos[random1].top);
				stos[random1].pop();
			}
			while (!stos[random2].empty())
			{
				players[0].pushbackCard(stos[random2].top);
				stos[random2].pop();
			}
			cout << "Player 1 WIN BATTLE" << endl << endl;
			return 0;
		}
		else
			if (stos[0].top->getValue() < stos[1].top->getValue())
			{
				stos[0].top->show();
				cout << " vs " << endl;
				stos[1].top->show();
				cout << endl;

				while (!stos[random1].empty())
				{
					players[1].pushbackCard(stos[random1].top);
					stos[random1].pop();
				}
				while (!stos[random2].empty())
				{
					players[1].pushbackCard(stos[random2].top);
					stos[random2].pop();
				}
				cout << "Player 2 WIN BATTLE" << endl << endl;
				return 0;
			}
			else
			{
				cout << "player1 size: " << players[0].size() << endl;
				cout << "player2 size: " << players[1].size() << endl << endl;
				stos[0].top->show();
				cout << " vs " << endl;
				stos[1].top->show();
				cout << endl;
				return 1;
			}
	}

	void battle()			//funkcja wojny dla mądrej wojny
	{
		if (!(players[0].empty() || players[1].empty()))
		{
			cout << "BATTLE!" << endl << endl;

			Card* card3;
			if (remainedCard != NULL)
				card3 = remainedCard;
			else
				card3 = players[(battleChecker + 1) % 2].firstCard();

			Card* card4 = players[battleChecker % 2].firstCard();

			cout << "X X" << endl << endl;

			stos[(battleChecker + 1) % 2].push(card3);
			stos[battleChecker % 2].push(card4);
			tourCounter++;

			players[0].popCard();
			players[1].popCard();
			remainedCard = NULL;

			if (!(players[0].empty() || players[1].empty()))
			{
				Card* card5 = players[(battleChecker + 1) % 2].firstCard();
				Card* card6 = players[battleChecker % 2].firstCard();

				stos[(battleChecker + 1) % 2].push(card5);
				stos[battleChecker % 2].push(card6);
				tourCounter++;

				players[0].popCard();
				players[1].popCard();
				compare();
			}
			else if ((players[0].empty() && players[1].empty()))
			{
				cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
				cout << "player1 size: " << players[0].size() << endl;
				cout << "player2 size: " << players[1].size() << endl << endl;
				return;
			}

		}
		else if ((players[0].empty() && players[1].empty()))
		{
			cout << "REMIS" << endl << "Liczba tur: " << tourCounter - 1;
			cout << "player1 size: " << players[0].size() << endl;
			cout << "player2 size: " << players[1].size() << endl << endl;
			return;
		}


	}
};

int main() {

	Game* game;
	int variant;
	srand(time(NULL));

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);

	cout << "MENU GLOWNE GRY WOJNA:" << endl;
	cout << "------------------------------------------" << endl;
	cout << "Wariant A: Click '1'" << endl;
	cout << "Wariant B: Click '2'" << endl;
	cout << "Madra Wojna player vs player: Click '3'" << endl;
	cout << "Madra Wojna Losowa vs Losowa: Click '4'" << endl;
	cout << "Madra Wojna Losowa vs Wsciekla: Click '5'" << endl;
	cout << "Madra Wojna Losowa vs Pokojowa: Click '6'" << endl;
	cout << "Madra Wojna Wsciekla vs Wsciekla: Click '7'" << endl;
	cout << "Madra Wojna Wsciekla vs Pokojowa: Click '8'" << endl;
	cout << "Madra Wojna Pokojowa vs Pokojowa: Click '9'" << endl;
	cout << "------------------------------------------" << endl;
	cout << "Warianty Dodatkowe - Zagraj z Botem:" << endl;
	cout << "Madra Wojna player vs Losowa: Click '100'" << endl;
	cout << "Madra Wojna player vs Wsciekla: Click '101'" << endl;
	cout << "Madra Wojna player vs Pokojowa: Click '102'" << endl;
	cout << "------------------------------------------" << endl;
	cout << "Wybor: ";
	cin >> variant;
	cout << endl << endl;

	switch (variant)
	{
	case 1:
		game = new Game_A();
		game->play();
		break;

	case 2:
		game = new Game_B();
		game->play();
		break;
	case 3:
		game = new Game_SMART(0, 0, 0);
		game->play();
		break;
	case 4:
		game = new Game_SMART(0, 1, 1);
		game->play();
		break;

	case 5:
		game = new Game_SMART(0, 1, 2);
		game->play();
		break;
	case 6:
		game = new Game_SMART(0, 1, 3);
		game->play();
		break;
	case 7:
		game = new Game_SMART(0, 2, 2);
		game->play();
		break;

	case 8:
		game = new Game_SMART(0, 2, 3);
		game->play();
		break;
	case 9:
		game = new Game_SMART(0, 3, 3);
		game->play();
		break;
	case 100:
		game = new Game_SMART(0, 0, 1);
		game->play();
		break;

	case 101:
		game = new Game_SMART(0, 0, 2);
		game->play();
		break;
	case 102:
		game = new Game_SMART(0, 0, 3);
		game->play();
		break;
	default:
		cout << "Bledny wybor";
		return -1;
	}

	return 0;
}

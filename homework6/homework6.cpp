/*
1. Создать программу, которая считывает целое число типа int. И поставить «защиту от дурака»: 
если пользователь вводит что-то кроме одного целочисленного значения, нужно вывести сообщение об ошибке и предложить ввести число еще раз. 
Пример неправильных введенных строк:
rbtrb
nj34njkn
1n

2. Создать собственный манипулятор endll для стандартного потока вывода, который выводит два перевода строки и сбрасывает буфер.
3. Реализовать класс Player, который наследует от класса GenericPlayer. У этого класса будет 4 метода:
• virtual bool IsHitting() const - реализация чисто виртуальной функции базового класса. Метод спрашивает у пользователя, 
нужна ли ему еще одна карта и возвращает ответ пользователя в виде true или false.
• void Win() const- выводит на экран имя игрока и сообщение, что он выиграл.
• void Lose() const - выводит на экран имя игрока и сообщение, что он проиграл.
• void Push() const - выводит на экран имя игрока и сообщение, что он сыграл вничью.

4. Реализовать класс House, который представляет дилера. Этот класс наследует от класса GenericPlayer. У него есть 2 метода:
• virtual bool IsHitting() const - метод указывает, нужна ли дилеру еще одна карта. Если у дилера не больше 16 очков, то он берет еще одну карту.
• void FlipFirstCard() - метод переворачивает первую карту дилера.

5. Написать перегрузку оператора вывода для класса Card. Если карта перевернута рубашкой вверх (мы ее не видим), вывести ХХ, 
если мы ее видим, вывести масть и номинал карты. 
Также для класса GenericPlayer написать перегрузку оператора вывода, который должен отображать имя игрока и его карты, а также общую сумму очков его карт.
*/

#include <iostream>
#include <vector>
#include <limits>
//Task #1
void verify() {
	char a;
	std::cout << "Enter a number: ";
	do
	{
		std::cin >> a;

		if (std::cin.good() && std::cin.peek() == '\n')
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << a << " is a number!";
			break;

		}
		std::cin.clear();
		std::cout << std::endl << a << " is not a number!";

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (true);
}

//Task #2
class endll {
	friend std::ostream& operator<< (std::ostream& os, const endll& e);
};

std::ostream& operator<< (std::ostream& os, const endll& e) {
	os << "\n\n";
	std::fflush(stdout);
	return os;
}

//Task #3, 4, 5
enum rank {
	ACE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	NINE = 9,
	TEN = 10,
	JACK = 10,
	QUEEN = 10,
	KING = 10
};
enum suit
{
	CLUBS,
	DIAMONDS,
	HEARTS,
	SPADES
};
class Card
{
public:

	Card(rank Rank, suit Suit) : Rank(Rank), Suit(Suit) {
		isFaceUp = true;
	}
	~Card() {}
	int GetValue() const {
		return Rank;
	}
	void Flip() {
		isFaceUp = !(isFaceUp);
	}

protected:
	rank Rank;
	suit Suit;
	bool isFaceUp;
};
class Hand
{
public:
	Hand();
	virtual ~Hand();
	void Add(Card* pCard)
	{
		myCards.push_back(pCard);
	}
	void Clear() {
		std::vector<Card*>::iterator i = myCards.begin();
		for (i = myCards.begin(); i != myCards.begin(); ++i)
		{
			delete* i;
			*i = 0;
		}
		myCards.clear();
	}
	int GetValue() const {
		if (myCards.empty())
		{
			return 0;
		}
		if (myCards[0]->GetValue() == 0)
		{
			return 0;
		}
		int total = 0;
		std::vector<Card*>::const_iterator i;
		for (i = myCards.begin(); i != myCards.end(); ++i)
		{
			total += (*i)->GetValue();
		}
		bool isAce = false;
		for (i = myCards.begin(); i != myCards.end(); ++i)
		{
			if ((*i)->GetValue() == rank::ACE)
			{
				isAce = true;
			}
		}
		if (isAce && total <= 11)
		{
			total += 10;
		}
		return total;
	}
protected:
	std::vector<Card*> myCards;
};

Hand::Hand()
{
	myCards.reserve(5);
}

Hand::~Hand()
{
	Clear();
}

class GenericPlayer : public Hand
{
	friend std::ostream& operator <<(std::ostream& os, const GenericPlayer& aGenericPlayer);
public:
	GenericPlayer(const std::string& name = "");
	virtual ~GenericPlayer();
	virtual bool IsHitting() const = 0;
	bool IsBusted() const {
		return (GetValue() > 21);
	}
	void Bust() const {
		std::cout << myName << "busts.\n";
	}
protected:
	std::string myName;
};
GenericPlayer::GenericPlayer(const std::string& name) : myName(name) {}
GenericPlayer::~GenericPlayer() {}

class Player :public GenericPlayer
{
public:
	Player(const std::string& name="");
	virtual ~Player();
	virtual bool IsHitting() const {
		std::cout << myName << ", do you want to hit? (Y/N): ";
		char response;
		std::cin >> response;
		return (response == 'y' || response == 'Y');
	}
	void Win() const {
		std::cout << myName << " wins.\n";
	}
	void Lose() const {
		std::cout << myName << " loses.\n";
	}
	void Push() const {
		std::cout << myName << " pushes.\n";
	}
};

Player::Player(const std::string& name) : GenericPlayer(name) {}
Player::~Player() {}

class House : public GenericPlayer
{
public:
	House(const std::string& name = "House");
	virtual ~House();
	virtual bool IsHitting() const {
		return (GetValue() <= 16);
	}
	void FlipFirstCard() {
		if (!(myCards.empty()))
		{
			myCards[0]->Flip();
		}
		else
		{
			std::cout << "No cards to flip!\n";
		}
	}
};

House::House(const std::string& name) : GenericPlayer(name) {}

House::~House() {}


int main()
{
    std::cout << "Hello World!" << endll() << "Hello World!" << endll();
	verify();
	return 0;
}

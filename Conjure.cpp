#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>



using Stats = std::unordered_map<std::string, std::vector<int>>;
Stats getstats()
{
	std::ifstream f("statblocks.txt");
	if (f.bad())
	{
		std::cout << "failed to open file\n";
		exit(1);
	}
	Stats stats;
	std::string line;
	while (getline(f, line))
	{
		std::istringstream linestrm(line);
		std::string name;
		linestrm >> name;
		if (name.empty())
		{
			std::cout << "empty name\n";
			exit(1);
		}
		if (name[0] == '#')
		{
			continue;
		}
		std::vector<int> statline;
		while (linestrm.good())
		{
			int i;
			linestrm >> i;
			statline.push_back(i);
		}
		if (statline.size() != 12)
		{
			std::cout << "wrong number of stats: " << line << "\n";
			exit(1);
		}
		stats[name] = std::move(statline);
	}
	return stats;
}

static auto stats = getstats();

enum class Reference
{
 MAX_HP = 0,
 HP,
 THP,
 HIT,
 STR,
 DEX,
 CON,
 INTEL,
 WIS,
 CHA,
 DICE,
 SIZE,

 END
};

enum class Conditions
{
	PRONE = 0,
	EXHAUSTION,
	STUNNED,
	BLIND,
	DEAF,
	PARALYSED,
	FRIGHTENED,
	CHARMED,
	GRAPPLED,
	INCAPACITATED,
	INVISIBLE,
	PETRIFIED,
	POISONED,
	RESTRAINED,
	UNCONSCIOUS,

	END
};

class Animal
{
private:
	std::string name;
	int max_hp;
	int hp;
	int thp;
	int hit;
	int str;
	int dex;
	int con;
	int intel;
	int wis;
	int cha;
	int dice;
	int size;
	static std::vector<Animal> gfield;

public:
	Animal(std::string creature)
	{
		const auto& s = stats[creature];
		if (s.size() != static_cast<size_t>(Reference::END))
		{
			std::cout << "Could not create animal from " << creature << "\n";
			exit(1);
		}
		name = creature;
		max_hp = s[static_cast<size_t>(Reference::MAX_HP)];
		hp = s[static_cast<size_t>(Reference::HP)];
		thp = s[static_cast<size_t>(Reference::THP)];
		hit = s[static_cast<size_t>(Reference::HIT)];
		str = s[static_cast<size_t>(Reference::STR)];
		dex = s[static_cast<size_t>(Reference::DEX)];
		con = s[static_cast<size_t>(Reference::CON)];
		intel = s[static_cast<size_t>(Reference::INTEL)];
		wis = s[static_cast<size_t>(Reference::WIS)];
		cha = s[static_cast<size_t>(Reference::CHA)];
		dice = s[static_cast<size_t>(Reference::DICE)];
		size = s[static_cast<size_t>(Reference::SIZE)];
	}

	void damage(std::vector<int> choices, int dmg)
	{
		for (int i = 0; i < choices.size(); i++)
		{
			Animal& ref = gfield[choices[i]];
			if (ref.thp < dmg)
			{
				ref.thp = 0;
				ref.hp -= (dmg - ref.thp);
			}
			else
			{
				ref.thp -= dmg;
			}
		}
	}

	static void summon(std::string creature, int n)
	{
		std::vector<Animal> field;
		int i = 0;
		while (i < n)
		{
			Animal a = Animal(creature);
			field.push_back(a);
			i++;
		}
		std::cout << field.size();
		gfield = field;
	}
};

int main()
{
	std::string name;
	std::string x;
	std::cin >> name;
	bool valid = false;
	while (!valid)
	{
		if (stats.contains(name))
		{
			valid = true;
		}
		else
		{
			std::cout << "Invalid name\n";
			std::cin >> name;
		}
	}
	std::cin >> x;
	int newx;
	valid = false;
	while (!valid)
	{
		valid = true;
		for (int i = 0; i < x.length(); i++)
		{
			if (!isdigit(x[i]))
			{
				valid = false;
			}
		}
		if (valid)
		{
			newx = stoi(x);
			if (newx < 1 || newx > 8)
			{
				valid = false;
			}
		}
		if (!valid)
		{
			std::cout << "invalid number\n";
			std::cin >> x;
		}
	}
	Animal::summon(name, newx);
}

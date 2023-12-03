#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

// Add a description comment of what the code does
// And it is better to make all the interesting stuff into a class or function, and have main just call that.  
// That way, if you wanted to use it somewhere else, you don't have to drag main along with it

using Stats = std::unordered_map<std::string, std::vector<int>>;
// convention is either pascalCase or snake_case for function names
// And really this ufnction creates a Stats object from a file, so it should be getStatsFromFile.
Stats getstats()
{
	std::ifstream f("statblocks.txt");
	if (f.bad())
	{
		// be nice to the user - say what the name of the file is in the error message
		std::cout << "failed to open file\n";
		exit(1);
	}
	Stats stats;
	std::string line;
	// maybe describe here what the expected format is.
	// Personally I would use JSON these days, but what you ahve done is certainly easier.
	while (getline(f, line))
	{
		std::istringstream linestrm(line);
		// I would make the processing of a line a separate function, perhaps a lambda within this function, so you would say
		// auto [name, statline] = readLine(line);
		std::string name;
		linestrm >> name;
		if (name.empty())
		{
			// output the complete line to make debugging easier
			std::cout << "empty name\n";
			exit(1);
		}
		if (name[0] == '#')
		{
			// add comment here to say that lines beginning with a '#' are ignored
			continue;
		}
		std::vector<int> statline;
		while (linestrm.good())
		{
			int i;
			linestrm >> i;
			// guard against invalid input - maybe you haven't created an int here in which case you should error and output the line
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

// can this be const?
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
	// initialise all these to 0 here	
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
	// Animal() = delete; // connot create an animal without a name
	// const std::string&
	Animal(std::string creature)
	{
		const auto& s = stats[creature];
		if (s.size() != static_cast<size_t>(Reference::END))
		{
			std::cout << "Could not create animal from " << creature << "\n";
			exit(1);
		}
		name = creature;
		// I think that the way you are using the vector it would in fact be better as a struct.
		// Then you would have to do all the casting malarky
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

	// const std::vector<int>&
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

	// const std::string&
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
		std::cout << field.size(); // << '\n'; ?
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
	// make this a lambda and return the values you want from it, ie newx
	// and the loop is better done as a while(true) with explicit break, rather than a variable which is declared outside the loop
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
	Animal::summon(name, newx); // what does this line do?  The program terminates immediately afterwards
}

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "csvparse.h"


struct entry
{
    int year;
    std::string title;
    std::string publication;
    std::string authors;
    int cat;
    int endorsement;
};

std::vector<entry> v;
std::vector<int> counts(8);
int lineno;


entry parse(const std::string& line)
{
    entry e = {};
    std::vector<std::string> values;
    int c = CsvParse(line, values);
    if (c != 6)
    {
	std::cout << "Incorrect number of elements in line " << lineno << std::endl;
    }
    else
    {
	e.year        = std::stoi(values[0]); 
	e.title       = values[1];
	e.publication = values[2];
	e.authors     = values[3];
	e.cat         = std::stoi(values[4]);
	e.endorsement = std::stoi(values[5]);
    }
    return e;
}

int main()
{
    std::string line;
    bool started = false;
    entry e;
    while(getline(std::cin, line))
    {
	lineno++;
	if (started)
	{
	    v.push_back(parse(line));
	}
	if (!started && line.substr(0, 4) == "Year")
	{
	    started = true;
	}
    }

    for(auto e : v)
    {
	assert(e.endorsement >= 1 && e.endorsement <= 7 && "Endorsement should be 1..7");
	counts[e.endorsement]++;
    }	
    int total = 0;
    for(int i = 1; i < 8; i++)
    {
	std::cout << i << " " << counts[i] << std::endl;
	total += counts[i];
    }
    std::cout << "T " << total << std::endl;
    
}

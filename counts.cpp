#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
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

// Split string separated by semicolon 
std::vector<std::string> make_list(std::string a)
{
    std::vector<std::string> v;
    std::string::size_type pos1 = 0, pos2 = 0;
    for(;;)
    {
	if ((pos2 = a.find(";", pos1)) == std::string::npos)
	{
	    v.push_back(a.substr(pos1));
	    break;
	}
	else
	{
	    v.push_back(a.substr(pos1, pos2-pos1));
	    pos1 = pos2+1;
	}
    }
    return v;
}

void list_author_views()
{
    std::map<std::string, std::vector<int>> m;
    for(auto e : v)
    {
	std::vector<std::string> a = make_list(e.authors);
	for(auto i : a)
	{
	    m[i].push_back(e.endorsement);
	}
    }
    int n_for = 0;
    int n_against = 0;
    int n = 0;
    for(auto a : m)
    {
	n++;
	int score = 0;
	for(auto i : a.second)
	{
	    score += i < 4;
	    score -= i > 4;
	}
	n_for += score > 0;
	n_against += score < 0;
    }
    std::cout << "Authors: " << n << " for: " << n_for << " against:" << n_against << std::endl;
}

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

int main(int argc, char **argv)
{
    std::string line;
    bool started = false;
    entry e;

    bool list_per_author = (argc > 1) && (std::string(argv[1]) == "authors");

    while(getline(std::cin, line))
    {
	lineno++;
	if (started)
	{
#if 1
/* Bland de författare som klassats som anhängare var välkända och uttalade skeptiker som 
   Willie Soon, Craig Idso, Nicola Scafetta, Nir Shaviv, Nils-Axel Morner and Alan Carlin. */

	    entry e = parse(line);
	    const char *names[] = { "Soon", "Idso", "Scafetta", "Shaviv", "Morner", "Carlin" };
	    bool skip = false;
	    for(auto n : names)
	    {
		if (e.authors.find(n) != std::string::npos)
		{
		    skip = true;
		}
	    }
	    if (!skip)
	    {
		v.push_back(e);
	    }
#else
	    v.push_back(parse(line));
#endif
	}
	if (!started && line.substr(0, 4) == "Year")
	{
	    started = true;
	}
    }

    if (list_per_author)
    {
	list_author_views();
    }
    else
    {
	for(auto e : v)
	{
	    assert(e.endorsement >= 1 && e.endorsement <= 7 && "Endorsement should be 1..7");
	    counts[e.endorsement]++;
	}	
	int total = 0;
	for(int i = 1; i < 8; i++)
	{
	    total += counts[i];
	}
	int total_less_4 = total -counts[4];
	
	std::cout << "Total: " << total << " Less undecided: " << total_less_4 << std::endl;
	for(int i = 1; i < 8; i++)
	{
	    double perc = 100.0 * counts[i] / total; 
	    double perc_less_4 = 100.0 * counts[i] / total_less_4;
	    // Only give percentage for  this for "not 4" values.
	    if (i == 4) perc_less_4 = 0; 
	    std::cout << i << " " << std::setw(6) << counts[i] << " " 
		      << std::setw(10) << std::fixed << std::setprecision(2) << perc << "% " 
		      << std::setw(10) << perc_less_4 << "%" << std::endl;
	}
    }
}


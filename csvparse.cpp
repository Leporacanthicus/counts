#include <iostream>
#include "csvparse.h"

int CsvParse(const std::string& input, std::vector<std::string>& values)
{
    std::string::size_type pos = 0;
    int cnt = 0;
    
    std::string val;
    for(;;)
    {
	if (pos == input.length())
	{
	    if (val != "")
	    {
		values.push_back(val);
		cnt++;
	    }
	    return cnt;
	}
	char c = input[pos++];
	switch(c)
	{
	case ',':
	    values.push_back(val);
	    val = "";
	    cnt++;
	    break;

#if 0	    
	case '"':
//	case '\'':
	{
	    if (val != "")
	    {
		std::cerr << "Expected quotes to start at the beginning of the string" << std::endl;
		std::cerr << "pos = " << pos << " input = " << input << std::endl;
		return -1;
	    }
	    char c2;
	    do
	    {
		c2 = input[pos++];
		if (c != c2)
		{
		    val += c2;
		    if (pos == input.length())
		    {
			std::cerr << "Expected " << c << " before end of line" << std::endl;
			return -1;
		    }
		}
	    } while (c2 != c);
	}		
	break;
#endif
	
	case ' ':
	case '\t':
	    // Ignore spaces.
	    break;
	    
	default:
	    val += c;
	    break;
	}
    }
}

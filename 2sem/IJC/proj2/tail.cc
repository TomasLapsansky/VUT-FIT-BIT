#include <iostream>
#include <deque>
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

void tail(istream &inp, unsigned int lines)
{
	deque<string> buffer;
	string line;
	
	while(getline(inp, line))
	{
		buffer.push_front(line);
		
		if(buffer.size() > lines)
			buffer.pop_back();
	}
	
	for(int i = (buffer.size() - 1); i >= 0; i -= 1)
	{
		cout << buffer[i] << endl;
	}
}

int main(int argc, char* argv[])
{
	std::ios::sync_with_stdio(false);

	istream *stdinp = &cin;
	fstream fileinp;
	
	if(argc > 4)
	{
	
		cerr << "Too many arguments!" << endl;
		return 1;
		
	} else if(argc == 1)
	{
	
		tail(*stdinp, 10);
		
	} else if(argc == 2)
	{
		string argcomp = argv[1];
		
		if(argcomp.compare("-n") == 0)
		{
			cerr << "Missing argument for -n!" << endl;
			return 1;
		}
		
		fileinp.open(argv[1], ios::in);
		
		if(!fileinp.is_open())
		{
			cerr << "Cant open file!" << endl;
		}
		
		stdinp = &fileinp;
		tail(*stdinp, 10);
		
		fileinp.close();
		
	} else if(argc == 3)
	{
		
		string argcomp = argv[1];
		
		if(argcomp.compare("-n") != 0)
		{
			cerr << "Wrong arguments!" << endl;
			return 1;
		}
		
		int n = stoi(argv[2], NULL, 10);
		
		if(n < 0)
		{
			cerr << "Wrong number for -n!" << endl;
			return 1;
		}
		
		tail(*stdinp, n);
	} else
	{
		
		string argcomp = argv[1];
		
		if(argcomp.compare("-n") != 0)
		{
			cerr << "Wrong arguments!" << endl;
			return 1;
		}
		
		fileinp.open(argv[3], ios::in);
		
		if(!fileinp.is_open())
		{
			cerr << "Cant open file!" << endl;
		}
		
		stdinp = &fileinp;
		
		int n = stoi(argv[2], NULL, 10);
		
		if(n < 0)
		{
			cerr << "Wrong number for -n!" << endl;
			return 1;
		}
		
		tail(*stdinp, n);
		
		fileinp.close();
	}
	
	return 0;
}

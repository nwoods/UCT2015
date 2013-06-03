#include<iostream>
#include<fstream>

using namespace std;

int main(int argc, char* argv[])
{
  if(argc <= 1)
    {
      cout << "Please give me a file name" << endl;
      return -1;
    }
  char* fName = argv[1];
  ofstream f;
  f.open(fName);
  if(f.is_open())
    {
      for(int iEta = -32; iEta <= 32; ++iEta)
	{
	  if(iEta == 0) continue;
	  for(unsigned iPhi = 1; iPhi <= 72; ++iPhi)
	    {
	      f << iEta << "   " 
		<< iPhi << "   " 
		<< 0 << endl;
	    }
	}
      f.close();
    }
  else
    cout << "Unable to open file. Did you give a valid name?" << endl;
  return 1;
}

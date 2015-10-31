#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <bitset>
using namespace std;

int getObvs(string x);
float getProb(float x, float y);

int main(int argc, char *argv[])
{
  //get the error number
  double error = stof(argv[2]);

  // set up the Difference table
  double D[5];
  for(int i = 0; i < 5; i++)
  {
    D[i] = pow(error, i) * pow((1-error), (4-i));
  }
  //print D table
  for(int i = 0; i < 5; i++)
  {
    cout << D[i] << endl;
  }
  cout << endl;

  //set up the observations
  double obvs[argc-3];
  for(int i = 3; i < argc; i++)
  {
    obvs[i-3] = getObvs(argv[i]); 
  }
  //print the obvs array
  for(int i =3; i < argc; i++)
  {
    cout << obvs[i-3] << ' ';
  }
  cout << endl << endl;

  //all this stuff to fill in the 2d vector of rooms
  vector< vector<int> > grid;
  int rowsCounted = 0;
  int row_size = 0;
  int col_size = 0;
  int rooms = 0;
  string line;
  ifstream myfile (argv[1]);
  if(myfile.is_open())
  {
    while(!myfile.eof())
    {
      getline(myfile,line);
      istringstream split(line);
      int x = 0;
      string room;
      vector <int> newRow;
      do
      {
        string room;
        split >> room;
        if(room.size() > 0)
        {
          int x = stoi(room);
          newRow.push_back(x);
          rooms++;
          if(!rowsCounted)
          {
            row_size++;
          }
        }
      } while(split);
      rowsCounted = 1;
      grid.push_back(newRow);
    }
  }

  //Create J
  float J[rooms];
  int usableRooms = 0;
  //see how many rooms are usable
  for(vector<vector<int>>::size_type i = 0; i < grid.size(); i++)
  {
    for(vector<int>::size_type j = 0; j < grid[i].size(); j++)
    {
      if(grid[i][j] != 15)
      {
        usableRooms = usableRooms+1;
      }
    }
  }
  //assign 1 to each of the rooms taht are usable
  for(int i = 0; i < rooms; i++)
  {
    if(grid[i/row_size][i%row_size] != 15)
    {
      J[i] = 1;
    }
    else
    {
      J[i] = 0;
    }
  }
  //set J table to proper number after setting to 1 or 0
  for(int i = 0; i < rooms; i++)
  {
    J[i] = J[i]/usableRooms;
  }
  //printing the J table
  for (int i = 0; i < rooms; i++)
  {
    cout << J[i] << ' ';
  }
  cout << endl;

  //just printing out the rooms for testing here
  for(vector<vector<int>>::size_type i = 0; i < grid.size(); i++)
  {
    for(vector<int>::size_type j = 0; j < grid[i].size(); j++)
    {
      cout << grid[i][j] << ' ';
    }
    cout << endl;
  }
  cout << "Rows: " << row_size << " Cols: " << col_size << 
    " Rooms: " << rooms << endl;

  //T is the transition matrix
  float T[rooms][rooms];
  int neighbors = 0;
  for(int i = 0; i < rooms; i++)
  {
    for(int j = 0; j < rooms; j++)
    {
      T[i][j] = 0;
    }
  }
  //i is to/destination j is from/origin
  for(int i = 0; i < rooms; i++)
  {
    for(int j = 0; j < rooms; j++)
    {
      if( i != j)
      {
        //figure our the grid x and y that we're looking at
        int orow;
        if(j < row_size)
        {
          orow = 0;
        }
        else
        {
          orow = j/row_size;
        }
        int ocol = j%row_size;

        int drow;
        if(i < row_size)
        {
          drow = 0;
        }
        else
        {
          drow = i/row_size;
        }
        int dcol = i%row_size;

        //Make sure the rooms are actually next to each other
        if(((drow == orow) && (abs(dcol-ocol) <= 1)) || 
             ((dcol == ocol) && (abs(drow - orow) <=1)))
        {
          //cout << "Dest: " << drow << dcol << endl <<
            //"Orig: " << orow << ocol << endl;

          if(dcol < ocol)      //coming from right
          {
            if(!(grid[orow][ocol] & 2 && grid[drow][dcol] & 1))
              T[i][j]++;
          }
          else if(dcol > ocol) //coming from left
          {
            if(!(grid[orow][ocol] & 1 && grid[drow][dcol] & 2))
              T[i][j]++;
          }
          else if(drow < orow) //coming from below
          {
            if(!(grid[orow][ocol] & 8 && grid[drow][dcol] & 4))
              T[i][j]++;
          }
          else if(drow > orow) //coming from above
          {
            if(!(grid[orow][ocol] & 4 && grid[drow][dcol] & 8))
              T[i][j]++;
          }
        }
        neighbors = 0;
      }
    }
  }
  //double fors to figure out how many transitions per row
  for(int j = 0; j < rooms; j++)
  {
    int rowTrans = 0;
    for(int i = 0; i < rooms; i++)
    {
      if(T[i][j] == 1)
      {
        rowTrans++;
      }
    }
    //changes the transitions from 1 to their actual number
    for(int i = 0; i < rooms; i++)
    {
      if(rowTrans > 0)
        T[i][j] = T[i][j]/rowTrans;
    }
    rowTrans = 0;
  }
  //prints the T table
  for(int i = 0; i < rooms; i++)
  {
    for(int j = 0; j < rooms; j++)
    {
      cout << T[i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;

  //do P(X1) before the for loop
  double Z[rooms];
  for(int i = 0; i < rooms; i++)
  {
    Z[i] = 0;
  }
  for(int i = 0; i < rooms; i++)
  {
    for(int j = 0; j < rooms; j++)
    {
      Z[i] = Z[i] + T[i][j] * J[j];
    }
  }
  for(int i = 0; i < rooms; i++)
  {
    cout << Z[i] << ' ';
  }
  cout << endl;

  //set J = Z
  for(int i = 0; i < rooms; i++)
  {
    J[i] = Z[i];
  }

  //set up O for the for loop
  float O[rooms][rooms];
  //initialize Y
  float Y[rooms];
  for(int i = 0; i < rooms; i++)
  {
    Y[i] = 0;
  }
  //initialize E
  float E[rooms];
  for(int i = 0; i < rooms; i++)
  {
    E[i] = 0;
  }
  for(int i = 0; i < (argc-3); i++)
  {
    //initialize O
    for(int j = 0; j < rooms; j++)
    {
      int differences = getProb(obvs[i], grid[j/row_size][j%row_size]);
      O[j][j] = D[differences];
    }
    //print O for testing
    for(int j = 0; j < rooms; j++)
    {
      cout << O[j][j] << endl;
    }
    //Y = O * J
    for(int i = 0; i < rooms; i++)
    {
      Y[i] = O[i][i] * J[i];
    }
    //print Y for testing
    for(int i = 0; i < rooms; i++)
    {
      cout << Y[i] << ' ';
    }
    cout << endl;
    //Sum Y
    double sumY = 0;
    for(int i = 0; i < rooms; i++)
    {
      sumY = sumY + Y[i];
    }
    cout << sumY << endl;
    //get E
    for(int i = 0; i < rooms; i++)
    {
      E[i] = Y[i]/sumY;
      cout << "E: " << E[i] << endl;
    }
    //clear out J
    for(int i = 0; i < rooms; i++)
    {
      J[i] = 0;
    }
    //J = T * Y
    for(int i = 0; i < rooms; i++)
    {
      for(int j = 0; j < rooms; j++)
      {
        J[i] = J[i] + T[i][j] * Y[j];
      }
    }
    //print J for testing
    for(int i = 0; i < rooms; i++)
    {
      cout << J[i] << ' ';
    }
    cout << endl;
  }

  //now figure out which rooms
  double best = 0;
  for(int i = 0; i < rooms; i++)
  {
    if(E[i] > best)
    {
      best = E[i];
    }
  }

}

//for getting probability for O matrix
float getProb(float obvs, float room)
{
  int differences = 0;
  //use bitset to compare the bit fields
  bitset<4> foo = obvs;
  bitset<4> bar = room;
  for(int i = 0; i < 4; i++)
  {
    if(foo[i] != bar[i])
    {
      differences = differences + 1;
      //cout << foo << ' ' << bar << ' ' << differences << endl;
    }
  }
  return differences;
}

//for getting the observation numbers
int getObvs(string x)
{
  size_t foundN = x.find('N');
  size_t foundS = x.find('S');
  size_t foundW = x.find('W');
  size_t foundE = x.find('E');

  int range = 0;

  if(foundN != string::npos)
  {
    range = range + 8;
  }
  if(foundS != string::npos)
  {
    range = range + 4;
  }
  if(foundW != string::npos)
  {
    range = range + 2;
  }
  if(foundE != string::npos)
  {
    range = range + 1;
  }
  return range;
}
























#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
using namespace std;

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
}























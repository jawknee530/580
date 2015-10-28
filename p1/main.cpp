#include<iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

int main(int arc, char *argv[])
{
  //get the error number
  int error = atoi(argv[2]);

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
  int T[rooms][rooms];
  int neighbors = 0;
  for(int i = 0; i < rooms; i++)
  {
    for(int j = 0; j < rooms; j++)
    {
      if( i != j)
      {
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
        cout << "From  grid :" << orow << ocol << endl;

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
        cout << "To grid :" << drow << dcol << endl;

        cout << "At Table Spot: " << i << " " << j << endl;
        if(j > 0) //left
        {
          neighbors++;
        }
        if(j < rooms-1) //right
        {
          neighbors++;
        }
        if(i > 0) //above
        {
          neighbors++;
        }
        if(i < rooms-1) //below
        {
          neighbors++;
        }
        cout << "Neighbors: " << neighbors << endl;
        cout << "-----------------------------" << endl;
        neighbors = 0;
      }
    }
  }
}

















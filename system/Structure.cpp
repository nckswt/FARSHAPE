#include "Structure.h"

int Structure::next_Piece(int* tt, float* tx, float* ty, float* tz)
{
  int cnt = 0;
  for(std::vector<std::vector<float> >::iterator it = Build.begin(); it != Build.end(); ++it) 
  {
    if ((*it)[2] == 0 && (*it)[1] == 0)//Next bar to go in, should there be anything about status? probably not for this.
    {
      *tt = (*it)[0];//Tells robot if it needs to get a beam or column
      *tx = (*it)[3];//coordinates x,y,z
      *ty = (*it)[4];
      *tz = (*it)[5];
      (*it)[1] = 1;//Load status of bar to busy
      return cnt; //Returns position in build order
    }
    cnt++;
  }
  return -1;//if all bars are being placed or already placed
}

void Structure::load_Piece(int buildorder, int stat, int id)
{
  (Build[buildorder])[1] = stat; //Placed or busy
  (Build[buildorder])[2] = id; //ID of bar to be placed
}
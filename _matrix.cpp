#include "matrix4.h"

int main(int argc,char *argv[])
{
  Matrix4 a,ai,r;

  a[0][0]=5; a[0][1]=4; a[0][2]=4; a[0][3]=1;
  a[1][0]=2; a[1][1]=2; a[1][2]=2; a[1][3]=5;
  a[2][0]=8; a[2][1]=9; a[2][2]=1; a[2][3]=2;
  a[3][0]=4; a[3][1]=1; a[3][2]=6; a[3][3]=1;

  ai=a.Inverse();

  cout << "A:\n" << a;
  cout << "Ai:\n" << ai;
  r=a*ai;
  cout << "A*Ai:\n" << r;
}

/*------------------------------------------------------
 * TITLE:  matrix4.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Matrix definition
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: matrix4.cpp,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:07:59  ady
 * Added new utilties for shadows
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:05:57  ady
 * Added Rotate() and stream I/O output
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "matrix4.h"
#include "vector3.h"
#include "useful.h"

//=======================================
// Constructor
//---------------------------------------
Matrix4::Matrix4()
{
  Identity();
}

//=======================================
// Identity
//---------------------------------------
void Matrix4::Identity()
{
  int i,j;

  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      element[i][j]=(i==j)?1:0;
}

Vector3 operator*(Matrix4 m,Vector3 v)
{
  int i,j;
  Vector3 r;

  r.x=(m[0][0]*v.x)+(m[0][1]*v.y)+(m[0][2]*v.z)+m[0][3];
  r.y=(m[1][0]*v.x)+(m[1][1]*v.y)+(m[1][2]*v.z)+m[1][3];
  r.z=(m[2][0]*v.x)+(m[2][1]*v.y)+(m[2][2]*v.z)+m[2][3];
  return r;
}

Matrix4 operator*(Matrix4 a,Matrix4 b)
{
  int i,j,k;
  Matrix4 r;

  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      r[i][j]=0;
      for(k=0;k<4;k++)
      {
        r[i][j]+=a[i][k]*b[k][j];
      }
    }
  }
  return r;
}

void Matrix4::Scale(Vector3 s)
{
  int i;
  Matrix4 m;

  m[0][0]=s.x;
  m[1][1]=s.y;
  m[2][2]=s.z;
  *this=(*this)*m;
}

void Matrix4::Translate(Vector3 t)
{
  int i;
  Matrix4 m;

  m[0][3]=t.x;
  m[1][3]=t.y;
  m[2][3]=t.z;
  *this=(*this)*m;
}

void Matrix4::Rotate(Vector3 r)
{
  Matrix4 m;
  float rx=DEGTORAD*r.x;
  float ry=DEGTORAD*r.y;
  float rz=DEGTORAD*r.z;

  m[1][1]=cos(rx);  m[1][2]=sin(rx);   
  m[2][1]=-sin(rx); m[2][2]=cos(rx);
  *this=(*this)*m;

  m.Identity(); 
  m[0][0]=cos(ry);  m[0][1]=sin(ry);   
  m[2][1]=-sin(ry); m[2][2]=cos(ry);
  *this=(*this)*m;

  m.Identity(); 
  m[0][0]=cos(rz);  m[0][1]=sin(rz);   
  m[1][0]=-sin(rz); m[1][1]=cos(rz);
  *this=(*this)*m;
}

ostream &operator<<(ostream &io,Matrix4 &m)
{
  int i,j;

  for(i=0;i<4;i++)
  {
    cout << "[";
    for(j=0;j<4;j++)
    {
      cout << m[i][j];
      if (j<3) cout << " ";
    }
    cout << "]\n";
  }
}

//=======================================
// Inverse
//---------------------------------------
Matrix4 Matrix4::Inverse()
{
  Matrix4 r; // result - starts of as identity matrix
  Matrix4 t=*this;
  int i,j,k;
  float scale;

  // Get zero's in non-identity positions
  for(k=0;k<4;k++)
  {
    for(j=0;j<4;j++)
    {
      if (j!=k)
      {
	scale=-t[j][k]/t[k][k];
        for(i=0;i<4;i++)
        {
          t[j][i]+=scale*t[k][i];
	  r[j][i]+=scale*r[k][i];
        }
      }
    }
  }

  // Now scale each row so diagonal elements are 1
  for(j=0;j<4;j++)
  {
    scale=1/t[j][j];
    for(i=0;i<4;i++)
    {
      t[j][i]=t[j][i]*scale;
      r[j][i]=r[j][i]*scale;
    }
  }

  return r;
}

void Matrix4::writeBinary(FILE *fp)
{
  int i,j;

  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
      fwrite(&element[i][j],sizeof(float),1,fp);
  }
}

void Matrix4::readBinary(FILE *fp)
{
  int i,j;

  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
      fread(&element[i][j],sizeof(float),1,fp);
  }
}


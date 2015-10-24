// Reyes includes
#include "ri.h"
#include "filter.h"
#include "texturemap.h"
#include "image.h"
#include "useful.h"

int main(int argc,char *argv[])
{
  TextureMap texmap;

  texmap.RiMakeTexture(argv[1],"texture.tx",RI_PERIODIC,RI_PERIODIC,RiGaussianFilter,1,1);
}

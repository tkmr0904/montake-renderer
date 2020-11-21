#include "../Header/rand.h"

/*************これは実装を定義するため以外に必要ないのでcppに書く*********************/
#include <random>
std::random_device dev_rnd;
std::mt19937 mt(dev_rnd());
std::uniform_real_distribution<> dist(0, 1);
/*****************************************************************************/

double rnd()
{
    return dist(mt);
}

void SampleDisk(double& x, double& y, double const& radius)
{
    double r = radius * sqrt(rnd());
    double phi = 2*M_PI*rnd();

    x = r*cos(phi);
    y = r*sin(phi);
}

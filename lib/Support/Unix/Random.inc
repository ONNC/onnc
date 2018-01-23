//===- Random.inc ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/MsgHandling.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

static FILE* g_RandDev = NULL;

//===----------------------------------------------------------------------===//
// Random
//===----------------------------------------------------------------------===//
onnc::Random::Random()
{
  if (NULL == g_RandDev) {
    g_RandDev = ::fopen("/dev/urandom", "rb");
    if (NULL == g_RandDev)
      fatal(fatal_open_random) << "/dev/urandom";
  }
}

onnc::Random::~Random()
{
  if (NULL != g_RandDev) {
    ::fclose(g_RandDev);
    g_RandDev = NULL;
  }
}

unsigned int onnc::Random::maximun()
{
  return RAND_MAX;
}

unsigned int onnc::Random::GetRandomNumberSeed() const
{
  unsigned int seed = 0x0;
  int count = ::fread((void *)&seed, sizeof(seed), 1, g_RandDev);

  // Return the seed if the read was successful.
  if (count == 1)
    return seed;

  return ::time(NULL);
}

onnc::Random::result_type onnc::Random::operator()() const
{
  static int x = (::srand(GetRandomNumberSeed()), 0);
  (void)x;
  return ::rand();
}

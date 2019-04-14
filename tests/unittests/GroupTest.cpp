//===- GroupTest.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/JSON/Group.h>
#include <onnc/JSON/Storage.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;
using namespace onnc::json;

//===----------------------------------------------------------------------===//
// GroupTest
//===----------------------------------------------------------------------===//
SKYPAT_F(GroupTest, iterator_test)
{
  Storage s;
  Group a(s.addGroup("a"));
  a.writeEntry("aa", 0);
  s.top().writeEntry("ea", "e1");
  Group b(s.addGroup("b"));
  b.writeEntry("bb", 1);
  s.top().writeEntry("eb", 0xb);
  Group c(s.addGroup("c"));
  c.writeEntry("cc", 2);

  for (auto gi = s.top().gBegin(); gi != s.top().gEnd(); gi.next()) {
    errs() << gi.group() << std::endl;
  }
}

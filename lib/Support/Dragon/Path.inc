//===- Path.inc -----------------------------------------------------------===//
//
//                               Skymizer
//
// Copyright (C) 2013, 2014, Skymizer Inc.. All rights reserved.
//
//===----------------------------------------------------------------------===//
#include <string>
#include <stack>

namespace skymizer {

const char Path::separator                   = '/';
const char Path::preferred_separator         = '/';
const Path::StringType Path::separator_str   = Path::StringType("/");
const Path::StringType Path::dylib_extension = Path::StringType("so");

namespace sys {

// return the last charactor being handled.
size_t canonicalize(std::string& pathname)
{
  // Variable Index //
  // SepTable - stack of result separators
  // LR(1) Algorithm //
  // traverse pPathName
  //   if we meet '//', '///', '////', ...
  //     -> ignore it
  //     -> push current into stack
  //     -> jump to the next not '/'
  //   if we meet '/./'
  //     -> ignore
  //     -> jump to the next not '/'
  //   if we meet '/../'
  //     -> pop previous position of '/' P
  //     -> erase P+1 to now
  //   if we meet other else
  //     -> go go go
  //   if we meet '/.../', '/..../', ... -> illegal
  if (pathname.empty())
    return 0;

  size_t handler = 0;
  std::stack<size_t> slash_stack;
  slash_stack.push(-1);
  while (handler < pathname.size()) {
    if (Path::separator == pathname[handler]) { // handler = 1st '/'
      size_t next = handler + 1;
      if (next >= pathname.size())
        return handler;
      switch(pathname[next]) { // next = handler + 1;
        case Path::separator: { // '//'
          while (next < pathname.size() && Path::separator == pathname[next])
            ++next;
          // next is the last not '/'
          pathname.erase(handler, next - handler - 1);
          // handler is the first '/'
          slash_stack.push(handler);
          break;
        }
        case '.': { // '/.'
          ++next; // next = handler + 2
          if (next >= pathname.size()) // '/.'
            return handler;
          switch (pathname[next]) {
            case Path::separator: { // '/./'
              pathname.erase(handler, 2);
              break;
            }
            case '.': { // '/..'
              ++next; // next = handler + 3;
              if (next >= pathname.size()) // '/..?'
                return handler;
              switch(pathname[next]) {
                case Path::separator: { // '/../'
                  handler = slash_stack.top();
                  slash_stack.pop();
                  pathname.erase(handler+1, next-handler);
                  if (static_cast<size_t>(-1) == handler) {
                    slash_stack.push(-1);
                    handler = pathname.find_first_of(Path::separator, handler);
                  }
                  break;
                }
                case '.': { // '/...', illegal
                  return handler;
                  break;
                }
                default : { // '/..a'
                  slash_stack.push(handler);
                  handler = pathname.find_first_of(Path::separator, handler+3);
                  break;
                }
              }
              break;
            }
            default : { // '/.a'
              slash_stack.push(handler);
              handler = pathname.find_first_of(Path::separator, handler+2);
              break;
            }
          }
          break;
        }
        default : { // '/a
          slash_stack.push(handler);
          handler = pathname.find_first_of(Path::separator, handler+1);
          break;
        }
      }
    }
    else {
      handler = pathname.find_first_of(Path::separator, handler);
    }
  }
  return handler;
}

bool is_separator(char value)
{
  return (value == Path::separator);
}

} // namespace of sys
} // namespace of skymizer


#include "ga_rc.h"
#include "quick_save.h"

#include <iostream>
#include <string>

void do_the_copy(vector<char> * s, vector<char> * h, vector<char> * c) {
  bool ih = true;
  bool ic = true;
  vector<string> rem(4);
  char ch1,ch2,ch3;
  unsigned long long term = s->size();
  unsigned long long i = 0;
  while (true) {
    if (i>=term) {
      return;
    }
    ch1 = s->at(i);
    if (term-i>2) {
      ch2 = s->at(i+1);
      ch3 = s->at(i+2);
      if ((ch1=='_') && (ch2)=='?') {
        if (ch3=='H') {
          ih = true;
          i += 3;
          continue;
        }
        if (ch3=='h') {
          ih = false;
          i += 3;
          continue;
        }
        if (ch3=='C') {
          ic = true;
          i += 3;
          continue;
        }
        if (ch3=='c') {
          ic = false;
          i += 3;
          continue;
        }
        if (ch3=='f') {
          h->push_back(';');
          ih = false;
          ic = true;
          i += 3;
          continue;
        }
        if (ch3=='F') {
          ih = true;
          i += 3;
          continue;
        }
        if (ch3=='r') {
          i += 3;
          ch1 = s->at(i);
          int ind = (int)(ch1-49);
          if ((ind<0) || (ind>3)) {
            cout << "variable index out of range" << endl;
            exit(0);
          }
          rem[ind].resize(0);
          i += 1;
          while (true) {
            ch1 = s->at(i);
            if (ch1==';') {
              break;
            }
            rem[ind].push_back(ch1);
            i += 1;
          }
          i += 1;
          continue;
        }
        if ((ch3>=49) && (ch3<=52)) {
          int ind = (int)(ch3-49);
          if ((ih) && (ch3>=51)) {
            for (int q=0;q<rem[ind].size();q++) {
              h->push_back(rem[ind][q]);
            }
          }
          if (ic) {
            for (int q=0;q<rem[ind].size();q++) {
              c->push_back(rem[ind][q]);
            }
          }
          i += 3;
          continue;
        }
        if (ch3=='!') {
          if (ih) {
            h->push_back('_');
            h->push_back('?');
          }
          if (ic) {
            c->push_back('_');
            c->push_back('?');
          }
          i += 3;
          continue;
        }
      }
    }
    // not a command sequence
    if (ih) {
      h->push_back(ch1);
    }
    if (ic) {
      c->push_back(ch1);
    }
    i += 1;
  }
}

int main(int argc, char * argv[]) {
  if (argc<4) {
    cout << "not enough arguments" << endl;
    cout << "ex: input.splitcies output.h output.cpp" << endl;
    exit(0);
  }
  gav<char> source = quick_save::readfile(argv[1]);
  vector<char> h;
  vector<char> c;
  do_the_copy(source.p(),&h,&c);
  quick_save::writefile(argv[2],&h);
  quick_save::writefile(argv[3],&c);
  return 0;
}

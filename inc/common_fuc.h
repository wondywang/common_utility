/*
* common func
*/

#ifndef foreach
#define foreach(container, it)\
  for(typeof((container).begin()) it=(container).begin(); it!=(container).end(); ++it)
#endif

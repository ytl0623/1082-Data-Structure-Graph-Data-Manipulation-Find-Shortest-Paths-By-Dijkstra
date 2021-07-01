// 10727130 黃意勛 10727124 劉宇廷

#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <vector>
#include <stack>
#include <queue>
#include <iomanip>
using namespace std ;

typedef vector<string> VeString ;

class DirectedGraph {

struct StudentPair {
  char sid1[10] ;
  char sid2[10] ;
  float weight ;
};

struct AdjListNode {
  string sid2 ;
  float weight ;
  int pos ;
  AdjListNode *next ;
};

struct AdjList {
  string sid1 ;
  AdjListNode *head = NULL ;
  int influenceValue ;
  vector<string> process ;
  vector<string> process2 ;
  //multimap<greater<string>,string> process2 ;
};

struct ForQ {
  string sid ;
  int pos ;
};

struct Form2 {
  string sid ;
  float weight ;
};

public:
string fileNum, fileName ;
fstream inFile, outFile ;
vector<AdjList> adjlist ;     // don't forget to clear.
vector<StudentPair> list ;     // don't forget to clear.
vector<VeString> outside ;      // don't forget to clear.
float realnum ;     // (0,1].
string findID ;
stack<Form2> path ;
Form2 tt ;

bool readFile() {
  list.clear() ;
  cout << "Input a real number in (0,1]: "  ;
  cin >> realnum ;

  while(true) {
    if( realnum > 1 || realnum == 0 ) {
      cout << "\n### It is NOT in (0,1] ###\n\n" ;
    }
    else {
      cout << "Input a file number ([0]Quit): " ;
      cin >> fileNum ;
      
      if( fileNum == "0" ) {
        cout << endl ;
        return false ;
      }
      else {
          fileName = "pairs" + fileNum + ".bin" ;
          inFile.open(  fileName.c_str(), fstream::in ) ;

          if( inFile.is_open() ) {
            cout << endl ;
            inFile.close() ;
            break ;
          }
          else {
            cout << "\n### " << fileName << " does not exist! ###\n\n" ;
            continue ;
          }
      }
    }
    cout << "Input a real number in (0,1]: "  ;
    cin >> realnum ;
  }

  StudentPair oneSt ;
  int stNo = 0 ;

  inFile.open( fileName.c_str(), fstream::in | fstream::binary ) ;
  if( inFile.is_open() ) {
    inFile.seekg( 0, inFile.end ) ;
    stNo = inFile.tellg() / sizeof(oneSt) ;
    inFile.seekg( 0, inFile.beg ) ;

    for( int i = 0 ; i < stNo ; i++ ) {
      inFile.read( (char*)&oneSt, sizeof(oneSt) ) ;
      list.push_back(oneSt) ;
    }
  }
  inFile.close() ;
  return true ;
}

void sortListNode() {
  for( int k = 0 ; k < adjlist.size() ; k++ ) {
    AdjListNode *tail = adjlist.at(k).head ;
    AdjListNode *temp = adjlist.at(k).head ;
    AdjListNode *cur = adjlist.at(k).head ;
    AdjListNode *pre = adjlist.at(k).head ;

    int size = 0 ;
    while( tail != NULL ) {
      tail = tail->next ;
      size++ ;
    }

    for( int i = size ; i > 0 ; i-- ) {
      cur = adjlist.at(k).head ;
      pre = adjlist.at(k).head ;

      for( int j = 0 ; j < i-1 && cur->next != NULL ; j++ ) {
        if( cur->sid2.compare( cur->next->sid2 ) > 0 ) {
          temp = cur->next ;
          cur->next = temp->next ;
          temp->next = cur ;

          if( cur == adjlist.at(k).head ) {
            adjlist.at(k).head = temp ;
            pre =temp ;
          }
          else {
            pre->next = temp ;
            pre = pre->next ;
          }
        }
        else {
          cur = cur->next ;
          if( j != 0 ) {
            pre = pre->next ;
          }
        }
      }
    }
  }
}

void sortListbyID() {
  int min ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    min = i ;

    for( int j = i+1 ; j < adjlist.size() ; j++ ) {
      if( adjlist.at(j).sid1.compare( adjlist.at(min).sid1 ) < 0 ) {
        min = j ;
      }
    }

    AdjList temp ;
    temp.sid1 = adjlist.at(i).sid1 ;
    temp.head = adjlist.at(i).head ;
    temp.process = adjlist.at(i).process ;
    temp.influenceValue = adjlist.at(i).influenceValue ;

    adjlist.at(i).sid1 = adjlist.at(min).sid1 ;
    adjlist.at(i).head = adjlist.at(min).head ;
    adjlist.at(i).process = adjlist.at(min).process ;
    adjlist.at(i).influenceValue = adjlist.at(min).influenceValue ;

    adjlist.at(min).sid1 = temp.sid1 ;
    adjlist.at(min).head = temp.head ;
    adjlist.at(min).process = temp.process ;
    adjlist.at(min).influenceValue = temp.influenceValue ;
  }
}

void m0() {
  adjlist.clear() ;
  bool found = false ;
  bool found2 = false ;
  string tempStr ;
  int i = 0, j = 0, k = 0 ;

  for( i = 0 ; i < list.size() ; i++ ) {     // list to adjlist.
    found = false ;
    tempStr.assign( list.at(i).sid1 ) ;

    for( j = 0 ; j < adjlist.size() && !found ; j++ ) {
      if( adjlist.at(j).sid1 == tempStr ) {
        found = true ;
        break ;
      }
    }

    found2 = false ;
    tempStr.assign( list.at(i).sid2 ) ;

    for( k = 0 ; k < adjlist.size() && !found2 ; k++ ) {
      if( adjlist.at(k).sid1 == tempStr ) {
        found2 = true ;
        break ;
      }
    }

    tempStr.assign( list.at(i).sid1 ) ;

    if( !found && list.at(i).weight <= realnum ) {
      AdjList tempAdjList ;
      tempAdjList.sid1 = tempStr ;
      tempAdjList.head = new AdjListNode ;
      tempStr.assign( list.at(i).sid2 ) ;
      tempAdjList.head->sid2 = tempStr ;
      tempAdjList.head->weight = list.at(i).weight ;
      tempAdjList.head->next = NULL ;
      adjlist.push_back( tempAdjList ) ;
    }
    else if( list.at(i).weight <= realnum ) {
      AdjListNode *walk = adjlist.at(j).head ;
      while( walk->next != NULL ){     // point to the last node.
        walk = walk->next ;
      }

      AdjListNode *tempNode = new AdjListNode ;
      tempStr.assign( list.at(i).sid2 ) ;
      tempNode->sid2 = tempStr ;
      tempNode->weight = list.at(i).weight ;
      tempNode->next = NULL ;
      walk->next = tempNode ;
    }

    tempStr.assign( list.at(i).sid2 ) ;

    if( !found2 && list.at(i).weight <= realnum ) {
      AdjList tempAdjList ;
      tempAdjList.sid1 = tempStr ;
      tempAdjList.head = new AdjListNode ;
      tempStr.assign( list.at(i).sid1 ) ;
      tempAdjList.head->sid2 = tempStr ;
      tempAdjList.head->weight = list.at(i).weight ;
      tempAdjList.head->next = NULL ;
      adjlist.push_back( tempAdjList ) ;
    }
    else if( list.at(i).weight <= realnum ) {
      AdjListNode *walk = adjlist.at(k).head ;
      while( walk->next != NULL ){     // point to the last node.
        walk = walk->next ;
      }

      AdjListNode *tempNode = new AdjListNode ;
      tempStr.assign( list.at(i).sid1 ) ;
      tempNode->sid2 = tempStr ;
      tempNode->weight = list.at(i).weight ;
      tempNode->next = NULL ;
      walk->next = tempNode ;
    }
  }
  
  sortListbyID() ;
  sortListNode() ;

  int count = 0 ;
  for( int i = 0 ; i < adjlist.size() ; i++ ) {     // count nodes.
    AdjListNode *walk = adjlist.at(i).head ;
    while( walk != NULL ) {     // point to the last node.
      walk = walk->next ;
      count++ ;
    }
  }

  char outRealnum[10] = {'\0'} ;
  sprintf( outRealnum, "%g", realnum ) ;
  string out ;
  out.assign(outRealnum) ;

  fileName = "pairs" + fileNum + "_" + out + ".adj" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if( outFile.is_open() ) {
    outFile << "<<< There are " << adjlist.size() << " IDs in total. >>>\n" ;
    cout << "<<< There are " << adjlist.size() << " IDs in total. >>>\n\n" ;

    for( i = 0 ; i < adjlist.size() ; i++ ) {
      outFile << "[" << setw(3) << i+1 << "] " << adjlist.at(i).sid1 << ": \n" ;

      AdjListNode *walk = adjlist.at(i).head ;
      for( j = 1 ; walk != NULL ; j++ ) {
        outFile << "\t(" << setw(2) << j << ") " << walk->sid2 << "," << setw(6) << walk->weight ;
        if( j % 10 == 0 ) {
          outFile << endl ;
        }
        walk = walk->next ;
      }
      outFile << endl ;
    }
    outFile << "<<< There are " << count << " nodes in total. >>>\n" ;
    cout << "<<< There are " << count << " nodes in total. >>>\n\n" ;
  }
  outFile.close() ;
}

void sortProcessbyID( vector<string> &process ) {
  int min ;

  for( int i = 0 ; i < process.size() ; i++ ) {
    min = i ;

    for( int j = i+1 ; j < process.size() ; j++ ) {
      if( process.at(j).compare( process.at(min) ) < 0 ) {
        min = j ;
      }
    }

    string temp ;
    temp = process.at(i) ;
    process.at(i) = process.at(min) ;
    process.at(min) = temp ;
  }
}

void setPos() {
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    AdjListNode *walk = adjlist.at(i).head ;
    
    while( walk != NULL ) {
      for( int i = 0 ; i < adjlist.size() ; i++ ) {
          if( walk->sid2 == adjlist.at(i).sid1 )
            walk->pos = i ;
      }
      walk = walk->next ;
    }
  }
}

void countStack() {
  stack<ForQ> s ;
  VeString x ;
  ForQ temp ;
  bool visited[adjlist.size()] ;
  bool storeINoutside = false ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {     // initialize visited = false.
    visited[i] = false ;
  }


  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    //cout << "i: " << i << endl ;
    //AdjListNode *walk = adjlist.at( i ).head ;
    temp.sid = adjlist.at(i).sid1 ;
    temp.pos = i ;
    s.push( temp ) ;
    
    if( !visited[i] ) {
      storeINoutside = true ;
      x.clear() ;
      x.push_back( adjlist.at(i).sid1 ) ;
      //cout << "---------------------------" << endl ;
    }
    visited[i] = true ;
    //ccount++ ;
    while( s.size() != 0 ) {
      int pos = s.top().pos ;
      //cout << "TOP " << s.top().sid << endl ;
      AdjListNode *walk = adjlist.at(pos).head ;
      //s.pop() ;
      while( walk != NULL && visited[walk->pos] ) {
        //cout << "asdasdsa" << walk->sid2 << endl ;
        walk = walk->next ;
      }

      if( walk != NULL) {
        //while( walk != NULL ) {
        pos = walk->pos ;
        //cout << pos << endl ;
        while( !visited[pos] ) {
          //adjlist.at(i).process.push_back(  adjlist.at(pos).sid1 ) ;
          x.push_back( adjlist.at(pos).sid1 ) ;
          //cout << "was not " << adjlist.at(pos).sid1 <<  endl ;
          visited[pos] = true ;

          temp.sid = adjlist.at(pos).sid1 ;
          temp.pos = pos ;
          s.push( temp ) ;

          if( adjlist.at(pos).head != NULL ) {
            pos = adjlist.at(pos).head->pos ;
            temp.sid = adjlist.at(pos).sid1 ;
            temp.pos = pos ;
          }
          //ccount++ ;
        }
        //if ( ccount == adjlist.size() )
          //break ;
      //}
      }
      else {
        s.pop() ;
      }
    }
    //adjlist.at(i).influenceValue = adjlist.at(i).process.size() ;
    //sortProcessbyID( x ) ;
    if( storeINoutside == true ) {
      outside.push_back(x) ;
      //for ( int i = 0 ; i < x.size() ; i++ ) {
        //cout << x.at(i) << endl ;
    //}
      storeINoutside = false ;
    }
  }
}

void sortListByValue() {
  int priority ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    priority = i ;

    for( int j = i+1 ; j < adjlist.size() ; j++ ) {
      if( adjlist.at(j).influenceValue > adjlist.at(priority).influenceValue ) {
        priority = j ;
      }
      else if( adjlist.at(j).influenceValue == adjlist.at(priority).influenceValue ) {
        if( adjlist.at(j).sid1 <  adjlist.at(priority).sid1 ) {
          priority = j ;
        }
      }
    }

    AdjList temp ;
    temp.sid1 = adjlist.at(i).sid1 ;
    temp.head = adjlist.at(i).head ;
    temp.process = adjlist.at(i).process ;
    temp.process2 = adjlist.at(i).process2 ;
    temp.influenceValue = adjlist.at(i).influenceValue ;


    adjlist.at(i).sid1 = adjlist.at(priority).sid1 ;
    adjlist.at(i).head = adjlist.at(priority).head ;
    adjlist.at(i).process = adjlist.at(priority).process ;
    adjlist.at(i).process2 = adjlist.at(priority).process2 ;
    adjlist.at(i).influenceValue = adjlist.at(priority).influenceValue ;

    adjlist.at(priority).sid1 = temp.sid1 ;
    adjlist.at(priority).head = temp.head ;
    adjlist.at(priority).process = temp.process ;
    adjlist.at(priority).process2 = temp.process2 ;
    adjlist.at(priority).influenceValue = temp.influenceValue ;
  }
}

void sortOutside() {
  int priority ;

  for( int i = 0 ; i < outside.size() ; i++ ) {
    priority = i ;

    for( int j = i+1 ; j < outside.size() ; j++ ) {
      if( outside.at(j).size() > outside.at(priority).size() ) {
        priority = j ;
      }
      else if( outside.at(j).size() == outside.at(priority).size()) {

        if( outside.at(j).at(0) > outside.at(priority).at(0) ) {
          priority = j ;
        }
      }
    }

    VeString temp ;
    temp = outside.at(i) ;
    outside.at(i) = outside.at(priority) ;
    outside.at(priority) = temp ;
  }
}

void m1() {
  outside.clear()  ;
  setPos() ;
  countStack() ;
  sortListByValue() ;
  sortOutside() ;

  for( int i = 0 ; i < outside.size() ; i++ ) {     // sort outside.string
    sortProcessbyID(outside.at(i)) ;
  }

  char outRealnum[10] = {'\0'};
  sprintf(outRealnum, "%g", realnum ) ;
  string out ;
  out.assign(outRealnum) ;

  fileName = "pairs" + fileNum + "_" + out + ".cc" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if( outFile.is_open() ) {
    outFile << "<<< There are " << outside.size() << " connected components in total. >>>\n" ;
    cout << "\n<<< There are " << outside.size() << " connected components in total.  >>>\n" ;

    for( int i = 0 ; i < outside.size() ; i++ ) {
      outFile << "{" << setw(2) << i+1 << "}" << " Connected Component: size = " << outside.at(i).size() << "\n" ;
      cout << "{" << setw(2) << i+1 << "}" << " Connected Component: size = " << outside.at(i).size() << "\n" ;

      for( int j = 0 ; j < outside.at(i).size() ; j++ ) {
        outFile << " \t(" << setw(3) << j+1 << ") " << outside.at(i).at(j) ;

        if( (j+1) % 8 == 0 ) {
          outFile << endl ;
        }
      }
      outFile << endl ;
    }
    cout << endl ;
  }
  outFile.close() ;
}

void m2() {
  cout << endl ;
  for( int i = 0 ; i < adjlist.size() ; i++ ) {     // print allID
    cout << "  " ;
    cout  << adjlist.at(i).sid1 ;
    if ( (i+1) % 8 == 0 )
      cout << endl ;
  }
  cout << endl ;
  cout << "Input a student ID [0: exit]: " ;
  cin >> findID ;

  while ( findID != "0" ) {
    //vector<int> forWeight ;
    float smallest ;
    int pos = 0 ;
    int pos2 = 0, mark = 0 ;
    string temp ;
    bool check = false ;

    for( int i = 0 ; i < outside.size() ; i++ ) {     // mark pos of findID.
      for( int j = 0 ; j < outside.at(i).size() ; j++ ) {
        if ( outside.at(i).at(j) == findID ) {
          mark = i ;
          check = true ;
        }
      }
    }

    if ( !check ) {
      cout << "\n### the student id does not exist! ###\n" << endl ;
    }
    else {
      Form2 ans[outside.at(mark).size()] ;
      bool visited[outside.at(mark).size()] ;

      for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {     // 權重歸零和定位
        ans[i].weight = 10 ;
        visited[i] = false ;
        ans[i].sid = outside.at(mark).at(i) ;
      }

      for( int i = 0 ; i < adjlist.size() ; i++ ) {     // 尋找finID在adjlist的位置
        if( adjlist.at(i).sid1 == findID ) {
          pos = i ;
          break ;
        }
      }

      AdjListNode *walk = adjlist.at(pos).head ;
      while( walk != NULL ) {     // 找出權重
        for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {
          if( outside.at(mark).at(i) == walk->sid2 ) {
            ans[i].weight = walk->weight ;
          }
        }
        walk = walk->next ;
      }

      smallest = 10 ;
      for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {     // find smallest weight
        if ( smallest > ans[i].weight ) {
          smallest = ans[i].weight ;
          pos2 = i ;
        }
      }

      visited[pos2] = true ;
      tt.sid = ans[pos2].sid ;
      tt.weight = ans[pos2].weight ;
      path.push(tt) ;

      //--------------------------------------------------------------------------------
      while( !path.empty() ) {
        temp = path.top().sid ;

        for( int i = 0 ; i < adjlist.size() ; i++ ) {     // 尋找temp在adjlist的位置
          if( adjlist.at(i).sid1 == temp ) {
            pos = i ;
            break ;
          }
        }

        AdjListNode *walk = adjlist.at(pos).head ;
        while( walk != NULL ) {     // 找出權重
          for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {
            if ( outside.at(mark).at(i) == walk->sid2 ) {
              if ( ans[i].weight > (walk->weight + path.top().weight) ) {
                ans[i].weight = walk->weight + path.top().weight ;
              }
            }
          }
          walk = walk->next ;
        }

        smallest = 10 ;
        for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {     // find smallest weight
          if( smallest > ans[i].weight && !visited[i] ) {
            smallest = ans[i].weight ;
            pos2 = i ;
          }
        }

        path.pop() ;
        tt.sid = ans[pos2].sid ;
        tt.weight = ans[pos2].weight ;
        if ( !visited[pos2] ) {
          path.push( tt ) ;
        }
        visited[pos2] = true ;
      }

      // --------------------------------------------------------sort
      int priority ;
      for( int i = 0 ; i < outside.at(mark).size() ; i++ ) {
        priority = i ;

        for( int j = i+1 ; j < outside.at(mark).size() ; j++ ) {
          if( ans[j].weight < ans[priority].weight ) {
            priority = j ;
          }
        }

        Form2 temp ;
        temp.sid = ans[i].sid ;
        temp.weight = ans[i].weight ;
        ans[i] = ans[priority] ;
        ans[priority] = temp ;
      }

      //--------------------------------------------------------write
      char outRealnum[10] = {'\0'};
      sprintf(outRealnum, "%g", realnum ) ;
      string out ;
      out.assign(outRealnum) ;

      fileName = "pairs" + fileNum + "_" + out + ".ds" ;
      outFile.open( fileName.c_str(), fstream::app ) ;

      if( outFile.is_open() ) {
        outFile <<  "\norigin: " << findID << "\n" ;
        int H = 0 ;

        for( int j = 0 ; j < outside.at(mark).size() ; j++ ) {
          if (ans[j].sid != findID ) {
            outFile << "(" << setw(2) << H+1 << ") " << "\t" << ans[j].sid << ", " << ans[j].weight << "\t" ;

            if( (H+1) % 8 == 0 ) {
              outFile << endl ;
            }
            H++ ;
          }
        }
        outFile << endl ;
      }
      outFile.close() ;
    }

    //---------------------------------------------------------print
    for ( int i = 0 ; i < adjlist.size() ; i++ ) {     // print allID
      cout << "  " ;
      cout  << adjlist.at(i).sid1 ;

      if ( (i + 1) % 8 == 0 ) {
        cout << endl ;
      }
    }

    cout << endl ;
    cout << "Input a student ID [0: exit]: " ;
    cin >> findID ;
  }
  cout << endl ;
}

};

int main() {
  string choice ;
  DirectedGraph work ;

  cout << "****** Graph data manipulation *******\n"
       << "* [Any other key: QUIT]              *\n"
       << "* 0. Create adjacency lists          *\n"
       << "* 1. Build connected components      *\n"
       << "* 2. Find shortest paths by Dijkstra *\n"
       << "**************************************\n"
       << "Input a choice(0, 1, 2) [Any other key: QUIT]: " ;
  cin >> choice ;

  while( choice == "0" || choice == "1" || choice == "2" ) {
    if( choice == "0" ) {
      if( work.readFile() ) {
        work.m0() ;
      }
    }
    else if( choice == "1" ) {
      if( work.list.empty() ) {
        cout << "\n### There is no graph and choose 0 first. ###\n\n" ;
      }
      else {
        work.m1() ;
      }
    }
    else if( choice == "2" ) {
      if( work.list.empty() ) {
        cout << "\n### There is no graph and choose 0 first. ###\n\n" ;
      }
      else if( work.outside.empty() )
        cout << "\n### Choose 1 to find connected components. ###\n\n" ;
      else {
        work.m2() ;
      }
  }
    else {
      cout << "\nCommand does not exit!\n\n" ;
    }

  cout << "****** Graph data manipulation *******\n"
       << "* [Any other key: QUIT]              *\n"
       << "* 0. Create adjacency lists          *\n"
       << "* 1. Build connected components      *\n"
       << "* 2. Find shortest paths by Dijkstra *\n"
       << "**************************************\n"
       << "Input a choice(0, 1, 2) [Any other key: QUIT]: " ;
  cin >> choice ;
  }
}

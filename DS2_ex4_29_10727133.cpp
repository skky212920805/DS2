// 第29組 10727133 鄧陳得 
#include <cstdlib>
#include<stdio.h>
#include<iostream>
#include<fstream> 
#include<vector> 
#include <cmath>
#include <stdlib.h>
#include<string >
#include <string.h> 
#include <cstdio>
#include <time.h>
#include <queue>
using namespace std;

struct aLN{
  string rid ;  // receiver     
  float weight ; // 權重 
  aLN *next = NULL ;
} AdjListNode ;

typedef aLN * ANode ;

struct sP {
  char sid[10] ; // sender
  char rid[10] ; // receiver
  float weight ; // 權重 
} StudentPair ;

struct aL{
  string sid;	
  ANode head = NULL ;
  int inf  = 0 ;
  bool visit = false;  
} adjList ;

class Graph {
	public:
        string fileNum, fileName ;
        fstream inFile, outFile ;	
        vector<aL> list;
        vector<aL> connect;
        int nodes = 0; 
	void readBin() {
        do {
            cout << "Input a file number([0]Quit): " ;
            cin >> fileNum ;

            if( !fileNum.compare("0") ) {
                cout << endl ;
                return;
            } // if
            else {
                fileName = "pairs" + fileNum + ".bin" ;
                inFile.open(  fileName.c_str(), fstream::in ) ;
                list.clear();
                connect.clear();
                nodes = 0;
                if( inFile.is_open() ) {
                    cout << endl ;
                    inFile.close() ;
                    break ;
                } // if
                else {
                    cout << "\n### " << fileName << " does not exist! ###\n\n" ;
                } // else
            } // else
        } while(true) ;

    sP oneSt ;
    int stNo = 0 ;

    inFile.open( fileName.c_str(), fstream::in | fstream::binary ) ;

    if( inFile.is_open() ) {
        inFile.seekg( 0, inFile.end ) ;     // 0~eof.
        stNo = inFile.tellg() / sizeof(oneSt) ;
        inFile.seekg( 0, inFile.beg ) ;

        for( int i = 0 ; i < stNo ; i++ ) {
           inFile.read( (char*)&oneSt, sizeof(oneSt) ) ;
           AdjList( oneSt );
        } // for
    } // if

    inFile.close() ;

    WriteData( list, 1 )  ;
  } // readBin()
  
  void AdjList( sP oneSt ){
	aL frontId, lastId;
	string tempRid = oneSt.rid;
	frontId.sid = oneSt.sid;
    PutInMain(  frontId, tempRid, oneSt.weight, 1 );   
	lastId.sid = oneSt.rid ;    
    PutInMain(  lastId, tempRid, oneSt.weight, 0 );    
  }  // AdjList()
  
  void PutInMain( aL sid, string rid, float weight, int times ) {
  	  int i = 0 ;
  	  for( i = 0; i < list.size() ; i++  ) {
		if( list[i].sid ==  sid.sid  ){
			if( times == 1 )//加收訊者學號 
			  LinkRid( rid, weight,  list[i].head, 1, "" );
			return;
	    }
		else if(  list[i].sid > sid.sid ){
			if( times == 1 )//加收訊者學號 
			  LinkRid( rid, weight,  sid.head, 1, "" );				
			list.insert( list.begin()+i, sid  );
		
			return;			
		}
			
		
	}
	list.push_back(sid);
	if( times == 1 )//加收訊者學號 
		LinkRid( rid, weight,  list[i].head, 1, "" );
  } // PutInMain()
  
  bool LinkRid( string rid, float weight, ANode &temp, int times, string sid ){
	bool run = true;
	string temp1 = rid;
	float temp2 = weight;
	if( temp == NULL ){
		temp = new aLN;
		temp->rid = rid;
		temp->weight = weight;
		temp->next = NULL;
		nodes++;
	}
	else if( ( temp->rid == rid || sid == rid ) && times == 2 )
	  return false;	
	else if( temp->rid > rid ){
		rid = temp->rid;
		weight = temp->weight;
		temp->rid = temp1;
		temp->weight = temp2;
		run = LinkRid( rid, weight, temp->next, times, sid  );
	}
	else
		run = LinkRid( rid, weight, temp->next, times, sid  );
	
	return run;		
  } // LinkRid()
  
  void Connection() {
  	nodes = 0 ;
  	connect.clear() ;
  	if ( list.size() == 0 ) {
  		cout << endl << " There is no graph and choose 1 first." << endl ;
  		return ;
	} // if
	for( int i = 0; i < list.size() ; i++ ){
	  aL sid ;
	  list[i].visit = true;
	  sid.sid = list[i].sid;
	  PutInRelated( sid, list[i].head );	
	  connect.push_back( sid ) ;
	  Initial(); 
	} // for
	WriteData( connect, 2 )  ;
	
	
  } // Connection()
  
  void PutInRelated( aL &sid, ANode head ) {
  	int n =0, j =0, s = 0;
    queue<int> q;
    ANode walk;
	while( head != NULL  ) {
		n = SearchId( head->rid, sid );  
	    q.push(n);
		head = head->next; 		 
	} // while

    while( q.size() != 0  ){
  	  j = q.front();
  	  q.pop();
  	  walk = list[j].head;
  	  s = 0;
  	  while(  walk != NULL ){
        n = SearchId( walk->rid, sid );
        if( n != -1 )
	      q.push(n);  	
           	  	
  	  	walk = walk->next;
	  } // while
	
    } // while  	
 		

  } // PutInRelated()
  
  int SearchId( string rid, aL &sid ) {
  	for( int i = 0; i < list.size(); i++ ) {
      if(  list[i].sid == rid ) {
      	if(  !list[i].visit  ) {
      	  list[i].visit = true;
		  LinkRid( list[i].sid, 0.0, sid.head, 1, "" ) ;
		  sid.inf++;  
		  return i;    	     		
		} // if
      	return  -1;
	  } // if
    } // for
  } // SearchId()
  
  void Initial(){
	for( int i = 0; i < list.size() ; i++ ){
      list[i].visit = false;
    } // for
  } // Initial()
  
  void WriteData( vector<aL> list, int mission ) {
  	if( mission == 1 )	
       fileName = "pairs" + fileNum + ".adj" ;
    else if ( mission == 2 ) 
       fileName = "pairs" + fileNum + ".cnt" ;
	else if ( mission == 3 )
	   fileName = "pairs" + fileNum + ".inf" ;  
    outFile.open( fileName.c_str(), fstream::out ) ;

    if( outFile.is_open() ) {	
        cout << "<<< There are " << list.size() << " IDs in total. >>>" << endl ;
        outFile << "<<< There are " << list.size() << " IDs in total. >>>\n" ;
        for( int i = 0 ; i < list.size() ; i++ ) {
          if( mission == 1 )
            outFile << "[" << i+1 << "] " << list[i].sid << ": \n" ;
          else
            outFile << "[" << i+1 << "] " << list[i].sid << "(" << list[i].inf << ")" << ": \n" ;
          ANode find = list[i].head ;
          for( int j = 1 ; find != NULL ; j++ ) {
            outFile << "\t("  << j << ") " << find->rid ;
            if( mission == 1 )
              outFile << ",\t"  << find->weight ;
            if( j % 10 == 0 ) {
              outFile << endl ;
            }  // if
            find = find->next ;
          } // for
          outFile << endl ;
        } // for
    
        if( mission == 1 )
          cout << "<<< There are " << nodes << " nodes in total. >>>" << endl ;
          outFile << "<<< There are " << nodes << " nodes in total. >>>\n" ;
    } // if
    outFile.close() ;	
	
  } // WriteData()
}; // Graph


int main() {
	int command = 0;
	string fileName;
	Graph graph ;
	do {
	    cout << endl << "****Graph data manipulation*****";
	    cout << endl << "* 0. QUIT                      *";
	    cout << endl << "* 1. Build adjacency lists     *";
	    cout << endl << "* 2. Compute connection counts *";
	    cout << endl << "********************************";
	    cout << endl << "Input a choice(0, 1, 2): "; 
	    cin >> command;
	    
	    if ( command == 0 ) {
		    break ;
	    } // if
	    else if ( command == 1 ) {
	      graph.readBin() ;
		} // else if
		else if ( command == 2 ) {
	      graph.Connection() ;
		} // else if
    } while ( command != 0 ); 
} // main()

// main.cpp
// Siegfried Nijssen, snijssen@liacs.nl, jan 2004.
#include <iostream>
#include <fstream>
#include "database.h"
#include "path.h"
#include "misc.h"
#include "graphstate.h"
#include <time.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

Frequency minfreq = 1;
Database database;
Statistics statistics;
bool dooutput = false;
int phase = 3;
int maxsize = ( 1 << ( sizeof(NodeId)*8 ) ) - 1; // safe default for the largest allowed pattern
FILE *output;

void Statistics::print () {
  int total = 0, total2 = 0, total3 = 0;
  for ( int i = 0; i < frequenttreenumbers.size (); i++ ) {
    cout << "Frequent " << i + 2
         << " cyclic graphs: " << frequentgraphnumbers[i]
         << " real trees: " << frequenttreenumbers[i]
         << " paths: " << frequentpathnumbers[i]
         << " total: " << frequentgraphnumbers[i] + frequenttreenumbers[i] + frequentpathnumbers[i] << endl;
    total += frequentgraphnumbers[i];
    total2 += frequenttreenumbers[i];
    total3 += frequentpathnumbers[i];
  }
  cout << "TOTAL:" << endl
       << "Frequent cyclic graphs: " << total << " real trees: " << total2 << " paths: " << total3 << " total: " << total + total2 + total3 << endl;
}

void puti ( FILE *f, int i ) {
  char array[100];
  int k = 0;
  do {
    array[k] = ( i % 10 ) + '0';
    i /= 10;
    k++;
  }
  while ( i != 0 );
  do {
    k--;
    putc ( array[k], f );
  } while ( k );
}

main ( int argc, char *argv[] ) {
  cout << "debug 0"<<endl;
  clock_t t1 = clock ();
  //cerr << "GASTON GrAph, Sequences and Tree ExtractiON algorithm" << endl;
  //cerr << "Version 1.0 with Occurrence Lists" << endl;
  //cerr << "Siegfried Nijssen, LIACS, 2004" << endl;

  cout << "debug 1"<<endl;
  int temp0 = 0;
  char opt;
  while ( ( opt = getopt ( argc, argv, "m:tp" ) ) != -1 ) {
    cout << "debug 2: "<< ++temp0<<endl;
    switch ( opt ) {
      case 'm': maxsize = atoi ( optarg ) - 1; break;
      case 't': phase = 2; break;
      case 'p': phase = 1; break;
    }
  }
  cout << "debug 3"<<endl;
  if ( argc - optind < 2 || argc - optind > 3 ) {
    cerr << "Parameters: [-m size] [-p] [-t] support input [output]" << endl;
    return 1;
  }

  char* datafile="dataset\\database_size10_v5_vMin4_vMax5_seed3571_win.txt";
  char* out_file_name = "dataset\\database_size10_v5_vMin4_vMax5_seed3571.txt.GASTON.output";
//  char* datafile="Chemical_340.txt";
//  char* out_file_name = "Chemical_340.txt.GASTON.output";

  //minfreq = atoi ( argv[optind] );
  minfreq = 4;
  cerr << "Read" << endl;
  //FILE *input = fopen ( argv[optind+1], "r" );
  FILE *input = fopen (datafile, "r" );
  if ( argc - optind == 3 ) {
    dooutput = true;
    //output = fopen ( argv[optind+2], "w" );
    output = fopen ( out_file_name, "w" );
  }

  cout << "debug 4"<<endl;

  cout << "input = " << input <<endl;
  database.read ( input );
  fclose ( input );

  cout << "debug 5"<<endl;

  cerr << "Edgecount" << endl;
  database.edgecount ();
  cerr << "Reorder" << endl;

  cout << "debug 6"<<endl;

  database.reorder ();

  cout << "debug 7"<<endl;

  initLegStatics ();
  graphstate.init ();
  for ( int i = 0; i < database.nodelabels.size (); i++ ) {
    if ( database.nodelabels[i].frequency >= minfreq &&
         database.nodelabels[i].frequentedgelabels.size () ) {
      Path path ( i );
      path.expand ();
    }
  }

  cout << "debug 8"<<endl;

  clock_t t2 = clock ();

  statistics.print ();
  cout << "Approximate total runtime: " << ( (float) t2 - t1 ) / CLOCKS_PER_SEC << "s" << endl;
  if ( argc - optind == 3 )
    fclose ( output );
  getchar();
}

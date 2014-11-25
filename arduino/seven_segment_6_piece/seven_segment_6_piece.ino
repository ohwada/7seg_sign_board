/*
 * seven segment 6 piece
 * 2014-11-01 K.OHWADA
 */
 
// command
// (1) set text
// example #abc$
// start #
// end $
// text : 0-9 a-z A-Z SAPCE

#include <EEPROM.h>

#define SERIAL_SPEED  9600 
#define SEG_NUM  8
#define ROW_NUM  6
#define DELAY_SEG  3
#define DELAY_TEST  500
#define MAX_LOOP  200
#define SPACE  ' '
#define MAX_TEXT  100

// 7 Seg       A B C D E F G p
// Ardunino 6 5 4 3 2 8 9 7
const int CATHODE[SEG_NUM] = {6,5,4,3,2,8,9,7};
//const int ANODE[ROW_NUM] = {10,11,12,13, A0, A1};
const int ANODE[ROW_NUM] = {A1,A0,13,12,11,10};

// test pattern
char SEG[][SEG_NUM] = {
	{1,0,0,0, 0,0,0,0},	// A
	{0,1,0,0, 0,0,0,0},	// B
	{0,0,1,0, 0,0,0,0},	// C
	{0,0,0,1, 0,0,0,0},	// D
	{0,0,0,0, 1,0,0,0},	// E
	{0,0,0,0, 0,1,0,0},	// F
	{0,0,0,0, 0,0,1,0},	// G
	{0,0,0,0, 0,0,0,1},	// DP
};
							
// 7 Seg Font
// http://ja.wikipedia.org/wiki/7%E3%82%BB%E3%82%B0%E3%83%A1%E3%83%B3%E3%83%88%E3%83%87%E3%82%A3%E3%82%B9%E3%83%97%E3%83%AC%E3%82%A4
char PAT[][SEG_NUM] = {	
	{1,1,1,1, 1,1,0,0},	// 0
	{0,1,1,0, 0,0,0,0},	// 1
	{1,1,0,1, 1,0,1,0},	// 2
	{1,1,1,1, 0,0,1,0},	// 3
	{0,1,1,0, 0,1,1,0},	// 4	
	{1,0,1,1, 0,1,1,0},	// 5	
	{1,0,1,1, 1,1,1,0},	// 6
	{1,1,1,0, 0,1,0,0},	// 7				
	{1,1,1,1, 1,1,1,0},	// 8		
	{1,1,1,1, 0,1,1,0},	// 9			

	{1,1,1,0, 1,1,1,0},	// A (10)
	{0,0,1,1, 1,1,1,0},	// b					
	{1,0,0,1, 1,1,0,0},	// C	
	{0,1,1,1, 1,0,1,0},	// d	
	{1,0,0,1, 1,1,1,0},	// E		
	{1,0,0,0, 1,1,1,0},	// F
	{1,0,1,1, 1,1,0,0},	// G
	{0,0,1,0, 1,1,1,0},	// h
	{0,0,1,0, 0,0,0,0},	// I
	{0,1,1,1, 1,0,0,0},	// J
	{0,1,0,1, 1,1,1,0},	// K (20)
	{0,0,0,1, 1,1,0,0},	// L
	{1,1,1,0, 1,1,0,0},	// M
	{0,0,1,0, 1,0,1,0},	// n
	{0,0,1,1, 1,0,1,0},	// o
	{1,1,0,0, 1,1,1,0},	// P
	{1,1,0,1, 1,1,1,0},	// Q
	{0,0,0,0, 1,0,1,0},	// r
	{0,0,1,1, 0,1,1,0},	// S
	{0,0,0,1, 1,1,1,0},	// t
	{0,0,1,1, 1,0,0,0},	// u (30)
	{0,1,1,1, 1,1,0,0},	// V
	{0,1,1,1, 1,1,1,0},	// W
	{0,1,1,0, 1,1,1,0},	// X
	{0,1,1,1, 0,1,1,0},	// y
	{1,1,0,1, 1,0,0,0},	// Z (35)

	{0,0,0,0, 0,0,0,0},	// space (36)							
	{1,1,1,1, 1,0,1,0},	// a											
	{0,0,0,1, 1,0,1,0},	// c
	{0,1,1,1, 0,0,0,0},	// j
	{1,1,1,0, 0,1,1,0},	// q (40)
	{0,0,0,0, 0,0,0,1},	// DP				
};

#define INDEX_SAPCE  36

// hash ascii to index of 7 Seg Font
const char HASH_LENGTH = 63;
const char HASH[][2] = {
	{'0', 0},
	{'1', 1},
	{'2', 2},
	{'3', 3},
	{'4', 4},
	{'5', 5},
	{'6', 6},
	{'7', 7},
	{'8', 8},
	{'9', 9},

	{'A', 10},
	{'B', 11},
	{'C', 12},
	{'D', 13},
	{'E', 14},
	{'F', 15},
	{'G', 16},
	{'H', 17},
	{'I', 18},
	{'J', 19},
	{'K', 20},
	{'L', 21},
	{'M', 22},
	{'N', 23},
	{'O', 24},
	{'P', 25},
	{'Q', 26},
	{'R', 27},
	{'S', 28},
	{'T', 29},
	{'U', 30},
	{'V', 31},
	{'W', 32},
	{'X', 33},
	{'Y', 34},
	{'Z', 35},

	{'a', 37},
	{'b', 11},
	{'c', 38},
	{'d', 13},
	{'e', 14},
	{'f', 15},
	{'g', 16},
	{'h', 17},
	{'i', 18},
	{'j', 39},
	{'k', 20},
	{'l', 21},
	{'m', 22},
	{'n', 23},
	{'o', 24},
	{'p', 25},
	{'q', 40},
	{'r', 27},
	{'s', 28},
	{'t', 29},
	{'u', 30},
	{'v', 31},
	{'w', 32},
	{'x', 33},
	{'y', 34},
	{'z', 35},
	
	{SPACE, INDEX_SAPCE}
};

// default text
char TEXT_DEFAULT[] ="FabLab ";
int TEXT_LENGTH_DEFAULT = 7;

// text buffer
char text_buf[ MAX_TEXT ];
int text_index = 0;

// pattern index buffer	
int pat_index[ MAX_TEXT ];
int pat_length = 0;

int index_offset = 0;
int text_cnt = 0;
int loop_cnt = 0;

// serial status
boolean is_reading = false;

/**
 * === setup ===
 */ 
void setup() {  
	Serial.begin( SERIAL_SPEED );
	Serial.println( "7 Seg x 6" );
	int i;
	// set output all cathode ports 
	for ( i=0; i<SEG_NUM; i++ ) {
		pinMode( CATHODE[i], OUTPUT ); 
	}
	// set output all anode ports 
	for ( i=0; i<ROW_NUM; i++ ) {
		pinMode( ANODE[i], OUTPUT );  
	}
	reset_anode( -1 );
	read_eeprom();	
	if ( text_index == 0 ) {
		// set default, if eeprom is not written
		convert_index( TEXT_DEFAULT, TEXT_LENGTH_DEFAULT );		
	} else {
		// set from buffer, if eeprom is written
		convert_index( text_buf, text_index );
	}			  	  	  	  	  	  	  	  	  	  	  	  	  	  	             
}

/**
 * === loop ===
 */ 
void loop() {
	loop_serial();
	loop_one_char();
}

/**
 * Display one character at a time
 */ 
void loop_one_char() {
	int i,j,n;
	int index = 0;
	// trun on one 7seg at a time
	for ( i=0; i<ROW_NUM; i++ ) {
		reset_anode( i );
		if (( text_cnt >= 7 )||( text_cnt == i )) {
  			n = pat_index[ i ];
  		} else {
  			n = INDEX_SAPCE;	
  		}
		turn_on_7seg( PAT[ n ] );
		delay( DELAY_SEG );
		loop_cnt ++;
	}
	// 0.6 sec elapse
	if ( loop_cnt > MAX_LOOP ) {
		loop_cnt = 0;
		text_cnt ++;
		if ( text_cnt > 9) {
			text_cnt = 0;
		}			
	}
}

/**
 * scroll one character
 */	
void loop_scroll() {	 
	int i,j,n;
	int index =0;
	// trun on one 7seg at a time
	for ( i=0; i<ROW_NUM; i++ ) {
		reset_anode( i );
		index = i + index_offset;
		if ( index >=  pat_length ) {
			index -= pat_length;
		}
  		n = pat_index[ index ];
		turn_on_7seg( PAT[ n ] );
		delay( DELAY_SEG );
		loop_cnt ++;
	}
	// 0.6 sec elapse
	if ( loop_cnt > MAX_LOOP ) {
		loop_cnt = 0;
		index_offset ++;
		if ( index_offset > pat_length ) {
			index_offset = 0;
		}	
	}		
}

/**
 * receive a command from the serial ports
 */	
void loop_serial() {
	// if char recieve
	if ( !Serial.available() ) return;
	char c = Serial.read();
	boolean flag = false;
	// # start
    if ( c == '#' ) {
    	Serial.print( c );
    	is_reading = true;
    	text_index = 0;
    } 
    // $ end
    if ( c == '$'  ) { 
		Serial.println( c );
    	write_eeprom( text_buf, text_index );
		convert_index( text_buf, text_index );	 
	}
	// other chars
	if ( c >= '0' && c <= '9' ) { flag = true; }
	if ( c >= 'a' && c <= 'z' ) { flag = true; }
	if ( c >= 'A' && c <= 'Z' ) { flag = true; }
	if ( c == SPACE  ) { flag = true; }
	// if valid char
	if ( flag ) {
 		Serial.print( c );
		text_buf[ text_index ] = c;
		text_index ++;
		if ( text_index > MAX_TEXT ) {
			is_reading = false;
		}	
    }
}	 

/**
 * test segment one by one
 */
void loop_test() {
	int i,j;
	// trun on one 7seg at a time
	for ( i=0; i<ROW_NUM; i++ ) {
		reset_anode( i );
		for ( j=0; j<SEG_NUM; j++ ) {
			turn_on_7seg( SEG[  j  ] );
			delay( DELAY_TEST );
		}
	}
}

/**
 * reset anode
 */
void reset_anode( int n ) {
	int i;
	// trun on or off 7seg
	for ( i=0; i<ROW_NUM; i++ ) { 
		if ( i == n ) {
			digitalWrite( ANODE[ i ], HIGH ); 
		} else {
			digitalWrite( ANODE[ i ], LOW ); 
		}				
	}
}

/**
 * convert text string into array of index
 */
void convert_index( char text[], int length ) {
	int i, n;
	int j = 0;
	// process each character
	for ( i=0; i<length; i++ ) {
		n = search_hash( text[ i ] );
		if ( n >=0 ) {
			pat_index[  j ] = n;
			j++;
		}
	}
	pat_length = j;
}

/**
 * search inxex from character
 */	
int search_hash( char c ) {
	int i;
	// search the hash one by one
	for ( i=0; i<HASH_LENGTH; i++ ) {
		if ( HASH[ i ][ 0 ] == c ) {
			return HASH[ i ][ 1 ];
		}
	}
	return -1;
}

/**
 * turn on/off one 7seg
 */
void turn_on_7seg( char p[] ) {
	int i;
  	for ( i=0; i<SEG_NUM; i++ ) {
		// turn on at LOW
  		if ( p[i] == 1 ) {
  			digitalWrite( CATHODE[ i ], LOW ); 
  		} else {
  		  	digitalWrite( CATHODE[ i ], HIGH ); 
  		}
	}  
}	

/**
 * write eeprom
 */
void write_eeprom( char text[], int length ) {
	int i;
	// number of characters : addr０
	EEPROM.write( 0, length );
	// characters : addr 1 - 100
	for ( i=0; i < length; i++ ) {
		EEPROM.write( i+1,  text[ i ] );
	}
}

/**
 * write eeprom
 */
void read_eeprom() {
	int i;
	// number of characters : addr０
	int len = EEPROM.read( 0 );
	// check the range from 1 to 100
	if ( len <= 0 ) return;
	if ( len > MAX_TEXT ) return;
	text_index = len;
	// characters : addr 1 - 100
	for ( i=0; i < len; i++ ) {
		text_buf[ i ] = EEPROM.read( i+1 );
	}
}

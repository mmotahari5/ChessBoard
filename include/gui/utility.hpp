#ifndef UTILITY
#define UTILITY

#include <iostream>
#include <string>
#include <map>
#include "StackPage.hpp"

//  extract position from a chessboard location to a pair of number
std::pair<int, int> positionExtraction(std::string);

//  extract position from a chessboard location to a pair of number. but this function of the pair numbers it returns is specific to the grid in Gtkmm
std::pair<int, int> gridPositionExtraction(std::string);

//  search in map that contain name and position of pieces and return name a piece that is on position.
std::string pieceNameByPosition(std::map< std::string, std::string> map, std::string position);

//	generate location code of chess board from a pair number
std::string generateLocationOfChessBoard( int, int);

//  check if that pawn are in front half of board. Is get score or no?
void checkPawnInFrontHalfScore( StackPage *pStack);

//  remove a widget by position from board
void removeWidgetFromBoard( Gtk::Grid*, std::string);
void removeWidgetFromGrid( Gtk::Grid*, int, int);

//  search in array for a value
int numberValueInArray(std::array<std::string, 32>, std::string);

//  search in map for a value
int numberPositionOfBlankCell( std::map< int, std::string>, std::string);

bool isCellEmpty( std::map<int, std::string> map, int x, int y);
#endif

#include "../include/ChessBoard.h"
#include "../include/handler.hpp"
#include "utility.hpp"
#include "Player.h"

ChessBoard ::ChessBoard()
{
  std::cout << "Please enter the name of the white Player" << '\n';
  std::cin >> plr1->Name;
  plr1->ColorOfPlayer = PlayersColor::White;
  std::cout << "Please enter the name of the Black Player" << '\n';
  std::cin >> plr2->Name;
  plr2->ColorOfPlayer = PlayersColor::Black;
}

ChessBoard::ChessBoard(Player *player1, Player *player2, StackPage *pStack)
{
  plr1 = player1;
  plr2 = player2;

  this->pStack = pStack;

  initBoard();
}

void ChessBoard::initBoard()
{

  Board = new Cell *[8];
  for (int i = 0; i < 8; i++)
    Board[i] = new Cell[8];

  //Setting white pawns
  Board[0][0].SetPawn(new Rook(0, 0, PlayersColor::White));
  Board[7][0].SetPawn(new Rook(7, 0, PlayersColor::White));

  Board[1][0].SetPawn(new Knight(1, 0, PlayersColor::White));
  Board[6][0].SetPawn(new Knight(6, 0, PlayersColor::White));

  Board[2][0].SetPawn(new Bishop(2, 0, PlayersColor::White));
  Board[5][0].SetPawn(new Bishop(5, 0, PlayersColor::White));

  Board[3][0].SetPawn(new Queen(3, 0, PlayersColor::White));
  Board[4][0].SetPawn(new King(4, 0, PlayersColor::White));

  for (int i = 0; i < 8; i++)
  {
    Board[i][1].SetPawn(new Pawn(i, 1, PlayersColor::White));
  }

  //Setting black pawns
  Board[0][7].SetPawn(new Rook(0, 7, PlayersColor::Black));
  Board[7][7].SetPawn(new Rook(7, 7, PlayersColor::Black));

  Board[1][7].SetPawn(new Knight(1, 7, PlayersColor::Black));
  Board[6][7].SetPawn(new Knight(6, 7, PlayersColor::Black));

  Board[2][7].SetPawn(new Bishop(2, 7, PlayersColor::Black));
  Board[5][7].SetPawn(new Bishop(5, 7, PlayersColor::Black));

  Board[3][7].SetPawn(new Queen(3, 7, PlayersColor::Black));
  Board[4][7].SetPawn(new King(4, 7, PlayersColor::Black));

  for (int i = 0; i < 8; i++)
  {
    Board[i][6].SetPawn(new Pawn(i, 6, PlayersColor::Black)); // چیدمان جایگاه اول سرباز ها به صورت حلقه
  }
}

vector<pair<int, int>> ChessBoard::GetFreeMovements(pair<int, int> cell) //همان تابع movepiece است
{
  /*
    تابعی است که تمام خانه هایی که یک مهره میتواند در یک حرکت جای بگیرد را مشخص میکند
  */
  Cell *c = &(Board[cell.first][cell.second]);
  auto movements = c->ptr->GetMovements(Board);

  std::cout << "movements: " << std::endl;
  for (auto it = movements.cbegin(); it != movements.cend(); it++)
  {
    std::cout << "\t(" << (*it).first << ',' << (*it).second << ')' << std::endl;
  }

  for (auto i = movements.begin(); i != movements.end(); i++)
  {
    if (!Board[i->first][i->second].IsEmpty() && Board[i->first][i->second].ptr->Type == ChessType::King)
    {
      i = movements.erase(i);
      i--;
    }
  }

  return movements;
}

pair<int, int> ChessBoard::FindKing(PlayersColor color) //جای شاه را مشخص میکند
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (!Board[i][j].IsEmpty() && Board[i][j].ptr->Type == ChessType::King && Board[i][j].ptr->Color == color)
        return {i, j};
    }
  }

  return {0, 0};
}

bool ChessBoard::IsCheckMated(PlayersColor color) //کیش و مات را مشخص میکند
{
  return IsChecked(color) && IsMated(color);
}

bool ChessBoard::IsChecked(PlayersColor color) //با استفاده از findking فقط کیش شدن را مشخص میکند
{
  auto king = FindKing(color);
  PlayersColor targetcolor = (color == PlayersColor::Black) ? PlayersColor::White : PlayersColor::Black;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (!Board[i][j].IsEmpty() && Board[i][j].ptr->Color == targetcolor)
      {
        auto moves = Board[i][j].ptr->GetMovements(Board);
        for (auto k = moves.begin(); k != moves.end(); k++)
        {
          if (k->first == king.first && k->second == king.second)
            return true;
        }
      }
    }
  }

  return false;
}

bool ChessBoard::IsMated(PlayersColor color) //فقط مات شدن
{
  auto king = FindKing(color);

  auto kingmoves = Board[king.first][king.second].ptr->GetMovements(Board);

  PlayersColor targetcolor = (color == PlayersColor::Black) ? PlayersColor::White : PlayersColor::Black;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (!Board[i][j].IsEmpty() && Board[i][j].ptr->Color == targetcolor)
      {
        auto moves = Board[i][j].ptr->GetMovements(Board);
        for (auto k = moves.begin(); k != moves.end(); k++)
        {
          auto it = kingmoves.end();
          for (auto f = kingmoves.begin(); f != kingmoves.end(); f++)
          {
            if (k->first == f->first && k->second == f->second)
            {
              it = f;
            }
          }

          if (it != kingmoves.end())
            kingmoves.erase(it);
        }
      }
    }
  }

  return (kingmoves.size() == 0);
}

void ChessBoard::HitScoring(struct Player *p, pair<int, int> position) //امتیاز دهی زدن مهره
{
  p->Score += Board[position.first][position.second].ptr->HitScore;
}

void ChessBoard::ThreatScoring(struct Player *p, pair<int, int> position) //اتیاز دهی تهدید مهره
{
  p->Score += Board[position.first][position.second].ptr->ThreatScore;
}

void ChessBoard::UndoScoring(struct Player *p) //کسر امتیاز وقتی که از حرکت undo استفاده کنیم
{
  p->NegativScore += 5;
}

std::pair<std::pair<int, int>, std::pair<int, int>> ChessBoard::RandomMove(struct Player *p) // حرکت رندوم که یک پوینتر از بازیکن میگیرد
{
  std::cout << "\n>>>>>>start ChessBoard::RandomMove(struct Player*);" << std::endl;

  srand(time(0));
  int i = (rand()) % 8;
  int j = (rand()) % 8;

  std::cout << "\t|-first i,j : " << i << ',' << j << std::endl;
  int asd = 0;
  while (true)
  {
    asd++;
    std::cout << "\t\tcounter While : " << asd << std::endl;
    //  if cell [i][j] is empty of is inverse color of chessman again create random number for i,j
    std::cout << "\t\t|- Board[i][j].ptr : " << Board[i][j].ptr << std::endl;

		if(Board[i][j].ptr == nullptr)
		{
    	i = (rand()) % 8;
    	j = (rand()) % 8;
    	std::cout << "\t\tRandomMove last i,j : " << i << ',' << j << std::endl;
			continue;
		}
//    std::cout << "\t\t|- Board[i][j].ptr -> Color : " << (int)(Board[i][j].ptr->Color) << "\tp -> ColorOfPlayer: " << (int)(p->ColorOfPlayer) << std::endl;
    if (Board[i][j].ptr != nullptr && (int)(Board[i][j].ptr->Color) == (int)(p->ColorOfPlayer))
    {
      break;
    }
    std::cout << "\t|-debug 216" << std::endl;
    std::cout << "\t|-Board[i][j].ptr-> Color: " << (int)(Board[i][j].ptr->Color) << "\tp-> ColorOfPlayer: " << (int)(p->ColorOfPlayer) << std::endl;
    i = (rand()) % 8;
    j = (rand()) % 8;
    std::cout << "\t\tRandomMove last i,j : " << i << ',' << j << std::endl;
  }

  std::cout << "\t|-final i,j : " << i << ',' << j << std::endl;
  //  vector<pair<int, int>> cell = GetFreeMovements({i , j});
  std::vector<std::pair<int, int>> movements;

  std::cout << "\t|-***\tbefore pChessman = Board[i][j].ptr\t***" << std::endl;
  Chessman *pChessman = Board[i][j].ptr;
  Pawn *pPawn = dynamic_cast<Pawn *>(pChessman);

  std::cout << "\t|-chessType : " << (int)(Board[i][j].ptr->Type) << "\tColor: " << (int)(Board[i][j].ptr->Color) << std::endl;
  if ((int)(pChessman->Type) == 3)
  {
    std::cout << "\t|-That Pawn ";
    std::cout << "p-> ColorOfPlayer: " << (int)(p->ColorOfPlayer) << std::endl;
    if ((int)(p->ColorOfPlayer) == 0)
    {
      std::cout << "White" << std::endl;
      if (pPawn->FirstMove)
      {
        movements.push_back({i, j + 2});
      }
      if (j != 7)
      {
        movements.push_back({i, j + 1});
      }
    }

    if ((int)(p->ColorOfPlayer) == 1)
    {
      std::cout << "Black" << std::endl;
      if (pPawn->FirstMove)
      {
        movements.push_back({i, j - 2});
      }
      if (j != 0)
      {
        movements.push_back({i, j - 1});
      }
    }
  }

  else if (pChessman->Type == ChessType::Rook)
  {
    for (int y = 0; y < 8; y++)
    {
      if (y != j)
      {
        movements.push_back({i, y});
      }
    }
    for (int x = 0; x < 8; x++)
    {
      if (x != i)
      {
        movements.push_back({x, j});
      }
    }
  }

  else if (pChessman->Type == ChessType::Knight)
  {
    if (i < 7 && j < 6)
    {
      movements.push_back({i + 1, j + 2});
    }
    if (i > 0 && j < 6)
    {
      movements.push_back({i - 1, j + 2});
    }
    if (i < 6 && j < 7)
    {
      movements.push_back({i + 2, j + 1});
    }
    if (i > 1 && j < 7)
    {
      movements.push_back({i - 2, j + 1});
    }
    if (i < 6 && j > 0)
    {
      movements.push_back({i + 2, j - 1});
    }
    if (i > 1 && j > 0)
    {
      movements.push_back({i - 2, j - 1});
    }
    if (i < 7 && j > 1)
    {
      movements.push_back({i + 1, j - 2});
    }
    if (i > 0 && j > 1)
    {
      movements.push_back({i - 1, j - 2});
    }
  }

  else if (pChessman->Type == ChessType::Bishop)
  {
    int x = i, y = j;

    for (; y < 0; y--, x--)
    {
    }
    for (; x <= 7 && y <= 7; x++, y++)
    {
      if (x != i && y != j)
      {
        movements.push_back({x, y});
      }
    }
    for (x = i, y = j; y < 0; y--, x++)
    {
    }
    for (; x >= 0 && y <= 7; x--, y++)
    {
      if (x != i && y != j)
      {
        movements.push_back({x, y});
      }
    }
  }

  else if (pChessman->Type == ChessType::Queen)
  {
    int x = i, y = j;
    for (; y < 0; y--, x--)
    {
    }
    for (; x <= 7 && y <= 7; x++, y++)
    {
      if (x != i && y != j)
      {
        movements.push_back({x, y});
      }
    }

    for (x = i, y = j; y < 0; y--, x++)
    {
    }
    for (; x >= 0 && y <= 7; x--, y++)
    {
      if (x != i && y != j)
      {
        movements.push_back({x, y});
      }
    }

    for (y = 0; y < 8; y++)
    {
      if (y != j)
      {
        movements.push_back({i, y});
      }
    }
    for (x = 0; x < 8; x++)
    {
      if (x != i)
      {
        movements.push_back({x, j});
      }
    }
  }

  else if (pChessman->Type == ChessType::King)
  {
    if (i > 0)
    {
      movements.push_back({i - 1, j});
    }
    if (i < 7)
    {
      movements.push_back({i + 1, j});
    }
    if (j > 0)
    {
      movements.push_back({i, j - 1});
    }
    if (j < 7)
    {
      movements.push_back({i, j + 1});
    }
  }

  std::cout << "\t|-movements can select" << std::endl;
  for (int i = 0; i < static_cast<int>( movements.size()); i++)
  {
    std::cout << "\t\t" << movements.at(i).first << ',' << movements.at(i).second << std::endl;
  }
  int randomNumber = (rand()) % (int)(movements.size());

  std::cout << "\t|-randomeNumber finall : " << randomNumber << std::endl;
  std::cout << "\t|-movements[randomNumber] = " << movements[randomNumber].first << ',' << movements[randomNumber].second << std::endl;
  auto correctMovements = GetFreeMovements({i, j});
  for (auto it = correctMovements.cbegin(); it != correctMovements.cend(); it++)
  {
    if (*it == movements.at(randomNumber))
    {
      return {{i, j}, movements.at(randomNumber)};
    }
  }

  return {{8, 8}, {8, 8}};
}

vector<pair<int, int>> ChessBoard::Threat(pair<int, int> cell)
{
  vector<pair<int, int>> ans;
  vector<pair<int, int>> FinallThreat;
  ans = GetFreeMovements(cell);

  for (auto i = ans.begin(); i != ans.end(); i++)
  {
    if (!Board[i->first][i->second].IsEmpty())
    {
      FinallThreat.push_back(*i);
      if (pStack->handler->get_round() == PlayersColor::White)
      {
        ThreatScoring(plr1, *i);
      }
      if (pStack->handler->get_round() == PlayersColor::Black)
      {
        ThreatScoring(plr2, *i);
      }
    }
  }
  return FinallThreat;
}

void ChessBoard::ThreatPlus()
{
  std::vector<std::pair<int, int>> vector;
  if (pStack->handler->get_round() == PlayersColor::White)
  {
    for (int i = 0; i <= 15; i++)
    {
      std::vector<std::pair<int, int>> ans;
      if (i == numberValueInArray(pStack->nameOfPieces, pStack->piece))
      {
        ans = GetFreeMovements(positionExtraction(pStack->cellDestination));
      }
      else
      {
        ans = GetFreeMovements(positionExtraction(pStack->positionOfPieces[pStack->nameOfPieces[i]]));
      }
      for (auto j = ans.cbegin(); j != ans.cend(); j++)
      {
        if (!Board[j->first][j->second].IsEmpty())
        {
          vector.push_back(*j);
        }
      }
    }
  }
  else
  {
    for (int i = 16; i <= 31; i++)
    {
      std::vector<std::pair<int, int>> ans;
      if (i == numberValueInArray(pStack->nameOfPieces, pStack->piece))
      {
        ans = GetFreeMovements(positionExtraction(pStack->cellDestination));
      }
      else
      {
        ans = GetFreeMovements(positionExtraction(pStack->positionOfPieces[pStack->nameOfPieces[i]]));
      }
      for (auto j = ans.cbegin(); j != ans.cend(); j++)
      {
        if (!Board[j->first][j->second].IsEmpty())
        {
          vector.push_back(*j);
        }
      }
    }
  }

  //	clear cells that repeated
  for (auto i = vector.begin(); i != vector.end(); i++)
  {
    for (auto j = i + 1; j != vector.end(); j++)
    {
      if (*i == *j)
      {
        j = vector.erase(j);
        j--;
      }
    }
  }

  //	scoring
  for (int i = 0; i < static_cast<int>( vector.size()); i++)
  {
    if (pStack->handler->get_round() == PlayersColor::White)
    {
      ThreatScoring(plr1, vector[i]);
    }
    else
    {
      ThreatScoring(plr2, vector[i]);
    }
  }
}

void ChessBoard::Move(pair<int, int> position, pair<int, int> toPosition)
{
  Board[position.first][position.second].ptr->Move(toPosition);
  if (!Board[toPosition.first][toPosition.second].IsEmpty())
  {
    TrashingList.push_back(Board[toPosition.first][toPosition.second].ptr);
  }
  Board[toPosition.first][toPosition.second].ptr = Board[position.first][position.second].ptr;
  Board[position.first][position.second].ptr = nullptr;
}

Cell **ChessBoard::GetBoard()
{
  return Board;
}

bool ChessBoard::verifyMove(std::string move)
{
  char x1 = move[1];
  x1 -= 49; // convert to {0, 1, 2, 3, 4, 5, 6, 7}
  char y1 = move[2];
  std::string xstr1, ystr1;
  xstr1 += x1;
  ystr1 += y1;
  int n1 = stoi(xstr1);
  int m1 = stoi(ystr1) - 1;
  std::pair<int, int> origin(n1, m1);

  char x2 = move[3];
  x2 -= 49; // convert to {0, 1, 2, 3, 4, 5, 6, 7}
  char y2 = move[4];
  std::string xstr2, ystr2;
  xstr2 += x2;
  ystr2 += y2;
  int n2 = stoi(xstr2);
  int m2 = stoi(ystr2) - 1;
  std::pair<int, int> destination(n2, m2);

  std::cout << "origin : " << origin.first << "," << origin.second << "\tdest : " << destination.first << "," << destination.second << std::endl;

  auto freeDestinations = this->GetFreeMovements(origin);

  std::cout << "after get free movements" << std::endl;
  for (auto it = freeDestinations.cbegin(); it != freeDestinations.cend(); it++)
  {
    if (*it == destination)
    {
      return true;
    }
  }
  return false;
}

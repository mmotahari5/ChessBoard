#include "ChessBoard.h"

ChessBoard :: ChessBoard()
{
  std::cout << "Please enter the name of the white Player" << '\n';
  std::cin >> plr1.Name;
  plr1.ColorOfPlayer = {White};
  std::cout << "Please enter the name of the Black Player" << '\n';
  std::cin >> plr2.Name;
  plr2.ColorOfPlayer = {Black};
}

void ChessBoard::initBoard()
{

  Board = new Cell*[8];
  for (int i = 0; i < 8; i++)
    Board[i] = new Cell[8];

  //Setting white pawns
  Board[0][0].SetPawn(new Rook(0, 0, ChessColor::White));
  Board[7][0].SetPawn(new Rook(7, 0, ChessColor::White));

  Board[1][0].SetPawn(new Knight(1, 0, ChessColor::White));
  Board[6][0].SetPawn(new Knight(6, 0, ChessColor::White));

  Board[2][0].SetPawn(new Bishop(2, 0, ChessColor::White));
  Board[5][0].SetPawn(new Bishop(5, 0, ChessColor::White));

  Board[3][0].SetPawn(new Queen(3, 0, ChessColor::White));
  Board[4][0].SetPawn(new King(4, 0, ChessColor::White));

  for (int i = 0; i < 8; i++)
  {
    Board[i][1].SetPawn(new Pawn(i, 1, ChessColor::White));
  }

  //Setting black pawns
  Board[0][7].SetPawn(new Rook(0, 7, ChessColor::Black));
  Board[7][7].SetPawn(new Rook(7, 7, ChessColor::Black));

  Board[1][7].SetPawn(new Knight(1, 7, ChessColor::Black));
  Board[6][7].SetPawn(new Knight(6, 7, ChessColor::Black));

  Board[2][7].SetPawn(new Bishop(2, 7, ChessColor::Black));
  Board[5][7].SetPawn(new Bishop(5, 7, ChessColor::Black));

  Board[3][7].SetPawn(new Queen(3, 7, ChessColor::Black));
  Board[4][7].SetPawn(new King(4, 7, ChessColor::Black));


  for (int i = 0; i < 8; i++)
  {
    Board[i][6].SetPawn(new Pawn(i, 6, ChessColor::Black));
  }
}

vector<pair<int, int>> ChessBoard::GetFreeMovements(pair<int, int> cell)
{
  Cell c = Board[cell.first][cell.second];
  auto movements = c.ptr->GetMovements(Board);

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

pair<int, int> ChessBoard::FindKing(ChessColor color)
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

bool ChessBoard::IsCheckMated(ChessColor color)
{
  return IsChecked(color) && IsMated(color);
}

bool ChessBoard::IsChecked(ChessColor color)
{
  auto king = FindKing(color);
  ChessColor targetcolor = (color == ChessColor::Black) ? ChessColor::White : ChessColor::Black;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (!Board[i][j].IsEmpty()
      && Board[i][j].ptr->Color == targetcolor)
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

bool ChessBoard::IsMated(ChessColor color)
{
  auto king = FindKing(color);

  auto kingmoves = Board[king.first][king.second].ptr->GetMovements(Board);



  ChessColor targetcolor = (color == ChessColor::Black) ? ChessColor::White : ChessColor::Black;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (!Board[i][j].IsEmpty()
      && Board[i][j].ptr->Color == targetcolor)
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

void ChessBoard::HitScoring(struct Player p , pair<int, int> position)
{
  p.Score += Board[position.first][position.second].ptr -> HitScore;

}

void ChessBoard::ThreatScoring(struct Player p , pair<int, int> position)
{
  p.Score += Board[position.first][position.second].ptr -> ThreatScore;

}


void ChessBoard::Move(pair<int, int> position, pair<int, int> toPosition)
{
  Board[position.first][position.second].ptr->Move(toPosition);
  Board[toPosition.first][toPosition.second].ptr = Board[position.first][position.second].ptr;
  Board[position.first][position.second].ptr = nullptr;
}

Cell** ChessBoard::GetBoard()
{
  return Board;
}

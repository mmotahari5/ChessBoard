#include "StackPage.hpp"
#include "handler.hpp"
#include "utility.hpp"
#include <iostream>
#include <string>
#include <algorithm>

void StackPage::on_i_drag_data_get(int i, Gtk::SelectionData &selection_data)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_drag_data_get( int, Gtk::SelectionData&)" << std::endl;
	//	Check periority round. White or Black??
	if (15 >= i /*White chessman*/ && handler->get_round() != PlayersColor::White)
	{
		if( language == Language::Persian){
			Gtk::MessageDialog dialog("نوبت تو نیست!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			dialog.set_secondary_text("الان نوبت مهره سیاه هست !!!!!!!!!!!!\tبی ادب");
			dialog.run();
			return;
		}
		if( language == Language::English){
			Gtk::MessageDialog dialog("It is not your round!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			dialog.set_secondary_text("Now it is the round of black");
			dialog.run();
			return;
		}
	}
	if (16 <= i /*Black chessman*/ && handler->get_round() != PlayersColor::Black)
	{
		if( language == Language::Persian){
			Gtk::MessageDialog dialog("نوبت تو نیست!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			dialog.set_secondary_text("الان نوبت مهره سفید هست !!!!!!!!!!!!\tبی ادب");
			dialog.run();
			return;
		}
		if( language == Language::English){
			Gtk::MessageDialog dialog("It is not your round!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			dialog.set_secondary_text("Now it is the round of white");
			dialog.run();
			return;
		}
	}

	piece = nameOfPieces[i];
	pointerPiece = pieces[i];
	cellOrigin = positionOfPieces[piece];

	selection_data.set(selection_data.get_target(), "I'm Dataaaaa");
	std::cout << "\t|-Exit from StackPage::on_i_drag_data_get( int, Gtk::SelectionData&)" << std::endl;
}
void StackPage::on_i_drag_data_get(int i)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_drag_data_get( int);" << std::endl;
	std::cout << "\t|-i = " << i << std::endl;
	//	Check periority round. White or Black??
	if (15 >= i /*White chessman*/ && handler->get_round() != PlayersColor::White)
	{
		Gtk::MessageDialog dialog("نوبت تو نیست!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
		dialog.set_secondary_text("الان نوبت مهره سیاه هست !!!!!!!!!!!!\tبی ادب");
		dialog.run();
		return;
	}
	if (16 <= i /*Black chessman*/ && handler->get_round() != PlayersColor::Black)
	{
		Gtk::MessageDialog dialog("نوبت تو نیست!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
		dialog.set_secondary_text("الان نوبت مهره سفید هست !!!!!!!!!!!!\tبی ادب");
		dialog.run();
		return;
	}

	piece = nameOfPieces[i];
	pointerPiece = pieces[i];
	cellOrigin = positionOfPieces[piece];

	std::cout << "\t|-Exit from StackPage::on_i_drag_data_get( int, Gtk::SelectionData&)" << std::endl;
}

void StackPage::on_i_chessman_drag_data_recieved(int i)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_chessman_drag_data_recieved( int);" << std::endl;
	//	Check periority round. White or Black??
	std::string pieceNameDest = nameOfPieces[i];
	cellDestination = positionOfPieces[pieceNameDest];
	if (motionVerification())
	{

		numberNewBlankSquars++;
		positionOfBlankSquars[numberNewBlankSquars + 32] = positionOfPieces[piece];

		pBoardGame->remove(*pointerPiece);
		pBoardGame->remove(*(pieces[i]));

		if (i <= 15)
		{
			//	move piece to removed pices list
			Gtk::Widget *pWidget = nullptr;
			pWidget = pRemovedPiecesGrid->get_child_at(numberWhitePiecesRemoved, 0);

			if (pWidget == nullptr)
			{
				std::cerr << "\t|-Can't connect to widget on removedPiecesGrid [" << numberBlackPiecesRemoved << ",2]" << std::endl;
				return;
			}
			pRemovedPiecesGrid->remove(*pWidget);

			m_refGlade->get_widget(nameOfPieces[i], pImageTemp);
			pRemovedPiecesGrid->attach(*pImageTemp, numberWhitePiecesRemoved, 0);
			numberWhitePiecesRemoved++;
		}
		else if (i >= 16)
		{
			//	move piece to removed pices list
			Gtk::Widget *pWidget = nullptr;
			pWidget = pRemovedPiecesGrid->get_child_at(numberBlackPiecesRemoved, 2);

			if (pWidget == nullptr)
			{
				std::cerr << "\t|-Can't connect to widget on removedPiecesGrid [" << numberBlackPiecesRemoved << ",2]" << std::endl;
				return;
			}
			pRemovedPiecesGrid->remove(*pWidget);

			m_refGlade->get_widget(nameOfPieces[i], pImageTemp);
			pRemovedPiecesGrid->attach(*pImageTemp, numberBlackPiecesRemoved, 2);
			numberBlackPiecesRemoved++;
		}

		pBoardGame->attach(*(pointerPiece), gridPositionExtraction(positionOfPieces[pieceNameDest]).first, gridPositionExtraction(positionOfPieces[pieceNameDest]).second);

		pBoardGame->attach(*(blankSquars[numberNewBlankSquars + 32]), gridPositionExtraction(positionOfPieces[piece]).first, gridPositionExtraction(positionOfPieces[piece]).second);

		positionOfPieces[pieceNameDest] = "removed";
		positionOfPieces[piece] = cellDestination;

		//	if pawn go to a8,b8,c8,d8,....
		if (piece[1] == 'p')
		{
			Gtk::Widget *pImageBishop, *pImageRook, *pImageQueen, *pImageKnight;

			if (piece[0] == 'w' && cellDestination[1] == '8')
			{
				m_refGlade->get_widget("wbDialog", pImageBishop);
				m_refGlade->get_widget("wrDialog", pImageRook);
				m_refGlade->get_widget("wqDialog", pImageQueen);
				m_refGlade->get_widget("wnDialog", pImageKnight);

				pQueenBtnDialogConvertPawn->set_image(*pImageQueen);
				pKnightBtnDialogConvertPawn->set_image(*pImageRook);
				pBishopBtnDialogConvertPawn->set_image(*pImageQueen);
				pRookBtnDialogConvertPawn->set_image(*pImageKnight);

				pDialogConvertPawn->run();
			}
			if (piece[0] == 'b' && cellDestination[1] == '1')
			{
				m_refGlade->get_widget("bbDialog", pImageBishop);
				m_refGlade->get_widget("brDialog", pImageRook);
				m_refGlade->get_widget("bqDialog", pImageQueen);
				m_refGlade->get_widget("bnDialog", pImageKnight);

				pQueenBtnDialogConvertPawn->set_image(*pImageQueen);
				pKnightBtnDialogConvertPawn->set_image(*pImageRook);
				pBishopBtnDialogConvertPawn->set_image(*pImageQueen);
				pRookBtnDialogConvertPawn->set_image(*pImageKnight);

				pDialogConvertPawn->run();
			}
		}

		updateScoreBoard();
	}
	std::cout << "\t|-Exit from StackPage::on_i_chessman_drag_data_recieved( int);" << std::endl;
}

void StackPage::on_i_chessman_drag_data_recieved(int i, const Glib::RefPtr<Gdk::DragContext> &context, guint time)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_chessman_drag_data_recieved( int, const Glib::RefPtr<Gdk::DragContext>& context, guint time);" << std::endl;
	std::string pieceNameDest = nameOfPieces[i];
	cellDestination = positionOfPieces[pieceNameDest];
	if (motionVerification())
	{

		numberNewBlankSquars++;
		positionOfBlankSquars[numberNewBlankSquars + 32] = positionOfPieces[piece];

		pBoardGame->remove(*pointerPiece);
		pBoardGame->remove(*(pieces[i]));

		if (i <= 15)
		{
			//	move piece to removed pices list
			Gtk::Widget *pWidget = nullptr;
			pWidget = pRemovedPiecesGrid->get_child_at(numberWhitePiecesRemoved, 0);

			if (pWidget == nullptr)
			{
				std::cerr << "\t|-Can't connect to widget on removedPiecesGrid [" << numberBlackPiecesRemoved << ",2]" << std::endl;
				return;
			}
			pRemovedPiecesGrid->remove(*pWidget);

			m_refGlade->get_widget(nameOfPieces[i], pImageTemp);
			pRemovedPiecesGrid->attach(*pImageTemp, numberWhitePiecesRemoved, 0);
			numberWhitePiecesRemoved++;
		}
		else if (i >= 16)
		{
			//	move piece to removed pices list
			Gtk::Widget *pWidget = nullptr;
			pWidget = pRemovedPiecesGrid->get_child_at(numberBlackPiecesRemoved, 2);

			if (pWidget == nullptr)
			{
				std::cerr << "\t|-Can't connect to widget on removedPiecesGrid [" << numberBlackPiecesRemoved << ",2]" << std::endl;
				return;
			}
			pRemovedPiecesGrid->remove(*pWidget);

			m_refGlade->get_widget(nameOfPieces[i], pImageTemp);
			pRemovedPiecesGrid->attach(*pImageTemp, numberBlackPiecesRemoved, 2);
			numberBlackPiecesRemoved++;
		}

		pBoardGame->attach(*(pointerPiece), gridPositionExtraction(positionOfPieces[pieceNameDest]).first, gridPositionExtraction(positionOfPieces[pieceNameDest]).second);

		pBoardGame->attach(*(blankSquars[numberNewBlankSquars + 32]), gridPositionExtraction(positionOfPieces[piece]).first, gridPositionExtraction(positionOfPieces[piece]).second);

		positionOfPieces[pieceNameDest] = "removed";
		positionOfPieces[piece] = cellDestination;

		//	if pawn go to a8,b8,c8,d8,....
		if (piece[1] == 'p')
		{
			Gtk::Widget *pImageBishop, *pImageRook, *pImageQueen, *pImageKnight;

			if (piece[0] == 'w' && cellDestination[1] == '8')
			{
				m_refGlade->get_widget("wbDialog", pImageBishop);
				m_refGlade->get_widget("wrDialog", pImageRook);
				m_refGlade->get_widget("wqDialog", pImageQueen);
				m_refGlade->get_widget("wnDialog", pImageKnight);

				pQueenBtnDialogConvertPawn->set_image(*pImageQueen);
				pKnightBtnDialogConvertPawn->set_image(*pImageRook);
				pBishopBtnDialogConvertPawn->set_image(*pImageQueen);
				pRookBtnDialogConvertPawn->set_image(*pImageKnight);

				pDialogConvertPawn->run();
			}
			if (piece[0] == 'b' && cellDestination[1] == '1')
			{
				m_refGlade->get_widget("bbDialog", pImageBishop);
				m_refGlade->get_widget("brDialog", pImageRook);
				m_refGlade->get_widget("bqDialog", pImageQueen);
				m_refGlade->get_widget("bnDialog", pImageKnight);

				pQueenBtnDialogConvertPawn->set_image(*pImageQueen);
				pKnightBtnDialogConvertPawn->set_image(*pImageRook);
				pBishopBtnDialogConvertPawn->set_image(*pImageQueen);
				pRookBtnDialogConvertPawn->set_image(*pImageKnight);

				pDialogConvertPawn->run();
			}
		}

		updateScoreBoard();
	}
	context->drag_finish(false, false, time);
	std::cout << "\t|-Exit from StackPage::on_i_chessman_drag_data_recieved();" << std::endl;
}

void StackPage::on_i_cell_drag_data_recieved(int i, const Glib::RefPtr<Gdk::DragContext> &context, guint time)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_cell_drag_data_recieved( int, const Glib::RefPtr<Gdk::DragContext>& context, guint time);" << std::endl;
	cellDestination = positionOfBlankSquars[i];
	std::cout << "\t|-cell destination : " << cellDestination << std::endl;
	if (motionVerification())
	{
		pBoardGame->remove(*pointerPiece);
		pBoardGame->remove(*(blankSquars[i]));

		pBoardGame->attach(*(pointerPiece), gridPositionExtraction(positionOfBlankSquars[i]).first, gridPositionExtraction(positionOfBlankSquars[i]).second);
		pBoardGame->attach(*(blankSquars[i]), gridPositionExtraction(positionOfPieces[piece]).first, gridPositionExtraction(positionOfPieces[piece]).second);

		positionOfBlankSquars[i] = positionOfPieces[piece];
		positionOfPieces[piece] = cellDestination;

		//	if pawn go to a8,b8,c8,d8,....
		if (piece[1] == 'p')
		{
			if (piece[0] == 'w' && cellDestination[1] == '8')
			{
				m_refGlade->get_widget("whiteQueenImgDialog", pWidget[0]);	//|
				pQueenBtnDialogConvertPawn->property_image() = pWidget[0];	//|
				m_refGlade->get_widget("whiteKnightImgDialog", pWidget[1]); //|
				pKnightBtnDialogConvertPawn->property_image() = pWidget[1]; //|=>	set color images
				m_refGlade->get_widget("whiteBishopImgDialog", pWidget[2]); //|
				pBishopBtnDialogConvertPawn->property_image() = pWidget[2]; //|
				m_refGlade->get_widget("whiteRookImgDialog", pWidget[3]);	//|
				pRookBtnDialogConvertPawn->property_image() = pWidget[3];	//|

				pDialogConvertPawn->run();
			}
			else if (piece[0] == 'b' && cellDestination[1] == '1')
			{
				m_refGlade->get_widget("blackQueenImgDialog", pWidget[0]);	//|
				pQueenBtnDialogConvertPawn->property_image() = pWidget[0];	//|
				m_refGlade->get_widget("blackKnightImgDialog", pWidget[1]); //|
				pKnightBtnDialogConvertPawn->property_image() = pWidget[1]; //|=>	set color images
				m_refGlade->get_widget("blackBishopImgDialog", pWidget[2]); //|
				pBishopBtnDialogConvertPawn->property_image() = pWidget[2]; //|
				m_refGlade->get_widget("blackRookImgDialog", pWidget[3]);	//|
				pRookBtnDialogConvertPawn->property_image() = pWidget[3];	//|

				pDialogConvertPawn->run();
			}
		}

		updateScoreBoard();
	}
	context->drag_finish(false, false, time);

	check_15_NegativScore();
	std::cout << "\t|-Exit from StackPage::on_i_cell_drag_data_recieved();" << std::endl;
}

void StackPage::on_i_cell_drag_data_recieved(int i)
{
	std::cout << "\n>>>>>>>Start StackPage::on_i_cell_drag_data_recieved( int);" << std::endl;
	cellDestination = positionOfBlankSquars[i];
	std::cout << "\t|-cell destination : " << cellDestination << std::endl;
	if (motionVerification())
	{
		pBoardGame->remove(*pointerPiece);
		pBoardGame->remove(*(blankSquars[i]));

		pBoardGame->attach(*(pointerPiece), gridPositionExtraction(positionOfBlankSquars[i]).first, gridPositionExtraction(positionOfBlankSquars[i]).second);
		pBoardGame->attach(*(blankSquars[i]), gridPositionExtraction(positionOfPieces[piece]).first, gridPositionExtraction(positionOfPieces[piece]).second);

		positionOfBlankSquars[i] = positionOfPieces[piece];
		positionOfPieces[piece] = cellDestination;

		//	if pawn go to a8,b8,c8,d8,....
		if (piece[1] == 'p')
		{
			if (piece[0] == 'w' && cellDestination[1] == '8')
			{
				m_refGlade->get_widget("whiteQueenImgDialog", pWidget[0]);	//|
				pQueenBtnDialogConvertPawn->property_image() = pWidget[0];	//|
				m_refGlade->get_widget("whiteKnightImgDialog", pWidget[1]); //|
				pKnightBtnDialogConvertPawn->property_image() = pWidget[1]; //|=>	set color images
				m_refGlade->get_widget("whiteBishopImgDialog", pWidget[2]); //|
				pBishopBtnDialogConvertPawn->property_image() = pWidget[2]; //|
				m_refGlade->get_widget("whiteRookImgDialog", pWidget[3]);	//|
				pRookBtnDialogConvertPawn->property_image() = pWidget[3];	//|

				pDialogConvertPawn->run();
			}
			else if (piece[0] == 'b' && cellDestination[1] == '1')
			{
				m_refGlade->get_widget("blackQueenImgDialog", pWidget[0]);	//|
				pQueenBtnDialogConvertPawn->property_image() = pWidget[0];	//|
				m_refGlade->get_widget("blackKnightImgDialog", pWidget[1]); //|
				pKnightBtnDialogConvertPawn->property_image() = pWidget[1]; //|=>	set color images
				m_refGlade->get_widget("blackBishopImgDialog", pWidget[2]); //|
				pBishopBtnDialogConvertPawn->property_image() = pWidget[2]; //|
				m_refGlade->get_widget("blackRookImgDialog", pWidget[3]);	//|
				pRookBtnDialogConvertPawn->property_image() = pWidget[3];	//|

				pDialogConvertPawn->run();
			}
		}

		updateScoreBoard();
	}

	std::cout << "\t|-Exit from StackPage::on_i_cell_drag_data_recieved( int);" << std::endl;
}

void StackPage::on_0_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(0, selection_data);
}
void StackPage::on_1_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(1, selection_data);
}
void StackPage::on_2_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(2, selection_data);
}
void StackPage::on_3_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(3, selection_data);
}
void StackPage::on_4_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(4, selection_data);
}
void StackPage::on_5_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(5, selection_data);
}
void StackPage::on_6_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(6, selection_data);
}
void StackPage::on_7_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(7, selection_data);
}
void StackPage::on_8_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(8, selection_data);
}
void StackPage::on_9_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(9, selection_data);
}
void StackPage::on_10_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(10, selection_data);
}
void StackPage::on_11_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(11, selection_data);
}
void StackPage::on_12_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(12, selection_data);
}
void StackPage::on_13_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(13, selection_data);
}
void StackPage::on_14_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(14, selection_data);
}
void StackPage::on_15_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(15, selection_data);
}
void StackPage::on_16_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(16, selection_data);
}
void StackPage::on_17_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(17, selection_data);
}
void StackPage::on_18_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(18, selection_data);
}
void StackPage::on_19_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(19, selection_data);
}
void StackPage::on_20_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(20, selection_data);
}
void StackPage::on_21_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(21, selection_data);
}
void StackPage::on_22_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(22, selection_data);
}
void StackPage::on_23_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(23, selection_data);
}
void StackPage::on_24_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(24, selection_data);
}
void StackPage::on_25_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(25, selection_data);
}
void StackPage::on_26_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(26, selection_data);
}
void StackPage::on_27_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(27, selection_data);
}
void StackPage::on_28_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(28, selection_data);
}
void StackPage::on_29_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(29, selection_data);
}
void StackPage::on_30_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(30, selection_data);
}
void StackPage::on_31_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_drag_data_get(31, selection_data);
}

void StackPage::on_1_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(1, context, time);
}
void StackPage::on_2_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(2, context, time);
}
void StackPage::on_3_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(3, context, time);
}
void StackPage::on_4_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(4, context, time);
}
void StackPage::on_5_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(5, context, time);
}
void StackPage::on_6_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(6, context, time);
}
void StackPage::on_7_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(7, context, time);
}
void StackPage::on_8_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(8, context, time);
}
void StackPage::on_9_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(9, context, time);
}
void StackPage::on_10_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(10, context, time);
}
void StackPage::on_11_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(11, context, time);
}
void StackPage::on_12_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(12, context, time);
}
void StackPage::on_13_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(13, context, time);
}
void StackPage::on_14_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(14, context, time);
}
void StackPage::on_15_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(15, context, time);
}
void StackPage::on_16_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(16, context, time);
}
void StackPage::on_17_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(17, context, time);
}
void StackPage::on_18_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(18, context, time);
}
void StackPage::on_19_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(19, context, time);
}
void StackPage::on_20_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(20, context, time);
}
void StackPage::on_21_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(21, context, time);
}
void StackPage::on_22_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(22, context, time);
}
void StackPage::on_23_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(23, context, time);
}
void StackPage::on_24_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(24, context, time);
}
void StackPage::on_25_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(25, context, time);
}
void StackPage::on_26_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(26, context, time);
}
void StackPage::on_27_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(27, context, time);
}
void StackPage::on_28_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(28, context, time);
}
void StackPage::on_29_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(29, context, time);
}
void StackPage::on_30_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(30, context, time);
}
void StackPage::on_31_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(31, context, time);
}
void StackPage::on_32_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(32, context, time);
}
void StackPage::on_33_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(33, context, time);
}
void StackPage::on_34_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(34, context, time);
}
void StackPage::on_35_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(35, context, time);
}
void StackPage::on_36_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(36, context, time);
}
void StackPage::on_37_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(37, context, time);
}
void StackPage::on_38_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(38, context, time);
}
void StackPage::on_39_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(39, context, time);
}
void StackPage::on_40_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(40, context, time);
}
void StackPage::on_41_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(41, context, time);
}
void StackPage::on_42_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(42, context, time);
}
void StackPage::on_43_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(43, context, time);
}
void StackPage::on_44_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(44, context, time);
}
void StackPage::on_45_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(45, context, time);
}
void StackPage::on_46_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(46, context, time);
}
void StackPage::on_47_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(47, context, time);
}
void StackPage::on_48_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(48, context, time);
}
void StackPage::on_49_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(49, context, time);
}
void StackPage::on_50_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(50, context, time);
}
void StackPage::on_51_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(51, context, time);
}
void StackPage::on_52_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(52, context, time);
}
void StackPage::on_53_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(53, context, time);
}
void StackPage::on_54_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(54, context, time);
}
void StackPage::on_55_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(55, context, time);
}
void StackPage::on_56_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(56, context, time);
}
void StackPage::on_57_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(57, context, time);
}
void StackPage::on_58_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(58, context, time);
}
void StackPage::on_59_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(59, context, time);
}
void StackPage::on_60_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(60, context, time);
}
void StackPage::on_61_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(61, context, time);
}
void StackPage::on_62_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(62, context, time);
}
void StackPage::on_63_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(63, context, time);
}
void StackPage::on_64_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_cell_drag_data_recieved(64, context, time);
}

void StackPage::on_0_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(0, context, time);
}
void StackPage::on_1_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(1, context, time);
}
void StackPage::on_2_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(2, context, time);
}
void StackPage::on_3_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(3, context, time);
}
void StackPage::on_4_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(4, context, time);
}
void StackPage::on_5_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(5, context, time);
}
void StackPage::on_6_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(6, context, time);
}
void StackPage::on_7_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(7, context, time);
}
void StackPage::on_8_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(8, context, time);
}
void StackPage::on_9_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(9, context, time);
}
void StackPage::on_10_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(10, context, time);
}
void StackPage::on_11_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(11, context, time);
}
void StackPage::on_12_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(12, context, time);
}
void StackPage::on_13_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(13, context, time);
}
void StackPage::on_14_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(14, context, time);
}
void StackPage::on_15_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(15, context, time);
}
void StackPage::on_16_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(16, context, time);
}
void StackPage::on_17_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(17, context, time);
}
void StackPage::on_18_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(18, context, time);
}
void StackPage::on_19_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(19, context, time);
}
void StackPage::on_20_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(20, context, time);
}
void StackPage::on_21_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(21, context, time);
}
void StackPage::on_22_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(22, context, time);
}
void StackPage::on_23_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(23, context, time);
}
void StackPage::on_24_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(24, context, time);
}
void StackPage::on_25_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(25, context, time);
}
void StackPage::on_26_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(26, context, time);
}
void StackPage::on_27_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(27, context, time);
}
void StackPage::on_28_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(28, context, time);
}
void StackPage::on_29_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(29, context, time);
}
void StackPage::on_30_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(30, context, time);
}
void StackPage::on_31_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &context, int x, int y, const Gtk::SelectionData &selection_data, guint info, guint time)
{
	on_i_chessman_drag_data_recieved(31, context, time);
}

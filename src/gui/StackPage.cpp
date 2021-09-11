#include <sys/stat.h>//	for search a file exist or no (this library is so fast) => https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
#include "StackPage.hpp"
#include "Cell.h"
#include "utility.hpp"
#include "handler.hpp"
#include <bits/stdc++.h>

StackPage::StackPage(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade) : Gtk::Stack(cobject),
																							pGameBtn(nullptr),
																							pExitBtn(nullptr),
																							pSettingBtnPage0(nullptr),
																							pStartGameBtn(nullptr),
																							handler(nullptr),
																							pBoardGame(nullptr),
																							pRemovedPiecesGrid(nullptr),
																							pScoreFirstPL(nullptr),
																							pNegativScoreFirstPL(nullptr),
																							pScoreSecondPL(nullptr),
																							pNegativScoreSecondPL(nullptr),
																							m_refGlade(refGlade)
{
	m_refGlade->get_widget("gameBtnPage0", pGameBtn);
	m_refGlade->get_widget("settingBtnPage0", pSettingBtnPage0);
	m_refGlade->get_widget("startGameBtnID", pStartGameBtn);
	m_refGlade->get_widget("exitBtnStack2", pExitBtnStack2);
	m_refGlade->get_widget("boardGameID", pBoardGame);
	m_refGlade->get_widget("overlayGameBoardID", pGameBoardOverlay);
	m_refGlade->get_widget("boardGameImageID", pGameBoardImage);
	m_refGlade->get_widget("gameNameLabelID", pGameNameLabel);
	m_refGlade->get_widget("removedPiecesGrid", pRemovedPiecesGrid);
	m_refGlade->get_widget("dialogConvertPawn", pDialogConvertPawn);
	m_refGlade->get_widget("dialogQueenBtn", pQueenBtnDialogConvertPawn);
	m_refGlade->get_widget("dialogKnightBtn", pKnightBtnDialogConvertPawn);
	m_refGlade->get_widget("dialogBishopBtn", pBishopBtnDialogConvertPawn);
	m_refGlade->get_widget("dialogRookBtn", pRookBtnDialogConvertPawn);
	m_refGlade->get_widget("doualMoveBtn", pDoualMoveBtn);
	m_refGlade->get_widget("undoBtn", pUndoBtn);
	m_refGlade->get_widget("languageComboBox", pLanguageComboBox);
	m_refGlade->get_widget("backBtnPage3", pBackBtnPage3);
	m_refGlade->get_widget("titleLabelPage0", pTitleLabelPage0);
	m_refGlade->get_widget("exitBtnPage0", pExitBtnPage0);
	m_refGlade->get_widget("gameNameLabelPage1", pGameNameLabelPage1);
	m_refGlade->get_widget("player1NameLabelPage1", pPlayer1NameLabelPage1);
	m_refGlade->get_widget("player2NameLabelPage1", pPlayer2NameLabelPage1);
	m_refGlade->get_widget("gameNameLabelPage2", pGameNameLabelPage2);
	m_refGlade->get_widget("scoresLabelPage2", pScoresLabelPage2);
	m_refGlade->get_widget("settingBtnPage2", pSettingBtnPage2);
	m_refGlade->get_widget("messageLabelDialogConvertPawn", pMessageLabelDialogConvertPawn);
	m_refGlade-> get_widget( "mainWindow", pWindow);
	m_refGlade -> get_widget("randomMoveBtn", pRandomMoveBtn);

	nameOfPieces[0] = "wrl";
	nameOfPieces[1] = "wnl";
	nameOfPieces[2] = "wbl";
	nameOfPieces[3] = "wq";
	nameOfPieces[4] = "wk";
	nameOfPieces[5] = "wbr";
	nameOfPieces[6] = "wnr";
	nameOfPieces[7] = "wrr";
	nameOfPieces[8] = "wp1";
	nameOfPieces[9] = "wp2";
	nameOfPieces[10] = "wp3";
	nameOfPieces[11] = "wp4";
	nameOfPieces[12] = "wp5";
	nameOfPieces[13] = "wp6";
	nameOfPieces[14] = "wp7";
	nameOfPieces[15] = "wp8";
	nameOfPieces[16] = "brl";
	nameOfPieces[17] = "bnl";
	nameOfPieces[18] = "bbl";
	nameOfPieces[19] = "bq";
	nameOfPieces[20] = "bk";
	nameOfPieces[21] = "bbr";
	nameOfPieces[22] = "bnr";
	nameOfPieces[23] = "brr";
	nameOfPieces[24] = "bp1";
	nameOfPieces[25] = "bp2";
	nameOfPieces[26] = "bp3";
	nameOfPieces[27] = "bp4";
	nameOfPieces[28] = "bp5";
	nameOfPieces[29] = "bp6";
	nameOfPieces[30] = "bp7";
	nameOfPieces[31] = "bp8";

	listOfNavigateBetweenPages.push_back( get_visible_child_name());

	std::string name;
	for (int i = 0; i <= 31; i++)
	{
		name = nameOfPieces[i] + "Btn";
		m_refGlade->get_widget(name, pieces[i]);
	}

	pGameBoardOverlay->add_overlay(*pBoardGame);

	pGameBtn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &StackPage::set_visible_child), "start_game_page", Gtk::STACK_TRANSITION_TYPE_NONE));
	
	pSettingBtnPage2->signal_clicked().connect([this]{
		this-> set_visible_child( "settingPage", Gtk::STACK_TRANSITION_TYPE_NONE);
		this-> listOfNavigateBetweenPages.push_back( get_visible_child_name());
	});

	try{
		pStartGameBtn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::startGameBtn_clicked));
	}
	catch(int error){
		std::cout << "ASdfasdfASdfasdf " << error << std::endl;
	}
	pExitBtnStack2->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::exitBtnStack2_clicked));

	//	connect images blank squars on UI file to program code and set target to drag&drop.
	std::vector<Gtk::TargetEntry> target;
	target.push_back(Gtk::TargetEntry("Target"));
	for (int i = 1; i <= 64; i++)
	{
		blankSquars[i] = (Gtk::Image *)nullptr;
		std::string id = (std::to_string(i)) + "sq";
		m_refGlade->get_widget(id, blankSquars[i]);
		blankSquars[i]->drag_dest_set(target);
	}
	//	Set pieces target for DnD
	for (size_t i = 0; i <= 31; i++)
	{
		pieces[i]->drag_source_set(target);
		pieces[i]->drag_dest_set(target);
	}

	//	combo box for change language of GUI
	pSettingBtnPage0->signal_clicked().connect([this]{
		this-> set_visible_child( "settingPage", Gtk::STACK_TRANSITION_TYPE_NONE);
		this-> listOfNavigateBetweenPages.push_back( get_visible_child_name());
		});
	pLanguageComboBox->signal_changed().connect(sigc::mem_fun(*this, &StackPage::on_languageComboBox_changed));
	//	back button on setting page
	pBackBtnPage3->signal_clicked().connect([this]{
		for(auto &it: listOfNavigateBetweenPages){
			std::cout << "\t<><><>\tlistOfNavigateBetweenPages >\t" << it << std::endl;
		}
		this-> set_visible_child( listOfNavigateBetweenPages.at( listOfNavigateBetweenPages.size() - 2), Gtk::STACK_TRANSITION_TYPE_NONE);
		listOfNavigateBetweenPages.push_back( get_visible_child_name());
	});
}

StackPage::~StackPage()
{
}

void StackPage::exitBtnStack2_clicked()
{
	file.close();
	pWindow-> close();
}

void StackPage::startGameBtn_clicked()
{

	//	Show the startGame Page
	set_visible_child("game_page", Gtk::STACK_TRANSITION_TYPE_NONE);

	listOfNavigateBetweenPages.push_back( get_visible_child_name());

	//	setup separators on page stack2 game
	m_refGlade->get_widget("separator0Stack2", pSeparators[0]);
	m_refGlade->get_widget("separator1Stack2", pSeparators[1]);
	m_refGlade->get_widget("separator2Stack2", pSeparators[2]);
	m_refGlade->get_widget("separator3Stack2", pSeparators[3]);
	m_refGlade->get_widget("separator5Stack2", pSeparators[5]);
	m_refGlade->get_widget("separator6Stack2", pSeparators[6]);
	pSeparators[0]->override_background_color(Gdk::RGBA("#8d99ae"));
	pSeparators[1]->override_background_color(Gdk::RGBA("#8d99ae"));
	pSeparators[2]->override_background_color(Gdk::RGBA("#8d99ae"));
	pSeparators[3]->override_background_color(Gdk::RGBA("#8d99ae"));
	pSeparators[5]->override_background_color(Gdk::RGBA("#8d99ae"));
	pSeparators[6]->override_background_color(Gdk::RGBA("#8d99ae"));

	//	create handler and set game name and player name to handler
	m_refGlade->get_widget("gameNameEntry", pGameNameEnt);
	m_refGlade->get_widget("playerFirstNameEntry", pPlayerFirstNameEnt);
	m_refGlade->get_widget("playerSecondNameEntry", pPlayerSecondNameEnt);

	handler = new Handler(pGameNameEnt->get_text(), pPlayerFirstNameEnt->get_text(), pPlayerSecondNameEnt->get_text(), this);

	//	setup scoreboard
	m_refGlade->get_widget("firstPLNameScoreLabel", pFirstPLNameScoreLabel);
	m_refGlade->get_widget("secondPLNameScoreLabel", pSecondPLNameScoreLabel);
	m_refGlade->get_widget("firstPLNameNegativScoreLabel", pFirstPLNameNegativScoreLabel);
	m_refGlade->get_widget("secondPLNameNegativScoreLabel", pSecondPLNameNegativScoreLabel);
	m_refGlade->get_widget("scoreFirstPL", pScoreFirstPL);
	m_refGlade->get_widget("negativScoreFirstPL", pNegativScoreFirstPL);
	m_refGlade->get_widget("scoreSecondPL", pScoreSecondPL);
	m_refGlade->get_widget("negativScoreSecondPL", pNegativScoreSecondPL);

	std::string str = "امتیاز ";
	str += (handler->player1.Name);
	str += ":";
	if (language == Language::English)
	{
		str = (handler->player1.Name);
		str += " score:";
	}
	pFirstPLNameScoreLabel->set_label(str);
	pScoreFirstPL->set_label(std::to_string(handler->player1.Score));

	str = "امتیاز منفی ";
	str += handler->player1.Name;
	str += ":";
	if (language == Language::English)
	{
		str = (handler->player1.Name);
		str += " negativ score:";
	}
	pFirstPLNameNegativScoreLabel->set_label(str);
	pNegativScoreFirstPL->set_label(std::to_string(handler->player1.NegativScore));

	str = "امتیاز ";
	str += handler->player2.Name;
	str += ":";
	if (language == Language::English)
	{
		str = (handler->player2.Name);
		str += " score:";
	}
	pSecondPLNameScoreLabel->set_label(str);
	pScoreSecondPL->set_label(std::to_string(handler->player2.Score));

	str = "امتیاز منفی ";
	str += handler->player2.Name;
	str += ":";
	if (language == Language::English)
	{
		str = (handler->player2.Name);
		str += " negativ score:";
	}
	pSecondPLNameNegativScoreLabel->set_label(str);
	pNegativScoreSecondPL->set_label(std::to_string(handler->player2.NegativScore));

	//	set game name on page Stack2
	pGameNameLabel->set_label(handler->get_gameName());

	//	set positions of pieces
	//	initial white pieces location
	int count = 0;
	for (size_t i = 0; i <= 1; i++)
	{
		for (size_t j = 0; j <= 7; j++)
		{
			positionOfPieces[nameOfPieces[count]] = generateLocationOfChessBoard(j, i);
			count++;
		}
	}
	//	initial black pieces location
	for (size_t i = 7; i >= 6; i--)
	{
		for (size_t j = 0; j <= 7; j++)
		{
			positionOfPieces[nameOfPieces[count]] = generateLocationOfChessBoard(j, i);
			count++;
		}
	}

	//	set positions of blankSquars
	count = 1;
	for (int i = 2; i <= 5; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			positionOfBlankSquars[count] = generateLocationOfChessBoard(j, i);
			count++;
		}
	}
	for (int i = 33; i <= 64; i++)
	{
		positionOfBlankSquars[i] = "null";
	}

	//	Place the pieces in the original position
	for (int i = 0; i <= 31; i++)
	{
		if (pieces[i]->get_parent())
		{ // if chessman is exist on board then remove it
			pBoardGame->remove(*(pieces[i]));
		}
	}
	for (int i = 0; i <= 31; i++)
	{ //	initial chessman on original locations
		pBoardGame->attach(*(pieces[i]), gridPositionExtraction(positionOfPieces[nameOfPieces[i]]).first, gridPositionExtraction(positionOfPieces[nameOfPieces[i]]).second);
	}

	//	Place the blank squars in the original position
	for (size_t i = 1; i <= 64; i++)
	{
		if (blankSquars[i]->get_parent())
		{ // if widget is exist on grid then remove it
			pBoardGame->remove(*(blankSquars[i]));
		}
	}
	for (size_t i = 1; i <= 32; i++)
	{
		pBoardGame->attach(*(blankSquars[i]), gridPositionExtraction(positionOfBlankSquars[i]).first, gridPositionExtraction(positionOfBlankSquars[i]).second);
	}

	//	check for exist file or no
	std::string fileName = handler-> player1.Name;
	fileName += "-" + handler-> player2.Name + "-" + handler-> get_gameName() + ".acd";
	std::cout << "fileName : " << fileName << std::endl;
	struct stat buffer;
	if( stat ( fileName.c_str(), &buffer) == 0){
		int response;
		if(language == Language::Persian){
			Gtk::MessageDialog alertDialog( "فایل تکراری!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
			std::string detailMessage = "قبلا بازی با همین بازیکن ها و همین اسم بازی انجام دادی\nالان اگر ادامه بدی فایل اون یکی بازی پاک میشه.\nاشکال نداره؟";
			alertDialog.set_secondary_text( detailMessage);
			response = alertDialog.run();
		}
		if(language == Language::English){
			Gtk::MessageDialog alertDialog( "Duplicate file!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_YES_NO);
			std::string detailMessage = "You have played with the same players and the same game name before\nNow if you continue, the file of that one game will be deleted.\nbe done?";
			alertDialog.set_secondary_text( detailMessage);
			response = alertDialog.run();
		}

		switch(response){
			case -8://	if "Yes" button clicked
				file.open( fileName);
				break;
			case -9://	if "No" button clicked
				pWindow-> close();
				break;
		}
	}
	else{
		file.open( fileName);
	}

	pieces[0]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_0_drag_data_get));
	pieces[1]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_1_drag_data_get));
	pieces[2]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_2_drag_data_get));
	pieces[3]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_3_drag_data_get));
	pieces[4]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_4_drag_data_get));
	pieces[5]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_5_drag_data_get));
	pieces[6]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_6_drag_data_get));
	pieces[7]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_7_drag_data_get));
	pieces[8]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_8_drag_data_get));
	pieces[9]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_9_drag_data_get));
	pieces[10]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_10_drag_data_get));
	pieces[11]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_11_drag_data_get));
	pieces[12]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_12_drag_data_get));
	pieces[13]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_13_drag_data_get));
	pieces[14]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_14_drag_data_get));
	pieces[15]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_15_drag_data_get));
	pieces[16]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_16_drag_data_get));
	pieces[17]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_17_drag_data_get));
	pieces[18]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_18_drag_data_get));
	pieces[19]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_19_drag_data_get));
	pieces[20]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_20_drag_data_get));
	pieces[21]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_21_drag_data_get));
	pieces[22]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_22_drag_data_get));
	pieces[23]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_23_drag_data_get));
	pieces[24]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_24_drag_data_get));
	pieces[25]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_25_drag_data_get));
	pieces[26]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_26_drag_data_get));
	pieces[27]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_27_drag_data_get));
	pieces[28]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_28_drag_data_get));
	pieces[29]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_29_drag_data_get));
	pieces[30]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_30_drag_data_get));
	pieces[31]->signal_drag_data_get().connect(sigc::mem_fun(*this, &StackPage::on_31_drag_data_get));
	blankSquars[1]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_1_cell_drag_data_received));
	blankSquars[2]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_2_cell_drag_data_received));
	blankSquars[3]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_3_cell_drag_data_received));
	blankSquars[4]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_4_cell_drag_data_received));
	blankSquars[5]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_5_cell_drag_data_received));
	blankSquars[6]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_6_cell_drag_data_received));
	blankSquars[7]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_7_cell_drag_data_received));
	blankSquars[8]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_8_cell_drag_data_received));
	blankSquars[9]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_9_cell_drag_data_received));
	blankSquars[10]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_10_cell_drag_data_received));
	blankSquars[11]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_11_cell_drag_data_received));
	blankSquars[12]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_12_cell_drag_data_received));
	blankSquars[13]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_13_cell_drag_data_received));
	blankSquars[14]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_14_cell_drag_data_received));
	blankSquars[15]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_15_cell_drag_data_received));
	blankSquars[16]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_16_cell_drag_data_received));
	blankSquars[17]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_17_cell_drag_data_received));
	blankSquars[18]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_18_cell_drag_data_received));
	blankSquars[19]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_19_cell_drag_data_received));
	blankSquars[20]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_20_cell_drag_data_received));
	blankSquars[21]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_21_cell_drag_data_received));
	blankSquars[22]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_22_cell_drag_data_received));
	blankSquars[23]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_23_cell_drag_data_received));
	blankSquars[24]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_24_cell_drag_data_received));
	blankSquars[25]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_25_cell_drag_data_received));
	blankSquars[26]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_26_cell_drag_data_received));
	blankSquars[27]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_27_cell_drag_data_received));
	blankSquars[28]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_28_cell_drag_data_received));
	blankSquars[29]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_29_cell_drag_data_received));
	blankSquars[30]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_30_cell_drag_data_received));
	blankSquars[31]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_31_cell_drag_data_received));
	blankSquars[32]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_32_cell_drag_data_received));
	blankSquars[33]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_33_cell_drag_data_received));
	blankSquars[34]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_34_cell_drag_data_received));
	blankSquars[35]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_35_cell_drag_data_received));
	blankSquars[36]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_36_cell_drag_data_received));
	blankSquars[37]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_37_cell_drag_data_received));
	blankSquars[38]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_38_cell_drag_data_received));
	blankSquars[39]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_39_cell_drag_data_received));
	blankSquars[40]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_40_cell_drag_data_received));
	blankSquars[41]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_41_cell_drag_data_received));
	blankSquars[42]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_42_cell_drag_data_received));
	blankSquars[43]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_43_cell_drag_data_received));
	blankSquars[44]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_44_cell_drag_data_received));
	blankSquars[45]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_45_cell_drag_data_received));
	blankSquars[46]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_46_cell_drag_data_received));
	blankSquars[47]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_47_cell_drag_data_received));
	blankSquars[48]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_48_cell_drag_data_received));
	blankSquars[49]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_49_cell_drag_data_received));
	blankSquars[50]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_50_cell_drag_data_received));
	blankSquars[51]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_51_cell_drag_data_received));
	blankSquars[52]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_52_cell_drag_data_received));
	blankSquars[53]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_53_cell_drag_data_received));
	blankSquars[54]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_54_cell_drag_data_received));
	blankSquars[55]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_55_cell_drag_data_received));
	blankSquars[56]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_56_cell_drag_data_received));
	blankSquars[57]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_57_cell_drag_data_received));
	blankSquars[58]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_58_cell_drag_data_received));
	blankSquars[59]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_59_cell_drag_data_received));
	blankSquars[60]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_60_cell_drag_data_received));
	blankSquars[61]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_61_cell_drag_data_received));
	blankSquars[62]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_62_cell_drag_data_received));
	blankSquars[63]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_63_cell_drag_data_received));
	blankSquars[64]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_64_cell_drag_data_received));

	pieces[0]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_0_chessman_drag_data_received));
	pieces[1]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_1_chessman_drag_data_received));
	pieces[2]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_2_chessman_drag_data_received));
	pieces[3]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_3_chessman_drag_data_received));
	pieces[4]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_4_chessman_drag_data_received));
	pieces[5]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_5_chessman_drag_data_received));
	pieces[6]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_6_chessman_drag_data_received));
	pieces[7]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_7_chessman_drag_data_received));
	pieces[8]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_8_chessman_drag_data_received));
	pieces[9]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_9_chessman_drag_data_received));
	pieces[10]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_10_chessman_drag_data_received));
	pieces[11]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_11_chessman_drag_data_received));
	pieces[12]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_12_chessman_drag_data_received));
	pieces[13]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_13_chessman_drag_data_received));
	pieces[14]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_14_chessman_drag_data_received));
	pieces[15]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_15_chessman_drag_data_received));
	pieces[16]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_16_chessman_drag_data_received));
	pieces[17]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_17_chessman_drag_data_received));
	pieces[18]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_18_chessman_drag_data_received));
	pieces[19]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_19_chessman_drag_data_received));
	pieces[20]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_20_chessman_drag_data_received));
	pieces[21]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_21_chessman_drag_data_received));
	pieces[22]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_22_chessman_drag_data_received));
	pieces[23]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_23_chessman_drag_data_received));
	pieces[24]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_24_chessman_drag_data_received));
	pieces[25]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_25_chessman_drag_data_received));
	pieces[26]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_26_chessman_drag_data_received));
	pieces[27]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_27_chessman_drag_data_received));
	pieces[28]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_28_chessman_drag_data_received));
	pieces[29]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_29_chessman_drag_data_received));
	pieces[30]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_30_chessman_drag_data_received));
	pieces[31]->signal_drag_data_received().connect(sigc::mem_fun(*this, &StackPage::on_31_chessman_drag_data_received));

	//	doual move button get signal and set handler
	pDoualMoveBtn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::doualMoveBtn_clicked));

	//	Random move botton get signal
	pRandomMoveBtn -> signal_clicked().connect( sigc::mem_fun(*this, &StackPage::randomMoveBtn_clicked));

	//	undo btn get signal and set handler
	pUndoBtn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::undoBtn_clicked));

	//	get signals dialog buttons when pawn go to last cell on board
	pQueenBtnDialogConvertPawn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::on_queenBtnDialog));
	pKnightBtnDialogConvertPawn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::on_knightBtnDialog));
	pBishopBtnDialogConvertPawn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::on_bishopBtnDialog));
	pRookBtnDialogConvertPawn->signal_clicked().connect(sigc::mem_fun(*this, &StackPage::on_rookBtnDialog));
}

int StackPage::cellIsEmpty(std::map<std::string, std::string> map, std::string cell)
{
	for (auto it = map.cbegin(); it != map.cend(); it++)
	{
		if ((*it).second == cell)
		{
			if ((*it).first == piece)
			{
				return -1;
			}
			return 0;
		}
	}
	return 1;
}

bool StackPage::motionVerification()
{
	char chessman = piece[1];
	chessman -= 32; //	captalize
	moveCode = "";
	moveCode += chessman;
	moveCode += cellOrigin;
	moveCode += cellDestination;

	std::cout << "move code : " << moveCode << std::endl;

	if (cellIsEmpty(positionOfPieces, cellDestination) == -1 /*cell destination is cell destination =))*/)
	{
		if (language == Language::English)
		{
			Gtk::MessageDialog dialog("Chessman touch!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			std::string message = "you touch ";
			message += chessman;
			message += " but did not make any move, and for the first time in this round you will receive a negative score.\nMove the nut once more.";
			dialog.set_secondary_text(message);
			dialog.run();
		}

		if (language == Language::Persian)
		{
			Gtk::MessageDialog dialog("دست به مهره!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			std::string message = "شما مهره ";
			message += chessman;
			message += " را برداشتید ولی هیچ حرکتی انجام ندادید و برای اولین بار در این نوبت امتیاز منفی کسب میکنید.\nیکبار دیگر مهره ای را جابجا کن.";
			dialog.set_secondary_text(message);
			dialog.run();
		}

		if (!handler->dastBeMohre)
		{
			handler->get_round_player()->NegativScore += 1;
			handler->dastBeMohre = true;
		}

		updateScoreBoard();
		return false;
	}

	if (handler->pChessboard->verifyMove(moveCode))
	{

		//	print moveCode to the file for save moves
		file << moveCode << "\n";

		if (cellIsEmpty(positionOfPieces, cellDestination) == 0 /*cell destination isn't empty*/)
		{
			moveCode = "";
			moveCode += chessman;
			moveCode += cellOrigin;
			moveCode += 'x';
			char temp = pieceNameByPosition(positionOfPieces, cellDestination)[1];
			temp -= 32;
			moveCode += temp;
			moveCode += cellDestination;
		}
		std::cout << "move code (after ChessBoard::verifyMove): " << moveCode << std::endl;

		listOfMoves.push_back(moveCode); //	push to list of movements

		if (handler->get_round_player()->doualMove)
		{ //if this move is second move in once round for a player
			handler->pChessboard->Move(positionExtraction(cellOrigin), positionExtraction(cellDestination));
			handler->changeRound();
			handler->get_round_player()->doualMove = false;
			return true;
		}

		//	attack movement scoring
		if (cellIsEmpty(positionOfPieces, cellDestination) == 0)
		{
			handler->pChessboard->HitScoring(handler->get_round_player(), positionExtraction(cellDestination));
		}

		handler->pChessboard->Move(positionExtraction(cellOrigin), positionExtraction(cellDestination));

		//	Scoring
		//	امتیاز نیمه دوم سرباز
		checkPawnInFrontHalfScore(this);
		//	بررسی ایا امتیاز تهدید کاربر میگیره یا نه و ثبت ان
		handler->pChessboard->Threat(positionExtraction(cellDestination));
		//	امتیاز تهدید (اختیاری)
		//handler-> pChessboard-> ThreatPlus();

		PlayersColor inverseColor = (int)(handler->get_round()) ? PlayersColor::White : PlayersColor::Black;
		std::cout << "round: " << (int)(inverseColor) << std::endl;
		if (handler->pChessboard->IsCheckMated(inverseColor))
		{
			handler->get_round_player()->Score += 50;
			std::cout << "IsCheckMated" << std::endl;

			if(language == Language::Persian){
				Gtk::MessageDialog dialog("تمام!", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
				std::string message = "خب ";
				handler->changeRound();
				message += handler->  get_round_player()-> Name + "کیش و مات شد. 😂";
				dialog.set_secondary_text(message);
				dialog.run();
			}
			if(language == Language::English){
				Gtk::MessageDialog dialog("Finish!", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
				std::string message = handler->  get_round_player()-> Name ;
				handler->changeRound();
				message += "is ckeckmated 😂";
				dialog.set_secondary_text(message);
				dialog.run();
			}
		}
		else if (handler->pChessboard->IsChecked(inverseColor))
		{
			handler->get_round_player()->Score += 10;
			std::cout << "IsChecked" << std::endl;
		}

		updateScoreBoard();
		//	change round
		handler->changeRound();

		return true;
	}
	else
	{
		if (language == Language::English)
		{
			Gtk::MessageDialog dialog("Motion warning!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			std::string messageDetail = "Move ";
			messageDetail += moveCode[0];
			messageDetail += std::string(" from ") + cellOrigin + " to " + cellDestination + " are invalid!";
			messageDetail += "\nYou can move this chessman to: ";

			auto movements = handler->pChessboard->GetFreeMovements(positionExtraction(cellOrigin));
			for (int i = 0; i < static_cast<int>(movements.size()); i++)
			{
				messageDetail += generateLocationOfChessBoard(movements.at(i).first, movements.at(i).second);
				messageDetail += " ";
			}

			dialog.set_secondary_text(messageDetail);
			dialog.run();
		}
		if (language == Language::Persian)
		{
			Gtk::MessageDialog dialog("حرکت اشتباه!!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
			std::string messageDetail = "حرکت ";
			messageDetail += moveCode[0];
			messageDetail += std::string(" از ") + cellOrigin + " به " + cellDestination + " اشتباه است!!";
			messageDetail += "\nشما میتوانید برای این مهره به مقصد های روبرو بروید: ";

			auto movements = handler->pChessboard->GetFreeMovements(positionExtraction(cellOrigin));
			for (int i = 0; i < static_cast<int>(movements.size()); i++)
			{
				messageDetail += generateLocationOfChessBoard(movements.at(i).first, movements.at(i).second);
				messageDetail += " ";
			}

			dialog.set_secondary_text(messageDetail);
			dialog.run();
		}

		return false;
	}
}

void StackPage::on_queenBtnDialog() { convertPawn("qNew"); }
void StackPage::on_knightBtnDialog() { convertPawn("nNew"); }
void StackPage::on_rookBtnDialog() { convertPawn("rNew"); }
void StackPage::on_bishopBtnDialog() { convertPawn("bNew"); }

void StackPage::convertPawn(std::string chessman)
{
	if (handler->get_round() != PlayersColor::White)
	{ // if round is for white. because before it round changed
		std::string temp = chessman;
		chessman = "w";
		chessman += temp;
		handler->numberOfWhiteConvertPawn++;
		chessman += std::to_string(handler->numberOfWhiteConvertPawn);
	}
	if (handler->get_round() != PlayersColor::Black)
	{
		std::string temp = chessman;
		chessman = "b";
		chessman += temp;
		handler->numberOfBlackConvertPawn++;
		chessman += std::to_string(handler->numberOfBlackConvertPawn);
	}

	positionOfPieces.erase(positionOfPieces.find(piece));

	int i;
	for (i = 0; i <= 31; i++)
	{
		if (nameOfPieces[i] == piece)
		{
			break;
		}
	}

	std::cout << "chessman input: " << chessman << std::endl;
	std::cout << "numberOfWhite and BlackConvertPawn\t" << handler->numberOfWhiteConvertPawn << '\t' << handler->numberOfBlackConvertPawn << std::endl;

	nameOfPieces[i] = chessman;
	positionOfPieces[chessman] = cellDestination;

	pDialogConvertPawn->close();
	pDialogConvertPawn->hide();

	std::cout << "chessman  = " << chessman << std::endl;
	m_refGlade->get_widget(chessman, pWidget[8]);
	pieces[i]->property_image() = pWidget[8];
}

void StackPage::doualMoveBtn_clicked()
{

	handler->changeRound();

	int response;
	if( language == Language::Persian){
		Gtk::MessageDialog dialog("دو حرکت در یک نوبت.", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
		std::string message = handler->get_round_player()->Name;
		message += " شما میتوانید یک حرکت دیگر انجام دهید\n و به ازای ان ۳۰ امتیاز از دست بدهید.";
		dialog.set_secondary_text(message);
		response = dialog.run();
	}
	if( language == Language::English){
		Gtk::MessageDialog dialog("Double move in once round.", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
		std::string message = handler->get_round_player()->Name;
		message += "You can make another moveand lose 30 points in return.\n";
		dialog.set_secondary_text(message);
		response = dialog.run();
	}

	switch (response)
	{
	case 0: // Yes button clicked
		handler->changeRound();
		handler->get_round_player()->NegativScore += 30;
		handler->get_round_player()->doualMove = true;
		break;
	case 1: // No button clicked
		break;
	}
}

void StackPage::undoBtn_clicked()
{
	handler->changeRound();
	if (listOfMoves.size() == 0)
	{
		Gtk::MessageDialog dialog("اشتباه داری میزنی!", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_CLOSE);
		std::string detailMessage = "دکمه undo برای این است که وقتی حرکتی انجام دادید ان را بزنید تا حرکت شما برگردانده شود!\nولی شما هنوز هیچ حرکتی انجام نداده اید!";
		dialog.set_secondary_text(detailMessage);
		dialog.run();
		handler->changeRound();

		return;
	}
	auto it = listOfMoves.cend();
	it--;

	if ((*it)[3] == 'x')
	{ //	if last move is attack
		if (handler->get_round() == PlayersColor::White)
		{
			//	remove image chessman on buttom grid removed pieces
			removeWidgetFromGrid(pRemovedPiecesGrid, numberBlackPiecesRemoved, 2);

			//	add blank image to buttom grid removed pieces
			std::string str = "removedImg";
			str += std::to_string(numberBlackPiecesRemoved);
			str += '2';

			Gtk::Image *pImage;
			m_refGlade->get_widget(str, pImage);
			pRemovedPiecesGrid->attach(*pImage, numberBlackPiecesRemoved, 2);

			numberBlackPiecesRemoved--;

			//	remove chessman from destination cell and move it to origin cell
			//	remove blank image that attached to origin cell
			removeWidgetFromBoard(pBoardGame, cellOrigin);
			for (auto positionOfBlankCellIt = positionOfBlankSquars.begin(); positionOfBlankCellIt != positionOfBlankSquars.end(); positionOfBlankCellIt++)
			{
				if ((*positionOfBlankCellIt).second == cellOrigin)
				{
					(*positionOfBlankCellIt).second = "null";
				}
			}
			numberNewBlankSquars--;

			//	remove white chessman that attacked
			removeWidgetFromBoard(pBoardGame, cellDestination);

			//	attach to origin cell
			pBoardGame->attach(*pointerPiece, gridPositionExtraction(cellOrigin).first, gridPositionExtraction(cellOrigin).second);
			for (auto positionOfPiecesIt = positionOfPieces.begin(); positionOfPiecesIt != positionOfPieces.end(); positionOfPiecesIt++)
			{
				if ((*positionOfPiecesIt).second == cellDestination)
				{
					(*positionOfPiecesIt).second = cellOrigin;
				}
			}

			//	attach removed black chessman to the previos cell
			str = "b";
			str += (moveCode[4] + 32);
			if (moveCode[4] != 'Q' && moveCode[4] != 'K' && moveCode[4] != 'P')
			{
				std::string temp = str;
				temp += 'l';
				if (positionOfPieces[temp] == "removed")
				{
					str += 'l';
				}
				else
				{
					str += 'r';
				}
			}
			else if (moveCode[4] == 'P')
			{
				for (int i = 1; i <= 8; i++)
				{
					std::string temp = str;
					temp += std::to_string(i);
					if (positionOfPieces[temp] == "removed")
					{
						str += std::to_string(i);
					}
				}
			}

			int i;
			for (int j = 0; j < static_cast<int>(nameOfPieces.size()); j++)
			{
				if (nameOfPieces[j] == str)
				{
					i = j;
				}
			}

			pBoardGame->attach(*(pieces[i]), gridPositionExtraction(cellDestination).first, gridPositionExtraction(cellDestination).second);
			positionOfPieces[str] = cellDestination;
		}
		else if (handler->get_round() == PlayersColor::Black)
		{
			removeWidgetFromGrid(pRemovedPiecesGrid, numberWhitePiecesRemoved, 0);

			std::string str = "removedImg";
			str += std::to_string(numberWhitePiecesRemoved);
			str += '0';

			Gtk::Image *pImage;
			m_refGlade->get_widget(str, pImage);
			pRemovedPiecesGrid->attach(*pImage, numberWhitePiecesRemoved, 0);
			numberWhitePiecesRemoved--;

			removeWidgetFromBoard(pBoardGame, cellOrigin);
			for (auto positionOfBlankCellIt = positionOfBlankSquars.begin(); positionOfBlankCellIt != positionOfBlankSquars.end(); positionOfBlankCellIt++)
			{
				if ((*positionOfBlankCellIt).second == cellOrigin)
				{
					(*positionOfBlankCellIt).second = "null";
				}
			}
			numberNewBlankSquars--;

			removeWidgetFromBoard(pBoardGame, cellDestination);

			pBoardGame->attach(*pointerPiece, gridPositionExtraction(cellOrigin).first, gridPositionExtraction(cellOrigin).second);
			for (auto positionOfPiecesIt = positionOfPieces.begin(); positionOfPiecesIt != positionOfPieces.end(); positionOfPiecesIt++)
			{
				if ((*positionOfPiecesIt).second == cellDestination)
				{
					(*positionOfPiecesIt).second = cellOrigin;
				}
			}

			str = "w";
			str += (moveCode[4] + 32);
			if (moveCode[4] != 'Q' && moveCode[4] != 'K' && moveCode[4] != 'P')
			{
				std::string temp = str;
				temp += 'l';
				if (positionOfPieces[temp] == "removed")
				{
					str += 'l';
				}
				else
				{
					str += 'r';
				}
			}
			else if (moveCode[4] == 'P')
			{
				for (int i = 1; i <= 8; i++)
				{
					std::string temp = str;
					temp += std::to_string(i);
					if (positionOfPieces[temp] == "removed")
					{
						str += std::to_string(i);
					}
				}
			}
			int i;
			for (int j = 0; j < static_cast<int>(nameOfPieces.size()); j++)
			{
				if (nameOfPieces[j] == str)
				{
					i = j;
				}
			}
			pBoardGame->attach(*(pieces[i]), gridPositionExtraction(cellDestination).first, gridPositionExtraction(cellDestination).second);
			positionOfPieces[str] = cellDestination;
		}
	}
	else
	{ //	if last movement is not an attack move
		//	move chessman to before location (in Logic)
		handler->pChessboard->Move(positionExtraction(cellDestination), positionExtraction(cellOrigin));

		if (piece[1] == 'p')
		{
			Cell **Board = handler->pChessboard->GetBoard();
			Pawn *ptr = dynamic_cast<Pawn *>(Board[positionExtraction(cellOrigin).first][positionExtraction(cellOrigin).second].ptr);
			ptr->FirstMove = true;
		}
		//	move chessman to before locain (in GUI)
		pBoardGame->remove(*pointerPiece);
		for (auto positionOfBlankCellIt = positionOfBlankSquars.cbegin(); positionOfBlankCellIt != positionOfBlankSquars.cend(); positionOfBlankCellIt++)
		{
			if ((*positionOfBlankCellIt).second == cellOrigin)
			{
				pBoardGame->remove(*(blankSquars[(*positionOfBlankCellIt).first]));
				pBoardGame->attach(*(blankSquars[(*positionOfBlankCellIt).first]), gridPositionExtraction(cellDestination).first, gridPositionExtraction(cellDestination).second);
				positionOfBlankSquars[(*positionOfBlankCellIt).first] = cellDestination;
			}
		}

		pBoardGame->attach(*pointerPiece, gridPositionExtraction(cellOrigin).first, gridPositionExtraction(cellOrigin).second);

		positionOfPieces[piece] = cellOrigin;
	}

	listOfMoves.pop_back();

	handler->pChessboard->UndoScoring(handler->get_round_player());

	updateScoreBoard();
}

void StackPage::updateScoreBoard()
{
	pScoreFirstPL->set_label(std::to_string(handler->player1.Score));
	pNegativScoreFirstPL->set_label(std::to_string(handler->player1.NegativScore));
	pScoreSecondPL->set_label(std::to_string(handler->player2.Score));
	pNegativScoreSecondPL->set_label(std::to_string(handler->player2.NegativScore));
}

void StackPage::check_15_NegativScore()
{
	std::cout << "\n\non check_15_NegativScore\n\t|NegativScore " << (int)(handler->get_round_player()->ColorOfPlayer) << " : " << handler->get_round_player()->NegativScore << std::endl;
	if (handler->get_round_player()->NegativScore < 15)
	{
		std::cout << "NegativScore < 15" << std::endl;
		return;
	}
	std::cout << "\t**\tNegativScore for player front equal zero\t" << (int)(handler->get_round_player()->ColorOfPlayer) << std::endl;
	handler->get_round_player()->NegativScore = 0;
	updateScoreBoard();

	std::cout << "\n>>>>>>>>>>>>>>>>>>\tcheck_15_NegativScore start" << std::endl;
	std::cout << "NegativScore >= 2" << std::endl;

	std::pair<std::pair<int, int>, std::pair<int, int>> randomPosition = handler->pChessboard->RandomMove(handler->get_round_player());

	std::pair<std::pair<int, int>, std::pair<int, int>> tempPair = {{8, 8}, {8, 8}};
	if (randomPosition == tempPair)
	{ //	if movements can't run randomPosition return {{8,8},{8,8}}
		return;
	}

	std::string orig = generateLocationOfChessBoard(randomPosition.first.first, randomPosition.first.second);
	std::string dest = generateLocationOfChessBoard(randomPosition.second.first, randomPosition.second.second);

	std::string nameOfPieceOrig = pieceNameByPosition(positionOfPieces, orig);
	std::string nameOfPieceDest = pieceNameByPosition(positionOfPieces, dest);
	int numberOfOrigPiece = numberValueInArray(nameOfPieces, nameOfPieceOrig);
	int numberOfDest;
	if (nameOfPieceDest != "")
	{
		numberOfDest = numberValueInArray(nameOfPieces, nameOfPieceDest);
	}
	else
	{
		numberOfDest = numberPositionOfBlankCell(positionOfBlankSquars, dest);
	}

	std::cout << "\n>>>>>>>>\tbefore drag data get" << std::endl;
	on_i_drag_data_get(numberOfOrigPiece);
	if (nameOfPieceDest != "")
	{
		std::cout << "\n>>>>>>>>\tbefore chessman drag data recieved" << std::endl;
		on_i_chessman_drag_data_recieved(numberOfDest);
	}
	else
	{
		std::cout << "\n>>>>>>>>\tbefore cell drag data recieved" << std::endl;
		on_i_cell_drag_data_recieved(numberOfDest);
	}
}

void StackPage::on_languageComboBox_changed()
{
	Glib::ustring text = pLanguageComboBox->get_active_text();

	if (!(text.empty()))
	{
		std::cout << "\n>>>>> on_languageComboBox_changed()" << std::endl;
		language =  text == "english" ? Language::English : Language::Persian;
		pBackBtnPage3->set_label( text == "English" ? "Back" : "بازگشت" );
		pTitleLabelPage0->set_label( text == "English" ? "Advance Chess" : "شطرنج پیشرفته");
		pGameBtn->set_label( text == "English" ? "Start" : "شروع");
		pSettingBtnPage0->set_label( text == "English" ? "Setting" : "تنظیمات");
		pExitBtnPage0->set_label( text == "English" ? "Exit" : "خروج");
		pGameNameLabelPage1->set_label( text == "English" ? ":Game name" : "اسم بازی:");
		pPlayer1NameLabelPage1->set_label( text == "English" ? "Player one" : "بازیکن اول");
		pPlayer2NameLabelPage1->set_label( text == "English" ? "Player two" : "بازیکن دوم");
		pGameNameLabelPage2->set_label( text == "English" ? "Game Name:" : "اسم بازی:");
		pScoresLabelPage2->set_label( text == "English" ? "Scores" : "امتیازات");
		pDoualMoveBtn->set_label( text == "English" ? "Double Move" : "دو حرکت");
		pSettingBtnPage2->set_label( text == "English" ? "Setting" : "تنظیمات");
		pExitBtnStack2->set_label( text == "English" ? "Exit" : "خروج");
		
		if (text == "فارسی")
		{
			pMessageLabelDialogConvertPawn-> set_label("تبریک، شما موفق شدید سربازتان را به اخرین خانه برسانید\nحالا از بین گزینه های زیر یکی را برای جایگزینی سربازت انتخاب کن");

			std::string str = "امتیاز ";
			str += (handler->player1.Name);
			str += ":";
			pFirstPLNameScoreLabel->set_label(str);
			pScoreFirstPL->set_label(std::to_string(handler->player1.Score));
		
			str = "امتیاز منفی ";
			str += handler->player1.Name;
			str += ":";
			pFirstPLNameNegativScoreLabel->set_label(str);
			pNegativScoreFirstPL->set_label(std::to_string(handler->player1.NegativScore));
		
			str = "امتیاز ";
			str += handler->player2.Name;
			str += ":";
			pSecondPLNameScoreLabel->set_label(str);
			pScoreSecondPL->set_label(std::to_string(handler->player2.Score));
		
			str = "امتیاز منفی ";
			str += handler->player2.Name;
			str += ":";
			pSecondPLNameNegativScoreLabel->set_label(str);
			pNegativScoreSecondPL->set_label(std::to_string(handler->player2.NegativScore));
		}
		if (text == "English")
		{
			std::cout << "\tGUI language change to english" << std::endl;
			language = Language::English;
	
			std::string str;
			str = (handler->player1.Name);
			str += " score :";
			pFirstPLNameScoreLabel->set_label(str);
			pScoreFirstPL->set_label(std::to_string(handler->player1.Score));
		
			str = handler->player1.Name;
			str += " negativ score:";
			pFirstPLNameNegativScoreLabel->set_label(str);
			pNegativScoreFirstPL->set_label(std::to_string(handler->player1.NegativScore));
		
			str = handler->player2.Name;
			str += " score:";
			pSecondPLNameScoreLabel->set_label(str);
			pScoreSecondPL->set_label(std::to_string(handler->player2.Score));
		
			str = handler->player2.Name;
			str += " negativ score:";
			pSecondPLNameNegativScoreLabel->set_label(str);
			pNegativScoreSecondPL->set_label(std::to_string(handler->player2.NegativScore));
			pMessageLabelDialogConvertPawn-> set_label("Congratulations, you have managed to get your pawn to the last cell\nNow choose one of the following options to replace your pawn");
		}
	}
}

void StackPage::randomMoveBtn_clicked(){
	std::pair < std::pair<int, int>, std::pair<int, int>> randomResult;
	randomResult = handler -> pChessboard -> RandomMove( handler -> get_round_player());

	if(randomResult != std::pair<std::pair<int, int>, std::pair<int, int>>({{8,8}, {8,8}}))
	{
		on_i_drag_data_get( numberValueInArray( nameOfPieces, pieceNameByPosition( positionOfPieces, generateLocationOfChessBoard( randomResult.first.first, randomResult.first.second))));
		
//		Cell** Board = handler -> pChessboard -> GetBoard();
//		if(Board[ randomResult.second.first , randomResult.second.second] -> ptr != nullptr)
//		std::string position = generateLocationOfChessBoard( randomResult.second.first, randomResult.second.second);
//		auto it1 = positionOfBlankSquars.begin();
//		auto it2 = positionOfBlankSquars.end();
//		/*std::map<int, std::string>::const_iterator*/auto itFind = std::find( it1, it2, position);
//		std::map<int, std::string>::const_iterator itEnd = positionOfBlankSquars.cend();
		if(isCellEmpty( positionOfBlankSquars, randomResult.second.first, randomResult.second.second))
		{
			on_i_cell_drag_data_recieved( numberPositionOfBlankCell( positionOfBlankSquars, generateLocationOfChessBoard( randomResult.second.first, randomResult.second.second)));
		}
		else
		{
			on_i_chessman_drag_data_recieved( numberValueInArray( nameOfPieces, pieceNameByPosition( positionOfPieces, generateLocationOfChessBoard( randomResult.second.first, randomResult.second.second))));
		}
	}
}

#ifndef INC_STACKPAGE
#define INC_STACKPAGE

#include <gdk/gdk.h>
#include <gtkmm/stack.h>
#include <gtkmm/button.h>
#include <gtkmm/builder.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/overlay.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/widget.h>
#include <gtkmm/separator.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/comboboxtext.h>
#include <map>
#include <fstream>
#include <utility>

class Handler;

enum class Language
{
	Persian = 0,
	English = 1
};
class StackPage : public Gtk::Stack
{
public:
	friend class ChessBoard;
	friend void checkPawnInFrontHalfScore(StackPage *);

	StackPage(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
	virtual ~StackPage();
	std::array<std::string, 32> nameOfPieces;
	std::map<std::string, std::string> positionOfPieces;
	std::string cellOrigin, piece, cellDestination;

private:
	Gtk::Button *pGameBtn, *pExitBtn, *pSettingBtnPage0, *pStartGameBtn, *pExitBtnStack2, *pReloadBtnStack2, *pDoualMoveBtn, *pUndoBtn, *pBackBtnPage3, *pExitBtnPage0, *pSettingBtnPage2, *pRandomMoveBtn;
	Gtk::Entry *pGameNameEnt, *pPlayerFirstNameEnt, *pPlayerSecondNameEnt;
	Handler *handler;
	Gtk::Grid *pBoardGame, *pRemovedPiecesGrid;
	Gtk::Overlay *pGameBoardOverlay;
	Gtk::Image *pGameBoardImage, *pWNLImg, *pBRLImg, *pImageTemp;
	Gtk::Label *pGameNameLabel, *pFirstPLNameScoreLabel, *pSecondPLNameScoreLabel, *pFirstPLNameNegativScoreLabel, *pSecondPLNameNegativScoreLabel, *pScoreFirstPL, *pNegativScoreFirstPL, *pScoreSecondPL, *pNegativScoreSecondPL, *pTitleLabelPage0, *pGameNameLabelPage1, *pPlayer1NameLabelPage1, *pPlayer2NameLabelPage1, *pGameNameLabelPage2, *pScoresLabelPage2, *pMessageLabelDialogConvertPawn;
	Gtk::Separator *pSeparators[10];
	Gtk::Window *pWindow;

	Language language;
	Gtk::ComboBoxText *pLanguageComboBox;
	void on_languageComboBox_changed();

	Glib::RefPtr<Gtk::Builder> m_refGlade;

	std::string moveCode; //	move code :)

	std::array<Gtk::Button *, 32> pieces;

	std::map<int, Gtk::Image *> blankSquars;

	std::map<int, std::string> positionOfBlankSquars;

	std::vector<std::string> listOfMoves;

	Gtk::Widget *pointerPiece;

	std::fstream file;

	Gtk::Dialog *pDialogConvertPawn;
	Gtk::Button *pQueenBtnDialogConvertPawn, *pKnightBtnDialogConvertPawn, *pBishopBtnDialogConvertPawn, *pRookBtnDialogConvertPawn;
	Gtk::Widget *pWidget[10];
	void convertPawn(std::string);
	void on_queenBtnDialog();
	void on_knightBtnDialog();
	void on_rookBtnDialog();
	void on_bishopBtnDialog();

	int numberNewBlankSquars = 0;
	int numberWhitePiecesRemoved = 0;
	int numberBlackPiecesRemoved = 0;

	std::vector<int> whitePawnsInFrontHalf;
	std::vector<int> blackPawnsInFrontHalf;

	std::pair<char, int> pair;

	void startGameBtn_clicked();
	void exitBtnStack2_clicked();

	void doualMoveBtn_clicked();
	void undoBtn_clicked();
	void randomMoveBtn_clicked();

	void updateScoreBoard();

	void on_settingBtnPage2();

	void check_15_NegativScore();

	std::vector< Glib::ustring> listOfNavigateBetweenPages;

	void on_0_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_1_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_2_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_3_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_4_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_5_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_6_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_7_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_8_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_9_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_10_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_11_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_12_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_13_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_14_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_15_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_16_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_17_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_18_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_19_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_20_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_21_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_22_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_23_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_24_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_25_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_26_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_27_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_28_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_29_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_30_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);
	void on_31_drag_data_get(const Glib::RefPtr<Gdk::DragContext> &context, Gtk::SelectionData &selection_data, guint info, guint time);

	void on_1_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_2_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_3_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_4_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_5_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_6_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_7_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_8_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_9_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_10_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_11_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_12_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_13_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_14_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_15_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_16_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_17_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_18_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_19_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_20_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_21_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_22_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_23_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_24_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_25_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_26_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_27_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_28_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_29_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_30_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_31_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_32_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_33_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_34_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_35_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_36_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_37_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_38_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_39_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_40_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_41_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_42_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_43_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_44_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_45_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_46_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_47_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_48_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_49_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_50_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_51_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_52_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_53_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_54_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_55_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_56_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_57_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_58_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_59_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_60_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_61_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_62_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_63_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_64_cell_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);

	void on_0_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_1_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_2_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_3_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_4_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_5_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_6_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_7_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_8_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_9_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_10_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_11_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_12_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_13_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_14_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_15_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_16_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_17_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_18_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_19_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_20_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_21_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_22_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_23_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_24_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_25_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_26_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_27_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_28_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_29_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_30_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);
	void on_31_chessman_drag_data_received(const Glib::RefPtr<Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint, guint);

	int cellIsEmpty(std::map<std::string, std::string> map, std::string cell);

	bool motionVerification();

	void on_i_drag_data_get(int, Gtk::SelectionData &);
	void on_i_cell_drag_data_recieved(int, const Glib::RefPtr<Gdk::DragContext> &, guint);
	void on_i_chessman_drag_data_recieved(int, const Glib::RefPtr<Gdk::DragContext> &, guint);

	void on_i_cell_drag_data_recieved(int);
	void on_i_chessman_drag_data_recieved(int);
	void on_i_drag_data_get(int);
};

#endif

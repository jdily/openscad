#pragma once 
#include "editor.h"
#include <QMenu>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QWidgetAction>

#include "iTree.h"
typedef stree<hnode*> tree_hnode;
#include "DMSolver.h"

class MyDMEditor : public EditorInterface
{
	Q_OBJECT
public:
	MyDMEditor(class QWidget *parent);
	~MyDMEditor();
	QSize sizeHint() const override;
	void setInitialSizeHint(const QSize&) override;
	QString	toPlainText() override;
	QString selectedText() override;
    int resetFindIndicators(const QString &findText, bool visibility = true) override;
	bool find(const QString &, bool findNext = false, bool findBackwards = false) override;
	void replaceSelectedText(const QString &newText) override;	
	void replaceAll(const QString &findText, const QString &replaceText) override;
	bool findString(const QString & exp, bool findBackwards) const;
	QStringList colorSchemes() override;
    bool canUndo() override;

	void check_selection();
	void set_shape_tree(tree_hnode* _tree);
	void set_solver(DMSolver *_solver);
	void editor_loc_shift(int shift, int cur_id);

	QAction *createSliderAct;
	QPoint *slider_pos;
	// need to know where the position is
	int selected_line_no;
	int selected_col_no;
	int selected_start;
	int selected_end;
	int selected_anchor;
	int selected_position;
	int var_prefix_len;
	int var_str_start;
	int var_str_end;
	int var_val_str;

	// the current selected node's parameter sets
	int param_start_ind;
	int param_end_ind;
	int param_start_pos;
	int param_end_pos;
	int param_str_len;
	QString cap_param_str;
	hnode* cur_selected_node;

	QString selected_var;
	float mani_val;
	int mani_val_len;

	int mani_line_no;
	bool mani_variable;
	QString mani_val_str;

	tree_hnode* shape_tree;
	DMSolver* m_solver;

	bool did_pre_param;

public slots:
	void zoomIn() override;
	void zoomOut() override;
	void setContentModified(bool) override;
	bool isContentModified() override;
	void indentSelection() override;
	void unindentSelection() override;
	void commentSelection() override;
	void uncommentSelection() override;
	void setPlainText(const QString&) override;
	void highlightError(int) override;
	void unhighlightLastError() override;
	void setHighlightScheme(const QString&) override;
	void insert(const QString&) override;
	void setText(const QString&) override;
	void undo() override;
	void redo() override;
	void cut() override;
	void copy() override;
	void paste() override;
	void initFont(const QString&, uint) override;
	void createSlider();

	void update_mani_val(double new_val);
	// TODO : check if we know all the parameter at this time???
	// Or how to do this 
	void opt_mani_val(double new_val);
	hnode* search_node(int select_line_no, int select_col);
	// QString search_all_params();
	void update_var_val(hnode* node, std::vector<double> u_params);
	void write_opted_val(Eigen::VectorXd sols);
	void pre_locate_param_string();
	
private:
	class QTextEdit *textedit;
	class Highlighter *highlighter;
	QSize initialSizeHint;
protected:
    void mousePressEvent(QMouseEvent *event) override;

};

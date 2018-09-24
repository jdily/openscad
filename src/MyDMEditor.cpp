#include "MyDMEditor.h"
#include "Preferences.h"
#include "highlighter.h"
#include "QSettingsCached.h"

MyDMEditor::MyDMEditor(QWidget *parent) : EditorInterface(parent)
{
	QVBoxLayout *legacyeditorLayout = new QVBoxLayout(this);
	legacyeditorLayout->setContentsMargins(0, 0, 0, 0);
	this->textedit = new QTextEdit(this);
	legacyeditorLayout->addWidget(this->textedit);
	this->textedit->setAcceptRichText(false);
	// This needed to avoid the editor accepting filename drops as we want
	// to handle these ourselves in MainWindow
	this->textedit->setAcceptDrops(false);
	this->textedit->setWordWrapMode(QTextOption::WrapAnywhere);
	this->textedit->setTabStopWidth(30);

	this->highlighter = new Highlighter(this->textedit->document());
	std::cout << "use dm editor" << std::endl;

	connect(this->textedit, SIGNAL(textChanged()), this, SIGNAL(contentsChanged()));
	connect(this->textedit->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));

	createSliderAct = new QAction(tr("Create Slider"), this);
	connect(this->createSliderAct, SIGNAL(triggered()), this, SLOT(createSlider()));

	mani_val = 0.0;
	mani_line_no = 0;
	selected_line_no = 0;
	mani_variable = false;
}

// use selected_line_no and selected_var
void MyDMEditor::check_selection() {
	QRegExp num_re("-?[0-9]+([.][0-9]+)?");
	if (num_re.exactMatch(selected_var)) {
		std::cout << "it's a number" << std::endl;
		mani_val = selected_var.toFloat();
		mani_line_no = selected_line_no;
		mani_variable = false;
	} else {
		std::cout << "its' not a number" << std::endl;
		mani_variable = true;
		// we have to find the val of the variable name
		// let's perform a simple heuristic here for testing.
		// 1. find all the lines in the document
		// QTextDocument* doc = this->textedit->document();
		QString var_exp = QString("\\b(%1)\\b=-?[0-9]+([.][0-9]+)?").arg(selected_var);
		QRegExp var_re(var_exp);
		QString plainTextEditContents = this->textedit->toPlainText();
		QStringList lines = plainTextEditContents.split("\n");
		QString desire = QString("%1=").arg(selected_var);
		var_prefix_len = desire.length();
		QList<int> line_nos;

		for (int i = 0; i < lines.length(); i++) {
			if (i > selected_line_no) {
				break;
			}
			int pos = var_re.indexIn(lines[i]);
			if (pos != -1) {
				QStringList slist = var_re.capturedTexts();
				int str_index = lines[i].indexOf(slist[0]);
				var_str_start = str_index + var_prefix_len;
				var_str_end = str_index + slist[0].length();
				QStringList cap_list = slist[0].split("=");
				mani_val_str = cap_list[1];
				mani_val = cap_list[1].toFloat();
				mani_line_no = i;
			}
		}
		



		// for (int i = 0; i < lines.length(); i++) {
		// 	if (lines[i].contains(desire)) {
		// 		line_nos.append(i);
		// 	}
		// }
		// // 2. find the closest line number
		// int min_diff = 100000, diff = 0, closest_line_no = -1;
		// for (int &line_no : line_nos) {
		// 	diff = abs(line_no - selected_line_no);
		// 	if (diff < min_diff) {
		// 		min_diff = diff;
		// 		closest_line_no = line_no;
		// 	}
		// }
		// std::cout << selected_line_no << ", closest line number : " << closest_line_no << std::endl;
		// // 3. parse the declaration and find the declare value..
		// QStringList split_line = lines[closest_line_no].split("=");
		// for (int i = 0; i < split_line.length(); i++) {
		// 	std::cout << split_line[i].toStdString() << std::endl;
		// }
		// float dec_val = split_line[1].toFloat();
		// std::cout << "dec val : " << dec_val << std::endl;

	}
}

void MyDMEditor::update_mani_val(double new_val) {
	if (mani_variable == false) {
		if (this->textedit->textCursor().hasSelection()) {
			std::cout << "has selection" << std::endl;
		} else {
			std::cout << "no selection at all" << std::endl;
		}
		QString new_val_str = QString("%1").arg(new_val);
		int str_len = new_val_str.length();
		// this->textedit->
		this->textedit->insertPlainText(new_val_str);
		for (int k = 0; k < str_len; k++) {
			this->textedit->moveCursor(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor);        
		}	

	} else {
		// move the cursor to "mani_line_no"
		QTextCursor text_cursor(this->textedit->document()->findBlockByLineNumber(mani_line_no));
		this->textedit->setTextCursor(text_cursor);
		// for (int i = 0; i < var_str_start)
		// text_cursor.setPosition(var_str_start, QTextCursor::MoveAnchor);
		for (int i = 0; i < var_str_start; i++) {
			this->textedit->moveCursor(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::MoveAnchor);
		}
		std::cout << mani_val_str.toStdString() << " " << mani_val_str.length() << std::endl;
		// this->textedit->textCursor().movePosition(QTextCursor::MoveOperation::Right,QTextCursor::MoveMode::KeepAnchor, mani_val_str.length());
		for (int i = 0; i < mani_val_str.length(); i++) {
			this->textedit->moveCursor(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor);
		}
		QString new_val_str = QString("%1").arg(new_val);
		int str_len = new_val_str.length();
		mani_val_str = new_val_str;
		
		std::cout << "str len : " << str_len << std::endl;
		this->textedit->insertPlainText(new_val_str);
		// for (int k = 0; k < 4; k++) {
		for (int k = 0; k < str_len; k++) {
			this->textedit->moveCursor(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor);
		}
	}
}

void MyDMEditor::createSlider() {
	std::cout << "create slider " << std::endl;

	// first find out whether what is the value to adjust
	// whether it's directly a value, a variable, or multiple variable...
	check_selection();
	// after check, we know the 
	// 1. start value of the manipulation
	// 2. where to modify the document.....
	QWidget *popup = new QWidget(this);

	QDoubleSpinBox *spinbox = new QDoubleSpinBox(popup);
	spinbox->setValue(mani_val);
	float min_val = (mani_val-2.0 > 0.0)? mani_val-1.0:0.0;
	float max_val = mani_val+2.0;
	spinbox->setRange(min_val, max_val);
	spinbox->setSingleStep(0.05);
	connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(update_mani_val(double)));
	// TODO : check if we can connect two different slot to a single signal.
	// if not, check how to combine both things into one single slot...

	QBoxLayout *popupLayout = new QHBoxLayout(popup);
    popupLayout->setMargin(2);
    // popupLayout->addWidget(slider);
	popupLayout->addWidget(spinbox);
    // popupLayout->addWidget(label);
	QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(popup);
    QMenu *menu = new QMenu(this);
    menu->addAction(action);
	// probably need a bit offset so that it will not occlude the selected variables.
	menu->popup(*slider_pos);
}

void MyDMEditor::indentSelection()
{	
	QTextCursor cursor = this->textedit->textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)), QString(QChar(8233)) + QString("\t"));
	if (txt.endsWith(QString(QChar(8233)) + QString("\t")))
		txt.chop(1);
	txt = QString("\t") + txt;
	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	this->textedit->setTextCursor(cursor);
}

void MyDMEditor::unindentSelection()
{
	QTextCursor cursor = this->textedit->textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)) + QString("\t"), QString(QChar(8233)));
	if (txt.startsWith(QString("\t")))
		txt.remove(0, 1);

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	this->textedit->setTextCursor(cursor);
}

void MyDMEditor::commentSelection()
{
	QTextCursor cursor = this->textedit->textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)), QString(QChar(8233)) + QString("//"));
	if (txt.endsWith(QString(QChar(8233)) + QString("//")))
		txt.chop(2);
	txt = QString("//") + txt;

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	this->textedit->setTextCursor(cursor);

}

void MyDMEditor::uncommentSelection()
{
	QTextCursor cursor = this->textedit->textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)) + QString("//"), QString(QChar(8233)));
	if (txt.startsWith(QString("//")))
		txt.remove(0, 2);

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	this->textedit->setTextCursor(cursor);
}

void MyDMEditor::zoomIn()
{
	// See also QT's implementation in QLegacyEditor.cpp
	QSettingsCached settings;
	QFont tmp_font = this->textedit->font() ;
	if (font().pointSize() >= 1)
		tmp_font.setPointSize(1 + font().pointSize());
	else
		tmp_font.setPointSize(1);
	settings.setValue("editor/fontsize", tmp_font.pointSize());
	this->textedit->setFont(tmp_font);
}

void MyDMEditor::zoomOut()
{

	QSettingsCached settings;
	QFont tmp_font = this->textedit->font();
	if (font().pointSize() >= 2)
		tmp_font.setPointSize(-1 + font().pointSize());
	else
		tmp_font.setPointSize(1);
	settings.setValue("editor/fontsize", tmp_font.pointSize());
	this->textedit->setFont(tmp_font);

}

void MyDMEditor::setPlainText(const QString &text)
{

	int y = this->textedit->verticalScrollBar()->sliderPosition();
	// Save current cursor position
	QTextCursor cursor = this->textedit->textCursor();
	int n = cursor.position();
	this->textedit->setPlainText(text);
	// Restore cursor position
	if (n < text.length()) {
		cursor.setPosition(n);
		this->textedit->setTextCursor(cursor);
		this->textedit->verticalScrollBar()->setSliderPosition(y);
	}
}

void MyDMEditor::highlightError(int error_pos)
{
	highlighter->highlightError(error_pos);
        QTextCursor cursor = this->textedit->textCursor();
        cursor.setPosition(error_pos);
        this->textedit->setTextCursor(cursor);

}

void MyDMEditor::unhighlightLastError()
{
	highlighter->unhighlightLastError();
}

void MyDMEditor::setHighlightScheme(const QString &name)
{
	highlighter->assignFormatsToTokens(name);
	highlighter->rehighlight(); // slow on large files
}

QSize MyDMEditor::sizeHint() const
{
	if (initialSizeHint.width() <= 0) {
		return this->textedit->sizeHint();
	} else {
		return initialSizeHint;
	}
}

void MyDMEditor::setInitialSizeHint(const QSize &size)
{
	initialSizeHint = size;
}
 
QString MyDMEditor::toPlainText()
{
	return this->textedit->toPlainText();
}

void MyDMEditor::insert(const QString &text)
{
	this->textedit->insertPlainText(text);
}

void MyDMEditor::setText(const QString &text)
{
	this->textedit->selectAll();
	this->textedit->insertPlainText(text);
}

bool MyDMEditor::canUndo()
{
    return (this->textedit->document()->availableUndoSteps() != 0);
}

void MyDMEditor::undo()
{
	this->textedit->undo();
}

void MyDMEditor::redo()
{
	this->textedit->redo();
}

void MyDMEditor::cut()
{
	this->textedit->cut();
}

void MyDMEditor::copy()
{
	std::cout << "copy" << std::endl;
	this->textedit->copy();
}

void MyDMEditor::paste()
{
	this->textedit->paste();
}

MyDMEditor::~MyDMEditor()
{
	delete highlighter;
}

void MyDMEditor::replaceSelectedText(const QString &newText)
{
	QTextCursor cursor = this->textedit->textCursor();
	if (cursor.selectedText() != newText) {
		cursor.insertText(newText);
	}
}

void MyDMEditor::replaceAll(const QString &findText, const QString &replaceText)
{
	QTextCursor cursor(this->textedit->textCursor());
	cursor.setPosition(0);
	this->textedit->setTextCursor(cursor);
	this->textedit->find(findText);
	while (this->textedit->textCursor().hasSelection()){
		this->textedit->textCursor().insertText(replaceText);
		this->textedit->find(findText);
	}
}

bool MyDMEditor::findString(const QString & exp, bool findBackwards) const
{
	return this->textedit->find(exp, findBackwards ? QTextDocument::FindBackward : QTextDocument::FindFlags(nullptr));
}

int MyDMEditor::resetFindIndicators(const QString & /*findText*/, bool /*visibility*/)//incomplete-place-holder
{
    int findwordcount = 0;
    // blank see scintillaeditor.cpp
    return findwordcount;
}

bool MyDMEditor::find(const QString &newText, bool /*findNext*/, bool findBackwards)
{
	bool success = this->findString(newText, findBackwards);
	if (!success) { // Implement wrap-around search behavior
		QTextCursor old_cursor = this->textedit->textCursor();
		QTextCursor tmp_cursor = old_cursor;
		tmp_cursor.movePosition(findBackwards ? QTextCursor::End : QTextCursor::Start);
		this->textedit->setTextCursor(tmp_cursor);
		bool success = this->findString(newText, findBackwards);
		if (!success) {
			this->textedit->setTextCursor(old_cursor);
		}
		return success;
	}
	return true;

}

void MyDMEditor::initFont(const QString& family, uint size)
{
	QFont font;
	if (!family.isEmpty()) font.setFamily(family);
	else font.setFixedPitch(true);
	if (size > 0)	font.setPointSize(size);
	font.setStyleHint(QFont::TypeWriter);
	this->textedit->setFont(font);

}

QString MyDMEditor::selectedText()
{
	// std::cout << textedit->textCursor().selectedText().toStdString() << std::endl;
	return textedit->textCursor().selectedText();
}

void MyDMEditor::setContentModified(bool y)
{
	this->textedit->document()->setModified(y);
}

bool MyDMEditor::isContentModified()
{
	return textedit->document()->isModified();
}

QStringList MyDMEditor::colorSchemes()
{
	QStringList colorSchemes;
    
	colorSchemes
	    << "For Light Background"
	    << "For Dark Background"
	    << "Off";
	
	return colorSchemes;
}

void MyDMEditor::mousePressEvent(QMouseEvent *event) {
	std::cout << "press mouse" << std::endl;
	// std::cout << selectedText().toStdString() << std::endl;

	// QTextCursor cur_cursor = this->textedit->textCursor();
	selected_line_no = this->textedit->textCursor().blockNumber();
	selected_col_no = this->textedit->textCursor().columnNumber();
	selected_start = this->textedit->textCursor().selectionStart();
	selected_end = this->textedit->textCursor().selectionEnd();
	std::cout << "start : end -> " << selected_start << " : " << selected_end << std::endl;
	selected_anchor = this->textedit->textCursor().anchor();
	selected_position = this->textedit->textCursor().position();
	selected_var = selectedText();

	QMenu *menu = this->textedit->createStandardContextMenu();
	menu->addAction(createSliderAct);
	slider_pos = new QPoint(mapToGlobal(event->pos()));
	menu->popup(mapToGlobal(event->pos()));
}
#include "EditorLoc.h"

EditorLoc::EditorLoc() {
    first_line = 0;
    first_col = 0;
    last_line = 0;
    last_col = 0;
}

EditorLoc::EditorLoc(int firstLine, int firstCol, int lastLine, int lastCol) {
    first_line = firstLine;
    first_col = firstCol;
    last_line = lastLine;
    last_col = lastCol;
}

EditorLoc::~EditorLoc() {}


bool EditorLoc::inside(int q_first_line, int q_first_col, int q_last_line, int q_last_col) {
	if ((q_first_line >= first_line) && (q_first_col >= first_col) 
	 && (q_last_line <= last_line) && (q_last_col <= last_col)) {
		return true;
	}
	return false;
}
#pragma once

#include <string>

class EditorLoc {
public:
    EditorLoc();
    EditorLoc(int firstLine, int firstCol, int lastLine, int lastCol);
    ~EditorLoc();
    // bool inside(int _first_line, int _first_col, int _last_line, int _last_col);
    bool inside(int select_line_no, int select_col_no);
// private:
    int first_line;
    int first_col;
    int last_line;
    int last_col;
};
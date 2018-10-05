#include "AST.h"
#include <sstream>

const Location Location::NONE(0, 0, 0, 0);

// bool Location::inside(int q_first_line, int q_first_col, int q_last_line, int q_last_col) {
// 	if ((q_first_line >= first_line) && (q_first_col >= first_col) 
// 	 && (q_last_line <= last_line) && (q_last_col <= last_col)) {
// 		return true;
// 	}
// 	return false;
// }

std::ostream &operator<<(std::ostream &stream, const ASTNode &ast)
{
	ast.print(stream, "");
	return stream;
}

std::string ASTNode::dump(const std::string &indent) const
{
	std::stringstream stream;
	print(stream, indent);
	return stream.str();
}

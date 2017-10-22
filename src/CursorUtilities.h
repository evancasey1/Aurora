#ifndef CURSORUTILITIES_H
#define CURSORUTILITIES_H

int getWindowHeight();
int getWindowWidth();
void resizeWindow(int h, int w);
void setAbsoluteCursorPosition(int row, int col);
void resetCommandInputCursorPosition(int current_cursor_pos, int ui_buf_height);
void clearUserInterfaceBuffer(int height);
void moveCursor(int up, int down, int right, int left);
void eraseLines(int num_lines);


#endif
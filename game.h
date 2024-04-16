
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include <ctime>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  int bottom_[BOARD_WIDTH];
  int left_[BOARD_HEIGHT];
  int right_[BOARD_WIDTH];

  int x_;
  int y_;
  int shadow_y_;
  int prev_x;
  int now_;
  int next_;
  int hold_;
  int play_time_min_;
  int play_time_sec_;
  int lines_;
  int x_left;
  int x_right;
  int line_count;

  bool falling_;
  bool push_space;
  bool checked_;
  bool exit_;

  Tetromino *tetromino_now;
  Tetromino *tetromino_next;
  Tetromino *tetromino_hold;

  clock_t start_;
  clock_t end_;

  void checkLine();
  void fillBoard();
  void drawBoard();
  void drawTime();
  void selectNextTetromino();
  void handleInput();
  void findLeft();
  void findRight();
  void findBottom();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif
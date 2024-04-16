#include "game.h"
#include "tetromino.h"
#include "console/console.h"
#include <cstdlib>
#include <ctime>



Game::Game(){
  //멤버 변수 초기화
  x_ = BOARD_WIDTH/2;
  y_ = 1;
  shadow_y_ = 20;
  now_ = rand()%7;
  next_ = rand()%7;
  hold_ = -1;
  play_time_min_ = 0;
  play_time_sec_ = 0;
  lines_ = LINES;
  push_space = false;
  falling_ = false;
  
  for(int i = 0; i < BOARD_WIDTH; i++){
    bottom_[i] = 20;
    for(int j = 0; j < BOARD_HEIGHT; j++){
      board_[i][j] = false;
    }
  }
  start_ = clock(); //시작 시간
}

void Game::findLeft(){
  //테트로미노의 가장 왼쪽 부분 x좌표를 찾는다.
  checked_ = false;
  for(int i = 0; i < tetromino_now -> size(); i++){
    for(int j = 0; j < tetromino_now -> size(); j++){
      if(tetromino_now -> check(i, j)){
        x_left = x_ + i;
        checked_ = true;
        break;
      }
    }
    if(checked_){
      break;
    }
  }
  if(x_left < 1){
    x_++;
  }
  else{
    for(int i = 0; tetromino_now -> size(); i++){
      if(tetromino_now -> check(x_left, i) && board_[x_left - 1][y_ + i - 1]){
        x_++;
        break;
      }
    }
  }
}

void Game::findRight(){
  //테트로미노의 오른쪽 x좌표를 찾는다
  checked_ = false;
  for(int i = tetromino_now -> size() - 1; i >= 0; i--){
    for(int j = 0; j < tetromino_now -> size(); j++){
      if(tetromino_now -> check(i, j)){
        x_right = x_ + i;
        checked_ = true;
        break;
      }
    }
    if(checked_){
      break;
    }
  }
  if(x_right > 10){
    x_--;
  }
  else{
    for(int i = 0; tetromino_now -> size(); i++){
      if(tetromino_now -> check(x_right, i) && board_[x_right][y_ + i - 1]){
        x_--;
        break;
      }
    }
  }
}

void Game::findBottom(){
  //테트로미노의 아래 y좌표를 찾는다.
  checked_ = false;
  for(int i = tetromino_now -> size() - 1; i >= 0; i--){
    for(int j = 0; j < tetromino_now -> size(); j++){
      if(tetromino_now -> check(j, i)){
        if(y_ + i > BOARD_HEIGHT || board_[x_ + j - 1][y_ - 1 + i]){
          fillBoard();
          selectNextTetromino();
          if(push_space){
            push_space = false;
          }
          checked_ = true;
          break;
        }
      }
    }
    if(checked_){
      break;
    }
  }

  checked_ = false;
  shadow_y_ = y_;
  while(true){
    for(int i = tetromino_now -> size() - 1; i >= 0; i--){
      for(int j = 0; j < tetromino_now -> size(); j++){
        if(tetromino_now -> check(j, i)){
          if(board_[x_ + j - 1][shadow_y_ + i] || shadow_y_ + i >= BOARD_HEIGHT){
            checked_ = true;
            break;
          }
        }
      }
      if(checked_)
        break;
    }
    if(checked_)
      break;
    shadow_y_++;
  }
}

void Game::fillBoard(){
  for(int i = 0; i < tetromino_now -> size(); i++){
    for(int j = tetromino_now -> size() - 1; j >= 0; j--){
      if(tetromino_now -> check(i, j)){
        board_[x_ - 1 + i][y_ - 2 + j] = true;
      }
    }
  }
}

void Game::checkLine(){
  line_count = 0;
  for(int i = 0; i < BOARD_HEIGHT; i++){
    for(int j = 0; j < BOARD_WIDTH; j++){
      if(board_[j][i]){
        line_count++;
      }
      if(line_count >= 10){
        lines_--;
        for(int m = 0; m < BOARD_WIDTH; m++){
          bottom_[m]++;
          if(i + 1 < BOARD_HEIGHT && !board_[m][i + 1]){
            bottom_[m]++;
          }
          for(int n = i; n >= 1; n--){
            board_[m][n] = board_[m][n - 1];
          }
        }
      }
    }
    line_count = 0;
  }
}

void Game::drawBoard(){
  for(int i = 0; i < BOARD_WIDTH; i++){
    for(int j = 0; j < BOARD_HEIGHT; j++){
      if(board_[i][j]){
        console::draw(i + 1, j + 1, BLOCK_STRING);
      }
    }
  }
}

void Game::handleInput(){
  //키 입력 제어
  if (console::key(console::K_LEFT)) {
    x_--;
  }
  if (console::key(console::K_RIGHT)) {
    x_++;
  }
  if (console::key(console::K_UP)) {
    y_ = shadow_y_ + 1;
  }
  if(console::key(console::K_SPACE) && !push_space){
    tetromino_hold = tetromino_now -> original();
    push_space = true;
    int tmp = 0;
    if(hold_ >= 0){
      tmp = hold_;
      hold_ = now_;
      now_ = tmp;
      falling_ = false;
    }
    else{
      hold_ = now_;
      selectNextTetromino();
    }
  }
  if(console::key(console::K_X)){
    *tetromino_now = tetromino_now -> rotatedCW();
  }
  if(console::key(console::K_Z)){
    *tetromino_now = tetromino_now -> rotatedCCW();
  }
  if(console::key(console::K_ESC)){
    exit_ = true;
  }
}

void Game::selectNextTetromino(){
  //다음 테트로미노를 선택한다.
  now_ = next_;
  next_ = rand()%7;
  x_ = BOARD_WIDTH / 2;
  y_ = 1;
  falling_ = false;
}

void Game::drawTime(){
  //플레이 시간을 출력한다.
  if(play_time_min_ < 10){
    if(play_time_sec_ < 10){
      console::draw(2, 23, "0" + std::to_string(play_time_min_) + ":0" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 4));
    }
    else{
      console::draw(2, 23, "0" + std::to_string(play_time_min_) + ":" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 5));
    }
  }
  else{
    if(play_time_sec_ < 10){
      console::draw(2, 23, std::to_string(play_time_min_) + ":0" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 4));
    }
    else{
      console::draw(2, 23, std::to_string(play_time_min_) + ":" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 5));
    }
  }
}

  // 게임의 한 프레임을 처리한다.
void Game::update(){
  end_ = clock();
  if(end_ - start_ >= 100/6*DROP_DELAY){
    start_ = end_;
    if(!console::key(console::K_UP))
      y_++;
    play_time_sec_++;
  }
  else if(console::key(console::K_DOWN)){
    y_++;
  }
  if(play_time_sec_ >= 60){
    play_time_sec_ = 0;
    play_time_min_++;
  }
}

  // 게임 화면을 그린다.
void Game::draw(){
  drawTime();
  console::drawBox(0, 0, 11, 21);
  console::drawBox(13, 0, 18, 5);
  console::drawBox(19, 0, 24, 5);
  console::draw(14, 0, "Next");
  console::draw(20, 0, "Hold");

  
  if(!falling_){
    //현재 테트로미노가 떨어지는 중이 아닐 때 새로운 테트로미노를 저장.
    Tetromino::I = *Tetromino::I.original();
    Tetromino::O = *Tetromino::O.original();
    Tetromino::T = *Tetromino::T.original();
    Tetromino::S = *Tetromino::S.original();
    Tetromino::Z = *Tetromino::Z.original();
    Tetromino::J = *Tetromino::J.original();
    Tetromino::L = *Tetromino::L.original();
    if(next_ == 0){
      tetromino_next = &Tetromino::I;
    }
    else if(next_ == 1){
      tetromino_next = &Tetromino::O;
    }
    else if(next_ == 2){
      tetromino_next = &Tetromino::T;
    }
    else if(next_ == 3){
      tetromino_next = &Tetromino::S;
    }
    else if(next_ == 4){
      tetromino_next = &Tetromino::Z;
    }
    else if(next_ == 5){
      tetromino_next = &Tetromino::J;
    }
    else if(next_ == 6){
      tetromino_next = &Tetromino::L;
    }


    if(now_ == 0){
      tetromino_now = &Tetromino::I;
    }
    else if(now_ == 1){
      tetromino_now = &Tetromino::O;
    }
    else if(now_ == 2){
      tetromino_now = &Tetromino::T;
    }
    else if(now_ == 3){
      tetromino_now = &Tetromino::S;
    }
    else if(now_ == 4){
      tetromino_now = &Tetromino::Z;
    }
    else if(now_ == 5){
      tetromino_now = &Tetromino::J;
    }
    else if(now_ == 6){
      tetromino_now = &Tetromino::L;
    }
    falling_ = true;
  }
  int prev_x = x_;
  Tetromino tetromino_prev = *tetromino_now;
  handleInput();
  findLeft();
  findRight();
  if(x_right > 11){
    x_--;
  }
  if(x_left < 1){
    x_++;
  }
  for(int i = 0; i < tetromino_now -> size(); i++){
    for(int j = 0; j < tetromino_now -> size(); j++){
      if(tetromino_now -> check(i, j) && board_[x_ + i - 1][y_ + j - 1]){
        x_ = prev_x;
        *tetromino_now = tetromino_prev;
      }
    }
  }
  findBottom();
  checkLine();
  drawBoard();
  tetromino_now -> drawAt(SHADOW_STRING, x_, shadow_y_);
  tetromino_now -> drawAt(BLOCK_STRING, x_, y_);
  tetromino_next -> original() -> drawAt(BLOCK_STRING, 16 - tetromino_next -> size() / 2, 2);
  tetromino_hold -> drawAt(BLOCK_STRING, 22 - tetromino_hold -> size() / 2, 2);
  
  if(lines_ <= 0){
    lines_ = 0;
    console::draw(3, 9, "You win");
    if(play_time_min_ < 10){
      if(play_time_sec_ < 10){
        console::draw(2, 10, "0" + std::to_string(play_time_min_) + ":0" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 4));
      }
      else{
        console::draw(2, 10, "0" + std::to_string(play_time_min_) + ":" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 5));
      }
    }
    else{
      if(play_time_sec_ < 10){
        console::draw(2, 10, std::to_string(play_time_min_) + ":0" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 4));
      }
      else{
        console::draw(2, 10, std::to_string(play_time_min_) + ":" + std::to_string((double)(end_-start_)/1000 + play_time_sec_).substr(0, 5));
      }
    }
    exit_ = true;
  }
  else{
    for(int i = 0; i < BOARD_WIDTH; i++){
      if(board_[i][0]){
        console::draw(2, 10, "You lose");
        exit_ = true;
        break;
      }
    }
  }
  console::draw(0, 22, std::to_string(lines_) + " lines left");
}

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit(){
  return exit_;
}
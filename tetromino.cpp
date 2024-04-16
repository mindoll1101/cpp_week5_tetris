#include "tetromino.h"
#include <string>
#include "console/console.h"

Tetromino original_I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino original_O("O", 2, "OOOO");
Tetromino original_T("T", 3, "XOXOOOXXX");
Tetromino original_S("S", 3, "XOOOOXXXX");
Tetromino original_Z("Z", 3, "OOXXOOXXX");
Tetromino original_J("J", 3, "OXXOOOXXX");
Tetromino original_L("L", 3, "XXOOOOXXX");

Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");

Tetromino::Tetromino(std::string name, int size, std::string shape){
  size_ = size;
  name_ = name;

  if(name == "I"){
    original_ = &original_I;
  }
  else if(name == "O"){
    original_ = &original_O;
  }
  else if(name == "T"){
    original_ = &original_T;
  }
  else if(name == "S"){
    original_ = &original_S;
  }
  else if(name == "Z"){
    original_ = &original_Z;
  }
  else if(name == "J"){
    original_ = &original_J;
  }
  else if(name == "L"){
    original_ = &original_L;
  }
  stringToBool(shape);
  setFalseState();
}

void Tetromino::stringToBool(std::string shape){
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
      if(shape[i*size_ + j] == 'O'){
        shape_[j][i] = true;
      }
      else if(shape[i*size_ + j] == 'X'){
        shape_[j][i] = false;
      }
    }
  }
}

void Tetromino::setFalseState(){
  int false_count = 0;
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
      if(!shape_[i][j]){
        false_count++;
      }
    }
    if(false_count >= size_){
      false_state_[i] = false;
    }
    else{
      false_state_[i] = true;
    }
    false_count = 0;
  }
}
  // 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW(){
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
        rotated_shape_[size_ - 1 - j][i] = shape_[i][j];
    }
  }
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
      shape_[i][j] = rotated_shape_[i][j];
    }
  }
  setFalseState();
  return *this;
}

// 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW(){
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
      rotated_shape_[j][size_ - 1 - i] = shape_[i][j];
    }
  }
  for(int i = 0; i < size_; i++){
    for(int j = 0; j < size_; j++){
      shape_[i][j] = rotated_shape_[i][j];
    }
  }
  setFalseState();
  return *this;
}

  // 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
void Tetromino::drawAt(std::string s, int x, int y){
  int true_count = 0;
  for(int i = 0; i < size_; i++){
    if(false_state_[i]){
      for(int j = 0; j < size_; j++){
        if(shape_[i][j]){
          console::draw(x + i, y + j, s);
        }
      }
      true_count++;
    }
  }
}



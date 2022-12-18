#include <HT1632.h>

int JoyStick_X = A1; //x
int JoyStick_Y = A0; //y 
int JoyStick_Z = 5; //key

HT1632Class LEDmatrix;

class Snake
{
public:
  static const int RIGHT = 0;
  static const int LEFT = 1;
  static const int UP = 2;
  static const int DOWN = 3;
  static const int X_MAX = 32;
  static const int Y_MAX = 8;

  Snake()
  {
    _cur_ts = millis();
    _ts_int = 200;
    randomSeed(_cur_ts);
    _snake[0].x = random(X_MAX);
    _snake[0].y = random(Y_MAX);
    gen_new_target();
    draw();
    show();
  }

  bool ready()
  {
    bool ready = false;
    unsigned long ts = millis();
    if ((ts - _cur_ts) > _ts_int)
    {
      ready = true;
      _cur_ts = ts;
    }
    return ready;
  }

  void correct_dir(int dir)
  {
    if (((dir == LEFT) && (_cur_dir == RIGHT)) ||
        ((dir == RIGHT) && (_cur_dir == LEFT)) ||
        ((dir == UP) && (_cur_dir == DOWN)) ||
        ((dir == DOWN) && (_cur_dir == UP)))
    {
      Serial.println("correct dir, conflict");
      dir = _cur_dir;
    }
    _cur_dir = dir;
  }

  void update(int dir)
  {
    correct_dir(dir);
  
    clear();
    update_tail();
    update_head();

    if (colide())
    {
      Serial.println("colide");
      delay(5000);
      clear();
      LEDmatrix.printChar(0, '0' + _snake_len/10);
      LEDmatrix.printChar(1, '0' + _snake_len%10);
      LEDmatrix.printChar(2, '#');
      LEDmatrix.printChar(3, '!');
      show();
      while (1);
      return;
    }
    target();

    draw();
    show();
  }

  void update_head()
  {
    switch (_cur_dir)
    {
      case LEFT:
        _snake[0].x -= 1;
        break;
      case RIGHT:
        _snake[0].x += 1;
        break;
      case UP:
        _snake[0].y -= 1;
        break;
      case DOWN:
        _snake[0].y += 1;
        break;
      default:
        Serial.println("unknown DIR, do nothing");
    }
  }

  void update_tail()
  {
    for (int i = _snake_len - 1; i >= 0; i--)
    {
      _snake[i + 1].x = _snake[i].x;
      _snake[i + 1].y = _snake[i].y;
    }
  }

  bool colide()
  {
    // wall
    if ((_snake[0].x < 0) || (_snake[0].x >= X_MAX) ||
        (_snake[0].y < 0) || (_snake[0].y >= Y_MAX))
        {
          Serial.println("hit wall");
          return true;
        }
    // snake itself
    for (int i = 4; i < _snake_len; i++)
    {
      if ((_snake[0].x == _snake[i].x) && (_snake[0].y == _snake[i].y))
      {
        Serial.println("hit itself");
        return true;
      }
    }
    return false;
  }

  void gen_new_target()
  {
    while (1)
    {
      randomSeed(millis());
      _target.x = random(X_MAX);
      _target.y = random(Y_MAX);

      int i = 0;
      for (i = 0; i < _snake_len; i++)
      {
        if ((_snake[i].x == _target.x) && (_snake[i].y == _target.y))
        {
          break;          
        }
      }

      if (i == _snake_len)
      {
        Serial.println("gen new target");
        return;
      }
    }        
  }

  void target()
  {
    if ((_snake[0].x == _target.x) && (_snake[0].y == _target.y))
    {
      if (_snake_len < SNAKE_MAX_LENGTH)
      {
        _snake_len += 1;
        Serial.println("get target");
      }
      else
      {
        Serial.println("stop, max big");
      }

      gen_new_target();
    }
  }

  void clear()
  {
    for (int i = 0; i < _snake_len; i++)
    {
      LEDmatrix.clearPixel(_snake[i].x, _snake[i].y);
    }
  }

  void draw()
  {
    for (int i = 0; i < _snake_len; i++)
    {
      LEDmatrix.setPixel(_snake[i].x, _snake[i].y);
    }

    LEDmatrix.setPixel(_target.x, _target.y);
  }

  void show()
  {
    LEDmatrix.render();
  }

private:
  struct _pos
  {
    int x;
    int y;
  };
  static const int SNAKE_MAX_LENGTH = 30;
  _pos _snake[SNAKE_MAX_LENGTH + 1];
  int _snake_len = 1;
  _pos _target;
  unsigned long _cur_ts = 0;
  unsigned long _ts_int = 0;
  int _cur_dir = -1;
};

void setup() 
{
  pinMode(JoyStick_Z, INPUT); 
  Serial.begin(9600); // 9600 bps

  LEDmatrix.begin(10,13,11);
  LEDmatrix.setBrightness(10);
  LEDmatrix.clear();
}
void loop() 
{
  Snake snake;
  int dir = -1;

  while(1)
  {
    int x,y,z;    
    x=analogRead(JoyStick_X);
    y=analogRead(JoyStick_Y);
    z=digitalRead(JoyStick_Z);
    /*
    Serial.print(x ,DEC);
    Serial.print(",");
    Serial.print(y ,DEC);
    Serial.print(",");
    Serial.println(z ,DEC);
    */
    if (x < 100)
    {
      dir = Snake::UP;
    }
    else if (x > 600)
    {
      dir = Snake::DOWN;
    }
    else if (y < 100)
    {
      dir = Snake::RIGHT;
    }
    else if (y > 600)
    {
      dir = Snake::LEFT;
    }

    if (snake.ready())
    {
      snake.update(dir);
    }
  }
}

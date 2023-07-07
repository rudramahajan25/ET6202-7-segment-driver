int stb = 4;
int sck = 3;
int dat = 2;

int clockspeed = 0;
long counter = 0;
unsigned long timer;

byte disp[14];

byte anim = 0;

void setup()
{
  pinMode(stb, OUTPUT);      // sets the digital pin as output
  pinMode(sck, OUTPUT);      // sets the digital pin as output
  pinMode(dat, OUTPUT);      // sets the digital pin as output
  // sets the digital pin as output

  for (byte a = 0; a < 14; a++)
  {
    disp[a] = 0;
  }
  disp[6] = 1 << 5;
  draw();

  randomSeed(analogRead(0));
  Serial.begin(9600);

  timer = millis();

  reset_chip();
  disp[6] = 0;
  draw();
}

void clock()
{
    digitalWrite(sck, LOW);
    delay(clockspeed);
    digitalWrite(sck, HIGH);
    delay(clockspeed);
}

void send_data(byte data)
{
  if (data & 1) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 2) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 4) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 8) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 16) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 32) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 64) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
  if (data & 128) digitalWrite(dat, HIGH); else digitalWrite(dat, LOW);
  clock();
}

void setAddress(byte address)
{
  address &= B00001111;
  
  digitalWrite(stb, HIGH);
  clock();
  digitalWrite(stb, LOW);
  send_data(B11000000 + address);
}
  
void displayControl(int brightness)
{
  brightness &= B00000111;
  
  digitalWrite(stb, HIGH);
  clock();
  digitalWrite(stb, LOW);
  send_data(B10001000 | brightness);
}

void displayMode()
{
  digitalWrite(stb, HIGH);
  clock();
  digitalWrite(stb, LOW);
  send_data(B11000011);
}

void orderData()
{
  digitalWrite(stb, HIGH);
  clock();
  digitalWrite(stb, LOW);
  send_data(B01000000);
}


void reset_chip()
{
  byte randNumber;
  
  for (int a = 0; a < 200; a++)
  {
    digitalWrite(stb, HIGH);
    digitalWrite(stb, LOW);
  
    randNumber = a; //random(256);
    send_data(randNumber);
  }
}

void draw()
{
  displayMode();
  orderData();
  setAddress(0);
  for (byte a = 0; a < 14; a++)
  {
    send_data(disp[a]);
  }  
  displayControl(7);
}

void clear()
{
  displayMode();
  orderData();
  setAddress(0);
  for (byte a = 0; a < 14; a++)
  {
    send_data(0);
  }
}

void show_digit(byte pos, bool a, bool b, bool c, bool d, bool e, bool f, bool g)
{
  byte add = 0;
  byte suffix = 0;

  switch (pos)
  {
    case 0:
      suffix = 1;
      break;
    case 1:
      suffix = 0;
      break;
    case 2:
      suffix = 2;
      break;
    case 3:
      suffix = 7;
      break;
    case 4:
      suffix = 0;
      add = 1;
      break;
  }

  if (a) disp[12+add] |= 1 << suffix; else disp[12+add] &= ~(1 << suffix);
  if (b) disp[2+add] |= 1 << suffix; else disp[2+add] &= ~(1 << suffix);
  if (c) disp[4+add] |= 1 << suffix; else disp[4+add] &= ~(1 << suffix);
  if (d) disp[6+add] |= 1 << suffix; else disp[6+add] &= ~(1 << suffix);
  if (e) disp[8+add] |= 1 << suffix; else disp[8+add] &= ~(1 << suffix);
  if (f) disp[10+add] |= 1 << suffix; else disp[10+add] &= ~(1 << suffix);
  if (g) disp[add] |= 1 << suffix; else disp[add] &= ~(1 << suffix);
}

void colon_anim()
{
  if (((millis() - timer) / 500) % 2)
  {
    disp[2] |= 1 << 3;
    disp[11] |= 1 << 1;
  } else {
    disp[2] &= ~(1 << 3);
    disp[11] &= ~(1 << 1);
  }
}

void print_char(char c, byte pos)
{
  switch (c)
  {
    case '0':
    case 'D':
    case 'O':
      show_digit(pos, true, true, true, true, true, true, false);
      break;
    case '1':
      show_digit(pos, false, false, false, false, true, true, false);
      break;
    case '2':
      show_digit(pos, true, false, true, true, false, true, true);
      break;
    case '3':
      show_digit(pos, true, false, false, true, true, true, true);
      break;
    case '4':
      show_digit(pos, false, true, false, false, true, true, true);
      break;
    case '5':
      show_digit(pos, true, true, false, true, true, false, true);
      break;
    case '6':
      show_digit(pos, true, true, true, true, true, false, true);
      break;
    case '7':
      show_digit(pos, true, false, false, false, true, true, false);
      break;
    case '8':
    case 'B':
      show_digit(pos, true, true, true, true, true, true, true);
      break;
    case '9':
    case 'g':
      show_digit(pos, true, true, false, true, true, true, true);
      break;
    case 'A':
    case 'a':
      show_digit(pos, true, true, true, false, true, true, true);
      break;
    case 'b':
      show_digit(pos, false, true, true, true, true, false, true);
      break;
    case 'c':
      show_digit(pos, false, false, true, true, false, false, true);
      break;
    case 'd':
      show_digit(pos, false, false, true, true, true, true, true);
      break;
    case 'e':
    case 'E':
      show_digit(pos, true, true, true, true, false, false, true);
      break;
    case 'f':
    case 'F':
      show_digit(pos, true, true, true, false, false, false, true);
      break;
    case 'h':
      show_digit(pos, false, true, true, false, true, false, true);
      break;
    case 'i':
      show_digit(pos, false, false, true, false, false, false, false);
      break;
    case 'j':
    case 'J':
      show_digit(pos, false, false, true, true, true, true, false);
      break;
    case 'k':
      show_digit(pos, false, true, true, true, false, false, true);
      break;
    case 'l':
    case 'L':
      show_digit(pos, false, true, true, true, false, false, false);
      break;
    case 'M':
    case 'N':
      show_digit(pos, true, true, true, false, true, true, false);
      break;
    case 'm':
    case 'n':
      show_digit(pos, false, false, true, false, true, false, true);
      break;
    case 'o':
      show_digit(pos, false, false, true, true, true, false, true);
      break;
    case 'p':
    case 'P':
      show_digit(pos, true, true, true, false, false, true, true);
      break;
 }
}

void print_digit(byte c, byte pos)
{
  switch (c)
  {
    case 0:
      print_char('0', pos);
      break;
    case 1:
      print_char('1', pos);
      break;
    case 2:
      print_char('2', pos);
      break;
    case 3:
      print_char('3', pos);
      break;
    case 4:
      print_char('4', pos);
      break;
    case 5:
      print_char('5', pos);
      break;
    case 6:
      print_char('6', pos);
      break;
    case 7:
      print_char('7', pos);
      break;
    case 8:
      print_char('8', pos);
      break;
    case 9:
      print_char('9', pos);
      break;
  }
}

void partition(byte pos, bool on)
{
  byte prefix;  
  switch (pos)
  {
    case 0:
      prefix = 10;
      break;
    case 1:
      prefix = 2;
      break;
    case 2:
      prefix = 0;
      break;
    case 3:
      prefix = 6;
      break;
    case 4:
      prefix = 4;
      break;
    case 5:
      prefix = 12;
      break;
  }

  if (on)
  {
    disp[prefix] |= B00010000;
  } else {
    disp[prefix] &= B11101111;
  }
}

void print_long(long value)
{
  for (byte i = 0; i < 5; i++)
  {
    if (value != 0 || i == 0)
    {
      print_digit(value % 10, 4-i);
      value /= 10;
    } else {
      print_char(' ', 4-i);
    }
  }
}

void loop()
{

  anim++;
  if (anim == 60)
  {
    anim = 0;
    partition(5, false);
  } else {
    partition((anim-1)/10, false);
  }
  partition((anim/10), true);

  if (Serial.available() > 0) {
    print_char(Serial.read(), 0);
    delay(800);
  }

  counter++;
  print_long((millis() - timer)/1000);

  colon_anim();

  draw();

  // reset_chip();

}

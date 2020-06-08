//  VTR emulator for DNF ST300 controller
//  Sony 9pin protocol
//
// Ferbi, 7.6.2020

const uint8_t buff_size = 40;
byte buff[buff_size];
int dolzina = 0;
int index = 0;

bool play = false;
bool rwd = false;
bool ffwd = false;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(38400, SERIAL_8O1);

  Serial.println("going");
  delay(2000);
  Serial.println("listening  RAW...");
}

void loop()
{

  // there are bytes in the serial buffer to read
  if (Serial1.available() > 0) {
    buff[index] = Serial1.read();
    //Serial.print(buff[index] , HEX);

    index++;
    if (index >= buff_size) {
      index = 0;                            //out of bounds. input error?
    }
  }

  // sense_req_1
  // 61 20 01 82
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x61 && buff[i + 1] == 0x20 && buff[i + 2] == 0x01  && buff[i + 3] == 0x82 ) {
        //Serial.print("- sense REQ1 + 1 data: ");
        reply(1);
        clear();
        break;
      }
    }
  }

  // sense_req_2
  // 61 20 06 87
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x61 && buff[i + 1] == 0x20 && buff[i + 2] == 0x06  && buff[i + 3] == 0x87 ) {
        //Serial.print("- sense REQ2 + 1 data: ");
        reply(2);
        clear();
        break;
      }
    }
  }

  // data_req
  // 30 02 1a c0 00 03 0f
  dolzina = 7;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x30 && buff[i + 1] == 0x02 && buff[i + 2] == 0x1A  && buff[i + 3] == 0xC0 &&
           buff[i + 4] == 0x00 && buff[i + 5] == 0x03 && buff[i + 6] == 0x0F ) {
        //Serial.print("- data REQ: ");
        reply(3);
        clear();
        break;
      }
    }
  }

  // preset_req
  // 41 30 00 71
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x41 && buff[i + 1] == 0x30 && buff[i + 2] == 0x00  && buff[i + 3] == 0x71 ) {
        //Serial.print("- preset REQ: ");
        reply(4);
        clear();
        break;
      }
    }
  }

  // play
  // 20 01 21
  dolzina = 3;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x20 && buff[i + 1] == 0x01 && buff[i + 2] == 0x21 ) {
        //Serial.println("- play REQ: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (play)");
        play = true;
        rwd = false;
        ffwd = false;
        clear();
        break;
      }
    }
  }

  // stop 1
  // 20 00 20
  dolzina = 3;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x20 && buff[i + 1] == 0x00 && buff[i + 2] == 0x20 ) {
        //Serial.println("- stop1 REQ: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (stop)");
        play = false;
        rwd = false;
        ffwd = false;
        clear();
        break;
      }
    }
  }

  // stop 2
  // 21 11 00 32
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x21 && buff[i + 1] == 0x11 && buff[i + 2] == 0x00 && buff[i + 3] == 0x32 ) {
        //Serial.println("- stop2 REQ: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (stop)");
        play = false;
        rwd = false;
        ffwd = false;
        clear();
        break;
      }
    }
  }

  // ffwd
  // 20 10 30
  // 21 13 53 87 (tipka)
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x21 && buff[i + 1] == 0x13 && buff[i + 2] == 0x53 && buff[i + 3] == 0x87 ) {
        //Serial.println("- ffwd REQ: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (ffwd)");
        play = false;
        rwd = false;
        ffwd = true;
        clear();
        break;
      }
    }
  }

  // rwd
  // 20 20 40
  // 21 23 53 97 (tipka)
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x21 && buff[i + 1] == 0x23 && buff[i + 2] == 0x53 && buff[i + 3] == 0x97 ) {
        //Serial.println("- rwd REQ: ");
        //Serial.println(paketi++);
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (rwd)");
        play = false;
        rwd = true;
        ffwd = false;
        clear();
        break;
      }
    }
  }

  // recue
  // 41 36 00 77 (tipka)
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x41 && buff[i + 1] == 0x36 && buff[i + 2] == 0x00 && buff[i + 3] == 0x77 ) {
        reply(5);
        buff_print(index, dolzina);
        Serial.println("- (recue)");
        play = false;
        rwd = false;
        ffwd = false;
        clear();
        break;
      }
    }
  }

  // jog & shuttle find
  // 21 11 DATA CRC
  // 21 21 DATA CRC
  // 21 13 DATA CRC
  // 21 23 DATA CRC
  //
  // VALUE = 10^(DATA_in_decimal/32-2)
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( (buff[i] == 0x21 && buff[i + 1] == 0x11) ||
           (buff[i] == 0x21 && buff[i + 1] == 0x21) ||
           (buff[i] == 0x21 && buff[i + 1] == 0x13) ||
           (buff[i] == 0x21 && buff[i + 1] == 0x23) )  {
        //Serial.println("- jog/shuttle FIND: ");
        reply(4);
        buff_print(index, dolzina);
        if ( buff[i + 1] == 0x11 ) {
          Serial.println("- (jog+)");
        }
        if ( buff[i + 1] == 0x13 ) {
          Serial.println("- (shuttle+)");
        }
        if ( buff[i + 1] == 0x21 ) {
          Serial.println("- (jog-)");
        }
        if ( buff[i + 1] == 0x23 ) {
          Serial.println("- (shuttle-)");
        }
        play = false;
        if ( buff[i + 1] == 0x11 || buff[i + 1] == 0x13) ffwd = true;
        else ffwd = false;
        if ( buff[i + 1] == 0x21 || buff[i + 1] == 0x23) rwd = true;
        else rwd = false;
        if ( buff[i + 2] == 0) {
          ffwd = false;
          rwd = false;
        }

        clear();
        break;
      }
    }
  }

  // t-bar find
  // 21 12 DATA CRC
  //
  // VALUE = 10^(DATA_in_decimal/32-2)
  dolzina = 4;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( (buff[i] == 0x21 && buff[i + 1] == 0x12) )  {
        //Serial.println("- t-bar FIND: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (t-bar)");
        play = false;
        rwd = false;
        if ( buff[i + 2] > 0) ffwd = true;
        else ffwd = false;
        clear();
        break;
      }
    }
  }

  // record / goto
  // 30 03 1A 82 00 00 18 E7
  dolzina = 8;
  if ( index >= dolzina ) {
    for (int i = 0; i <= index - dolzina; i++) {
      if ( buff[i] == 0x30 && buff[i + 1] == 0x03 && buff[i + 2] == 0x1A  && buff[i + 3] == 0x82 &&
           buff[i + 4] == 0x00 && buff[i + 5] == 0x00 && buff[i + 6] == 0x18 && buff[i + 7] == 0xE7 ) {
        //Serial.print("- record REQ: ");
        reply(4);
        buff_print(index, dolzina);
        Serial.println("- (record/go to)");
        clear();
        break;
      }
    }
  }

}

void reply(int tip) {

  if (tip == 1) {
    // 71 20 00 91
    byte message[] = { 0x71, 0x20, 0x00, 0x91 };
    Serial1.write(message, sizeof(message));
    //Serial.println("- sense1 RET ");
  }

  else if (tip == 2) {
    // 76 20 00 a0 00 00 00 00 36
    byte message0[] = {0x76, 0x20, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x36};
    // change status bytes for LED on controller
    if (play)      {
      message0[3] = 0x01;
      message0[8] = 0x97;
    }
    else if (rwd)  {
      message0[3] = 0x08;
      message0[8] = 0x9E;
    }
    else if (ffwd) {
      message0[3] = 0x04;
      message0[8] = 0x9A;
    }
    Serial1.write(message0, sizeof(message0));
    //Serial.println("- sense2 RET ");
  }

  else if (tip == 3) {
    // 30 0f 1b c0 00 03 04 10 29 32 02 23 32 38 02 00 00 00 00 1d
    //byte message1[] = { 0x30, 0x0f, 0x1b, 0xc0, 0x00, 0x03, 0x04, 0x10, 0x29, 0x32, 0x02, 0x23, 0x32, 0x38, 0x02, 0x00, 0x00, 0x00, 0x00, 0x1d };
    byte message1[] = { 0x30, 0x0f, 0x1b, 0xc0, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x18, 0x41, 0x38, 0x02, 0x00, 0x00, 0x00, 0x00, 0xb4 };
    Serial1.write(message1, sizeof(message1));
    //Serial.println("- preset RET ");
  }

  else if (tip == 5) {
    // 30 02 1A C0 00 03 0F
    byte message1[] = { 0x30, 0x02, 0x1a, 0xc0, 0x00, 0x03, 0x0f };
    Serial1.write(message1, sizeof(message1));
    //Serial.println("- preset RET ");
  }

  else if (tip == 4) {
    // 10 01 11
    byte message2[] = { 0x10, 0x01, 0x11 };
    Serial1.write(message2, sizeof(message2));
    //Serial.println("- ack RET ");
  }

  else {
    Serial.print("unknown state - error: ");
    Serial.println(tip);
  }
}

void clear() {
  memset(buff, 0, sizeof(buff));
  index = 0;
}

void buff_print(int neki, int dolz) {
  for (int i = neki - dolzina; i < neki; i++) {
    Serial.print(buff[i], HEX);
    Serial.print(" ");
  }
}

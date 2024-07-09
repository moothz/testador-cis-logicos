//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------TFT Screen functions-----------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tft_init()
{
  tft.reset();
  uint16_t identifier = tft.readID();
  if (identifier == 0x0101) identifier = 0x9341;
  tft.begin(identifier);
  Serial.println("TFT initialized!!");
  tft.setRotation(2);//3
  screenStatus = 1;
}
//----------------------------------------------------------------------------------------------------------------------//
// ----------------------------------------------StartUp/Mode screen----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void modeScreen() //Main menu screen
{
  tft.fillScreen(NUPEDEE_AZUL_CLARO);
  tft.setCursor(10, 10);
  tft.setTextColor(NUPEDEE_CINZINHA);  tft.setTextSize(4);
  tft.println(F(" NUPEDEE "));
  tft.setCursor(12, 12);
  tft.setTextColor(NUPEDEE_AZUL_FORTE);  tft.setTextSize(4);
  tft.println(F(" NUPEDEE "));

  for (int i = 0; i < 5; i++) {
    tft.drawFastHLine(32, 45 + i, 170, NUPEDEE_AMARELO);
  }

  tft.setCursor(26, 55);
  tft.setTextColor(NUPEDEE_AZUL_FORTE);  tft.setTextSize(2);
  tft.println(F("Testador de CIs"));

  tft.setCursor(5, 90);
  tft.setTextColor(NUPEDEE_AZUL_FORTE);  tft.setTextSize(3);
  tft.println(F("Selecione:"));
  tft.setCursor(8, 91);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println(F("Selecione:"));


  //  buttonsMenus[4].initButton(&tft, 120, 100, 200, 30, GREY, BLACK, BLUE, "LastTest", 2); //last test menu button
  //  buttonsMenus[4].drawButton();
  //  if (switches.fastMode == 0)
  //  {
  //    buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, WHITE, GREY, BLACK, "Fast OFF", 2); //faster test/search mode OFF
  //    buttonsMenus[1].drawButton();
  //  }
  //  if (switches.fastMode == 1)
  //  {
  //    buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, WHITE, GREY, BLACK, "Fast ON", 2); //faster test/search mode ON
  //    buttonsMenus[1].drawButton(true);
  //  }

  buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, WHITE, WHITE, NUPEDEE_AZUL_FORTE, "Comuns", 2); //faster test/search mode ON
  buttonsMenus[1].drawButton(true);
  buttonsMenus[3].initButton(&tft, 120, 180, 200, 30, WHITE, WHITE, BLACK, "Digitar", 2); //search mode
  buttonsMenus[3].drawButton();
  buttonsMenus[0].initButton(&tft, 120, 220, 200, 30, WHITE, WHITE, BLACK, "Detectar", 2); //test mode
  buttonsMenus[0].drawButton();
  buttonsMenus[2].initButton(&tft, 120, 260, 200, 30, WHITE, WHITE, BLACK, "Ver Pinos", 2); //pinout mode
  buttonsMenus[2].drawButton();
  buttonsMenus[5].initButton(&tft, 120, 300, 200, 30, BLACK, BLUE, WHITE, "Config", 2); //configuration
  buttonsMenus[5].drawButton();
}
//----------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------Configuration--------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void configScreen() //Main menu screen
{
  tft.fillScreen(BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));

  tft.setCursor(3, 56);
  tft.setTextColor(BLUE);  tft.setTextSize(3);
  tft.println(F("Configuration"));
  tft.setCursor(5, 55);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println(F("Configuration"));

  tft.setCursor(10, 95);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Last IC Displayed On Keypad"));

  tft.setCursor(10, 125);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Enable Input With Clock"));

  tft.setCursor(10, 155);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Clear CSV After Each Run"));

  tft.setCursor(10, 185);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Auto set to diagram"));

  tft.setCursor(10, 245);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Speed Of Clock: "));

  tft.setCursor(115, 243);
  tft.setTextColor(CYAN);  tft.setTextSize(2);
  speed = map(speed, 255, 2, 1, 10);
  tft.println(speed);
  speed = map(speed, 1, 10, 255, 2);
  tft.setCursor(10, 215);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("Free Memory: "));

  tft.setCursor(10, 275);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println(F("ScreenSaver: "));

  tft.setCursor(185, 213);
  if (freeMemory() >= 1000) {
    tft.setTextColor(CYAN);
  }
  if (freeMemory() <  1000) {
    tft.setTextColor(RED);
  }
  tft.setTextSize(2);
  tft.println(freeMemory());

  if (switches.config1 == 1) {
    buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "ON", 2);   //last IC keypad ON
    buttonsMenus[4].drawButton(true);
    switches.keypadIC = 1;
    switches.config1Status = 1;
  }
  if (switches.config1 == 0) {
    buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "OFF", 2);  //last IC keypad OFF
    buttonsMenus[4].drawButton();
    switches.keypadIC = 0;
    switches.config1Status = 0;
  }
  if (switches.config2 == 1) {
    buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "ON", 2);   //auto pushbuttons ON
    buttonsMenus[5].drawButton(true);
    switches.pushingButtons = 1;
    switches.config2Status = 1;
  }
  if (switches.config2 == 0) {
    buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "OFF", 2);  //auto pushbuttons OFF
    buttonsMenus[5].drawButton();
    switches.pushingButtons = 0;
    switches.config2Status = 0;
  }
  if (switches.config3 == 1) {
    buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "ON", 2);   //clear CSV ON
    buttonsMenus[7].drawButton(true);
    switches.clearCSV = 1;
    switches.config3Status = 1;
  }
  if (switches.config3 == 0) {
    buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "OFF", 2);  //clear CSV OFF
    buttonsMenus[7].drawButton();
    switches.clearCSV = 0;
    switches.config3Status = 0;
  }
  if (switches.config4 == 1) {
    buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "ON", 2);   //hold buttons ON
    buttonsMenus[8].drawButton(true);
    switches.diagram = 1;
    switches.config4Status = 1;
  }
  if (switches.config4 == 0) {
    buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "OFF", 2);  //hold buttons OFF
    buttonsMenus[8].drawButton();
    switches.diagram = 0;
    switches.config4Status = 0;
  }
  if (switches.config5 == 1) {
    buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "ON", 2);   //hold buttons ON
    buttonsMenus[9].drawButton(true);
    switches.macgyver = 1;
    switches.config5Status = 1;
  }
  if (switches.config5 == 0) {
    buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "OFF", 2);  //hold buttons OFF
    buttonsMenus[9].drawButton();
    switches.macgyver = 0;
    switches.config5Status = 0;
  }

  buttonsMenus[6].initButton(&tft, 200, 250, 70, 20, GREY, BLACK, WHITE, "Speed", 2); //change the clock speed
  buttonsMenus[6].drawButton();
  buttonsMenus[0].initButton(&tft, 120, 300, 100, 30, WHITE, GREY, BLACK, "Save", 3);// go back to main menu
  buttonsMenus[0].drawButton();
}
//----------------------------------------------------------------------------------------------------------------------//
//------------------------------------------Sign of life for the home screen--------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void mainScreenDemoScene()
{
  tft.setTextSize(4);
  if (fade >= 1900) {
    fade = 100;
  }
  tft.setCursor(10, 10); tft.setTextColor(NUPEDEE_AMARELO);
  if (fade == 100) {
    tft.println(F(" "));
  }
  if (fade == 200) {
    tft.println(F(" N"));
  }
  if (fade == 300) {
    tft.println(F("  U"));
  }
  if (fade == 400) {
    tft.println(F("   P"));
  }
  if (fade == 500) {
    tft.println(F("    E"));
  }
  if (fade == 600) {
    tft.println(F("     D"));
  }
  if (fade == 700) {
    tft.println(F("      E"));
  }
  if (fade == 800) {
    tft.println(F("       E"));
  }
  if (fade == 900) {
    tft.println(F("         "));
  }
  if (fade == 1800) {
    tft.println(F(" "));
  }
  if (fade == 1700) {
    tft.println(F(" N"));
  }
  if (fade == 1600) {
    tft.println(F("  U"));
  }
  if (fade == 1500) {
    tft.println(F("   P"));
  }
  if (fade == 1400) {
    tft.println(F("    E"));
  }
  if (fade == 1300) {
    tft.println(F("     D"));
  }
  if (fade == 1200) {
    tft.println(F("      E"));
  }
  if (fade == 1100) {
    tft.println(F("       E"));
  }
  if (fade == 1000) {
    tft.println(F("         "));
  }
  tft.setTextColor(NUPEDEE_AZUL_FORTE);
  if (fade == 150) {
    tft.println(F(" "));
  }
  if (fade == 250) {
    tft.println(F(" N"));
  }
  if (fade == 350) {
    tft.println(F("  U"));
  }
  if (fade == 450) {
    tft.println(F("   P"));
  }
  if (fade == 550) {
    tft.println(F("    E"));
  }
  if (fade == 650) {
    tft.println(F("     D"));
  }
  if (fade == 750) {
    tft.println(F("      E"));
  }
  if (fade == 850) {
    tft.println(F("       E"));
  }
  if (fade == 950) {
    tft.println(F("         "));
  }
  if (fade == 1850) {
    tft.println(F(" "));
  }
  if (fade == 1750) {
    tft.println(F(" N"));
  }
  if (fade == 1650) {
    tft.println(F("  U"));
  }
  if (fade == 1550) {
    tft.println(F("   P"));
  }
  if (fade == 1450) {
    tft.println(F("    E"));
  }
  if (fade == 1350) {
    tft.println(F("     D"));
  }
  if (fade == 1250) {
    tft.println(F("      E"));
  }
  if (fade == 1150) {
    tft.println(F("       E"));
  }
  if (fade == 1050) {
    tft.println(F("         "));
  }
  fade = fade + 1;
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------Test completed "graphics"----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void testCompleted()
{
  if (fade >= 150) {
    fade = 0;
  }
  if (fade == 0)
  {
    tft.setCursor(5, 45);
    tft.setTextColor(WHITE);  tft.setTextSize(3);
    tft.println(F("Test Complete"));
    tft.setCursor(6, 44);
    tft.setTextColor(GREY);  tft.setTextSize(3);
    tft.println(F("Test Complete"));
  }
  if (fade == 140)
  {
    tft.setCursor(5, 45);
    tft.setTextColor(BLUE);  tft.setTextSize(3);
    tft.println(F("Test Complete"));
    tft.setCursor(6, 44);
    tft.setTextColor(RED);  tft.setTextSize(3);
    tft.println(F("Test Complete"));
  }
  fade = fade + 1;
}
//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------Truthtable/Diagram buttons----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void truthtableButtons()
{
  tft.setTextSize(1);
  tft.setCursor(170, 267); tft.setTextColor(YELLOW);
  tft.println(F("Cycles:"));
  tft.fillRect(208, 260 , 70, 20, BLACK);
  tft.setCursor(210, 267); tft.setTextColor(YELLOW);
  tft.println(cycle);
  buttonsMenus[0].initButton(&tft, 20, 305, 30, 30, WHITE, GREY, BLACK, "M", 2);
  buttonsMenus[0].drawButton();
  if (switches.clockmenuToggle == 1)
  {
    buttonsMenus[1].initButton(&tft, 120, 305, 70, 30, BLACK, BLUE, WHITE, "Clock", 2);
    buttonsMenus[1].drawButton();
  }
  buttonsMenus[2].initButton(&tft, 200, 305, 70, 30, BLACK, BLUE, WHITE, "Cycles", 2);
  buttonsMenus[2].drawButton();
  buttonsMenus[6].initButton(&tft, 20, 270, 30, 30, WHITE, BLACK, WHITE, "P", 2);
  buttonsMenus[6].drawButton();
  buttonsMenus[7].initButton(&tft, 60, 270, 30, 30, WHITE, BLACK, WHITE, "C", 2);
  buttonsMenus[7].drawButton();
  if (switches.diagram == 0)
  {
    buttonsMenus[8].initButton(&tft, 120, 270, 70, 30, WHITE, BLACK, BLUE, "Diagram", 1);
    buttonsMenus[8].drawButton(); switches.statusDiagram = 0;
  }
  if (switches.diagram == 1)
  {
    buttonsMenus[8].initButton(&tft, 120, 270, 70, 30, BLACK, WHITE, BLUE, "Table", 1);
    buttonsMenus[8].drawButton(); switches.statusDiagram = 1;
  }
  if (switches.clockmenuToggle == 0)
  {
    buttonsMenus[9].initButton(&tft, 120, 305, 70, 30, BLACK, BLUE, WHITE, "Trig", 2);
    buttonsMenus[9].drawButton();
  }
  buttonsMenus[11].initButton(&tft, 60, 305, 30, 30, WHITE, GREY, BLACK, "B", 2);
  buttonsMenus[11].drawButton();
}
//----------------------------------------------------------------------------------------------------------------------//
// ---------------------------------------Select number of pins for IC search-------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void autoScreen() //Select number of pins for auto search
{
  tft.fillScreen(BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(20, 50);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println(F("Identify IC"));
  tft.setCursor(22, 48);
  tft.setTextColor(GREY);  tft.setTextSize(3);
  tft.println(F("Identify IC"));
  tft.setCursor(42, 82);
  tft.setTextColor(BLUE);  tft.setTextSize(2);
  tft.println(F("Number of Pins"));
  tft.setCursor(40, 80);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.println(F("Number of Pins"));
  buttonsMenus[3].initButton(&tft, 60, 150, 100, 40, WHITE, GREY, BLACK, "14", 3);
  buttonsMenus[3].drawButton();
  buttonsMenus[4].initButton(&tft, 180, 150, 100, 40, WHITE, BLUE, BLACK, "16", 3);
  buttonsMenus[4].drawButton();
  buttonsMenus[6].initButton(&tft, 60, 200, 100, 40, WHITE, BLUE, WHITE, "20", 3);
  buttonsMenus[6].drawButton();
  buttonsMenus[7].initButton(&tft, 180, 200, 100, 40, WHITE, WHITE, BLACK, "24", 3);
  buttonsMenus[7].drawButton();
  buttonsMenus[8].initButton(&tft, 120, 300, 200, 40, WHITE, GREY, BLACK, "Main menu", 3);
  buttonsMenus[8].drawButton();
}
//----------------------------------------------------------------------------------------------------------------------//
// -----------------------------------------The results of identifying ICs----------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void autoSearchResult(uint8_t mode)
{
  tft.fillScreen(BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(22, 43);
  tft.setTextColor(GREY);  tft.setTextSize(3);
  tft.println(F("Identifying"));
  tft.setCursor(20, 45);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println(F("Identifying"));
  tft.setTextColor(GREY);
  tft.setCursor(39, 72);
  tft.println(F("Completed"));
  tft.setTextColor(WHITE);
  tft.setCursor(40, 70);
  tft.println(F("Completed"));
  tft.setTextColor(WHITE); tft.setTextSize(1);

  if (chipDescription.length() < 38) {
    tft.setCursor(5, 110);
    tft.println(chipDescription);
  }
  if (chipDescription.length() >= 38)
  {
    tft.setCursor(20, 110);  tft.println(chipDescription.substring(0, chipDescription.lastIndexOf(' ', 38)));
    tft.setCursor(13, 120);  tft.println(chipDescription.substring(chipDescription.lastIndexOf(' ', 38), chipDescription.length()));
  }
  if (numberofIcs == 0) //No IC that matched any tests
  {
    tft.setTextColor(GREY);
    tft.setCursor(28, 162); tft.setTextSize(3);
    tft.println(F("IC Matched "));
    tft.setCursor(41, 192); tft.setTextSize(3);
    tft.println(F("No Tests!"));
    tft.setTextColor(RED);
    tft.setCursor(30, 160); tft.setTextSize(3);
    tft.println(F("IC Matched "));
    tft.setCursor(40, 190); tft.setTextSize(3);
    tft.println(F("No Tests!"));
  }
  if (numberofIcs == 1) //If only one IC matche a test
  {
    numberAuto[1].toCharArray(converter, sizeof(converter));  //converts the string "number" to Char so it can be used as text on the buttons
    buttonsMenus[1].initButton(&tft, 120, 190, 200, 30, BLUE, BLACK, WHITE, converter, 2);
    buttonsMenus[1].drawButton();
  }

  byte b = 0; //variables for graphical formatting
  byte c = 1;

  if (numberofIcs > 1) //If more than 1 IC matched a test
  {
    for (uint8_t a = 1; a < numberofIcs + 1; a++)
    {
      numberAuto[a].toCharArray(converter, sizeof(converter));  //converts the string "number" to Char so it can be used as text on the buttons
      if (b == 0) {
        buttonsMenus[a].initButton(&tft, 70, 300 - (c * 35), 100, 30, BLUE, BLACK, WHITE, converter, 2);
      }
      if (b == 1) {
        buttonsMenus[a].initButton(&tft, 170, 300 - (c * 35), 100, 30, BLUE, BLACK, WHITE, converter, 2);
        c++;
      }
      buttonsMenus[a].drawButton();
      b++; if (b == 2) {
        b = 0;
      }
    }
  }
  buttonsMenus[0].initButton(&tft, 120, 300, 200, 30, WHITE, GREY, BLACK, "MainMenu", 3);
  buttonsMenus[0].drawButton(); //back to main menu from autosearch result
  for (uint8_t i = 0; i < pinNumberRouting; i++)
  {
    pinMode(pin[i], INPUT_PULLUP);
    digitalWrite(pin[i], LOW);
  }
}
//----------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------Results from IC test-------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void looptestResult(const String& name, word good, word bad, byte linecount, word fail, word passed)
{
  tft.fillScreen(BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));

  if (numberofIcs == 0) //IC not found in database
  {
    tft.setCursor(40, 100);
    tft.setTextColor(BLUE);  tft.setTextSize(3);
    tft.println(F("IC not in"));
    tft.setCursor(50, 130);
    tft.println(F("Database"));
    tft.setCursor(40, 102);
    tft.setTextColor(RED);  tft.setTextSize(3);
    tft.println(F("IC not in"));
    tft.setCursor(51, 132);
    tft.println(F("Database"));
  }

  if (numberofIcs == 1) //Looptest completed
  {
    tft.setTextColor(BLUE); tft.setTextSize(3);
    if (!isDigit(numberRouting[4])) //this is a position for those chips with 4 numbers in their ID
    {
      tft.setCursor(85, 75);
    }
    if (isDigit(numberRouting[4])) //this is a position for those chips with 4 numbers in their ID
    {
      tft.setCursor(75, 75);
    }
    tft.println(numberRouting);

    tft.setTextColor(WHITE); tft.setTextSize(1);
    if (name.length() < 38) {
      tft.setCursor(5, 110);
      tft.println(name);
    }

    if (name.length() >= 38)
    {
      tft.setCursor(20, 110);  tft.println(name.substring(0, name.lastIndexOf(' ', 38)));
      tft.setCursor(13, 120);  tft.println(name.substring(name.lastIndexOf(' ', 38), name.length()));
    }

    tft.setCursor(10, 150);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.print(F("Failed Tests: ")); tft.println(fail);
    tft.setCursor(10, 170);
    tft.setTextColor(GREEN);  tft.setTextSize(2);
    tft.print(F("Passed Tests: ")); tft.println(passed);
    tft.setCursor(10, 190);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.print(F("Failed Lines: ")); tft.println(bad);
    tft.setCursor(10, 210);
    tft.setTextColor(GREEN);  tft.setTextSize(2);
    tft.print(F("Passed Lines: ")); tft.println(good);
  }
  buttonsMenus[0].initButton(&tft, 60, 300, 100, 40, WHITE, GREY, BLACK, "Menu", 2);
  buttonsMenus[0].drawButton();
  buttonsMenus[1].initButton(&tft, 60, 250, 100, 40, WHITE, WHITE, BLACK, "Pinout", 2);
  buttonsMenus[1].drawButton();
  buttonsMenus[5].initButton(&tft, 180, 250, 100, 40, WHITE, BLACK, GREY, "Repeat", 2);
  buttonsMenus[5].drawButton();
  buttonsMenus[6].initButton(&tft, 180, 300, 100, 40, WHITE, WHITE, BLUE, "Errors", 2);
  buttonsMenus[6].drawButton();
}

//----------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------Tela CIs Rápida-------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//

void quickSelectScreen(int quickScreenPage) //Main menu screen
{
  tft.fillScreen(NUPEDEE_AZUL_CLARO);

  buttonsMenus[0].initButton(&tft, 120, 300, 120, 40, WHITE, RED, WHITE, "Voltar", 3);// go back to main menu
  buttonsMenus[11].initButton(&tft, 30, 300, 40, 40, WHITE, GREEN, WHITE, "<", 3);// go back to main menu
  buttonsMenus[12].initButton(&tft, 210, 300, 40, 40, WHITE, GREEN, WHITE, ">", 3);// go back to main menu

  int botao_largura = 90;
  int botao_altura = 40;
  int botao_espH = 110;
  int botao_espV = 65;
  int botao_tFonte = 3;

  switch (quickScreenPage) {
    case 0:
      buttonsMenus[1].initButton(&tft, 65, 45, botao_largura, botao_altura, RED, NUPEDEE_CINZINHA, RED, "7408", botao_tFonte);
      buttonsMenus[2].initButton(&tft, 65 + botao_espH, 45, botao_largura, botao_altura, ORANGE, NUPEDEE_CINZINHA, ORANGE, "7432", botao_tFonte);
      buttonsMenus[3].initButton(&tft, 65, 45 + botao_espV, botao_largura, botao_altura, WHITE, NUPEDEE_CINZINHA, WHITE, "7486", botao_tFonte);
      buttonsMenus[4].initButton(&tft, 65 + botao_espH, 45 + botao_espV, botao_largura, botao_altura, GREEN, NUPEDEE_CINZINHA, GREEN, "7404", botao_tFonte);
      buttonsMenus[5].initButton(&tft, 65, 45 + botao_espV * 2, botao_largura, botao_altura, YELLOW, NUPEDEE_CINZINHA, YELLOW, "7400", botao_tFonte);
      buttonsMenus[6].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 2, botao_largura, botao_altura, BLUE, NUPEDEE_CINZINHA, BLUE, "7402", botao_tFonte);
      buttonsMenus[7].initButton(&tft, 65, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "7411", botao_tFonte);
      buttonsMenus[8].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "7446", botao_tFonte);
      break;

    case 1:
      buttonsMenus[1].initButton(&tft, 65, 45, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "7474", botao_tFonte);
      buttonsMenus[2].initButton(&tft, 65 + botao_espH, 45, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "7476", botao_tFonte);
      buttonsMenus[3].initButton(&tft, 65, 45 + botao_espV, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4013", botao_tFonte);
      buttonsMenus[4].initButton(&tft, 65 + botao_espH, 45 + botao_espV, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4027", botao_tFonte);
      buttonsMenus[5].initButton(&tft, 65, 45 + botao_espV * 2, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "7446", botao_tFonte);
      buttonsMenus[6].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 2, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4511", botao_tFonte);
      buttonsMenus[7].initButton(&tft, 65, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "74193", botao_tFonte);
      buttonsMenus[8].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4520", botao_tFonte);
      break;

    case 2:
      buttonsMenus[1].initButton(&tft, 65, 45, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4011", botao_tFonte);
      buttonsMenus[2].initButton(&tft, 65 + botao_espH, 45, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4001", botao_tFonte);
      buttonsMenus[3].initButton(&tft, 65, 45 + botao_espV, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4030", botao_tFonte);
      buttonsMenus[4].initButton(&tft, 65 + botao_espH, 45 + botao_espV, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4070", botao_tFonte);
      buttonsMenus[5].initButton(&tft, 65, 45 + botao_espV * 2, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4077", botao_tFonte);
      buttonsMenus[6].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 2, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4010", botao_tFonte);
      buttonsMenus[7].initButton(&tft, 65, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4009", botao_tFonte);
      buttonsMenus[8].initButton(&tft, 65 + botao_espH, 45 + botao_espV * 3, botao_largura, botao_altura, BLACK, NUPEDEE_CINZINHA, BLACK, "4049", botao_tFonte);
      break;

  }

  for (int i = 0; i < 13; i++) {
    buttonsMenus[i].drawButton();
  }


}

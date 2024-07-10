//----------------------------------------------------------------------------------------------------------------------//
// --------------------------------------------------Main Sequence------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
int quickScreenPage = 0;
String quickMenu1[9] = {"", "7408", "7432", "7486", "7404", "7400", "7402", "7411", "7446"};
String quickMenu2[9] = {"", "7474", "7476", "4013", "4027", "7446", "4511", "74193", "4520"};
String quickMenu3[9] = {"", "4011", "4001", "4030", "4070", "4077", "4010", "4009", "4049"};

void flow()
{

  //Serial.print(F("[flow] ScreenStatus: "));
  //Serial.println(screenStatus);
  //------------------------------------------Touch monitoring and responce-----------------------------------------------//
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  bool pressed = false;
  switches.pushed = 0;
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    // Might have to switch the two last values below on either one if your touch is inverted
    p.y = map(p.y, TS_MINX, TS_MAXX, tft.height(), 0);
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.width());

    delay(150);//eh...lets see if this is really needed the debounce is shitty even so...
    pressed = true; switches.pushed = 1;
  }

  // ----------------------------------------------Pinout buttons---------------------------------------------------------//
  for (uint8_t a = 0; a < *pinNumberRoutingPointer; a++)
  {
    if (buttonsPinout[a].contains(p.x, p.y) && pressed) {
      buttonsPinout[a].press(true); // tell the button it is pressed
    }
    else {
      buttonsPinout[a].press(false); // tell the button it is NOT pressed
    }
    if (buttonsPinout[a].justReleased()) {
      if (buttonStatus[a] == 1) {
        buttonsPinout[a].drawButton(); // draw normal
      }
    }
    if (buttonsPinout[a].justPressed()) {
      if (buttonStatus[a] == 0) {
        buttonsPinout[a].drawButton(true); // draw invert!
      }
    }
    if (buttonsPinout[a].isPressed()) {
      buttonsPinout[a].drawButton(true); // draw invert!
    }
  }
  // ------------------------------------------------Menu buttons---------------------------------------------------------//
  for (uint8_t a = 0; a < 14; a++)
  {
    if (buttonsMenus[a].contains(p.x, p.y) && pressed) {
      buttonsMenus[a].press(true); // tell the button it is pressed
    }
    else {
      buttonsMenus[a].press(false); // tell the button it is NOT pressed
    }
    if (buttonsMenus[a].justReleased()) {} //
    if (buttonsMenus[a].justPressed()) {} //
    if (buttonsMenus[a].isPressed()) {} //
  }
  //-----------------------------------Development and debugging monitoring-----------------------------------------------//
  //Serial.println("(" +  String(p.x) + " , " + String(p.y) + " , " + String(p.z) + ")");//uncomment this line to see the touch values on the serial monitor
  //Serial.print("screenstatus: "); Serial.println(screenStatus); //uncomment this line to see in what screenStatus the program is
  //Serial.println(freeMemory());//Uncomment this to see how much memory is left on the device while it´s working
  //---------------------------------------------ScreenStatus Routines---------------------------------------------------//
  switch (screenStatus) //every time the program waits for input from the user excluding the keypad one of those cases are in effect
  {
    case 0: switches.circumvent = 0;  screenStatus = 1; //screenStatus 0 is so it makes sure that it has changed status, circumvent is so you can get to the main menu from the keypad screen :/
    case 1://The main menu
      screenSaver();
      mainScreenDemoScene();//a little graphic functions for the home screen
      if (buttonsMenus[4].justPressed())//last test Screen
      {
        readSaveState();//Reads the saved state variables from the SD card regarding the last test
        if (screenStatusSD == 32) {
          screenStatus = 72; //if the last test was a loop test
        }
        if (screenStatusSD == 69) {
          switches.saveStateSD = 1;  //if the last "test" was a truthtable
          switches.circumventTFT = 1;
          screenStatus = 69;
        }
      }
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 2; //IC search screen
      }
      if (buttonsMenus[2].justPressed()) {
        screenStatus = 3; //Pinout screen
      }
      if (buttonsMenus[3].justPressed()) {
        screenStatus = 32; //IC test screen
      }
      if (buttonsMenus[5].justPressed()) {
        screenStatus = 80; //Config screen
      }

      // Botões NUPEDEE
      if (buttonsMenus[1].justPressed()) {
        buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, WHITE, GREEN, BLACK, "Comuns", 2);
        buttonsMenus[1].drawButton();
        screenStatus = 100;
      }
      break;
    case 2://Select number of pins on the IC for IC search
      screenSaver();
      if (buttonsMenus[8].justPressed()) {
        screenStatus = 0; //Main menu
      }
      if (buttonsMenus[3].justPressed()) {
        pinCount = 14;  //14pins
        screenStatus = 21;
      }
      if (buttonsMenus[4].justPressed()) {
        pinCount = 16;  //16pins
        screenStatus = 21;
      }
      if (buttonsMenus[6].justPressed()) {
        pinCount = 20;  //20pins
        screenStatus = 21;
      }
      if (buttonsMenus[7].justPressed()) {
        pinCount = 24;  //24pins
        screenStatus = 21;
      }
      break;
    case 3://Pinout from Main menu screen
    case 4://Pinout from IC search result screen
    case 5://Pinout from Test result screen
      screenSaver();
      clockAndmuxButtons();//this function contains the clock and mux/demux button actions
      // -------------------------Scrolling through input buttons looking for keypresses--------------------------------------//
      for (uint8_t a = 0; a < *pinNumberRoutingPointer; a++) //
      {
        if (buttonsPinout[a].justPressed() && buttonStatus[a] == 0) //Input selected ON
        {
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], HIGH); buttonNonToggleStatus[a] = 1;
        }
        if (buttonsPinout[a].justPressed() && buttonStatus[a] == 1) //Input selected OFF
        {
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonNonToggleStatus[a] = 0;
        }
        if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 0) {
          buttonStatus[a] = 1;
        }
        if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 1) {
          buttonStatus[a] = 0;
        }

        lastbuttonStatus[a] = buttonStatus[a];//Updates the last read state of a button
        // ----------------------------Scrolling through outputs, reading them and clock pulse----------------------------------//
        if (digitalRead(pin[a]) != lastoutputState[a]) //So the program doesn't write to the screen when there isn't anything new to write to the screen
        {
          switch (outputs[a]) //This variable keeps track of which pins are outputs
          {
            case 1:
              switch (digitalRead(pin[a])) //digitalRead is very slow.... :( But not as slooowwww as writing to the screen ;)
              {
                case 0: rectTest(0, a); //graphical output LOW on the left side
                  break;
                case 1: rectTest(1, a); //graphical output HIGH on the left side
                  break;
              }
              break;
            case 2:
              switch (digitalRead(pin[a])) //digitalRead is very slow.... :( But not as slooowwww as writing to the screen ;)
              {
                case 0: rectTest(2, a); //graphical output LOW on the RIGHT side
                  break;
                case 1: rectTest(3, a); //graphical output HIGH on the RIGHT side
                  break;
              }
              break;
          }
        }
        lastoutputState[a] = digitalRead(pin[a]);//Updates the last read state of an output
        if (buttonsMenus[0].justPressed())//the menubuttons are inside the for loop so they can be read faster
        {
          for (uint8_t i = 0; i < *pinNumberRoutingPointer; i++) {
            pinMode(pin[i], INPUT_PULLUP);  //Clear pin assignments  and return to main menu
            digitalWrite(pin[i], LOW);
          }
          screenStatus = 0;
        }
      }//Buttons and output ends
      switches.lastclockbuttonStatus = switches.clockbuttonStatus;//Updates the last read state of the clock button also used for demux!!!
      switches.lastmuxdemuxToggleStatus = switches.muxdemuxToggleStatus;
      break;
    case 21: //aftermath of search for an IC
      if (switches.autosearchEnd == 1)
      {
        if (buttonsMenus[0].justPressed()) {
          screenStatus = 0; //Main menu
        }
        for (uint8_t a = 1; a < numberofIcs + 1; a++) {
          if (buttonsMenus[a].justPressed()) {
            numberRouting = numberAuto[a];  //Pinout
            screenStatus = 4;
          }
        }
      }
      break;
    case 32://testing IC result screen
      screenSaver();
      testCompleted();
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //Main menu
      }
      if (buttonsMenus[1].justPressed()) {
        screenStatus = 5; //Pinout from Manual search result screen
      }
      if (buttonsMenus[5].justPressed()) {
        repeatTest(numberRouting, numberofTests.toInt()); //Repeat last test
      }
      if (buttonsMenus[6].justPressed()) {
        screenStatus = 41; //Pinout from Manual search result screen
      }
      break;
    case 41: //Error screen
      screenSaver();
      if (buttonsMenus[3].justPressed()) {
        screenStatus = 42; //Raw data screen
      }
      if (buttonsMenus[4].justPressed()) {
        screenStatus = 45; //Repeat last test
      }
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //main menu
      }
      break;
    case 42: //Raw data screen
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //Main menu
      }
      break;
    case 45: //repeat last test IC result screen from the error screen
      screenSaver();
      testCompleted();
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //Main menu
      }
      if (buttonsMenus[1].justPressed()) {
        screenStatus = 5; //Pinout from Manual search result screen
      }
      if (buttonsMenus[5].justPressed()) {
        repeatTest(numberRouting, numberofTests.toInt()); //Repeat last test
      }
      if (buttonsMenus[6].justPressed()) {
        screenStatus = 41; //Pinout from Manual search result screen
      }
      break;
    case 69: // truthtable screen
      //-------------------------------------------------Inputs---------------------------------------------------------------//
      screenSaver();
      for (uint8_t a = 0; a < *pinNumberRoutingPointer; a++) //Polling for input buttons
      {
        if (buttonsPinout[a].justPressed() && buttonStatus[a] == 0) //Input selected ON
        {
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], HIGH); clockCounter++;  rowsOfValuesCounter++; buttonNonToggleStatus[a] = 1;
        }
        if (buttonsPinout[a].justPressed() && buttonStatus[a] == 1) //Input selected OFF
        {
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); if (switches.fullCycle == 0) {
            clockCounter++;
            rowsOfValuesCounter++;
          } buttonNonToggleStatus[a] = 0;
        }
        if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 0) {
          buttonStatus[a] = 1;
        }
        if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 1) {
          buttonStatus[a] = 0;
        }
        if (lastInputPushing[a] == 0 && inputs[a] == 1 && buttonStatus[a] == 0 && inputPushing[a] == 1 ) {
          buttonsPinout[a].drawButton(true);
        }
        if (lastInputPushing[a] == 1 && inputs[a] == 1 && buttonStatus[a] == 0 && inputPushing[a] == 0 ) {
          buttonsPinout[a].drawButton();
        }
        lastInputPushing[a] = inputPushing[a];
        lastbuttonStatus[a] = buttonStatus[a];//Updates the last read state of a button
      }
      //----------------------------------------------------------------------------------------------------------------------//
      //--------------------------------------------Menu Buttons functions----------------------------------------------------//
      //----------------------------------------------------------------------------------------------------------------------//

      //-----------------------------------------------Back to main menu------------------------------------------------------//
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //Main menu
      }
      //-------------------------------------------------Starts clock---------------------------------------------------------//
      if (switches.clockmenuToggle == 1) {
        if (buttonsMenus[1].justPressed()) {
          switches.clockToggle = 1; //turns the clock ON
        }
      }

      if (clockCounter != lastclockCounter)
      {
        getTouch(); //Serial.println("clockCounter != lastclockCounter");
        if (switches.status == 1 && cycle > 0) {
          cycle = 0;
        }
      }
      //-----------------------------------------------Cycle input routine----------------------------------------------------//
      if (buttonsMenus[2].justPressed() && clockCounter == lastclockCounter)
      {
        lastclockCounter = 0; clockCounter = 0; lastOutputCounter = 0; rowsOfValuesCounter = 0; switches.clockToggle = 0;//clearing variables so they wont interfere with the next cycle input
        cycle = getIC(2).toInt(); if (switches.circumvent == 1)break; //user selects the number of cycles
        tft.fillScreen(BLACK);//clear screen to erase the keypad screen
        if (switches.diagram == 0) {
          table(); //switch to truthtable mode if that was the mode previously selected
        }
        anotherSpacer = 0; outputCounter = 0;
        if (switches.config3 == 1) {
          SD.remove(fname3);  //clear the CSV file if it's set so in the config
          Serial.println("Clearing Table.csv");
        }
        truthtableButtons(); outputText(); writeTableInputs(); if (switches.clockmenuToggle == 1) {
          writeOutputs(); //write graphics to the screen
        } if (switches.clockmenuToggle == 0) {
          writeIOs();
        }
        SD.remove(fname4);//clear State.txt
        saveState(); //save the state of program
        tft.fillRect(208, 260 , 70, 20, BLACK);
        tft.setCursor(210, 267); tft.setTextColor(YELLOW);
        tft.println(cycle);
      }
      //--------------------------------Prints out on serial the truthtable that was generated--------------------------------//
      if (buttonsMenus[6].justPressed())
      {
        myFile = SD.open(fname3);// open the Table.txt
        if (myFile)
        {
          while (myFile.available())
          {
            Serial.write(myFile.read());// read from the file until there's nothing else in it:
          }
          myFile.close(); screenStatus = 69; switches.clockToggle = 0;// close the file, make sure that the screenStatus remains 69 and the clock is OFF
        }
        else
        {
          Serial.println("error opening Table.csv");// if the file didn't open, print an error:
        }
      }
      //------------------------------------------Clears the table.csv file---------------------------------------------------//
      if (buttonsMenus[7].justPressed())
      {
        screenStatus = 69; switches.clockToggle = 0;
        SD.remove(fname3); Serial.println("Clearing Table.csv"); if (switches.clockmenuToggle == 1) {
          writeOutputs();
        } if (switches.clockmenuToggle == 0) {
          writeIOs();
        }
      }
      //----------------------------------Switches between a diagram and truthtable-------------------------------------------//
      if (buttonsMenus[8].justPressed())
      {
        tft.fillScreen(BLACK);//clear screen
        anotherSpacer = 0; outputCounter = 0; switches.clockToggle = 0;
        truthtableButtons(); outputText(); writeTableInputs();
        if (switches.statusDiagram == 0) {
          switches.diagram = 1;  //timing diagram ON
          buttonsMenus[8].initButton(&tft, 120, 270, 70, 30,  WHITE, BLACK, BLUE, "Table", 1);
          buttonsMenus[8].drawButton();
        }
        if (switches.statusDiagram == 1) {
          switches.diagram = 0;  //timing diagram OFF
          table();
          buttonsMenus[8].initButton(&tft, 120, 270, 70, 30,  WHITE, BLACK, BLUE, "Diagram", 1);
          buttonsMenus[8].drawButton();
        }
        if (switches.lastDiagram == 0) {
          switches.statusDiagram = 1;  //flip the bit for toggle effect
        }
        if (switches.lastDiagram == 1) {
          switches.statusDiagram = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastDiagram = switches.statusDiagram; //Updates the last read state of a fast button
      //---------------------------------Switches between different types of trigger------------------------------------------//
      if (switches.clockmenuToggle == 0)
      {
        if (buttonsMenus[9].justPressed() && !buttonsMenus[8].justPressed())
        {
          if (switches.statusfullCycle == 0) {
            switches.fullCycle = 1;  //full cycle trigger
            buttonsMenus[9].initButton(&tft, 120, 305, 70, 30, BLACK, WHITE, BLUE, "Trig", 2);
            buttonsMenus[9].drawButton();
          }
          if (switches.statusfullCycle == 1) {
            switches.fullCycle = 0;  //half cycle trigger
            buttonsMenus[9].initButton(&tft, 120, 305, 70, 30, BLACK, BLUE, WHITE, "Trig", 2);
            buttonsMenus[9].drawButton();
          }
          if (switches.lastfullCycle == 0) {
            switches.statusfullCycle = 1; //flip the bit for toggle effect
          }
          if (switches.lastfullCycle == 1) {
            switches.statusfullCycle = 0; //flip the bit for toggle effect
          }
        }
        switches.lastfullCycle = switches.statusfullCycle; //Updates the last read state of a fast button
      }
      //---------------------------------------Goes back to pinout mode-------------------------------------------------------//
      if (buttonsMenus[11].justPressed() && !buttonsMenus[7].justPressed())
      {
        switches.saveStateSD = 1; switches.circumventTFT = 0;
        tableRowCounter = 0; clockCounter = 0; lastclockCounter = 0; anotherSpacer = 0; outputCounter = 0; OutputIndex = 1; cycle = 0; rowsOfValuesCounter = 0;
        switches.muxdemuxToggleStatus = 0;
        for (uint8_t i = 0; i < 24; i++) //Clear variables
        {
          lastoutputState[i] = clearArray[0];
          tablet.sortedOutputs[i] = 0;
          tablet.refBuffer[i] = clearArray[0];
          tablet.sortReference[i] = "";
          *globalpinFunctionPointer[i] = "";
        }
        screenStatus = 3; //go to pinout mode
      }
      //-----------------------------------refreshes the table when it gets to the end of the table---------------------------//
      if (countingOutputs < 14)
      {
        if (rowsOfValuesCounter == 9)
        {
          tft.fillRect(60, 1 , 250, (outputCounter + 1) * 15, BLACK); rowsOfValuesCounter = 1; lastOutputCounter = -1; //truthtableData(); truthtableButtons();
          if (switches.diagram == 0) {
            table();
          }
        }
      }
      if (countingOutputs >= 14)
      {
        if (rowsOfValuesCounter == 7)
        {
          tft.fillRect(60, 1 , 140, (outputCounter + 1) * 15, BLACK); rowsOfValuesCounter = 1; lastOutputCounter = -1; //truthtableData(); truthtableButtons();
          if (switches.diagram == 0) {
            table();
          }
        }
      }
      //-----------------------------------Switches between diagram and truthtable function-----------------------------------//
      switch (switches.diagram)
      {
        case 0: truthTableValues();
          break;
        case 1: timingDiagram();
          break;
      }
      //---------------------Stops generation of diagram/table when number of cycles have been completed----------------------//
      if (clockCounter >= cycle) {
        switches.clockToggle = 0;
        cycle = 0;
        automaticInputButtonPusher = 0;
      }
      lastOutputCounter = cycle;
      break;
    case 72://loop test if it was a last test(circumvents the keypad inputs)
      screenSaver();
      testCompleted();
      if (buttonsMenus[0].justPressed()) {
        screenStatus = 0; //Main menu
      }
      if (buttonsMenus[1].justPressed()) {
        screenStatus = 5; //Pinout from Manual search result screen
      }
      if (buttonsMenus[5].justPressed()) {
        repeatTest(numberRouting, numberofTests.toInt()); //Repeat last test
      }
      if (buttonsMenus[6].justPressed()) {
        screenStatus = 41; //Pinout from Manual search result screen
      }
      break;
    case 73: screenStatus = 69;
      break;
    case 80: //Config screen buttons monitoring
      if (buttonsMenus[0].justPressed()) {
        SD.remove(fname5);  //Main menu
        writeConfig();
        screenStatus = 0;
      }
      if (buttonsMenus[6].justPressed()) {
        screenStatus = 81; //Clock speed select
      }
      if (buttonsMenus[4].justPressed())
      {
        if (switches.config1Status == 0) {
          switches.config1 = 1;  //
          buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "ON", 2);
          buttonsMenus[4].drawButton(true);
          switches.keypadIC = 1;
        }
        if (switches.config1Status == 1) {
          switches.config1 = 0;  //
          buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "OFF", 2);
          buttonsMenus[4].drawButton();
          switches.keypadIC = 0;
        }
        if (switches.lastconfig1Status == 0) {
          switches.config1Status = 1;  //flip the bit for toggle effect
        }
        if (switches.lastconfig1Status == 1) {
          switches.config1Status = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastconfig1Status = switches.config1Status; //Updates the last read state of a config 1 button
      if (buttonsMenus[5].justPressed())
      {
        if (switches.config2Status == 0) {
          switches.config2 = 1;  //
          buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "ON", 2);
          buttonsMenus[5].drawButton(true);
          switches.pushingButtons = 1;
        }
        if (switches.config2Status == 1) {
          switches.config2 = 0;  //
          buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "OFF", 2);
          buttonsMenus[5].drawButton();
          switches.pushingButtons = 0;
        }
        if (switches.lastconfig2Status == 0) {
          switches.config2Status = 1;  //flip the bit for toggle effect
        }
        if (switches.lastconfig2Status == 1) {
          switches.config2Status = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastconfig2Status = switches.config2Status; //Updates the last read state of a config 1 button
      if (buttonsMenus[7].justPressed())
      {
        if (switches.config3Status == 0) {
          switches.config3 = 1;  //
          buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "ON", 2);
          buttonsMenus[7].drawButton(true);
          switches.clearCSV = 1;
        }
        if (switches.config3Status == 1) {
          switches.config3 = 0;  //
          buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "OFF", 2);
          buttonsMenus[7].drawButton();
          switches.clearCSV = 0;
        }
        if (switches.lastconfig3Status == 0) {
          switches.config3Status = 1;  //flip the bit for toggle effect
        }
        if (switches.lastconfig3Status == 1) {
          switches.config3Status = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastconfig3Status = switches.config3Status; //Updates the last read state of a config 1 button
      if (buttonsMenus[8].justPressed())
      {
        if (switches.config4Status == 0) {
          switches.config4 = 1;  //
          buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "ON", 2);
          buttonsMenus[8].drawButton(true);
          switches.diagram = 1;
          switches.statusDiagram = 1;
        }
        if (switches.config4Status == 1) {
          switches.config4 = 0;  //
          buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "OFF", 2);
          buttonsMenus[8].drawButton();
          switches.diagram = 0;
          switches.statusDiagram = 0;
        }
        if (switches.lastconfig4Status == 0) {
          switches.config4Status = 1;  //flip the bit for toggle effect
        }
        if (switches.lastconfig4Status == 1) {
          switches.config4Status = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastconfig4Status = switches.config4Status; //Updates the last read state of a config 1 button
      if (buttonsMenus[9].justPressed())
      {
        if (switches.config5Status == 0) {
          switches.config5 = 1;  //
          buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "ON", 2);
          screenStatus = 82;
        }
        if (switches.config5Status == 1) {
          switches.config5 = 0;  //
          buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "OFF", 2);
          buttonsMenus[9].drawButton();
          switches.macgyver = 0;
        }
        if (switches.lastconfig5Status == 0) {
          switches.config5Status = 1;  //flip the bit for toggle effect
        }
        if (switches.lastconfig5Status == 1) {
          switches.config5Status = 0;  //flip the bit for toggle effect
        }
      }
      switches.lastconfig5Status = switches.config5Status; //Updates the last read state of a config 1 button
      break;
    case 81: screenStatus = 80;
      break;
    case 82: screenStatus = 80;
      break;
    case 90: //used to set the program into the mode it was before the screen went to sleep.
      if (previousScreenstatus == 69)
      {
        switches.saveStateSD = 1; switches.circumventTFT = 0;
        tableRowCounter = 0; clockCounter = 0; lastclockCounter = 0; anotherSpacer = 0; outputCounter = 0; OutputIndex = 1; cycle = 0; rowsOfValuesCounter = 0;
        switches.muxdemuxToggleStatus = 0;
        for (uint8_t i = 0; i < 24; i++)
        {
          tablet.sortedOutputs[i] = 0;
          tablet.refBuffer[i] = clearArray[0];
          tablet.sortReference[i] = "";
          *globalpinFunctionPointer[i] = "";
        }
      }
      screenStatus = previousScreenstatus;
      break;
    case 100:
      // Navegação
      if (buttonsMenus[0].justPressed()) {
        Serial.println("Clicou voltar!!");
        screenStatus = 0;
      }
      if (buttonsMenus[11].justPressed()) {
        Serial.println("Clicou <");
        quickScreenPage--;
        if (quickScreenPage < 0) {
          quickScreenPage = 2;
        }
        quickSelectScreen(quickScreenPage);
      }

      if (buttonsMenus[12].justPressed()) {
        Serial.println("Clicou >");
        quickScreenPage++;
        if (quickScreenPage > 2) {
          quickScreenPage = 0;
        }

        quickSelectScreen(quickScreenPage);
      }

      // Botão dos CIs
      // Chamam repeatTest(numberRouting, numberofTests.toInt());//Loop test routine
      if (buttonsMenus[1].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[1]);
            numberRouting = quickMenu1[1];
            screenStatus = 32;
            repeatTest(quickMenu1[1], 1);
            break;
          case 1:
            Serial.println(quickMenu2[1]);
            numberRouting = quickMenu2[1];
            screenStatus = 32;
            repeatTest(quickMenu2[1], 1);
            break;
          case 2:
            Serial.println(quickMenu3[1]);
            numberRouting = quickMenu3[1];
            screenStatus = 32;
            repeatTest(quickMenu3[1], 1);
            break;
        }
      }

      if (buttonsMenus[2].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[2]);
            numberRouting = quickMenu1[2];
            screenStatus = 32;
            repeatTest(quickMenu1[2], 1);
            break;
          case 1:
            Serial.println(quickMenu2[2]);
            numberRouting = quickMenu2[2];
            screenStatus = 32;
            repeatTest(quickMenu2[2], 1);
            break;
          case 2:
            Serial.println(quickMenu3[2]);
            numberRouting = quickMenu3[2];
            screenStatus = 32;
            repeatTest(quickMenu3[2], 1);
            break;
        }
      }

      if (buttonsMenus[3].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[3]);
            numberRouting = quickMenu1[3];
            screenStatus = 32;
            repeatTest(quickMenu1[3], 1);
            break;
          case 1:
            Serial.println(quickMenu2[3]);
            numberRouting = quickMenu2[3];
            screenStatus = 32;
            repeatTest(quickMenu2[3], 1);
            break;
          case 2:
            Serial.println(quickMenu3[3]);
            numberRouting = quickMenu3[3];
            screenStatus = 32;
            repeatTest(quickMenu3[3], 1);
            break;
        }
      }

      if (buttonsMenus[4].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[4]);
            numberRouting = quickMenu1[4];
            screenStatus = 32;
            repeatTest(quickMenu1[4], 1);
            break;
          case 1:
            Serial.println(quickMenu2[4]);
            numberRouting = quickMenu2[4];
            screenStatus = 32;
            repeatTest(quickMenu2[4], 1);
            break;
          case 2:
            Serial.println(quickMenu3[4]);
            numberRouting = quickMenu3[4];
            screenStatus = 32;
            repeatTest(quickMenu3[4], 1);
            break;
        }
      }

      if (buttonsMenus[5].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[5]);
            numberRouting = quickMenu1[5];
            screenStatus = 32;
            repeatTest(quickMenu1[5], 1);
            break;
          case 1:
            Serial.println(quickMenu2[5]);
            numberRouting = quickMenu2[5];
            screenStatus = 32;
            repeatTest(quickMenu2[5], 1);
            break;
          case 2:
            Serial.println(quickMenu3[5]);
            numberRouting = quickMenu3[5];
            screenStatus = 32;
            repeatTest(quickMenu3[5], 1);
            break;
        }
      }

      if (buttonsMenus[6].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[6]);
            numberRouting = quickMenu1[6];
            screenStatus = 32;
            repeatTest(quickMenu1[6], 1);
            break;
          case 1:
            Serial.println(quickMenu2[6]);
            numberRouting = quickMenu2[6];
            screenStatus = 32;
            repeatTest(quickMenu2[6], 1);
            break;
          case 2:
            Serial.println(quickMenu3[6]);
            numberRouting = quickMenu3[6];
            screenStatus = 32;
            repeatTest(quickMenu3[6], 1);
            break;
        }
      }

      if (buttonsMenus[7].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[7]);
            numberRouting = quickMenu1[7];
            screenStatus = 32;
            repeatTest(quickMenu1[7], 1);
            break;
          case 1:
            Serial.println(quickMenu2[7]);
            numberRouting = quickMenu2[7];
            screenStatus = 32;
            repeatTest(quickMenu2[7], 1);
            break;
          case 2:
            Serial.println(quickMenu3[7]);
            numberRouting = quickMenu3[7];
            screenStatus = 32;
            repeatTest(quickMenu3[7], 1);
            break;
        }
      }

      if (buttonsMenus[8].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[8]);
            numberRouting = quickMenu1[8];
            screenStatus = 32;
            repeatTest(quickMenu1[8], 1);
            break;
          case 1:
            Serial.println(quickMenu2[8]);
            numberRouting = quickMenu2[8];
            screenStatus = 32;
            repeatTest(quickMenu2[8], 1);
            break;
          case 2:
            Serial.println(quickMenu3[8]);
            numberRouting = quickMenu3[8];
            screenStatus = 32;
            repeatTest(quickMenu3[8], 1);
            break;
        }
      }

      if (buttonsMenus[9].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[9]);
            numberRouting = quickMenu1[9];
            screenStatus = 32;
            repeatTest(quickMenu1[9], 1);
            break;
          case 1:
            Serial.println(quickMenu2[9]);
            numberRouting = quickMenu2[9];
            screenStatus = 32;
            repeatTest(quickMenu2[9], 1);
            break;
          case 2:
            Serial.println(quickMenu3[9]);
            numberRouting = quickMenu3[9];
            screenStatus = 32;
            repeatTest(quickMenu3[9], 1);
            break;
        }
      }

      if (buttonsMenus[10].justPressed()) {
        switch (quickScreenPage) {
          case 0:
            Serial.println(quickMenu1[10]);
            numberRouting = quickMenu1[10];
            screenStatus = 32;
            repeatTest(quickMenu1[10], 1);
            break;
          case 1:
            Serial.println(quickMenu2[10]);
            numberRouting = quickMenu2[10];
            screenStatus = 32;
            repeatTest(quickMenu2[10], 1);
            break;
          case 2:
            Serial.println(quickMenu3[10]);
            numberRouting = quickMenu3[10];
            screenStatus = 32;
            repeatTest(quickMenu3[10], 1);
            break;
        }
      }


      //if(quickScreenPage == 1){
      break;

  }
  pinMode(XM, OUTPUT);
  digitalWrite(XM, LOW);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);
  //----------------------------------------------------------------------------------------------------------------------//
  //------------------------------------------------Switch Case Routines--------------------------------------------------//
  //----------------------------------------------------------------------------------------------------------------------//
  if (screenStatus != lastStatus)//so it only updates once per statechange(screenStatus)
  {
    switch (screenStatus)
    {
      case 1://All roads lead to Rome.
        //---------------------------------Debugging and truobleshooting monitoring---------------------------------------------//
        //SD.remove(fname5);   //!!!!!uncomment this line if the program doesn't load properly after uploading because the config file might be corrupt!!!!!//
        //readSavedFile();     //reads from the SD card relevent informations regarding previous test and prints it on the serial monitor
        //readConfigFile();    //reads from the SD card the last configuration state and prints it on the serial monitor
        //readICsInPinout();   //prints out all the ICs in pinout.txt
        //readICsInDatabase(); //prints out all the ICs in database.txt
        //----------------------------------------------------------------------------------------------------------------------//
        clearingRoutines();//clearing routines so previous tests don't interfere with future tests.
        readConfig();      //assigns config variables with saved config data from the SD card
        modeScreen();      //Main menu screen
        break;
      case 2:   autoScreen(); //Select number of pins for auto search
        break;
      case 3:   switches.manualSwitch = 1; if (switches.saveStateSD == 0) {
          numberRouting = getIC(0);
        } if (switches.circumvent == 1)break; Test_init(pinCount); lastoutputState[0] = 4; switches.saveStateSD = 0; //Manual pinout select
        break;
      case 4:   switches.manualSwitch = 0; Test_init(pinCount); SD.remove(fname4); saveState();  // Pinout after automatic search result
        break;
      case 5:   switches.manualSwitch = 1; Test_init(pinCount); SD.remove(fname4); saveState(); // Pinout after IC testing
        break;
      case 21:  autoSearch(pinCount); switches.autosearchEnd = 1; autoSearchResult();//Searches the SD card for the right chip
        break;
      case 32:
        numberRouting.reserve(6);
        numberRouting = getIC(0);
        if (switches.circumvent == 1) break;
        numberofTests.reserve(8); numberofTests = getIC(1);
        if (switches.circumvent == 1)break;
        SD.remove(fname4); saveState();
        repeatTest(numberRouting, numberofTests.toInt());//Loop test routine
        break;
      case 41:  errorInfo();//error info screen
        break;
      case 42:  rawData();//raw info screen
        break;
      case 45:  repeatTest(numberRouting, numberofTests.toInt());//Check if case 72 can be 45 instead
        break;
      case 69:
        if (switches.saveStateSD == 1) {
          switches.manualSwitch = 1;  //this gets executed if the last state saved was a truthtable
          switches.circumventTFT = 1;
          Test_init(pinCount);
        }
        clockCounter = 0; truthtableData(); if (switches.clockmenuToggle == 1) {
          writeOutputs();
        } if (switches.clockmenuToggle == 0) {
          writeIOs();
        } anotherSpacer = 0; outputCounter = 0; outputText();
        if (switches.diagram == 0) {
          table();
        } truthtableButtons(); numberofTests = 1; //number of tests gets set here so there is a value to be written on the SD card files even though it isn't needed for this routine
        break;
      case 72:  repeatTest(numberRouting, numberofTests.toInt());//if last test that was saved to the SD card was a looptest it runs this routine
        break;
      case 80:  configScreen();
        break;
      case 81:  speed = getIC(3).toInt(); speed = map(speed, 10, 1, 2, 255); if (switches.circumvent == 1)break;
        break;
      case 82:  macgyverTimingMechanism = getIC(4).toInt(); macgyverTimingMechanism = map(macgyverTimingMechanism, 1, 2, 30000, 65000); if (switches.circumvent == 1) {
          break;
        } macgyverCounter = 0; switches.macgyver = 1;
        break;
      case 100:
        quickSelectScreen(0);
        break;
    }
    //---------------------------------------------Routines end------------------------------------------------------
    lastStatus = screenStatus;
  }
  lapsCounter++;//keeps track of how many times the main routine has been executed
  macgyverCounter++;//screensaver timer
}

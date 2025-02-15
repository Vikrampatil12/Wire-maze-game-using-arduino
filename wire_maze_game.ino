#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzerPin = 6;
const int greenLedPin = 4;
const int redLedPin = 5;
const int wireMazePin = 3;
const int resetButtonPin = 2; // Manual reset button

int count = 0;
bool gameLost = false;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(wireMazePin, INPUT_PULLUP); // Enable internal pull-up for maze input
  pinMode(resetButtonPin, INPUT_PULLUP); // Enable internal pull-up for reset button

  Serial.begin(9600);

  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();
  lcd.print("Start the game!");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check if the reset button is pressed at any time
  if (digitalRead(resetButtonPin) == LOW) {
    resetGame(); // Reset the game immediately
    return;      // Exit this iteration of the loop
  }

  if (gameLost) {
    // If the game is lost, display the lost message
    lcd.clear();
    lcd.print("You lost!");
    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 1000); // Continuous buzzer sound
    delay(300);           // Allow time to realize loss
    noTone(buzzerPin);
    lcd.clear();
    lcd.print("Press Reset Btn");
    return; // Stop the loop until reset is pressed (handled above)
  }

  // Detect if the wire touches the maze
  if (digitalRead(wireMazePin) == LOW) {
    count++;
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    tone(buzzerPin, 1000); // Play buzzer sound at 1000 Hz
    Serial.println("PIN Grounded - Hit Detected");
    delay(500); // Delay for debounce
    noTone(buzzerPin);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    Serial.println("PIN not grounded");
  }

  // Update hit count on the LCD
  lcd.clear();
  lcd.print("Hit count: ");
  lcd.setCursor(0, 1);
  lcd.print(count);

  // Check if the user has lost
  if (count >= 5) {
    gameLost = true; // Mark the game as lost
  }

  delay(100); // Small delay for smooth updates
}

void resetGame() {
  lcd.clear();
  lcd.print("Resetting...");
  delay(2000);

  // Reset the game state
  count = 0;
  gameLost = false;

  lcd.clear();
  lcd.print("Start the game!");
  delay(2000);
  lcd.clear();
}













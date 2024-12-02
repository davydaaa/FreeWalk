#define trigPin 2
#define echoPin 3
#define ledPin 13         // Вбудована лампочка на платі
#define buzzerPin 9       // Пін для базера
#define buttonPin 4       // Пін для кнопки

bool systemState = false;  // Стан системи (ввімкнено/вимкнено)
bool lastButtonState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Вмикаємо внутрішній підтягуючий резистор
}

void loop() {
  long duration, distance;
  
  // Зчитуємо стан кнопки
  bool buttonState = digitalRead(buttonPin);

  // Перевірка зміни стану кнопки (для уникнення помилок через дрижання контактів)
  if (buttonState == LOW && lastButtonState == HIGH) {
    systemState = !systemState;   // Змінюємо стан системи на протилежний
    delay(50);                    // Затримка для уникнення дрижання контактів
  }
  
  lastButtonState = buttonState;  // Оновлюємо стан кнопки

  // Якщо система вимкнена, вимикаємо лампочку і базер та виходимо з loop
  if (!systemState) {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    return;
  }

  // Якщо система ввімкнена, вимірюємо відстань
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Отримуємо тривалість сигналу
  duration = pulseIn(echoPin, HIGH);

  // Розраховуємо відстань у сантиметрах
  distance = duration * 0.034 / 2;

  // Виводимо відстань у серійну консоль
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Якщо відстань менша за 50 см, вмикаємо лампочку та базер
  if (distance < 50) {
    digitalWrite(ledPin, HIGH);    // Лампочка вмикається
    digitalWrite(buzzerPin, HIGH); // Базер вмикається
  } else {
    digitalWrite(ledPin, LOW);     // Лампочка вимикається
    digitalWrite(buzzerPin, LOW);  // Базер вимикається
  }

  delay(500); // Затримка перед наступним вимірюванням
}

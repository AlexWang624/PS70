#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TFT_eSPI.h>

// Create display objects
Adafruit_SSD1306 display(128, 64, &Wire, -1);
TFT_eSPI tft = TFT_eSPI();

// Button and display control variables
bool displayOn = true;
int buttonPin1 = 33;  // First button
int buttonPin2 = 32;  // Second button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastPotValue = -1;
int currentCardIndex = 0;
int currentPage = 0;  // Current text page

const char* tarotNames[] = {
  "The Fool", "The Magician", "The High Priestess", "The Empress", "The Emperor", 
  "The Hierophant", "The Lovers", "The Chariot", "Strength", "The Hermit", 
  "Wheel of Fortune", "Justice", "The Hanged Man", "Death", "Temperance", 
  "The Devil", "The Tower", "The Star", "The Moon", "The Sun", "Judgement", "The World"
};

const char* tarotDescriptions[] = {
  "In a city of ever-shifting streets, Elias discovered a map that led to elusive realms. Each road traveled presented doors to impossible places—a library of lost words, a garden of singing trees. Elias realized the map was a gateway to alternate dimensions, but it also led him closer to a truth about his own existence, a truth that would shatter his perception of reality.",
  "In a city cocooned within twilight, Elias unearthed an ancient codex on his 33rd birthday. Each page bore symbols that endowed him with mystical abilities tied to earthly elements. But what began as a wondrous journey soon turned ominous, as Elias realized the codex was an enigmatic gateway, beckoning him to plunder the world's magic for an unfathomable purpose.",
  "In an ancient library, the librarian whispered forgotten knowledge to enchanted books, their pages shimmering with long-lost histories. Each book was a gate to a different world, a memory of civilizations that existed only in dreams. The librarian herself was the gatekeeper, guiding those who sought to unravel the mysteries of these realms. But what happened when she ventured into a book whose tale was unfinished?",
  "In a city that bloomed with each of her footsteps, Maya discovered an inexplicable power—a touch that brought forth life in all its forms. Every plant and creature flourished under her hands, but soon she realized that her power could create both beauty and chaos. Each day a new life emerged, and Maya had to reckon with the responsibilities of her extraordinary gift, while the city transformed into a wild, unpredictable jungle.",
  "In the sprawling megacity, a man emerged every midnight to claim dominion over deserted streets. He wielded a scepter that commanded architecture, reshaping buildings with a sweep. Each night, the city transformed into an unruly sentient entity, defying his attempts to control it. The man yearned for order, but the city revealed the futility of such desires, teaching him the beauty in chaos and the allure of unpredictability. The city, a sentient labyrinth, became his eternal companion in this surreal exploration of power and liberation.",
  "In an enigmatic city, a bridge appeared every midnight, connecting two realms. It was atop this bridge that Kira, a young painter, encountered a figure known as The Hierophant. With each meeting, Kira's perception of reality blurred, and the bridge became a gateway to a spectral world. Kira's art began to manifest as living entities, and the bridge became a sentient entity, seeking liberation. A narrative of freedom, control, and the power of creation unfolded, revealing a city that was an ever-shifting tapestry of dream and waking life.",
  "In a labyrinthine city, a mirror aficionado named Luca discovered a doorway that led to a surreal reflection realm. There, he met reflections that held the secret desires and untapped potential of those they mirrored. But as Luca dove deeper into this realm, he found himself becoming a reflection, his identity fading into a mere echo. To reclaim himself, he must confront the enigmatic reflection of a lost love, bound by a love that transcended the boundaries of reality.",
  "In a city of perpetual motion, Asher discovered a mysterious rickshaw drawn by invisible steeds. This chariot carried him between dimensions, exploring realms where buildings pulsed with life and streets echoed ancient rhythms. But each journey unraveled Asher's own reality, blurring the line between dream and waking world. The chariot became a paradox, both liberation and prison, forcing Asher to confront his deepest fears and desires.",
  "In a city where buildings transformed at will, Kassandra discovered an old tome, each page depicting elusive structures. As she unraveled its mysteries, Kassandra found herself in a labyrinthine edifice, wherein rooms echoed her past and future. The more she explored, the more the building shaped her, merging her strength and fears with its architecture, until she became a guardian of the ever-changing cityscape.",
  "On a midnight subway, Elias discovered a hidden platform that materialized once a year. This platform was a gateway to a city of eternal darkness, where time stood still. Every visit revealed specters seeking guidance and companionship. Grappling with isolation and enlightenment, Elias became the guiding light for lost souls, illuminating both their paths and his own journey in this enigmatic city of shadows.",
  "In a city governed by luck, Elias discovered an enigmatic wheel that determined his fate and fortune. Each spin unraveled a multitude of possibilities, from abundant wealth to miserable destitution. But Elias soon realized the wheel's real power - not just a manifestation of luck, but a mirror reflecting the true nature of one's desires. As the wheel spun, Elias's grip on reality and his own identity began to waver, leading him down a path of self-discovery and madness.",
  "In a city governed by elusive time, Melody, a violinist, discovered an enchanted pocket watch that could pause and rewind moments. Playing melodies with the watch, she created symphonies that altered reality, making time flow backward or hang like a frozen aria. Every harmony, however, came with a consequence, as the city itself began to mimic her music, caught in a cycle of echoes and reversals.",
  "In a city of perpetual twilight, a detective discovered a shadowy figure who turned reality upside down. His investigations unraveled a cosmic riddle, revealing a world suspended in delicate balance between dream and waking.",
  "In a world ruled by a grim corporation, Lana discovered an antique wristwatch that could turn back time. Each rewind revealed an elusive figure, the Timekeeper, growing closer. Lana learned that every reversal brought consequences; people's lives became unhinged, reality itself twisted. The Timekeeper offered an ultimatum: stop using the watch, or become a living paradox trapped in an eternal cycle.",
  "In the city of Liquare, where emotions flowed like rivers, Temperance Lazara was a bartender, mixing drinks that could change moods. One night, a mysterious traveler ordered a concoction, and its effects were beyond anything Temperance had experienced. As she delved into a labyrinth of liquid emotions, each drink revealed hidden desires and fears, teaching her the true nature of balance and transformation.",
  "In a city where shadows masked secrets, there stood a haunted alley never appearing twice in the same place. Named 'The Devil's Thicket,' it appeared only to those desperate for a contract, a pact that came at a sinister price. The alley itself was a labyrinth, each turn revealing a darker truth about oneself. As midnight neared, the Thicket's power grew, threatening to ensnare the city in its web of twisted souls.",
  "In a ruinous metropolis rose a tower entwined with ancient vines. Each night, the tower transformed, a gateway to realms of surreal beauty or abject dread. Among those who ascended, some returned forever changed, their souls a mosaic of dream and reality. As the tower's transformations grew wilder, it beckoned to a dreamer attuned to its enigma, offering a glimpse of the city's true nature.",
  "In a city where streetlights mysteriously flickered out past midnight, a girl named Vega discovered a hidden shrine. Rendered a celestial being each night, she navigated dreamlike constellations and met spirits embodying starlight. However, as this astral connection grew, the city darkened, its shadows alive with ancient yearnings. Vega's quest unravelled the enigma of the city's lights and her own celestial nature, posing questions about the balance between illumination and darkness, and the transcendence of one's origins.",
  "In a metropolis that vanished at twilight, Maxine discovered a secret realm controlled by moonlight. Each night, she followed a lunar path through surreal streets, where buildings receded into darkness or sprouted fantastic architecture. This realm's enigmatic residents, the Lunarians, wore masks and cloaks, and communicated through ethereal music. Maxine's goal: unravel the mystery of the Lunarians and their connection to the city's vanishing.",
  "In a city clouded under eternal night, a man discovered a lamp that radiated sunlight, shaped like the elusive Sun card. When he transformed into a being of light, he found himself in a realm where buildings pulsed with energy and dreams took on physical form. Yet, every beam of sunlight he emitted cast a shadow that whispered ancient secrets. The line between enlightenment and darkness blurred as he sought an understanding of his newfound nature.",
  "In a city veiled by perennial mist, every dawn revealed a world beyond reality. Lucius, a trumpeter, played a haunting melody that awakened long-lost souls from their slumber. But these awakenings had consequences, as memories and echoes intertwined, revealing a truth about the city's forgotten past. The mist was a shroud that hid not just a city but also its judgments and reckonings.",
  "In a city that flowed with an ethereal light, Rhea discovered a pocket watch that controlled time itself. Every tick forward warped reality, while each backward turn unraveled Rhea's past. As the city transformed into a living labyrinth, Rhea realized the watch was a key to a cosmic puzzle, a gateway to untangle the threads of existence."
};

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  tft.init();
  tft.invertDisplay(1);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  displayText(tarotNames[currentCardIndex]);  // Display initial Tarot name on OLED
}

void loop() {
  int readButton1 = digitalRead(buttonPin1);
  int readButton2 = digitalRead(buttonPin2);

  if (readButton1 != lastButtonState1 && millis() - lastDebounceTime > debounceDelay) {
    if (readButton1 == HIGH && lastButtonState1 == LOW) {
      displayOn = !displayOn;
      Serial.println(displayOn ? "Display ON" : "Display OFF");
      lastDebounceTime = millis();
      if (displayOn) {
        displayText(tarotNames[currentCardIndex]);  // Update OLED display
        displayTypingEffect(tarotDescriptions[currentCardIndex], currentPage * 500, 500);  // Display text with typing effect on TFT
      } else {
        clearDisplay();
      }
    }
    lastButtonState1 = readButton1;
  }

  if (readButton2 != lastButtonState2 && millis() - lastDebounceTime > debounceDelay) {
    if (readButton2 == HIGH && lastButtonState2 == LOW) {
      currentPage++;
      displayTypingEffect(tarotDescriptions[currentCardIndex], currentPage * 500, 500);
      lastDebounceTime = millis();
    }
    lastButtonState2 = readButton2;
  }

  updateDisplay();  // Update display based on potentiometer
}

void displayText(const char* text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
  Serial.println("Text updated: " + String(text));
}

void updateDisplay() {
  int potValue = analogRead(25);
  if (potValue != lastPotValue) {
    currentCardIndex = map(potValue, 0, 4095, 0, 21);
    displayText(tarotNames[currentCardIndex]);
    currentPage = 0;  // Reset to the first page of the new text
    lastPotValue = potValue;
  }
}

void displayTypingEffect(const char* text, int start, int length) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  for (int i = start; text[i] != '\0' && i < start + length; i++) {
    tft.print(text[i]);
    delay(50);
  }
}

void clearDisplay() {
  display.clearDisplay();
  display.display();
  tft.fillScreen(TFT_BLACK);
  Serial.println("Display cleared.");
}

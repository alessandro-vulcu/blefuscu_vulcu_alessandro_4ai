#include <stdlib.h>
int ledRosso = 13;
int fotoresistore = A5;
int t1 = 0;
int t2 = 0;
int cond = 0;
String morse = "";
char letters [] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', ':', '?', '=', '-', '(', ')', '"', '/', '@', '!'};
String Rilev [] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "---...", "..--..", "-...-", "-....-", "-.--.", "-.--.", "-.--.-", ".-..-.", "-..-.", ".--.-.", "-.-.--", "X"};

typedef struct nodo {
  char lettera;
  struct nodo* next;
} Lista;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledRosso, OUTPUT);
  pinMode(fotoresistore, INPUT);
  Serial.begin(9600);
  Serial.println("Sono pronto a ricevere:");
}

Lista* parola = NULL;

Lista* Add(Lista* l, char d) {
  if (l == NULL) {
    Lista* n = (Lista*)malloc(sizeof(Lista));
    n->lettera = d;
    n->next = NULL;
    Serial.print(n->lettera);
    return n;
  }
  else {
    l->next = Add(l->next, d);
    return l;
  }
}

void stampaLista(Lista* l) // Metodo che stampa la lista
{
  if (l != NULL) // Se 'l' è NULL siamo alla fine della lista
  {
    Serial.print("Comunicazione completa: ");
    Serial.println(l -> lettera); // stampa il dato della lista
    stampaLista(l -> next); // richiama la funzione per stampare tutti gli elementi successivi
  }
}

void ControllaCarattere() {
  if (cond < 2020) {
    if (480 < cond < 520)
    {
      morse += ".";
    }
    else if (980 < cond < 1200 )
    {
      morse += "-";
    }
    if (morse == ".-.-.-") {// END CHARACTER E' IL PUNTO
      parola = Add(parola, '.');
      lampeggiarosso();
      morse = "";
      return;
    }
  }
  else {
    lettera();
  }

}
void lampeggiarosso() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledRosso, HIGH);
    delay(200);
    digitalWrite(ledRosso, LOW);
    delay(200);
  }
  stampaLista(parola);
  Serial.println("Trasmissione finita, pronto per iniziarne un'altra");
  Serial.println();
}
void lettera() {
  for (int i = 0; i < sizeof(Rilev); i++) {
    if (Rilev[i] == morse)
    {
      parola = Add(parola, letters[i]);
      morse = "";
      break;
    }
    i++;
  }
}
void tempo() {
  t1 = millis();
  while (analogRead(fotoresistore) > 400);
  t2 = millis();
  cond = t2 - t1;
}
void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(fotoresistore) > 400)
  {
    tempo();
    ControllaCarattere();
    delay(100);
  }
}

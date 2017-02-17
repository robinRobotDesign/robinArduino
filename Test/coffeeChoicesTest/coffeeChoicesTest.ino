#include <QueueList.h>
#define HOTCHOCO 6
#define LATTEMACCHIATO 7
#define WIENERMELANGE 5
#define TEA 4
#define CAFFELATTE 3
#define CAPPUCCINO 2
#define COFFEE 1
#define ESPRESSO 0
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
int coffeechoices[] = {HOTCHOCO, LATTEMACCHIATO, WIENERMELANGE,
                       TEA, CAFFELATTE, CAPPUCCINO,
                       COFFEE, ESPRESSO
                      };

QueueList<int> orderlist;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  orderlist.push(1);
  int drink = orderlist.pop();
  Serial.print(NELEMS(coffeechoices));
  Serial.print(contains(0, drink));
  delay(1000);
}

bool contains(int drinks, int value) {
  if (drinks == 0) {
    for (int i = 0; i < NELEMS(coffeechoices) ; i++) {
      if (coffeechoices[i] == value) {
        return true;
      }
    }

  }
  if (drinks == 1) {
    for (int i = 0; i < NELEMS(coffeechoices) ; i++) {
      if (coffeechoices[i] == value) {
        return true;
      }
    }
  }
  return false;
}

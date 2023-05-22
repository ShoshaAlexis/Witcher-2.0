#include "functions.h"
void dialevent(){
if (abs(p->x - 600) < 80){
    text.setString("Press Space to talk");
    text.setPosition(70, 650);
    window.draw(text);
}
}

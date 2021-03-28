
#include "mbed.h"
#include "platform/mbed_thread.h"
#include "Adafruit_SSD1306.h"
#include <string> 

// an SPI sub-class that provides a constructed default
class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk)
    {
        format(8,3);
        frequency(2000000);
    };
};
SPIPreInit gSpi(D11,D12,D13);
Adafruit_SSD1306_Spi lcd(gSpi,D9,D8,D10,64,128);

InterruptIn btn(USER_BUTTON);
Thread game_thread;
EventQueue queue;


void button_listener();
void cancel_events();

void print(string label){
    for (int i = 0; i < label.size(); i++){
        lcd.writeChar(label[i]);
    }
}

void end_game(){ 
    // finish current game
    cancel_events();
    lcd.setTextCursor(10, 45);
    print("Oops. Play again?");
    lcd.display();
    game_mode=0;
    printf("%d\n", game_mode);
}

void update_cactus(){
    // update cactus position
    lcd.drawBitmap(i,ground_level+2,bObst.data,bObst.x,bObst.y,1);
    //lcd.drawBitmap(bObst,i+16,ground_level);
    i = round(i-s);
    if (i<-16){
        i=130;
    }
    
    // detect dino collision
    if((i>26 && i<30) && curr_h+bDino.y > ground_level) {
        end_game();
    }
}

void update_dino(){
    if(isJumping){
        if(isMovingUp){
            curr_h -= jmp_step;
            if(curr_h == max_jmp_y){
                isMovingUp = false;
            }
        } else {
            curr_h += jmp_step;
            if (curr_h == ground_level){
                isJumping = false;
            }
        }
    }
    lcd.drawBitmap(20,curr_h,bDino.data,bDino.x,bDino.y,1);
}

int curr_dino_bitmap = 1;

void start_screen(){
    lcd.setTextCursor(28, 10);
    print("Press F to start");
    lcd.display();
}

void update_frame(){
    lcd.clearDisplay();
    lcd.drawLine(0, 31, 130, 31, 1);
    lcd.setTextCursor(90,5);
    print(to_string(score));
    update_dino();
    update_cactus();
    lcd.display();
}

    int call_id1;
    int call_id2;
    int call_id3;

void reload_params(){
    // reset game process params
    score = 0;
    i = 130;
    isJumping = false;
    curr_h = ground_level;
    bool isMovingUp = true;
}
    
void play_game(){
    // init game loop callbacks
    reload_params();
    call_id1 = queue.call_every(30, &update_frame); // 16 ticks ~ 60 frames per second
    call_id2 = queue.call_every(100, &score_tick); // update score
    call_id3 = queue.call_every(400, &shift_dino_bm); // anim dino (^_^)
}

void cancel_events(){
    // stop game loop
    printf("--stop game issued\n");
    queue.cancel(call_id1);
    queue.cancel(call_id2);
    queue.cancel(call_id3);
}

void button_listener(){
    switch (game_mode){
        case 0:
            play_game();
            game_mode=1;
        break;
        case 1:
            jump();
        break;
        case 2:
        break;
        default:
            printf("--error in SWITCH\n");
        break;
    }
    printf("%d\n", game_mode);
}

int main() {
    //lcd setup
    lcd.clearDisplay();
    lcd.display();
    //start game loop
    start_screen();
    btn.fall(queue.event(&button_listener));
    queue.dispatch_forever();
  }

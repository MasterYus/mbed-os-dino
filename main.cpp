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
}

void update_cactus(){
}

//lcd.drawBitmap(20,curr_h,bDino.data,bDino.x,bDino.y,1);

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
    //print(to_string(score));
    //update_dino();
    update_cactus();
    lcd.display();
}

    int call_id1;
    int call_id2;
    int call_id3;

void reload_params(){
    // reset game process params
}
    
void play_game(){
    // init game loop callbacks
    reload_params();
    call_id1 = queue.call_every(30, &update_frame); // 30 ticks ~ 30 frames per second
    //call_id2 = queue.call_every(100, &score_tick); // update score
    //call_id3 = queue.call_every(400, &shift_dino_bm); // anim dino (^_^)
}

void cancel_events(){
    // stop game loop
    printf("--stop game issued\n");
    queue.cancel(call_id1);
    queue.cancel(call_id2);
    queue.cancel(call_id3);
}

void button_listener(){
    /*
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
    */
}

int main() {
    //lcd setup
    lcd.clearDisplay();
    lcd.display();
    //start game loop
    start_screen();
    btn.fall(queue.event(&button_listener));
    game_thread.start(callback(&queue,&EventQueue::dispatch_forever));
  }

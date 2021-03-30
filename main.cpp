#include "mbed.h"
#include "platform/mbed_thread.h"
#include "Adafruit_SSD1306.h"
#include <cstddef>
#include <cstdlib>
#include <string> 
#include <Game.cpp>

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
Game* game;


void button_listener();
void cancel_events();

void print(string label){
    for (int i = 0; i < label.size(); i++){
        lcd.writeChar(label[i]);
    }
}

 // finish current game
void end_game(){ 
    cancel_events();
    lcd.setTextCursor(10, 45);
    print("Oops. Play again?");
    lcd.display();
}


// draw start screen
void start_screen(){
    lcd.setTextCursor(28, 10);
    print("Press F to start");
    lcd.display();
}

// output game engine models
void draw_out(){
    lcd.clearDisplay();
    if (game != nullptr){
        for (auto model : game->get_all_models())
        {
            switch (model.getType()){
                case TYPE_BITMAP:
                    lcd.drawBitmap(model.getX(),model.getY(),model.getBitmap()->data,model.getBitmap()->x,model.getBitmap()->y,1);
                break;
                case TYPE_TEXT:
                    lcd.setTextCursor(model.getX(),model.getY());
                    print(model.getText());
                break;
                case TYPE_LINE:
                    lcd.drawLine(model.getX(),model.getY(), model.getToX(), model.getToY(), 1);
                break;
            }
        }
    } else {
        lcd.setTextCursor(10,32);
        print("Internal error while loading models!");
    }
    lcd.display();
}

    int call_id1;
    int call_id2;
    int call_id3;

// thread game shedule tickers 
void game_ticker(){
    game->on_game_tick();
    draw_out();
}

void score_ticker(){
    game->score_tick();
}

void dino_ticker(){
    game->shift_dino_bm();
}

// setup EventQueve thread
void play_game(){
    game = new Game();
    game -> set_end_callback(&end_game);
    // init game loop callbacks
    call_id1 = queue.call_every(30, &game_ticker); // 30 ticks ~ 30 frames per second
    call_id2 = queue.call_every(100, &score_ticker); // update score
    call_id3 = queue.call_every(400, &dino_ticker); // anim dino (^_^)
}

void cancel_events(){
    // stop game loop
    printf("--stop game issued\n");
    free(game);
    queue.cancel(call_id1);
    queue.cancel(call_id2);
    queue.cancel(call_id3);
}

void button_listener(){
    if (game == nullptr){
        play_game();
    } else {
        game ->jump();
    }
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

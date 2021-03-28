#include <iostream>
#include <Model.cpp>
#include <vector>

using namespace std;

class Game {
    // basic parameters
    int ground_level = 19;
    int i = 130;
    double s=2;
    bool isJumping = false;
    bool isMovingUp = true;
    int game_mode = 0;
    int score = 0;
    int curr_dino_bitmap = 1;
    Bitmap bDino= bDino1;
    vector<Model> models;

    // jmp parameters
    int curr_h = ground_level;
    int max_jmp_y = 5;
    int jmp_step = 1;
    
    public:
    Game(){
        models.push_back(Model(0,31,130,31,"ground"));
        models.push_back(Model(0,0,&bDino1,"dino"));
        models.push_back(Model(90,5,to_string(score),"score"));
    }
    void on_game_tick(){
        
    }

    int find_model_index_by_tag(string tag){
        int position = -1;
        for (auto md : models){
            position++;
            if (md.getTag() == tag) return position;
        }
        return position;
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
        models.at(2).move_to(20,curr_h);
        }   
    }

    void jump(){
        if (!isJumping){
            isJumping = true;
            isMovingUp = true;
            shift_dino_bm();
            curr_h=ground_level;
        }
    }

    void score_tick(){
        score += 1;
        //s+=0.005; // dynamic speed increase
    }

    void shift_dino_bm(){
        if (!isJumping){
            if (curr_dino_bitmap == 1){
                // set 2nd bitmap
                bDino = bDino2;
                curr_dino_bitmap = 2;
            } else {
                // set 1st bitmap
                bDino = bDino1;
                curr_dino_bitmap = 1;
            }
        } else {
            // jump bitmap
            bDino = bDino3;
            curr_dino_bitmap = 3;
        }
    }

    void update_cactus(){
        // update cactus position
        //lcd.drawBitmap(i,ground_level+2,bObst.data,bObst.x,bObst.y,1);
        i = round(i-s);
        if (i<-16){
            i=130;
        }
        
        // detect dino collision
        if((i>26 && i<30) && curr_h+bDino.y > ground_level) {
            //end_game();
        }
    }

};
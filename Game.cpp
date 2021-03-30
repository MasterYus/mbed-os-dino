#include <iostream>
#include <Model.cpp>
#include <vector>
#include <random>

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
    vector<Model> obsts;

    // jmp parameters
    int curr_h = ground_level;
    int max_jmp_y = 3;
    int jmp_step = 1;

    random_device rd;

    void (*end_callback)();
    
    public:
    Game(){
        models.push_back(Model(0,31,130,31,"ground"));
        models.push_back(Model(0,0,&bDino1,"dino"));
        models.push_back(Model(90,5,to_string(score),"score"));
    }

    void on_game_tick(){
        update_dino();
        update_cactus();
    }

    void set_end_callback(void (*callback)()){
        end_callback = callback;
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
                models.at(2).shiftBitmap(&bDino2);
                curr_dino_bitmap = 2;
            } else {
                // set 1st bitmap
                models.at(2).shiftBitmap(&bDino1);
                curr_dino_bitmap = 1;
            }
        } else {
            // jump bitmap
            models.at(2).shiftBitmap(&bDino3);
            curr_dino_bitmap = 3;
        }
    }

    void update_cactus(){
        // update cactus position
        if (obsts.empty()){
            add_cactus();
        } else {
            if (obsts.back().getTtl() <= 0)
            add_cactus();
        }

        for (int i = 0; i < obsts.size(); i++){
            int x = obsts.at(i).getX();
            x = round(x-s);
            obsts.at(i).setTtl(obsts.at(i).getTtl()-1);
            if (x<-16){
                if (obsts.at(i).getTtl() <= 0)
                    obsts.erase(obsts.begin()+i);
            } else {
                obsts.at(i).move_to(x);
            }
            if((x>26 && x<30) && curr_h+bDino.y > ground_level) {
                if (end_callback != nullptr){
                    (*end_callback)();
                }
            }
        }
    }

    void add_cactus(){
        mt19937 rng(rd());
        uniform_int_distribution<int> uni(50,130);

        Model new_obst = Model(130,ground_level+2,&bObst,"obst");
        new_obst.setTtl(uni(rng));
        obsts.push_back(new_obst);
    }

    vector<Model> get_all_models(){
        vector<Model> tmp = models;
        tmp.insert( tmp.end(), obsts.begin(), obsts.end());
        return tmp;
    }

};
#include <iostream>

using namespace std;

static uint8_t Obst[] = {
  0x38, 0x38, 0x20, 0xfe, 0xfe, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00
};

static uint8_t Dino1[] = {
0x3c, 0x70, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xfe, 0xff, 0xf9, 0xf9, 0x3f, 0x2f, 0x6f, 0x0f, 
0x0f, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x07, 0x05, 0x01, 0x00, 0x0f, 0x09, 0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00
};

static uint8_t Dino2[] = {
0x3c, 0x70, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xfe, 0xff, 0xf9, 0xf9, 0x3f, 0x2f, 0x6f, 0x0f, 
0x0f, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x09, 0x01, 0x00, 0x07, 0x05, 0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00
};

static uint8_t Dino3[] = {
0x3c, 0x70, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xfe, 0xff, 0xf9, 0xf9, 0x3f, 0x2f, 0x6f, 0x0f, 
0x0f, 0x0e, 0x08, 0x08, 0x08, 0x09, 0x0f, 0x0d, 0x09, 0x08, 0x0f, 0x0d, 0x09, 0x08, 0x08, 0x08, 
0x08, 0x08, 0x08, 0x08
};

static string TYPE_LINE = "line";
static string TYPE_BITMAP = "bitmap";
static string TYPE_TEXT = "text";


struct Bitmap{
    int16_t x;
    int16_t y;
    int Byte_in_Line;
    uint8_t* data;
};

static Bitmap bObst = {
  8, // XSize
  10, // YSize
  1, // Bytes in Line
  Obst,  // Pointer to picture data
};

static Bitmap bDino1= {
  18, // XSize
  12, // YSize
  3, // Bytes in Line
  Dino1,  // Pointer to picture data
};

static Bitmap bDino2= {
  18, // XSize
  12, // YSize
  3, // Bytes in Line
  Dino2,  // Pointer to picture data
};

static Bitmap bDino3= {
  18, // XSize
  11, // YSize
  3, // Bytes in Line
  Dino3,  // Pointer to picture data
};

class Model {
    int x = 0;
    int y = 0;
    int to_x = 0;
    int to_y = 0;
    int ttl = 100;
    string tag = "";
    string type;
    Bitmap* bm;
    
    public:
    Model(int x, int y, Bitmap* bm, string tag = ""){
        this->x = x;
        this->y = y;
        this->bm = bm;
        this->tag = tag;
        type = TYPE_BITMAP;
    }

    Model(int x, int y, string text, string tag = ""){
        this->x = x;
        this->y = y;
        this->tag = tag;
        type = TYPE_TEXT;
    }

    Model(int x, int y, int to_x, int to_y, string tag = ""){
        this->x = x;
        this->y = y;
        this->to_x = to_x;
        this->to_y = to_y;
        this->tag = tag;
        type = TYPE_LINE;
    }
    void move_to(int x, int y){
        this->x = x;
        this->y = y;
    }

    void move_to(int x){
        this->x = x;
    }

    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    Bitmap* getBitmap(){
        return bm;
    }

    void setTtl(int ttl){
        this->ttl = ttl;
    }
    int getTtl(){
        return ttl;
    }
    
    void shiftBitmap(Bitmap* bm){
        if (type == TYPE_BITMAP)
            this->bm = bm;
    }
    string getTag(){
        return tag;
    }
    
};
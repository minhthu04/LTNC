#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H


#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "MainObject.h"


class MainObject : public BaseObject {
public:
    MainObject();
    ~MainObject();

    enum Walktype// cac doi tuong xu ly
    {
        WALK_STOP = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
        WALK_UP = 3,
        WALK_DOWN = 4,
        ATTACK = 5,

    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandelInputAction(SDL_Event events, SDL_Renderer* screen);
    void set_clips();

    void DoPlayer(Map &map_data);
    void CheckToMap(Map &map_data);
    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data);
private:
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int width_frame;
    int height_frame;
    int start_frame;
    int attack_frame;
    int start_attack;
    bool check_attack;
    int dead_frame;
    int short_frame;
    int delay_frame;


    bool check_hp_player;
    bool check_dead_player;
    bool check_menu_dead;


    SDL_Rect frame_clip[MAX_FRAME];
    Input input_type_;
    int frame_;
    int status_;

    int get_width_frame() const {return width_frame;}
    int get_height_frame() const {return height_frame;}
    
    int map_x_;
    int map_y_;

    SDL_RendererFlip flip_ = SDL_FLIP_NONE;
};

#endif
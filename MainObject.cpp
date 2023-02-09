#include "CommonFunc.h"
#include "MainObject.h"

#define PLAYER_SPEED 8

MainObject::MainObject() {
    frame_ = 0;
    x_pos = 0;
    y_pos = 0;
    x_val = 0;
    y_val = 0;
    width_frame = 0;
    height_frame = 0;
    start_frame = 0;
    attack_frame = 2;

    delay_frame = DELAY_FRAME;
    dead_frame = 4;
    check_dead_player = false;

    start_attack = 0;
    check_attack = false;


    status_ = -1;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    map_x_ = 0;
    map_y_ = 0;
};

MainObject::~MainObject() {

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if(ret == true)
    {
        width_frame = rect_.w/MAX_FRAME;
        height_frame = rect_.h/MAX_FRAME_H;
    }

    return ret;
}

void MainObject::set_clips() {
    if(width_frame > 0 && height_frame > 0) {
        frame_clip[0].x = 0;
        frame_clip[0].y = start_frame * height_frame;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;

        for(int i = 1; i < MAX_FRAME - short_frame; i++) {
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = start_frame * height_frame;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }
}

void MainObject::Show(SDL_Renderer* des) {
    LoadImg("img//player.png", des);

    if (!check_dead_player) {
        frame_ ++;
    }
    if (frame_ >= MAX_FRAME && !check_dead_player) {
        frame_ = 0;
    }
    rect_.x = x_pos - map_x_;// vi tri ban dau - vitri map cuon => vt hien tai
    rect_.y = y_pos - map_y_;
    
          
        
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame*RATIO_PLAYER, height_frame*RATIO_PLAYER};
    SDL_Rect* current_clip = &frame_clip[frame_];
    SDL_RenderCopyEx(des, p_Object_, current_clip, &renderQuad, 0, NULL, flip_);
    
    SDL_DestroyTexture(p_Object_);  //da dung xong
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen) {
    if (!check_dead_player) {
        if (events.key.keysym.sym == SDLK_RIGHT) {
            input_type_.direction_ = false;
        }
        else {
            if (events.key.keysym.sym == SDLK_LEFT)
                input_type_.direction_ = true;
        }
        if (events.type == SDL_KEYDOWN) {
            switch (events.key.keysym.sym) {
            case SDLK_RIGHT:
                {
                    status_ = WALK_RIGHT;
                    input_type_.right_ = 1;
                    input_type_.left_ = 0;
                    start_frame = 1;
                    short_frame = 0;
                }
                break;

            case SDLK_LEFT:
                {
                    status_ = WALK_LEFT;
                    input_type_.left_ = 1;
                    input_type_.right_ = 0;
                    start_frame = 1;
                    short_frame = 0;
                }
                break;

            case SDLK_UP:
                {
                status_ = WALK_UP;
                input_type_.up_ = 1;
                input_type_.down_ = 0;
                start_frame = 1;
                short_frame = 0;
                }
                break;

            case SDLK_DOWN:
                {
                status_ = WALK_DOWN;
                input_type_.down_ = 1;
                input_type_.up_ = 0;
                start_frame = 1;
                short_frame = 0;
                }
                break;       
            case SDLK_SPACE:
                {
                status_ = ATTACK;
                input_type_.attack_ = 1;
                // input_type_.down_ = 0;       //danh -> dung yen
                // input_type_.up_ = 0;
                // input_type_.right_ = 0;
                // input_type_.left_ = 0;
                start_frame = 2;
                short_frame = attack_frame;
                check_attack = true;
                }
                break;
            } 
        }
        else {
            if (events.type == SDL_KEYUP) {
                switch(events.key.keysym.sym) {
                    case SDLK_RIGHT:
                        {
                            input_type_.right_ = 0;
                            input_type_.direction_ = false;
                        }
                        break;
                    case SDLK_LEFT:
                        {
                            input_type_.left_ = 0;
                            input_type_.direction_ = true;
                        }
                        break;
                    case SDLK_UP:
                        {
                            input_type_.up_ = 0;
                        }
                        break;
                    case SDLK_DOWN:
                        {
                            input_type_.down_ = 0;
                        }
                        break;
                    case SDLK_SPACE:
                        {
                            input_type_.attack_ = 0;                    
                        }
                    default:
                        break;
                }
                //if (!check_attack) {
                    if (input_type_.right_ == 0 && input_type_.left_ == 0 && input_type_.up_ == 0 && input_type_.down_ == 0) {
                        status_ = WALK_STOP;
                        start_frame = 0;
                        short_frame = 0;
                    }
                //}
            }
        }
    }
}

void MainObject::DoPlayer(Map& map_data) {
     x_val = 0;
     y_val = 0;
    if (input_type_.left_ == 1) {
        x_val -= PLAYER_SPEED;
    }
    else if (input_type_.right_ == 1) {
        x_val += PLAYER_SPEED;
    }

    if (input_type_.up_ == 1) {
        y_val -= PLAYER_SPEED;
    }
    else if(input_type_.down_ == 1) {
        y_val += PLAYER_SPEED;
    }
    CheckToMap(map_data);
    CenterEntityOnMap(map_data); // tinh ra thong so cua ban do
 }

 void MainObject::CenterEntityOnMap(Map& map_data) {
    map_data.start_x_ = x_pos - (SCREEN_WIDTH/2);// + width_frame*RATIO_PLAYER;// di chuyen 1/2 ban dao thi ban do bat dau cuon theo
    if (map_data.start_x_ < 0) {
        map_data.start_x_ = 0;
    }
    else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_) {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos - (SCREEN_HEIGHT/2);// + height_frame*RATIO_PLAYER;; // cuon doc 1/2 map
    if (map_data.start_y_ < 0) {
        map_data.start_y_  = 0;
    }
    else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_) {
        map_data.start_y_ = map_data.start_y_ - SCREEN_HEIGHT;
    }
 }

 void MainObject::CheckToMap(Map& map_data)
 {
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //check ...(ngang)
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;
    x1 = (x_pos + x_val)/TILE_SIZE;
    x2 = (x_pos + x_val + width_frame*RATIO_PLAYER - 1)/TILE_SIZE;

    y1 = (y_pos)/TILE_SIZE;
    y2 = (y_pos + height_min*RATIO_PLAYER - 1) /TILE_SIZE;

    /*
        x1,y1..........x2,y1
        *...............*
        *...............*
        *...............*
        x1,y2...........x2,y2
    */

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if (x_val > 0) {//mainobject is moving to right;
            for(int i = y1 + SIZE_BACK*2; i <= y2; i++) {
                if (map_data.tile[i][x2] != BLANK_TILE) {
                    x_pos = (x2 + SIZE_BACK)*TILE_SIZE;
                    x_pos -= (width_frame*RATIO_PLAYER + 1);
                    x_val = 0;
                }
            }                       
        }
        else if (x_val < 0) { // sang trai
            for(int i = y1 + SIZE_BACK*2; i <= y2; i++) {
                if (map_data.tile[i][x1] != BLANK_TILE) {
                    x_pos = (x1 - SIZE_BACK + 1)*TILE_SIZE;
                    x_val = 0;
                }
            }
        }
    }

    //check vertical (doc)

    int width_min = width_frame < TILE_SIZE ? width_frame : TILE_SIZE;
    x1 = (x_pos)/TILE_SIZE;
    x2 = (x_pos + width_min*RATIO_PLAYER)/TILE_SIZE;

    y1 = (y_pos + y_val)/TILE_SIZE;
    y2 = (y_pos + y_val + height_frame*RATIO_PLAYER - 1)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if (y_val > 0) { // xuong
            for(int i = x1 + SIZE_BACK*2; i <= x2 - SIZE_BACK; i++) {
                if (map_data.tile[y2][i] != BLANK_TILE) {
                    y_pos = y2*TILE_SIZE;
                    y_pos -= (height_frame*RATIO_PLAYER + 1);
                    y_val = 0;
                    // bool ... = true; // check con tren ban do 
                }
            }                      
        }        
        else if (y_val < 0) {
            for(int i = x1 + SIZE_BACK*2; i <= x2 - SIZE_BACK; i++) {
                if (map_data.tile[y2][i] != BLANK_TILE) {
                    y_pos = (y1 - SIZE_BACK + 1)*TILE_SIZE;
                    y_val = 0;
                }
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    if (x_pos < 0) {
        x_pos = 0;
    }
    else if (x_pos + width_frame > map_data.max_x_) {
        x_pos = map_data.max_x_ - width_frame - 1;
    }
    
    if (y_pos + SIZE_BACK*TILE_SIZE < 0) {
        y_pos = 0 - SIZE_BACK*TILE_SIZE;
    }
    else if (y_pos + height_frame + TILE_SIZE > map_data.max_y_) {
        y_pos = map_data.max_y_ - TILE_SIZE - height_frame - 1;
    }
 }
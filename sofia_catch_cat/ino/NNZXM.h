#ifndef NNZXM_H
#define NNZXM_H

#include "Arduino.h"
#include "common.h"
#include "screen.h"
#include "input.h"

class NNZXM {
    public:
        NNZXM(screen screen, input input, character player, character NPC, game_map map)
            : screen{screen}, input{input}, player{player}, NPC{NPC}, map{map}
        {
            _cur_ts = millis();
            _ts_int = 200;
        }
        void init();
        void loop();
        void finish();

        bool ready();

        void showMap();
        void showPlayer();
        void showNPC();

        bool updatePlayer(int dir);
        void updateNPC();

    private:
        screen& screen;
        input& input;
        character player;
        character NPC;
        game_map map;

        unsigned long _cur_ts = 0;
        unsigned long _ts_int = 0;

        bool poseCmp(struct pos &p1, struct pos &p2);
        long distance(const struct pos &p1, const struct pos &p2);
};

#endif

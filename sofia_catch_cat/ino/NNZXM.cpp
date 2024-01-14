#include "NNZXM.h"

#undef DEBUG

void NNZXM::init() {

    player.pos = {4, 6, 20, 20};
    NPC.pos = {4, 151, 20, 20};

    screen.printText(60, 40, "NNZXM", Terminal12x16, COLOR_BLACK, COLOR_RED);
    screen.printText(40, 150, "press to continue", Terminal6x8, COLOR_BLACK, COLOR_BLUE);
    while (false == input.getPressed())
    {
        delay(10);
    }
}

void NNZXM::loop() {
    int dir;
    
    screen.clean();
    showMap();
    showPlayer();
    showNPC();

    while(true)
    {
        while (false == ready())
        {
            Serial.println("wait for round ready");
            delay(100);
        }

        int dir = input.getDir();
        if (updatePlayer(dir))
        {
            break;
        }
        updateNPC();
    }
}

void NNZXM::finish() {
    screen.clean();
    screen.printText(80, 40, "END", Terminal12x16, COLOR_BLACK, COLOR_GREEN);
    screen.printText(40, 150, "press to restart", Terminal6x8, COLOR_BLACK, COLOR_BLUE);
    while (false == input.getPressed())
    {
        delay(100);
    }
}

bool NNZXM::ready()
{
    return true;
    bool ready = false;
    unsigned long ts = millis();
    if ((ts - _cur_ts) > _ts_int)
    {
      ready = true;
      _cur_ts = ts;
    }
    return ready;
}

void NNZXM::showMap()
{
    screen.showLines(map.lines, map.num);
}

void NNZXM::showPlayer()
{
    screen.drawColorPixelmap(player.pos.x, player.pos.y, player.pos.w, player.pos.h, player.pixels);
}

void NNZXM::showNPC()
{
    screen.drawColorPixelmap(NPC.pos.x, NPC.pos.y, NPC.pos.w, NPC.pos.h, NPC.pixels);
}

bool NNZXM::poseCmp(struct pos &p1, struct pos &p2)
{
    if (p1.x == p2.x && p1.y == p2.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#define PLAYER_MOVESTEPS 2

bool NNZXM::updatePlayer(int dir)
{
    struct pos pos = player.pos;
    struct pos backup = player.pos;
    bool update = true;

    if (dir & input::UP)
    {
        pos.y -= PLAYER_MOVESTEPS;
    }
    if (dir & input::DOWN)
    {
        pos.y += PLAYER_MOVESTEPS;
    }
    if (dir & input::LEFT)
    {
        pos.x -= PLAYER_MOVESTEPS;
    }
    if (dir & input::RIGHT)
    {
        pos.x += PLAYER_MOVESTEPS;
    }

    if ((pos.x <= BORDER_LEFT || (pos.x + pos.w) >= BORDER_RIGHT) ||
        (pos.y <= BORDER_UP || (pos.y + pos.h) >= BORDER_DOWN))
    {
        if (PLAYER_MOVESTEPS > 1)
        {
          pos = player.pos;

          if (dir & input::UP)
          {
              pos.y -= 1;
          }
          if (dir & input::DOWN)
          {
              pos.y += 1;
          }
          if (dir & input::LEFT)
          {
              pos.x -= 1;
          }
          if (dir & input::RIGHT)
          {
              pos.x += 1;
          }
        }
    }

    if ((pos.x <= BORDER_LEFT || (pos.x + pos.w) >= BORDER_RIGHT) ||
        (pos.y <= BORDER_UP || (pos.y + pos.h) >= BORDER_DOWN))
    {
        update = false;
    }
    else if (poseCmp(player.pos, pos))
    {
        update = false;
    }
    else
    {
        update = true;
        player.pos = pos;
    }

    if (update || (distance(backup, NPC.pos) < 3000))
    {
        screen.cleanArea(backup.x, backup.y, backup.w, backup.h);
        showPlayer();
    }

    if (poseCmp(player.pos, NPC.pos))
    {
        return true;
    }
    else
    {
        return false;
    }
}

#define NPC_MOVESTEPS 1

long NNZXM::distance(const struct pos &p1, const struct pos &p2)
{
  int x1 = (int)p1.x;
  int y1 = (int)p1.y;
  int x2 = (int)p2.x;
  int y2 = (int)p2.y;
  long dis;
  long disx = abs(x1 - x2);
  long disy = abs(y1 - y2);
  
  dis = disx * disx + disy * disy;
  return dis;
}

void NNZXM::updateNPC()
{
    struct pos pos = NPC.pos;
    struct pos backup = NPC.pos;
    long bestdis = 0;
    long dis = 0;
    int dis_case = 0;

    #ifdef DEBUG
    Serial.print("--> player ");
    Serial.print(player.pos.x);
    Serial.print(" ");
    Serial.print(player.pos.y);
    Serial.print(" NPC ");
    Serial.print(pos.x);
    Serial.print(" ");
    Serial.print(pos.y);
    Serial.print(" ");
    #endif

    /* leftup */
    pos = NPC.pos;
    if ((pos.x - NPC_MOVESTEPS) > BORDER_LEFT && (pos.y - NPC_MOVESTEPS) > BORDER_UP)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
        pos.y = pos.y - NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("1-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 1;
        }
    }

    /* up */
    pos = NPC.pos;
    if ((pos.y - NPC_MOVESTEPS) > BORDER_UP)
    {
        pos.y = pos.y - NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("2-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 2;
        }
    }

    /* rightup */
    pos = NPC.pos;
    if ((pos.x + pos.w + NPC_MOVESTEPS) < BORDER_RIGHT && (pos.y - NPC_MOVESTEPS) > BORDER_UP)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
        pos.y = pos.y - NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("3-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 3;
        }
    }

    /* right */
    pos = NPC.pos;
    if ((pos.x + pos.w + NPC_MOVESTEPS) < BORDER_RIGHT)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("4-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 4;
        }
    }

    /* rightdown */
    pos = NPC.pos;
    if ((pos.x + pos.w + NPC_MOVESTEPS) < BORDER_RIGHT && (pos.y + pos.h + NPC_MOVESTEPS) < BORDER_DOWN)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
        pos.y = pos.y + NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("5-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 5;
        }
    }

    /* down */
    pos = NPC.pos;
    if ((pos.y + pos.h + NPC_MOVESTEPS) < BORDER_DOWN)
    {
        pos.y = pos.y + NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("6-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 6;
        }
    }

    /* leftdown */
    pos = NPC.pos;
    if ((pos.x - NPC_MOVESTEPS) > BORDER_LEFT && (pos.y + pos.h + NPC_MOVESTEPS) < BORDER_DOWN)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
        pos.y = pos.y + NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("7-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 7;
        }
    }

    /* left */
    pos = NPC.pos;
    if ((pos.x - NPC_MOVESTEPS) > BORDER_LEFT)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
        dis = distance(pos, player.pos);

        #ifdef DEBUG
        Serial.print(" ");
        Serial.print("8-");
        Serial.print(dis);
        #endif

        if (dis > bestdis)
        {
            bestdis = dis;
            dis_case = 8;
        }
    }

    /* still */
    pos = NPC.pos;
    dis = distance(pos, player.pos);

    #ifdef DEBUG
    Serial.print(" ");
    Serial.print("9-");
    Serial.print(dis);
    #endif

    if (dis > bestdis)
    {
        bestdis = dis;
        dis_case = 9;
    }

    #ifdef DEBUG
    Serial.print("NPC case ");
    Serial.println(dis_case);
    delay(50);
    #endif

    pos = NPC.pos;
    
    if (dis_case == 1)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
        pos.y = pos.y - NPC_MOVESTEPS;
    }
    else if (dis_case == 2)
    {
        pos.y = pos.y - NPC_MOVESTEPS;
    }
    else if (dis_case == 3)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
        pos.y = pos.y - NPC_MOVESTEPS;
    }
    else if (dis_case == 4)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
    }
    else if (dis_case == 5)
    {
        pos.x = pos.x + NPC_MOVESTEPS;
        pos.y = pos.y + NPC_MOVESTEPS;
    }
    else if (dis_case == 6)
    {
        pos.y = pos.y + NPC_MOVESTEPS;
    }
    else if (dis_case == 7)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
        pos.y = pos.y + NPC_MOVESTEPS;
    }
    else if (dis_case == 8)
    {
        pos.x = pos.x - NPC_MOVESTEPS;
    }

    if (!poseCmp(NPC.pos, pos))
    {
        screen.cleanArea(backup.x, backup.y, backup.w, backup.h);
        NPC.pos = pos;
        showNPC();
    }
}
#include <ncurses.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

class Stage { //main 화면 부터 stage까지 생성.
public:
	int sx, sy; //stage window 생성 위치
	int s1_h, s1_w; //stage1의 가로 세로 길이
	//Window Function
	Stage();
	void InitHome();// 처음 시작화면(윈도우 생성)
	void Stage_1(); // First Stage
};

class Snake {
public:

    int set_x, set_y; // Snake 생성 좌표 지정.
    int min_len = 3;
    int max_len = 12; // Snake 최소 최대 길이 지정.
    vector<pair<int, int>> body; //Snake의 몸 좌표 저장. 0(head) ~ -1(tail)
    int h, w;

    char map[1][32][63] = {
      {
      "99999999999999999999999999999999999999999999999999999999999999",
      "92111111111111111111111111111111111111111111111111111111111129",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000011111111110000000000000000000000001111111111000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000111111111100000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000011111111110000000000000000000000001111111111000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000111111111100000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000011111111110000000000000000000000001111111111000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000111111111100000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000011111111110000000000000000000000001111111111000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000111111111100000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000011111111110000000000000000000000001111111111000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "91000000000000000000000000000000000000000000000000000000000019",
      "92111111111111111111111111111111111111111111111111111111111129",
      "99999999999999999999999999999999999999999999999999999999999999"}
    };

    //Snake Function
    Snake(int y, int x, int h, int w); //생성자.
    void ShowSnake(int stage_num); // 뱀 화면에 출력.
    
    bool CrushBody(int stage_num); // Snake의 head가 Body에 닿았는지 체크.
    bool UnableItem(int stage_num, int p1, int p2); // 아이템이 생성 가능한 지 확인.
    
    void ShowWin(WINDOW* w1);
    void Game(WINDOW* w1, WINDOW* score, int stage_num);// 게임 시작!
    void UpdateSnake();// 꼬리부터 머리까지 움직이는 방향으로 좌표 최신
};
Snake::Snake(int y, int x, int height, int width) :set_y(y), set_x(x), h(height), w(width) {//생성자
    for (int i = 0; i < 3; i++) {
        body.push_back(make_pair(set_y, set_x - i));
    } // 뱀 위치, 크기 초기화
}
void Snake::ShowSnake(int stage_num) {
    map[stage_num][body[0].first][body[0].second] = '3';
    for (int i = 1; i < body.size(); i++) {
        map[stage_num][body[i].first][body[i].second] = '4';
    }
}
bool Snake::CrushBody(int stage_num) { // Snake의 head가 Body에 닿았는지 체크.
    if (body.size() < 3) { return true; }
    if (map[stage_num][body[0].first][body[0].second] == '1' ||
        map[stage_num][body[0].first][body[0].second] == '4') {
        return true;
    }
    return false;
}
void Snake::UpdateSnake() { //진행방향으로 Snake 꼬리부터 머리쪽으로 이동해줌.
    for (int i = body.size() - 1; i > 0; i--) {
        body[i].first = body[i - 1].first;
        body[i].second = body[i - 1].second;
    }
}
void Snake::ShowWin(WINDOW* w1) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            switch (map[0][i][j]) {
            case 48:
                mvwaddch(w1, i, j, ' ');
                break;
            case 49:
                mvwaddch(w1, i, j, '-');
                break;
            case 50:
                mvwaddch(w1, i, j, 'X');
                break;
            case 51:
                mvwaddch(w1, i, j, 'H');
                break;
            case 52:
                mvwaddch(w1, i, j, 'B');
                break;
            case 53:
                mvwaddch(w1, i, j, 'G');
                break;
            case 54:
                mvwaddch(w1, i, j, 'P');
                break;
            case 55:
                mvwaddch(w1, i, j, 'A');
                break;
            case 57:
                mvwaddch(w1, i, j, ' ');
            }
        }
    }
}
void Snake::Game(WINDOW* w1, WINDOW* score, int stage_num) {

    int d = KEY_RIGHT; // Snake 진행방향
    int old_d = 3;// Snake 이전 진행방향
    int q = 0;
    int g; //gate 들어갔는지 확인 변수
    int Gcount = 0, Pcount = 0, Gatecount = 0;

    while (1) {

        d = wgetch(w1);
        flushinp();
        usleep(150000);

        //꼬리 자르기
        map[stage_num][body.back().first][body.back().second] = '0';

        switch (d) {
        case KEY_UP:
            if (old_d == 2) { q = 1; }
            old_d = 1;
            break;
        case KEY_DOWN:
            if (old_d == 1) { q = 1; }
            old_d = 2;
            break;
        case KEY_RIGHT:
            if (old_d == 4) { q = 1; }
            old_d = 3;
            break;
        case KEY_LEFT:
            if (old_d == 3) { q = 1; }
            old_d = 4;
        }
        q = CrushBody(stage_num);

        if (q == 1) { break; }

        ShowSnake(stage_num);
        ShowWin(w1);
        wrefresh(w1);
        wrefresh(score);
    }
}
Stage::Stage() { // 생성자.
    sx = 5; sy = 5;
    s1_h = 32; s1_w = 62;
}
void Stage::InitHome() {
    int key;

    initscr(); // 처음 Main 화면 생성.

    keypad(stdscr, TRUE); // 특수키 입력 가능.
    curs_set(0); // cursor 안 보이게.
    noecho(); // 입력 값 출력 안 되게 하기.

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_WHITE); // 배경이 white, 글씨가 green

    bkgd(COLOR_PAIR(1)); // pair 1의 색으로 배경 색칠

    mvprintw(3, 4, "Test Game -> prees enter key");

    border('|', '|', '-', '-', '0', '0', '0', '0');
    refresh();

    key = getch();
    switch (key) { // ENTER입력 시 sTAGE1 실행
    case 10:
        Stage_1();
    }
    mvprintw(3, 4, "EndGame , press anykey -> shutdown");
    refresh();
    getch();
    endwin();
}

void Stage::Stage_1() {

    WINDOW* s1 = newwin(s1_h, s1_w, sy, sx); //stage1 화면 생성.
    WINDOW* score = newwin(15, 40, 5, 80);
    WINDOW* mission = newwin(15, 40, 21, 80);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(2));
    wbkgd(s1, COLOR_PAIR(2));

    keypad(s1, TRUE);
    nodelay(s1, TRUE); // 입력을 안 받아도 넘어가게 해 주는 함수.

    Snake s(10, 10, s1_h, s1_w); //20, 20위치에 뱀 생성.
    s.Game(s1, score, 0);
    getch();
    delwin(s1);
}
int main() {
    Stage start;
    start.InitHome();
}

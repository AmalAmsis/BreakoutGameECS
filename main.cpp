//#include "Pong.h"
//using namespace pong;
//
//int main() {
//	Pong p;
//	if (p.valid())
//		p.run();
//	return 0;
//}


#include "breakout/breakout_game.h"
#include "bagel.h"
#include <iostream>

int main() {
    using namespace breakout;

    // יצירת ישויות
    int ballID = CreateBall();
    int paddleID = CreatePaddle(1, 2);
    int brickID = CreateBrick(2);
    int floorID = CreateFloor();
    int uiID = CreateUIManager();

    std::cout << "Entities created:\n";
    std::cout << "Ball ID: " << ballID << "\n";
    std::cout << "Paddle ID: " << paddleID << "\n";
    std::cout << "Brick ID: " << brickID << "\n";
    std::cout << "Floor ID: " << floorID << "\n";
    std::cout << "UIManager ID: " << uiID << "\n";

    // מיקום לצורך בדיקת התנגשות - נשנה את כולם לאותו מקום
    auto& ballPos = bagel::World::getComponent<Position>({ballID});
    ballPos = {100, 100};
    auto& brickPos = bagel::World::getComponent<Position>({brickID});
    brickPos = {100, 100};
    auto& paddlePos = bagel::World::getComponent<Position>({paddleID});
    paddlePos = {100, 100};

    std::cout << "\nRunning systems...\n";

    // הרצת מערכת התנגשות פעמיים כדי לראות שהלבנה מתרסקת
    CollisionSystem();
    CollisionSystem();

    std::cout << "Done.\n";
    return 0;
}

// game variables
class game
{
    int score = 0;
    int lives = 3;
    int level = 1;
    int levelScore = 0;
    int levelLives = 3;
    int levelEnemies = 0;
    int levelEnemiesKilled = 0;
    int levelEnemiesKilledTotal = 0;

    // game states
    bool gamePaused = false;
    bool gameOver = false;
    bool gameWon = false;
    bool gameStarted = false;
    bool gameLevelStarted = false;
    bool gameLevelEnded = false;
    bool gameLevelWon = false;
    bool gameLevelLost = false;
    bool gameLevelRestart = false;
    bool gameLevelNext = false;
    bool gameLevelPrevious = false;
};
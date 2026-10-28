// One function per game state, each runs while GameState is its own
#ifndef STATES_H
#define STATES_H

void Intro();
void TitleScreen();
void GameTypeMenu();
void Credits();
void ReadyGo();
void Game();
void TimeOver();
void GetHighScoreName(char NameIn[21], int Place, int PScore);
void ShowHighScores();

#endif

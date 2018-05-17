#ifndef MAIN_H_
#define MAIN_H_

namespace logic {

void update();
void end_program();
} // namespace logic

namespace input {

void keyoard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
} // namespace input

namespace render {

void display();
} // namespace render

namespace display {

void reshape(int width, int height);
void init(int argc, char **argv);
void run();
} // namespace display

#endif // MAIN_H_


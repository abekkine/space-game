#ifndef GAME_RENDER_H_
#define GAME_RENDER_H_

class GameRender {
public:
    explicit GameRender(bool test_mode)
    : test_mode_(test_mode) {}
    ~GameRender() {}
    void Init() {}
    void Render() {
        if (test_mode_) {
            TestPrimitives();
            TestTextures();
            TestFonts();
        }
        else {
            // TODO Render game content +render +game
            // - Implement methods required to render game content.
        }
    }
private:
    // TODO Testing primitives +render +test
    // - Implement test code to render primitives
    void TestPrimitives() {}
    // TODO Testing textures +render +texture +test
    // Implement test code to render textures
    void TestTextures() {}
    // TODO Testing fonts +render +font +test
    // Implement test code to render fonts & text.
    void TestFonts() {}
private:
    const bool test_mode_;
};

#endif  // GAME_RENDER_H_

#pragma once
#include "widget.hpp"
#include "color.hpp"
#include "coord.hpp"
#include "layout.hpp"
#include "signal_slot.hpp"
#include <string>

class BaseTextBuffer;
class StatusBar;

class Screen: public Widget
{
public:
    Screen(Widget * = nullptr);
    ~Screen();
    struct Char
    {
        Char(wchar_t = L'\0', Color fg = Black, Color bg = White);
        bool operator!=(const Char &) const;
        wchar_t ch;
        Color fg;
        Color bg;
    };
    int widthCh() const;
    int heightCh() const;
    Char &ch(int x, int y);
    const Char &ch(int x, int y) const;
    Coord cursor() const;
    void setCursor(Coord);
    void setCursor(int x, int y);
    BaseTextBuffer *textBuffer() const;
    void setTextBuffer(BaseTextBuffer *);
    int hScroll() const;
    void setHScroll(int);
    int vScroll() const;
    void setVScroll(int);
    Coord startSelection() const;
    void setStartSelection(Coord);
    Coord endSelection() const;
    void setEndSelection(Coord);
    bool isSelected(Coord) const;
    StatusBar *statusBar() const;
    void setStatusBar(StatusBar *);

    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorHome();
    void moveCursorEnd();
    void moveCursorPageUp();
    void moveCursorPageDown();
    int copy();
    void paste();
    void cut();
    void selectAll();
    void startRIsearch();
    void startIsearch();
    void escStatusBar();
protected:
    int glyphHeight() const;
    int glyphWidth() const;
private:
    int glyphWidth_;
    int glyphHeight_;
    Coord cursor_;
    Coord startSelection_;
    Coord endSelection_;
    int hScroll_;
    int vScroll_;
    BaseTextBuffer *textBuffer_;
    StatusBar *statusBar_;
    int prevX_;
    int prevY_;
    std::vector<std::vector<Char> > ch_;
    std::vector<std::vector<Char> > prevCh_;
    void select(void (Screen::*moveCursor)());
    void moveCursor(void (Screen::*moveCursor)());
    std::wstring getSelected() const;
    void render();
    int deleteSelected();
protected:
    virtual void resizeEvent(ResizeEvent &);
    virtual void paintEvent(PaintEvent &);
    virtual bool keyPressEvent(KeyEvent &);
    virtual bool textInputEvent(TextInputEvent &);
};

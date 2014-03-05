#pragma once
#include "widget.hpp"
#include "color.hpp"
#include "coord.hpp"

class BaseTextBuffer;

class Screen: public Widget
{
public:
    Screen(Widget * = nullptr);
    struct Char
    {
        Char(wchar_t = L'\0', Color fg = Black, Color bg = White);
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
    std::vector<std::vector<Char> > ch_;
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorHome();
    void moveCursorEnd();
    void moveCursorPageUp();
    void moveCursorPageDown();
    void select(void (Screen::*moveCursor)());
    void moveCursor(void (Screen::*moveCursor)());
    int copy();
    void paste();
    void cut();
    void selectAll();
protected:
    virtual void resizeEvent(ResizeEvent &);
    virtual void paintEvent(PaintEvent &);
    virtual bool keyPressEvent(KeyEvent &);
    virtual bool textInputEvent(TextInputEvent &);
};

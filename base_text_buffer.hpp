#pragma once
#include "coord.hpp"
#include "cpp_highlighter.hpp"
#include "undo_stack.hpp"
#include <string>
#include <vector>

class Screen;

class BaseTextBuffer
{
public:
    BaseTextBuffer();
    virtual ~BaseTextBuffer();
    const std::wstring &operator[](int line) const;
    std::wstring &operator[](int line);
    int size() const;
    void undo(Coord &cursor);
    void redo(Coord &cursor);
    bool canUndo() const;
    bool canRedo() const;
    bool isModified() const;
    void clearModified();
    void render(Screen *) const;
    void insert(Coord &cursor, std::wstring);
    void del(Coord &cursor, int = 1);
    void backspace(Coord &cursor, int = 1);
    bool isReadOnly() const;
    void setReadOnly(bool);
    std::wstring name() const;
    void setName(std::wstring);
    Coord cursor() const;
    void setCursor(Coord);
protected:
    std::vector<std::wstring> buffer_;
    bool isReadOnly_;
    UndoStack undoStack_;
    std::wstring name_;
    Coord cursor_;
    CppHighlighter *highlighter_;
    virtual std::wstring preInsert(Coord &cursor, std::wstring);
    virtual void postInsert(Coord &cursor, std::wstring);
    virtual int preDel(Coord &cursor, int = 1);
    virtual void postDel(Coord &cursor, int = 1);
    virtual int preBackspace(Coord &cursor, int = 1);
    virtual void postBackspace(Coord &cursor, int = 1);
private:
    void internalInsert(Coord &cursor, std::wstring);
    std::wstring internalDelete(const Coord cursor, int = 1);
    std::wstring internalBackspace(Coord &cursor, int = 1);
};

#pragma once
#include "coord.hpp"
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
    virtual void backspace(Coord &cursor, int = 1);
    bool isReadOnly() const;
    void setReadOnly(bool);
protected:
    virtual void internalInsert(Coord &cursor, std::wstring);
    virtual std::wstring internalDelete(const Coord cursor, int = 1);
    virtual std::wstring internalBackspace(Coord &cursor, int = 1);
    std::vector<std::wstring> buffer_;
    bool isReadOnly_;
    UndoStack undoStack_;
};

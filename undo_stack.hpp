#pragma once
#include "coord.hpp"
#include <vector>

class BaseUndoCommand
{
public:
    virtual ~BaseUndoCommand();
    virtual void undo(Coord &cursor) = 0;
    virtual void redo(Coord &cursor) = 0;
};

template <typename RedoFunc, typename UndoFunc>
class UndoCommand: public BaseUndoCommand
{
public:
    UndoCommand(Coord cursor, const RedoFunc &redoFunc, const UndoFunc &undoFunc):
        redoFunc_(redoFunc),
        undoFunc_(undoFunc),
        cursor_(cursor)
    {}
    virtual void undo(Coord &cursor)
    {
        Coord tmp = cursor_;
        undoFunc_(tmp, data_);
        cursor = tmp;
    }
    virtual void redo(Coord &cursor)
    {
        Coord tmp = cursor_;
        data_ = redoFunc_(tmp);
        cursor = tmp;
    }
private:
    RedoFunc redoFunc_;
    UndoFunc undoFunc_;
    typedef decltype(std::declval<RedoFunc>()(std::declval<Coord &>())) Data;
    Data data_;
    Coord cursor_;
};

class UndoStack
{
public:
    UndoStack();
    ~UndoStack();
    template <typename RedoFunc, typename UndoFunc>
    void push(Coord &cursor, const RedoFunc &redoFunc, const UndoFunc &undoFunc)
    {
        auto command = new UndoCommand<RedoFunc, UndoFunc>{cursor, redoFunc, undoFunc};
        undoStack_.push_back(command);
        command->redo(cursor);
        for (auto c: redoStack_)
            delete c;
        redoStack_.clear();
    }   
    void undo(Coord &cursor);
    void redo(Coord &cursor);
    bool canUndo() const;
    bool canRedo() const;
    void clean();
    bool isModified() const;
    void clearModified();
public:
    std::vector<BaseUndoCommand *> undoStack_;
    std::vector<BaseUndoCommand *> redoStack_;
    std::vector<BaseUndoCommand *>::size_type originalState_;
};
